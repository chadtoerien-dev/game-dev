"""Fail-closed validation of The Veil's UE 5.8 automation export (Python 3)."""
import argparse
import datetime as dt
import json
from pathlib import Path
import re
import sys


def expected_tests(source_root, test_filter):
    names = []
    for path in Path(source_root).rglob('*.cpp'):
        text = path.read_text(encoding='utf-8-sig')
        # Preserve quoted strings while discarding commented-out registrations.
        text = re.sub(r'"(?:\\.|[^"\\])*"|//[^\n]*|/\*[\s\S]*?\*/',
                      lambda m: m[0] if m[0].startswith('"') else ' ', text)
        registrations = re.findall(r'\bIMPLEMENT_\w*AUTOMATION_TEST\s*\(', text)
        matches = re.findall(r'\bIMPLEMENT_SIMPLE_AUTOMATION_TEST\s*\(\s*\w+\s*,\s*"([^"]+)"', text)
        if len(registrations) != len(matches):
            raise ValueError(f'Unsupported automation registration in {path}; extend discovery explicitly.')
        names.extend(matches)
    if not names or len(names) != len(set(names)):
        raise ValueError('Source registrations are empty or duplicated.')
    expected = set()
    for term in test_filter.split('+'):
        term = term.strip()
        if not term or any(c in term for c in ';\r\n"'):
            raise ValueError('Empty or unsafe test filter.')
        if term.lower().startswith('group:'):
            raise ValueError('Group: filters require configuration-aware discovery and are not supported.')
        start = term.startswith('^')
        end = term.endswith('$')
        if term.lower().startswith('startswith:'):
            term = term[len('StartsWith:'):].strip().rstrip('.') + '.'
            start = True
        else:
            term = term[int(start):len(term) - int(end)]
        if not term:
            raise ValueError('Empty test filter term.')
        pattern = ('^' if start else '') + re.escape(term) + ('$' if end else '')
        matched = {name for name in names if re.search(pattern, name, re.IGNORECASE)}
        if not matched:
            raise ValueError(f'No source tests match requested term: {term}')
        expected.update(matched)
    return expected


def no_duplicate_keys(pairs):
    result = {}
    for key, value in pairs:
        if key in result:
            raise ValueError(f'Duplicate JSON key: {key}')
        result[key] = value
    return result


def validate_report(report_path, expected, started_at):
    path = Path(report_path)
    if not path.is_file():
        raise ValueError(f'Missing report: {path}')
    if path.stat().st_mtime < started_at:
        raise ValueError('Report predates this run.')
    report = json.loads(path.read_text(encoding='utf-8-sig'), object_pairs_hook=no_duplicate_keys)
    if not isinstance(report, dict):
        raise ValueError('Report must be a JSON object.')
    # UE exports UTC timestamps with one-second precision.
    created = dt.datetime.strptime(report['reportCreatedOn'], '%Y.%m.%d-%H.%M.%S').replace(tzinfo=dt.timezone.utc).timestamp()
    if created < int(started_at) or created > dt.datetime.now(dt.timezone.utc).timestamp() + 1:
        raise ValueError('Report creation timestamp is stale or in the future.')
    tests = report.get('tests')
    if not isinstance(tests, list) or not tests:
        raise ValueError('Report contains no tests.')
    for field in ('succeeded', 'succeededWithWarnings', 'failed', 'notRun', 'inProcess'):
        value = report.get(field)
        if type(value) is not int or value < 0:
            raise ValueError(f'Invalid summary count: {field}')
        if value != (len(tests) if field == 'succeeded' else 0):
            raise ValueError(f'Unsuccessful or inconsistent summary: {field}={value}')
    actual = []
    for test in tests:
        name = test['fullTestPath']
        if not isinstance(name, str) or not name:
            raise ValueError('Invalid test name.')
        actual.append(name)
        if test.get('state') != 'Success':
            raise ValueError(f'Test did not succeed: {name}')
        for field in ('warnings', 'errors'):
            if type(test.get(field)) is not int or test[field] != 0:
                raise ValueError(f'Test has invalid/non-zero {field}: {name}')
        entries = test.get('entries')
        if not isinstance(entries, list):
            raise ValueError(f'Missing entries: {name}')
        for entry in entries:
            if entry['event']['type'] != 'Info':
                raise ValueError(f'Non-informational test entry: {name}')
    if len(actual) != len(set(actual)):
        raise ValueError('Duplicate test results.')
    if not expected or set(actual) != set(expected):
        raise ValueError(f'Test coverage mismatch; missing={sorted(set(expected)-set(actual))}, unexpected={sorted(set(actual)-set(expected))}')
    return len(actual)


def main():
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument('--source-root', required=True)
    parser.add_argument('--filter', default='TheVeil')
    parser.add_argument('--report')
    parser.add_argument('--started-at', type=float)
    parser.add_argument('--discover-only', action='store_true')
    args = parser.parse_args()
    try:
        expected = expected_tests(args.source_root, args.filter)
        if args.discover_only:
            print(f'Expected tests: {len(expected)}')
        else:
            if args.report is None or args.started_at is None:
                raise ValueError('--report and --started-at are required for validation.')
            count = validate_report(args.report, expected, args.started_at)
            print(f'Validated {count} successful tests; zero warnings, failures or skips.')
        return 0
    except (ValueError, TypeError, KeyError, OSError) as error:
        print(f'Automation validation failed: {error}', file=sys.stderr)
        return 1


if __name__ == '__main__':
    sys.exit(main())
