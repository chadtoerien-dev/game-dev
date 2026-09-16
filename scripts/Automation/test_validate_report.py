import copy
import datetime as dt
import json
import os
from pathlib import Path
import tempfile
import time
import unittest

from validate_report import expected_tests, validate_report


class ReportTests(unittest.TestCase):
    def setUp(self):
        self.temp = tempfile.TemporaryDirectory()
        self.addCleanup(self.temp.cleanup)
        self.path = Path(self.temp.name) / 'index.json'
        self.started = time.time() - 2
        self.expected = {'TheVeil.A'}
        self.good = dict(reportCreatedOn=dt.datetime.now(dt.timezone.utc).strftime('%Y.%m.%d-%H.%M.%S'),
                         succeeded=1, succeededWithWarnings=0, failed=0, notRun=0, inProcess=0,
                         tests=[dict(fullTestPath='TheVeil.A', state='Success', warnings=0, errors=0, entries=[])])

    def check(self, data):
        self.path.write_text(json.dumps(data), encoding='utf-8')
        return validate_report(self.path, self.expected, self.started)

    def test_good_and_info(self):
        self.good['tests'][0]['entries'] = [{'event': {'type': 'Info', 'message': 'ok'}}]
        self.assertEqual(self.check(self.good), 1)

    def test_missing_malformed_duplicate_json(self):
        with self.assertRaises(ValueError):
            validate_report(self.path, self.expected, self.started)
        for text in ('{', '{"tests":[],"tests":[]}', 'null', '[]'):
            self.path.write_text(text)
            with self.subTest(text=text), self.assertRaises((ValueError, TypeError)):
                validate_report(self.path, self.expected, self.started)

    def test_summary_failures(self):
        for field in ('succeeded', 'succeededWithWarnings', 'failed', 'notRun', 'inProcess'):
            for value in (None, True, '0', -1, 2):
                bad = copy.deepcopy(self.good)
                bad[field] = value
                with self.subTest(field=field, value=value), self.assertRaises(ValueError):
                    self.check(bad)

    def test_bad_tests(self):
        for field, values in {'state': ['Fail', 'NotRun', 'InProcess', 'Skipped', None],
                              'warnings': [1, None, False], 'errors': [1, None, False],
                              'entries': [None, [{'event': {'type': 'Warning'}}], [{'event': {'type': 'Error'}}]],
                              'fullTestPath': ['', 'TheVeil.B']}.items():
            for value in values:
                bad = copy.deepcopy(self.good)
                bad['tests'][0][field] = value
                with self.subTest(field=field, value=value), self.assertRaises(ValueError):
                    self.check(bad)

    def test_empty_missing_and_duplicate_tests(self):
        for tests in ([], None, self.good['tests'] * 2):
            bad = copy.deepcopy(self.good)
            bad['tests'] = tests
            bad['succeeded'] = len(tests or [])
            with self.assertRaises(ValueError):
                self.check(bad)
        self.expected.add('TheVeil.Missing')
        with self.assertRaises(ValueError):
            self.check(self.good)

    def test_stale(self):
        self.check(self.good)
        os.utime(self.path, (self.started - 10, self.started - 10))
        with self.assertRaises(ValueError):
            validate_report(self.path, self.expected, self.started)
        self.good['reportCreatedOn'] = '2000.01.01-00.00.00'
        with self.assertRaises(ValueError):
            self.check(self.good)

    def test_source_discovery_and_filters(self):
        source = Path(self.temp.name) / 'Tests.cpp'
        source.write_text('''// IMPLEMENT_SIMPLE_AUTOMATION_TEST(X, "Ignored", flags)
        IMPLEMENT_SIMPLE_AUTOMATION_TEST(
          A, "TheVeil.A", flags)
        IMPLEMENT_SIMPLE_AUTOMATION_TEST(B, "TheVeil.B", flags)
        /* IMPLEMENT_SIMPLE_AUTOMATION_TEST(C, "Ignored", flags) */''')
        for term, expected in [('TheVeil', {'TheVeil.A', 'TheVeil.B'}),
                               ('^TheVeil.A$', {'TheVeil.A'}), ('A$+B$', {'TheVeil.A', 'TheVeil.B'}),
                               ('StartsWith:TheVeil', {'TheVeil.A', 'TheVeil.B'})]:
            self.assertEqual(expected_tests(self.temp.name, term), expected)
        for term in ('Absent', 'TheVeil+Absent', '', 'TheVeil;', 'Group:All', 'TheVeil+'):
            with self.subTest(term=term), self.assertRaises(ValueError):
                expected_tests(self.temp.name, term)
        source.write_text('IMPLEMENT_COMPLEX_AUTOMATION_TEST(A, "TheVeil.A", flags)')
        with self.assertRaises(ValueError):
            expected_tests(self.temp.name, 'TheVeil')


if __name__ == '__main__':
    unittest.main()
