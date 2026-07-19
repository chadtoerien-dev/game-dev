"""Create or repair the bounded Playable Foundation Editor integration.

The nine non-map assets are covered by the normal apply guard. The sandbox map
remains read-only unless the separate map apply guard is explicitly supplied by
Prepare-PlayableFoundationEditor.ps1.
"""

from __future__ import annotations

import json
import os
import sys
import traceback
from typing import Any

import unreal

SCRIPT_DIRECTORY = os.path.dirname(os.path.abspath(__file__))
if SCRIPT_DIRECTORY not in sys.path:
    sys.path.insert(0, SCRIPT_DIRECTORY)

from playable_foundation_contract import (  # noqa: E402
    CURRENT_BLUEPRINT_STATUSES,
    EXPECTED_ASSET_PATHS,
    blueprint_status_is_current,
    enum_name,
    validate_manifest_contract,
)


APPLY_CONFIRMATION = "CREATE_OR_REPAIR_PLAYABLE_FOUNDATION"
MAP_APPLY_CONFIRMATION = "SET_PLAYABLE_FOUNDATION_SANDBOX_GAMEMODE"
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


def object_identity(value: Any) -> Any:
    if value is None:
        return None
    if hasattr(value, "get_path_name"):
        return value.get_path_name()
    return str(value)


class Setup:
    def __init__(self, manifest: dict, mode: str, apply_map: bool) -> None:
        self.manifest = manifest
        self.mode = mode
        self.apply_map = apply_map
        self.asset_tools = unreal.AssetToolsHelpers.get_asset_tools()
        self.report = {
            "schema_version": 1,
            "engine_version": unreal.SystemLibrary.get_engine_version(),
            "mode": mode,
            "status": "running",
            "operations": [],
            "errors": [],
            "map_mutated": False,
            "map_apply_requested": apply_map,
            "map_persisted": False,
            "content_persisted": False,
            "saved_content_packages": [],
            "human_remaining": manifest.get("human_only", []),
        }

    @property
    def applying(self) -> bool:
        return self.mode in {"apply", "probe-apply"}

    @property
    def persisting(self) -> bool:
        return self.mode == "apply"

    @property
    def applying_map(self) -> bool:
        return self.persisting and self.apply_map

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
        package_path = asset.get_path_name().split(".", 1)[0]
        if package_path not in self.report["saved_content_packages"]:
            self.report["saved_content_packages"].append(package_path)

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
    def assert_blueprint_current(blueprint: Any) -> None:
        status = blueprint.get_editor_property("status")
        if not blueprint_status_is_current(status):
            raise RuntimeError(
                f"Blueprint {blueprint.get_path_name()} has non-current compiler status "
                f"{enum_name(status)}; expected one of {sorted(CURRENT_BLUEPRINT_STATUSES)}"
            )

    @classmethod
    def compile_blueprint(cls, blueprint: Any) -> None:
        library = getattr(unreal, "BlueprintEditorLibrary", None)
        if library is None:
            raise RuntimeError("BlueprintEditorLibrary is unavailable; compile status cannot be proven")
        library.compile_blueprint(blueprint)
        cls.assert_blueprint_current(blueprint)

    def preflight_apply(self) -> None:
        """Validate every predictable dependency before the first package save."""

        for definition in self.manifest["input_actions"]:
            existing = self.load_asset(definition["asset"])
            if existing is not None and not isinstance(existing, unreal.InputAction):
                raise TypeError(f"{definition['asset']} exists but is not an Input Action")
            getattr(unreal.InputActionValueType, definition["value_type"])
            getattr(
                unreal.InputActionAccumulationBehavior,
                definition["accumulation_behavior"],
            )

        mapping_definition = self.manifest["mapping_context"]
        existing_context = self.load_asset(mapping_definition["asset"])
        if existing_context is not None and not isinstance(
            existing_context, unreal.InputMappingContext
        ):
            raise TypeError(
                f"{mapping_definition['asset']} exists but is not an Input Mapping Context"
            )
        for mapping in mapping_definition["mappings"]:
            key = unreal.Key()
            if not key.import_text(mapping["key"]):
                raise ValueError(f"Unreal rejected input key {mapping['key']}")
            for modifier in mapping.get("modifiers", []):
                modifier_type = modifier["type"]
                if modifier_type == "swizzle":
                    getattr(unreal.InputAxisSwizzle, modifier["order"])
                elif modifier_type == "dead_zone":
                    getattr(unreal.DeadZoneType, modifier["mode"])

        for definition in self.manifest["blueprints"].values():
            parent_class = unreal.load_class(None, definition["parent_class"])
            if parent_class is None:
                raise RuntimeError(
                    f"Native parent class is unavailable: {definition['parent_class']}"
                )
            existing_blueprint = self.load_asset(definition["asset"])
            if existing_blueprint is None:
                continue
            if not isinstance(existing_blueprint, unreal.Blueprint):
                raise TypeError(f"{definition['asset']} exists but is not a Blueprint")
            actual_parent = existing_blueprint.get_blueprint_parent_class()
            if object_identity(actual_parent) != object_identity(parent_class):
                raise TypeError(
                    f"{definition['asset']} does not derive from {definition['parent_class']}"
                )
            self.compile_blueprint(existing_blueprint)

        visual_definition = self.manifest["blueprints"]["character"][
            "placeholder_visual"
        ]
        if not isinstance(unreal.load_asset(visual_definition["mesh"]), unreal.StaticMesh):
            raise RuntimeError(
                f"Placeholder mesh is unavailable: {visual_definition['mesh']}"
            )

        if self.applying_map:
            map_path = self.manifest["map"]["asset"]
            if unreal.EditorLoadingAndSavingUtils.load_map(map_path) is None:
                raise RuntimeError(f"Unreal could not load the authorised map: {map_path}")

        self.record("manifest", "preflight_passed", "all predictable apply dependencies")

    @staticmethod
    def blueprint_subobjects(blueprint: Any) -> list[tuple[Any, str, Any]]:
        subsystem = unreal.get_engine_subsystem(unreal.SubobjectDataSubsystem)
        library = unreal.SubobjectDataBlueprintFunctionLibrary
        snapshots = []
        for handle in subsystem.k2_gather_subobject_data_for_blueprint(blueprint):
            data = library.get_data(handle)
            variable_name = str(library.get_variable_name(data))
            component = library.get_object_for_blueprint(data, blueprint)
            snapshots.append((handle, variable_name, component))
        return snapshots

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
            self.record(
                asset_path,
                "would_repair",
                "value, accumulation and empty action-level trigger/modifier defaults",
            )
            return action

        value_type = getattr(unreal.InputActionValueType, definition["value_type"])
        accumulation = getattr(
            unreal.InputActionAccumulationBehavior,
            definition["accumulation_behavior"],
        )
        action.modify()
        action.set_editor_property("value_type", value_type)
        action.set_editor_property("accumulation_behavior", accumulation)
        action.set_editor_property("triggers", [])
        action.set_editor_property("modifiers", [])
        self.save_asset(action)
        self.record(
            asset_path,
            "repaired",
            "value, accumulation and empty action-level trigger/modifier defaults",
        )
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
        generated_mappings = []
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
            generated_mappings.append(mapping)

        # Python receives MapKey's UStruct return value by copy. Assign the
        # completed mappings back to the reflected container so instanced
        # modifiers are serialised into the Input Mapping Context asset.
        mapping_data = context.get_editor_property("default_key_mappings")
        mapping_data.set_editor_property("mappings", generated_mappings)
        context.set_editor_property("default_key_mappings", mapping_data)

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

        self.assert_blueprint_current(blueprint)

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

    def ensure_character_placeholder_visual(self, definition: dict, blueprint: Any) -> None:
        asset_path = definition["asset"]
        visual = definition["placeholder_visual"]
        component_name = visual["component_name"]

        if not self.applying:
            self.record(
                asset_path,
                "would_repair",
                f"collision-free {component_name} ellipsoid visual",
            )
            return

        subsystem = unreal.get_engine_subsystem(unreal.SubobjectDataSubsystem)
        library = unreal.SubobjectDataBlueprintFunctionLibrary
        subobjects = self.blueprint_subobjects(blueprint)
        parent_matches = [
            (handle, component)
            for handle, variable_name, component in subobjects
            if variable_name == visual["attach_to"]
        ]
        if len(parent_matches) != 1:
            raise RuntimeError(
                f"Expected one {visual['attach_to']} subobject on {asset_path}; "
                f"found {len(parent_matches)}"
            )
        parent_handle, _ = parent_matches[0]

        visual_matches = [
            (handle, component)
            for handle, variable_name, component in subobjects
            if variable_name == component_name
        ]
        if len(visual_matches) > 1:
            raise RuntimeError(f"Found duplicate {component_name} components on {asset_path}")

        if visual_matches:
            visual_handle, component = visual_matches[0]
            if not isinstance(component, unreal.StaticMeshComponent):
                raise TypeError(f"{component_name} exists but is not a StaticMeshComponent")
            if not subsystem.attach_subobject(parent_handle, visual_handle):
                raise RuntimeError(f"Could not attach {component_name} to {visual['attach_to']}")
            operation = "repaired"
        else:
            parameters = unreal.AddNewSubobjectParams(
                parent_handle=parent_handle,
                new_class=unreal.StaticMeshComponent,
                blueprint_context=blueprint,
                conform_transform_to_parent=True,
            )
            visual_handle, failure_reason = subsystem.add_new_subobject(parameters)
            if not library.is_handle_valid(visual_handle):
                raise RuntimeError(
                    f"Could not add {component_name} to {asset_path}: {failure_reason}"
                )
            if not subsystem.rename_subobject(visual_handle, unreal.Text(component_name)):
                raise RuntimeError(f"Could not rename new component to {component_name}")
            self.compile_blueprint(blueprint)

            visual_matches = [
                (handle, component)
                for handle, variable_name, component in self.blueprint_subobjects(blueprint)
                if variable_name == component_name
            ]
            if len(visual_matches) != 1:
                raise RuntimeError(
                    f"Could not resolve newly created {component_name} on {asset_path}"
                )
            _, component = visual_matches[0]
            operation = "created"

        placeholder_mesh = unreal.load_asset(visual["mesh"])
        if not isinstance(placeholder_mesh, unreal.StaticMesh):
            raise RuntimeError(f"Placeholder mesh is unavailable: {visual['mesh']}")

        component.modify()
        current_mesh = component.get_editor_property("static_mesh")
        if (
            object_identity(current_mesh) != object_identity(placeholder_mesh)
            and not component.set_static_mesh(placeholder_mesh)
        ):
            raise RuntimeError(f"Unreal rejected the placeholder mesh on {component_name}")
        component.set_collision_profile_name(unreal.Name(visual["collision_profile"]))
        component.set_editor_property(
            "can_ever_affect_navigation",
            bool(visual["can_ever_affect_navigation"]),
        )
        location = visual["relative_location"]
        component.set_editor_property(
            "relative_location",
            unreal.Vector(
                float(location["x"]),
                float(location["y"]),
                float(location["z"]),
            ),
        )
        rotation = visual["relative_rotation"]
        component.set_editor_property(
            "relative_rotation",
            unreal.Rotator(
                float(rotation["roll"]),
                float(rotation["pitch"]),
                float(rotation["yaw"]),
            ),
        )
        scale = visual["relative_scale"]
        component.set_relative_scale3d(
            unreal.Vector(float(scale[0]), float(scale[1]), float(scale[2]))
        )
        blueprint.modify()
        self.compile_blueprint(blueprint)
        self.save_asset(blueprint)
        self.record(
            asset_path,
            operation,
            f"collision-free ellipsoid {component_name} attached to {visual['attach_to']}",
        )

    def ensure_map_game_mode(self) -> None:
        definition = self.manifest["map"]
        map_path = definition["asset"]
        if not self.apply_map:
            self.record(
                map_path,
                "map_not_requested",
                "pass -ApplyMap to allow the sandbox GameMode override",
            )
            return
        if not self.applying_map:
            self.record(map_path, "would_set", "sandbox GameMode override")
            return

        expected_class = unreal.EditorAssetLibrary.load_blueprint_class(
            definition["game_mode_override"]
        )
        if expected_class is None:
            raise RuntimeError(
                f"Required map GameMode class is unavailable: {definition['game_mode_override']}"
            )

        world = unreal.EditorLoadingAndSavingUtils.load_map(map_path)
        if world is None:
            raise RuntimeError(f"Unreal could not load the authorised map: {map_path}")
        world_settings = world.get_world_settings()
        current_class = world_settings.get_editor_property("default_game_mode")
        if object_identity(current_class) == object_identity(expected_class):
            self.report["map_persisted"] = True
            self.record(map_path, "verified", "sandbox GameMode override already correct")
            return

        world_settings.modify()
        world_settings.set_editor_property("default_game_mode", expected_class)
        if not unreal.EditorLoadingAndSavingUtils.save_map(world, map_path):
            raise RuntimeError(f"Unreal failed to save the authorised map: {map_path}")
        self.report["map_mutated"] = True
        self.report["map_persisted"] = True
        self.record(map_path, "set_and_saved", "sandbox GameMode override")

    def run(self) -> dict:
        if self.applying:
            self.preflight_apply()

        for action_definition in self.manifest["input_actions"]:
            self.ensure_input_action(action_definition)

        self.ensure_mapping_context(self.manifest["mapping_context"])

        blueprint_definitions = self.manifest["blueprints"]
        blueprints = {}
        for label in ("character", "controller", "game_mode"):
            blueprints[label] = self.ensure_blueprint(label, blueprint_definitions[label])

        self.ensure_character_placeholder_visual(
            blueprint_definitions["character"], blueprints["character"]
        )
        self.ensure_map_game_mode()

        if self.mode == "dry-run":
            self.report["status"] = "dry_run_complete"
        elif self.mode == "probe-apply":
            self.report["status"] = "probe_applied_in_memory"
        else:
            self.report["status"] = "applied"
            self.report["content_persisted"] = set(
                self.report["saved_content_packages"]
            ) == set(EXPECTED_ASSET_PATHS)
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
    apply_map = os.environ.get("TV_PLAYABLE_APPLY_MAP") == "1"
    if apply_map and mode != "apply":
        raise PermissionError("Map apply is only available with normal apply mode")
    if apply_map and os.environ.get("TV_PLAYABLE_MAP_CONFIRM") != MAP_APPLY_CONFIRMATION:
        raise PermissionError(
            "Map apply requires "
            "TV_PLAYABLE_MAP_CONFIRM=SET_PLAYABLE_FOUNDATION_SANDBOX_GAMEMODE"
        )

    manifest_path = os.environ.get("TV_PLAYABLE_MANIFEST", DEFAULT_MANIFEST)
    report_path = os.environ.get("TV_PLAYABLE_REPORT", DEFAULT_REPORT)
    manifest = load_json(manifest_path)
    validate_manifest_contract(manifest)
    setup = Setup(manifest, mode, apply_map)

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
