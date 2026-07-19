"""Focused regression tests for the shared Playable Foundation manifest contract."""

from __future__ import annotations

import copy
import json
import pathlib
import unittest

from playable_foundation_contract import (
    blueprint_status_is_current,
    validate_manifest_contract,
)


MANIFEST_PATH = pathlib.Path(__file__).with_name("playable-foundation-manifest.json")


class PlayableFoundationContractTests(unittest.TestCase):
    def setUp(self) -> None:
        self.manifest = json.loads(MANIFEST_PATH.read_text(encoding="utf-8"))

    def test_checked_manifest_is_accepted(self) -> None:
        validate_manifest_contract(self.manifest)

    def test_weakened_mapping_set_is_rejected(self) -> None:
        weakened = copy.deepcopy(self.manifest)
        weakened["mapping_context"]["mappings"].pop()
        with self.assertRaisesRegex(ValueError, "thirteen"):
            validate_manifest_contract(weakened)

    def test_mapping_must_reference_owned_action(self) -> None:
        weakened = copy.deepcopy(self.manifest)
        weakened["mapping_context"]["mappings"][0]["action"] = "/Game/Other/IA_Move"
        with self.assertRaisesRegex(PermissionError, "manifest-owned"):
            validate_manifest_contract(weakened)

    def test_map_scope_is_exact(self) -> None:
        weakened = copy.deepcopy(self.manifest)
        weakened["map"]["player_start_count"] = 2
        with self.assertRaisesRegex(PermissionError, "exactly one PlayerStart"):
            validate_manifest_contract(weakened)

    def test_only_current_blueprint_statuses_are_accepted(self) -> None:
        self.assertTrue(blueprint_status_is_current("BlueprintStatus.UP_TO_DATE"))
        self.assertTrue(
            blueprint_status_is_current("BlueprintStatus.UP_TO_DATE_WITH_WARNINGS")
        )
        self.assertFalse(blueprint_status_is_current("BlueprintStatus.ERROR"))
        self.assertFalse(blueprint_status_is_current("BlueprintStatus.DIRTY"))


if __name__ == "__main__":
    unittest.main()
