"""Read-only structural validation for the Playable Foundation Editor integration."""

from __future__ import annotations

import json
import os
import traceback
from typing import Any

import unreal


DEFAULT_MANIFEST = os.path.join(
    unreal.Paths.project_dir(),
    "scripts",
    "PlayableFoundation",
    "playable-foundation-manifest.json",
)
DEFAULT_REPORT = os.path.join(
    unreal.Paths.project_saved_dir(),
    "PlayableFoundation",
    "validation-report.json",
)
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


def load_json(path: str) -> dict:
    with open(path, "r", encoding="utf-8") as source_file:
        return json.load(source_file)


def write_json(path: str, payload: dict) -> None:
    absolute_path = os.path.abspath(path)
    os.makedirs(os.path.dirname(absolute_path), exist_ok=True)
    with open(absolute_path, "w", encoding="utf-8") as destination_file:
        json.dump(payload, destination_file, indent=2, sort_keys=True)
        destination_file.write("\n")


def validate_manifest_scope(manifest: dict) -> None:
    if manifest.get("schema_version") != 1:
        raise ValueError("Only Playable Foundation manifest schema version 1 is supported")
    action_paths = [definition["asset"] for definition in manifest["input_actions"]]
    blueprint_definitions = manifest["blueprints"]
    declared_paths = set(action_paths)
    declared_paths.add(manifest["mapping_context"]["asset"])
    declared_paths.update(
        definition["asset"] for definition in blueprint_definitions.values()
    )
    if declared_paths != EXPECTED_ASSET_PATHS:
        raise PermissionError(
            "Manifest asset scope differs from the nine authorised Playable Foundation paths"
        )
    if manifest["map"]["asset"] != "/Game/Maps/L_Dev_Sandbox":
        raise PermissionError("Only L_Dev_Sandbox may be inspected by this validator")

    placeholder = blueprint_definitions["character"].get("placeholder_visual", {})
    if placeholder != {
        "component_name": "PlaceholderVisual",
        "component_class": "/Script/Engine.StaticMeshComponent",
        "attach_to": "CapsuleComponent",
        "mesh": "/Engine/BasicShapes/Sphere.Sphere",
        "relative_location": {"x": 0.0, "y": 0.0, "z": 0.0},
        "relative_rotation": {"pitch": 0.0, "yaw": 0.0, "roll": 0.0},
        "relative_scale": [0.84, 0.84, 1.92],
        "collision_profile": "NoCollision",
        "can_ever_affect_navigation": False,
    }:
        raise PermissionError("Character placeholder visual differs from the authorised contract")


def object_path_for_asset(asset_path: str) -> str:
    return f"{asset_path}.{asset_path.rsplit('/', 1)[-1]}"


def class_path_for_blueprint(asset_path: str) -> str:
    asset_name = asset_path.rsplit("/", 1)[-1]
    return f"{asset_path}.{asset_name}_C"


def identity(value: Any) -> Any:
    if value is None:
        return None
    if hasattr(value, "get_path_name"):
        return value.get_path_name()
    return str(value)


def enum_name(value: Any) -> str:
    return getattr(value, "name", str(value).split(".")[-1].split(":")[0].strip(" <>"))


def load_asset_if_present(asset_path: str) -> Any:
    if not unreal.EditorAssetLibrary.does_asset_exist(asset_path):
        return None
    return unreal.EditorAssetLibrary.load_asset(asset_path)


class Validator:
    def __init__(self, manifest: dict) -> None:
        self.manifest = manifest
        self.dirty_maps_before = {
            identity(package)
            for package in unreal.EditorLoadingAndSavingUtils.get_dirty_map_packages()
        }
        self.dirty_content_before = {
            identity(package)
            for package in unreal.EditorLoadingAndSavingUtils.get_dirty_content_packages()
        }
        self.report = {
            "schema_version": 1,
            "engine_version": unreal.SystemLibrary.get_engine_version(),
            "automated_status": "running",
            "acceptance_status": "human_required",
            "checks": [],
            "failures": [],
            "manual_checks": manifest.get("human_only", []),
            "map_mutated": False,
            "content_mutated": False,
        }

    def check(
        self,
        check_id: str,
        passed: bool,
        expected: Any,
        actual: Any,
        detail: str = "",
    ) -> None:
        result = {
            "id": check_id,
            "status": "pass" if passed else "fail",
            "expected": expected,
            "actual": actual,
            "detail": detail,
        }
        self.report["checks"].append(result)
        if not passed:
            self.report["failures"].append(result)

    def validate_input_actions(self) -> None:
        for definition in self.manifest["input_actions"]:
            asset_path = definition["asset"]
            action = load_asset_if_present(asset_path)
            self.check(
                f"input_action.exists:{asset_path}",
                isinstance(action, unreal.InputAction),
                "InputAction",
                action.get_class().get_name() if action else None,
            )
            if not isinstance(action, unreal.InputAction):
                continue

            actual_value_type = enum_name(action.get_editor_property("value_type"))
            self.check(
                f"input_action.value_type:{asset_path}",
                actual_value_type == definition["value_type"],
                definition["value_type"],
                actual_value_type,
            )
            actual_accumulation = enum_name(
                action.get_editor_property("accumulation_behavior")
            )
            self.check(
                f"input_action.accumulation:{asset_path}",
                actual_accumulation == definition["accumulation_behavior"],
                definition["accumulation_behavior"],
                actual_accumulation,
            )
            triggers = action.get_editor_property("triggers")
            modifiers = action.get_editor_property("modifiers")
            self.check(
                f"input_action.action_level_triggers:{asset_path}",
                len(triggers) == 0,
                0,
                len(triggers),
            )
            self.check(
                f"input_action.action_level_modifiers:{asset_path}",
                len(modifiers) == 0,
                0,
                len(modifiers),
            )

    @staticmethod
    def modifier_snapshot(modifier: Any) -> dict:
        if isinstance(modifier, unreal.InputModifierNegate):
            return {"type": "negate"}
        if isinstance(modifier, unreal.InputModifierSwizzleAxis):
            return {
                "type": "swizzle",
                "order": enum_name(modifier.get_editor_property("order")),
            }
        if isinstance(modifier, unreal.InputModifierDeadZone):
            return {
                "type": "dead_zone",
                "mode": enum_name(modifier.get_editor_property("type")),
                "lower_threshold": round(
                    float(modifier.get_editor_property("lower_threshold")), 4
                ),
                "upper_threshold": round(
                    float(modifier.get_editor_property("upper_threshold")), 4
                ),
            }
        if isinstance(modifier, unreal.InputModifierScalar):
            scalar = modifier.get_editor_property("scalar")
            return {
                "type": "scalar",
                "x": round(float(scalar.x), 4),
                "y": round(float(scalar.y), 4),
                "z": round(float(scalar.z), 4),
            }
        return {"type": modifier.get_class().get_name()}

    def validate_mapping_context(self) -> None:
        definition = self.manifest["mapping_context"]
        asset_path = definition["asset"]
        context = load_asset_if_present(asset_path)
        self.check(
            f"mapping_context.exists:{asset_path}",
            isinstance(context, unreal.InputMappingContext),
            "InputMappingContext",
            context.get_class().get_name() if context else None,
        )
        if not isinstance(context, unreal.InputMappingContext):
            return

        mapping_data = context.get_editor_property("default_key_mappings")
        actual_mappings = []
        for mapping in mapping_data.get_editor_property("mappings"):
            action = mapping.get_editor_property("action")
            key = mapping.get_editor_property("key")
            actual_mappings.append(
                {
                    "action": (
                        action.get_path_name().split(".", 1)[0] if action else None
                    ),
                    "key": key.export_text(),
                    "modifiers": [
                        self.modifier_snapshot(modifier)
                        for modifier in mapping.get_editor_property("modifiers")
                    ],
                }
            )

        expected_mappings = definition["mappings"]
        self.check(
            f"mapping_context.exact_mappings:{asset_path}",
            actual_mappings == expected_mappings,
            expected_mappings,
            actual_mappings,
            "Order, actions, keys and modifiers must match the manifest exactly.",
        )

    def validate_blueprint_parent(self, label: str, definition: dict) -> Any:
        asset_path = definition["asset"]
        blueprint = load_asset_if_present(asset_path)
        generated_class = (
            unreal.EditorAssetLibrary.load_blueprint_class(asset_path)
            if blueprint is not None
            else None
        )
        parent_class = unreal.load_class(None, definition["parent_class"])
        exists = isinstance(blueprint, unreal.Blueprint) and generated_class is not None
        self.check(
            f"blueprint.exists:{asset_path}",
            exists,
            "Blueprint with generated class",
            blueprint.get_class().get_name() if blueprint else None,
        )
        if not exists or parent_class is None:
            return None

        actual_parent_class = blueprint.get_blueprint_parent_class()
        derives = identity(actual_parent_class) == identity(parent_class)
        self.check(
            f"blueprint.parent:{asset_path}",
            derives,
            definition["parent_class"],
            identity(actual_parent_class),
        )
        return generated_class if derives else None

    @staticmethod
    def blueprint_subobjects(blueprint: Any) -> list[tuple[Any, str, Any, str | None]]:
        subsystem = unreal.get_engine_subsystem(unreal.SubobjectDataSubsystem)
        library = unreal.SubobjectDataBlueprintFunctionLibrary
        snapshots = []
        for handle in subsystem.k2_gather_subobject_data_for_blueprint(blueprint):
            data = library.get_data(handle)
            parent_handle = library.get_parent_handle(data)
            parent_name = None
            if library.is_handle_valid(parent_handle):
                parent_data = library.get_data(parent_handle)
                parent_name = str(library.get_variable_name(parent_data))
            snapshots.append(
                (
                    handle,
                    str(library.get_variable_name(data)),
                    library.get_object_for_blueprint(data, blueprint),
                    parent_name,
                )
            )
        return snapshots

    def validate_character_placeholder(self, definition: dict) -> None:
        blueprint = load_asset_if_present(definition["asset"])
        if not isinstance(blueprint, unreal.Blueprint):
            return

        visual = definition["placeholder_visual"]
        matches = [
            (component, parent_name)
            for _, variable_name, component, parent_name in self.blueprint_subobjects(blueprint)
            if variable_name == visual["component_name"]
        ]
        self.check(
            "blueprint.character.placeholder_visual.count",
            len(matches) == 1,
            1,
            len(matches),
        )
        if len(matches) != 1:
            return

        component, parent_name = matches[0]
        self.check(
            "blueprint.character.placeholder_visual.class",
            isinstance(component, unreal.StaticMeshComponent),
            visual["component_class"],
            identity(component.get_class()) if component else None,
        )
        if not isinstance(component, unreal.StaticMeshComponent):
            return

        self.check(
            "blueprint.character.placeholder_visual.parent",
            parent_name == visual["attach_to"],
            visual["attach_to"],
            parent_name,
        )
        mesh = component.get_editor_property("static_mesh")
        self.check(
            "blueprint.character.placeholder_visual.mesh",
            identity(mesh) == visual["mesh"],
            visual["mesh"],
            identity(mesh),
        )
        collision_profile = str(component.get_collision_profile_name())
        self.check(
            "blueprint.character.placeholder_visual.collision_profile",
            collision_profile == visual["collision_profile"],
            visual["collision_profile"],
            collision_profile,
        )
        affects_navigation = bool(
            component.get_editor_property("can_ever_affect_navigation")
        )
        self.check(
            "blueprint.character.placeholder_visual.can_ever_affect_navigation",
            affects_navigation == bool(visual["can_ever_affect_navigation"]),
            bool(visual["can_ever_affect_navigation"]),
            affects_navigation,
        )
        location = component.get_editor_property("relative_location")
        actual_location = {
            "x": round(float(location.x), 4),
            "y": round(float(location.y), 4),
            "z": round(float(location.z), 4),
        }
        expected_location = {
            key: round(float(value), 4)
            for key, value in visual["relative_location"].items()
        }
        self.check(
            "blueprint.character.placeholder_visual.relative_location",
            actual_location == expected_location,
            expected_location,
            actual_location,
        )
        rotation = component.get_editor_property("relative_rotation")
        actual_rotation = {
            "pitch": round(float(rotation.pitch), 4),
            "yaw": round(float(rotation.yaw), 4),
            "roll": round(float(rotation.roll), 4),
        }
        expected_rotation = {
            key: round(float(value), 4)
            for key, value in visual["relative_rotation"].items()
        }
        self.check(
            "blueprint.character.placeholder_visual.relative_rotation",
            actual_rotation == expected_rotation,
            expected_rotation,
            actual_rotation,
        )
        scale = component.get_editor_property("relative_scale3d")
        actual_scale = [
            round(float(scale.x), 4),
            round(float(scale.y), 4),
            round(float(scale.z), 4),
        ]
        expected_scale = [round(float(value), 4) for value in visual["relative_scale"]]
        self.check(
            "blueprint.character.placeholder_visual.relative_scale",
            actual_scale == expected_scale,
            expected_scale,
            actual_scale,
        )

    def validate_blueprints(self) -> None:
        definitions = self.manifest["blueprints"]
        generated_classes = {
            label: self.validate_blueprint_parent(label, definition)
            for label, definition in definitions.items()
        }

        character_class = generated_classes.get("character")
        if character_class is not None:
            character_default = unreal.get_default_object(character_class)
            auto_possess = enum_name(
                character_default.get_editor_property("auto_possess_player")
            )
            self.check(
                "blueprint.character.auto_possess_player",
                auto_possess == "DISABLED",
                "DISABLED",
                auto_possess,
            )
            self.validate_character_placeholder(definitions["character"])

        controller_class = generated_classes.get("controller")
        if controller_class is not None:
            controller_default = unreal.get_default_object(controller_class)
            for property_name, target_path in definitions["controller"][
                "defaults"
            ].items():
                actual_path = identity(
                    controller_default.get_editor_property(property_name)
                )
                expected_path = object_path_for_asset(target_path)
                self.check(
                    f"blueprint.controller.{property_name}",
                    actual_path == expected_path,
                    expected_path,
                    actual_path,
                )

        game_mode_class = generated_classes.get("game_mode")
        if game_mode_class is not None:
            game_mode_default = unreal.get_default_object(game_mode_class)
            for property_name, target_path in definitions["game_mode"][
                "defaults"
            ].items():
                actual_path = identity(game_mode_default.get_editor_property(property_name))
                expected_path = (
                    class_path_for_blueprint(target_path)
                    if target_path.startswith("/Game/")
                    else target_path
                )
                self.check(
                    f"blueprint.game_mode.{property_name}",
                    actual_path == expected_path,
                    expected_path,
                    actual_path,
                )

    def validate_map(self) -> None:
        definition = self.manifest["map"]
        map_path = definition["asset"]
        world = unreal.EditorLoadingAndSavingUtils.load_map(map_path)
        self.check(
            f"map.loads:{map_path}",
            world is not None,
            "loadable editor world",
            identity(world),
        )
        if world is None:
            return

        world_settings = world.get_world_settings()
        actual_game_mode = identity(
            world_settings.get_editor_property("default_game_mode")
        )
        expected_game_mode = class_path_for_blueprint(
            definition["game_mode_override"]
        )
        self.check(
            "map.game_mode_override",
            actual_game_mode == expected_game_mode,
            expected_game_mode,
            actual_game_mode,
        )

        actors = unreal.EditorLevelLibrary.get_all_level_actors()
        player_starts = [actor for actor in actors if isinstance(actor, unreal.PlayerStart)]
        self.check(
            "map.player_start_count",
            len(player_starts) == int(definition["player_start_count"]),
            int(definition["player_start_count"]),
            len(player_starts),
            "World Partition must have the intended PlayerStart cell loaded for this check.",
        )

        auto_possessed_pawns = []
        for actor in actors:
            if not isinstance(actor, unreal.Pawn):
                continue
            try:
                auto_possess = enum_name(actor.get_editor_property("auto_possess_player"))
            except Exception:
                continue
            if auto_possess != "DISABLED":
                auto_possessed_pawns.append(actor.get_path_name())
        self.check(
            "map.placed_auto_possessed_pawn_count",
            len(auto_possessed_pawns)
            == int(definition["placed_auto_possessed_pawn_count"]),
            int(definition["placed_auto_possessed_pawn_count"]),
            len(auto_possessed_pawns),
            ", ".join(auto_possessed_pawns),
        )

        for player_start in player_starts:
            location = player_start.get_actor_location()
            self.report["checks"].append(
                {
                    "id": f"map.player_start_location:{player_start.get_name()}",
                    "status": "human",
                    "expected": "walkable ground, full capsule clearance, no BADsize",
                    "actual": {
                        "x": round(float(location.x), 2),
                        "y": round(float(location.y), 2),
                        "z": round(float(location.z), 2),
                    },
                    "detail": "Placement quality requires an Editor viewport and PIE judgement.",
                }
            )

    def run(self) -> dict:
        self.validate_input_actions()
        self.validate_mapping_context()
        self.validate_blueprints()
        self.validate_map()
        self.report["automated_status"] = (
            "pass" if not self.report["failures"] else "incomplete"
        )
        dirty_maps_after = {
            identity(package)
            for package in unreal.EditorLoadingAndSavingUtils.get_dirty_map_packages()
        }
        dirty_content_after = {
            identity(package)
            for package in unreal.EditorLoadingAndSavingUtils.get_dirty_content_packages()
        }
        new_dirty_maps = sorted(dirty_maps_after - self.dirty_maps_before)
        new_dirty_content = sorted(dirty_content_after - self.dirty_content_before)
        self.report["new_dirty_map_packages"] = new_dirty_maps
        self.report["new_dirty_content_packages"] = new_dirty_content
        self.report["map_mutated"] = bool(new_dirty_maps)
        self.report["content_mutated"] = bool(new_dirty_content)
        return self.report


def main() -> None:
    manifest_path = os.environ.get("TV_PLAYABLE_MANIFEST", DEFAULT_MANIFEST)
    report_path = os.environ.get("TV_PLAYABLE_REPORT", DEFAULT_REPORT)
    allow_incomplete = os.environ.get("TV_PLAYABLE_ALLOW_INCOMPLETE") == "1"
    manifest = load_json(manifest_path)
    validate_manifest_scope(manifest)
    report = Validator(manifest).run()
    write_json(report_path, report)
    unreal.log(f"TV_PLAYABLE_VALIDATION_REPORT={os.path.abspath(report_path)}")
    if report["failures"] and not allow_incomplete:
        raise RuntimeError(
            f"Playable Foundation validation has {len(report['failures'])} failure(s)"
        )


try:
    main()
except Exception:  # pragma: no cover - last-resort Unreal logging
    unreal.log_error(traceback.format_exc())
    raise
