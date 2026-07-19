"""Create or repair the non-map Playable Foundation Editor assets.

The script is intentionally map-safe: it never opens, changes, or saves a level.
Run it through Prepare-PlayableFoundationEditor.ps1 so the manifest, report and
explicit apply guard are supplied consistently.
"""

from __future__ import annotations

import json
import os
import traceback
from typing import Any

import unreal


APPLY_CONFIRMATION = "CREATE_OR_REPAIR_PLAYABLE_FOUNDATION"
EXPECTED_ASSET_PATHS = {
    "/Game/TheVeil/Input/IA_Move",
    "/Game/TheVeil/Input/IA_Look",
    "/Game/TheVeil/Input/IA_Jump",
    "/Game/TheVeil/Input/IA_Sprint",
    "/Game/TheVeil/Input/IA_Crouch",
    "/Game/TheVeil/Input/IMC_Player",
    "/Game/TheVeil/Characters/BP_TheVeilCharacter",
    "/Game/TheVeil/Core/BP_TheVeilPlayerController",
    "/Game/TheVeil/Core/BP_TheVeilGameMode",
}
DEFAULT_MANIFEST = os.path.join(
    unreal.Paths.project_dir(),
    "scripts",
    "PlayableFoundation",
    "playable-foundation-manifest.json",
)
DEFAULT_REPORT = os.path.join(
    unreal.Paths.project_saved_dir(),
    "PlayableFoundation",
    "setup-report.json",
)


def load_json(path: str) -> dict:
    with open(path, "r", encoding="utf-8") as source_file:
        return json.load(source_file)


def write_json(path: str, payload: dict) -> None:
    absolute_path = os.path.abspath(path)
    os.makedirs(os.path.dirname(absolute_path), exist_ok=True)
    with open(absolute_path, "w", encoding="utf-8") as destination_file:
        json.dump(payload, destination_file, indent=2, sort_keys=True)
        destination_file.write("\n")


def split_asset_path(asset_path: str) -> tuple[str, str]:
    package_path, asset_name = asset_path.rsplit("/", 1)
    return package_path, asset_name


def validate_manifest_scope(manifest: dict) -> None:
    if manifest.get("schema_version") != 1:
        raise ValueError("Only Playable Foundation manifest schema version 1 is supported")

    action_paths = [definition["asset"] for definition in manifest["input_actions"]]
    mapping_context = manifest["mapping_context"]
    blueprint_definitions = manifest["blueprints"]
    if set(blueprint_definitions) != {"character", "controller", "game_mode"}:
        raise ValueError("Manifest must define exactly character, controller and game_mode Blueprints")

    declared_paths = set(action_paths)
    declared_paths.add(mapping_context["asset"])
    declared_paths.update(
        definition["asset"] for definition in blueprint_definitions.values()
    )
    if declared_paths != EXPECTED_ASSET_PATHS:
        raise PermissionError(
            "Manifest asset scope differs from the nine authorised Playable Foundation paths"
        )
    if len(action_paths) != 5 or len(set(action_paths)) != 5:
        raise ValueError("Manifest must define five unique Input Actions")
    if len(mapping_context["mappings"]) != 13:
        raise ValueError("Manifest must define exactly thirteen player mappings")
    if any(
        mapping["action"] not in set(action_paths)
        for mapping in mapping_context["mappings"]
    ):
        raise PermissionError("Every mapping must reference a manifest-owned Input Action")
    if manifest["map"]["asset"] != "/Game/Maps/L_Dev_Sandbox":
        raise PermissionError("Only L_Dev_Sandbox may be inspected by this workflow")


def object_identity(value: Any) -> Any:
    if value is None:
        return None
    if hasattr(value, "get_path_name"):
        return value.get_path_name()
    return str(value)


class Setup:
    def __init__(self, manifest: dict, mode: str) -> None:
        self.manifest = manifest
        self.mode = mode
        self.asset_tools = unreal.AssetToolsHelpers.get_asset_tools()
        self.report = {
            "schema_version": 1,
            "engine_version": unreal.SystemLibrary.get_engine_version(),
            "mode": mode,
            "status": "running",
            "operations": [],
            "errors": [],
            "map_mutated": False,
            "content_persisted": mode == "apply",
            "human_remaining": manifest.get("human_only", []),
        }

    @property
    def applying(self) -> bool:
        return self.mode in {"apply", "probe-apply"}

    @property
    def persisting(self) -> bool:
        return self.mode == "apply"

    def record(self, asset_path: str, operation: str, detail: str) -> None:
        self.report["operations"].append(
            {"asset": asset_path, "operation": operation, "detail": detail}
        )
        unreal.log(f"TV_PLAYABLE_SETUP {operation}: {asset_path} - {detail}")

    def load_asset(self, asset_path: str) -> Any:
        if not unreal.EditorAssetLibrary.does_asset_exist(asset_path):
            return None
        return unreal.EditorAssetLibrary.load_asset(asset_path)

    def save_asset(self, asset: Any) -> None:
        if not self.persisting:
            return
        if not unreal.EditorAssetLibrary.save_loaded_asset(asset):
            raise RuntimeError(f"Unreal failed to save {asset.get_path_name()}")

    def create_data_asset(self, asset_path: str, asset_type: type) -> Any:
        package_path, asset_name = split_asset_path(asset_path)
        factory = unreal.DataAssetFactory()
        factory.set_editor_property("data_asset_class", asset_type)
        asset = self.asset_tools.create_asset(
            asset_name,
            package_path,
            asset_type,
            factory,
        )
        if asset is None:
            raise RuntimeError(f"Unreal failed to create {asset_path}")
        return asset

    def create_blueprint(self, asset_path: str, parent_class_path: str) -> Any:
        parent_class = unreal.load_class(None, parent_class_path)
        if parent_class is None:
            raise RuntimeError(f"Native parent class is unavailable: {parent_class_path}")

        package_path, asset_name = split_asset_path(asset_path)
        factory = unreal.BlueprintFactory()
        factory.set_editor_property("parent_class", parent_class)
        blueprint = self.asset_tools.create_asset(
            asset_name,
            package_path,
            unreal.Blueprint,
            factory,
        )
        if blueprint is None:
            raise RuntimeError(f"Unreal failed to create {asset_path}")
        self.compile_blueprint(blueprint)
        self.save_asset(blueprint)
        return blueprint

    @staticmethod
    def compile_blueprint(blueprint: Any) -> None:
        library = getattr(unreal, "BlueprintEditorLibrary", None)
        if library is not None:
            library.compile_blueprint(blueprint)

    def ensure_input_action(self, definition: dict) -> Any:
        asset_path = definition["asset"]
        action = self.load_asset(asset_path)

        if action is None:
            if not self.applying:
                self.record(asset_path, "would_create", "Input Action")
                return None
            action = self.create_data_asset(asset_path, unreal.InputAction)
            self.record(asset_path, "created", "Input Action")
        elif not isinstance(action, unreal.InputAction):
            raise TypeError(f"{asset_path} exists but is not an Input Action")

        if not self.applying:
            self.record(asset_path, "would_repair", "value and accumulation defaults")
            return action

        value_type = getattr(unreal.InputActionValueType, definition["value_type"])
        accumulation = getattr(
            unreal.InputActionAccumulationBehavior,
            definition["accumulation_behavior"],
        )
        action.modify()
        action.set_editor_property("value_type", value_type)
        action.set_editor_property("accumulation_behavior", accumulation)
        self.save_asset(action)
        self.record(asset_path, "repaired", "value and accumulation defaults")
        return action

    def create_modifier(self, context: Any, definition: dict) -> Any:
        modifier_type = definition["type"]
        if modifier_type == "negate":
            return unreal.new_object(unreal.InputModifierNegate, outer=context)
        if modifier_type == "swizzle":
            modifier = unreal.new_object(unreal.InputModifierSwizzleAxis, outer=context)
            modifier.set_editor_property(
                "order", getattr(unreal.InputAxisSwizzle, definition["order"])
            )
            return modifier
        if modifier_type == "dead_zone":
            modifier = unreal.new_object(unreal.InputModifierDeadZone, outer=context)
            modifier.set_editor_property(
                "type", getattr(unreal.DeadZoneType, definition["mode"])
            )
            modifier.set_editor_property(
                "lower_threshold", float(definition["lower_threshold"])
            )
            modifier.set_editor_property(
                "upper_threshold", float(definition["upper_threshold"])
            )
            return modifier
        if modifier_type == "scalar":
            modifier = unreal.new_object(unreal.InputModifierScalar, outer=context)
            modifier.set_editor_property(
                "scalar",
                unreal.Vector(
                    float(definition["x"]),
                    float(definition["y"]),
                    float(definition["z"]),
                ),
            )
            return modifier
        raise ValueError(f"Unsupported input modifier type: {modifier_type}")

    def ensure_mapping_context(self, definition: dict) -> Any:
        asset_path = definition["asset"]
        context = self.load_asset(asset_path)

        if context is None:
            if not self.applying:
                self.record(asset_path, "would_create", "Input Mapping Context")
                return None
            context = self.create_data_asset(asset_path, unreal.InputMappingContext)
            self.record(asset_path, "created", "Input Mapping Context")
        elif not isinstance(context, unreal.InputMappingContext):
            raise TypeError(f"{asset_path} exists but is not an Input Mapping Context")

        if not self.applying:
            self.record(
                asset_path,
                "would_replace_mappings",
                f"exact manifest set ({len(definition['mappings'])} mappings)",
            )
            return context

        context.modify()
        context.unmap_all()
        for mapping_definition in definition["mappings"]:
            action = self.load_asset(mapping_definition["action"])
            if not isinstance(action, unreal.InputAction):
                raise RuntimeError(
                    f"Required Input Action is unavailable: {mapping_definition['action']}"
                )

            key = unreal.Key()
            if not key.import_text(mapping_definition["key"]):
                raise ValueError(f"Unreal rejected input key {mapping_definition['key']}")

            mapping = context.map_key(action, key)
            modifiers = [
                self.create_modifier(context, modifier_definition)
                for modifier_definition in mapping_definition.get("modifiers", [])
            ]
            mapping.set_editor_property("modifiers", modifiers)

        self.save_asset(context)
        self.record(
            asset_path,
            "replaced_mappings",
            f"exact manifest set ({len(definition['mappings'])} mappings)",
        )
        return context

    def ensure_blueprint(self, label: str, definition: dict) -> Any:
        asset_path = definition["asset"]
        blueprint = self.load_asset(asset_path)
        if blueprint is None:
            if not self.applying:
                self.record(
                    asset_path,
                    "would_create",
                    f"Blueprint derived from {definition['parent_class']}",
                )
                return None
            blueprint = self.create_blueprint(asset_path, definition["parent_class"])
            self.record(
                asset_path,
                "created",
                f"Blueprint derived from {definition['parent_class']}",
            )
        elif not isinstance(blueprint, unreal.Blueprint):
            raise TypeError(f"{asset_path} exists but is not a Blueprint")

        generated_class = (
            unreal.EditorAssetLibrary.load_blueprint_class(asset_path)
            if unreal.EditorAssetLibrary.does_asset_exist(asset_path)
            else None
        )
        parent_class = unreal.load_class(None, definition["parent_class"])
        if generated_class is None or parent_class is None:
            if self.applying:
                raise RuntimeError(f"Could not load generated or parent class for {asset_path}")
            return blueprint
        actual_parent_class = blueprint.get_blueprint_parent_class()
        if object_identity(actual_parent_class) != object_identity(parent_class):
            raise TypeError(
                f"{asset_path} does not derive from {definition['parent_class']}"
            )

        if not self.applying:
            self.record(asset_path, "would_repair", f"{label} class defaults")
            return blueprint

        default_object = unreal.get_default_object(generated_class)
        default_object.modify()

        if label == "character":
            default_object.set_editor_property(
                "auto_possess_player", unreal.AutoReceiveInput.DISABLED
            )
        elif label == "controller":
            for property_name, target_path in definition.get("defaults", {}).items():
                target = self.load_asset(target_path)
                if target is None:
                    raise RuntimeError(f"Required controller asset is unavailable: {target_path}")
                default_object.set_editor_property(property_name, target)
        elif label == "game_mode":
            for property_name, target_path in definition.get("defaults", {}).items():
                if target_path.startswith("/Game/"):
                    target = unreal.EditorAssetLibrary.load_blueprint_class(target_path)
                else:
                    target = unreal.load_class(None, target_path)
                if target is None:
                    raise RuntimeError(f"Required GameMode class is unavailable: {target_path}")
                default_object.set_editor_property(property_name, target)
        else:
            raise ValueError(f"Unsupported Blueprint label: {label}")

        blueprint.modify()
        self.compile_blueprint(blueprint)
        self.save_asset(blueprint)
        self.record(asset_path, "repaired", f"{label} class defaults")
        return blueprint

    def run(self) -> dict:
        for action_definition in self.manifest["input_actions"]:
            self.ensure_input_action(action_definition)

        self.ensure_mapping_context(self.manifest["mapping_context"])

        blueprint_definitions = self.manifest["blueprints"]
        for label in ("character", "controller", "game_mode"):
            self.ensure_blueprint(label, blueprint_definitions[label])

        if self.mode == "dry-run":
            self.report["status"] = "dry_run_complete"
        elif self.mode == "probe-apply":
            self.report["status"] = "probe_applied_in_memory"
        else:
            self.report["status"] = "applied"
        return self.report


def main() -> None:
    mode = os.environ.get("TV_PLAYABLE_MODE", "dry-run").strip().lower()
    if mode not in {"dry-run", "apply", "probe-apply"}:
        raise ValueError(
            "TV_PLAYABLE_MODE must be 'dry-run', 'apply' or internal 'probe-apply'"
        )
    if mode == "apply" and os.environ.get("TV_PLAYABLE_CONFIRM") != APPLY_CONFIRMATION:
        raise PermissionError(
            "Apply mode requires TV_PLAYABLE_CONFIRM=CREATE_OR_REPAIR_PLAYABLE_FOUNDATION"
        )

    manifest_path = os.environ.get("TV_PLAYABLE_MANIFEST", DEFAULT_MANIFEST)
    report_path = os.environ.get("TV_PLAYABLE_REPORT", DEFAULT_REPORT)
    manifest = load_json(manifest_path)
    validate_manifest_scope(manifest)
    setup = Setup(manifest, mode)

    try:
        report = setup.run()
    except Exception as error:
        setup.report["status"] = "failed"
        setup.report["errors"].append(str(error))
        write_json(report_path, setup.report)
        raise

    write_json(report_path, report)
    unreal.log(f"TV_PLAYABLE_SETUP_REPORT={os.path.abspath(report_path)}")


try:
    main()
except Exception:  # pragma: no cover - last-resort Unreal logging
    unreal.log_error(traceback.format_exc())
    raise
