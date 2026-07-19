"""Read-only UE 5.8 API capability probe for playable-foundation tooling."""

from __future__ import annotations

import json
import os
import traceback

import unreal


REPORT_PATH = os.environ.get(
    "TV_PLAYABLE_REPORT",
    os.path.join(unreal.Paths.project_saved_dir(), "PlayableFoundation", "api-probe.json"),
)


def describe_type(type_name: str) -> dict:
    unreal_type = getattr(unreal, type_name, None)
    if unreal_type is None:
        return {"available": False}

    description = {
        "available": True,
        "module": getattr(unreal_type, "__module__", None),
        "attributes": sorted(
            name
            for name in dir(unreal_type)
            if not name.startswith("_")
        ),
    }
    return description


def main() -> None:
    type_names = [
        "AssetToolsHelpers",
        "BlueprintFactory",
        "Blueprint",
        "Class",
        "DataAssetFactory",
        "EditorAssetLibrary",
        "EditorLevelLibrary",
        "EditorLoadingAndSavingUtils",
        "EnhancedActionKeyMapping",
        "InputAction",
        "InputActionFactory",
        "InputMappingContext",
        "InputMappingContextFactory",
        "InputModifierDeadZone",
        "InputModifierNegate",
        "InputModifierScalar",
        "InputModifierSwizzleAxis",
        "InputActionAccumulationBehavior",
        "InputActionValueType",
        "InputAxisSwizzle",
        "DeadZoneType",
        "Key",
        "KismetEditorUtilities",
        "LevelEditorSubsystem",
        "SubobjectDataSubsystem",
    ]

    native_classes = {
        "character": "/Script/TheVeil.TheVeilCharacter",
        "controller": "/Script/TheVeil.TheVeilPlayerController",
        "game_mode": "/Script/TheVeil.TheVeilGameMode",
    }

    report = {
        "schema_version": 1,
        "engine_version": unreal.SystemLibrary.get_engine_version(),
        "project_dir": unreal.Paths.project_dir(),
        "types": {name: describe_type(name) for name in type_names},
        "native_classes": {},
        "errors": [],
        "callable_docs": {},
        "transient_checks": {},
    }

    callables = {
        "asset_tools.create_asset": unreal.AssetToolsHelpers.get_asset_tools().create_asset,
        "input_mapping_context.map_key": unreal.InputMappingContext.map_key,
        "new_object": unreal.new_object,
        "key.import_text": unreal.Key.import_text,
    }
    report["callable_docs"] = {
        name: getattr(callable_object, "__doc__", None)
        for name, callable_object in callables.items()
    }

    try:
        data_asset_factory = unreal.DataAssetFactory()
        report["transient_checks"]["data_asset_factory"] = {
            "available": True,
            "data_asset_class": str(data_asset_factory.get_editor_property("data_asset_class")),
        }
    except Exception as error:
        report["transient_checks"]["data_asset_factory"] = {
            "available": False,
            "error": str(error),
        }

    try:
        blueprint_factory = unreal.BlueprintFactory()
        blueprint_factory.set_editor_property(
            "parent_class",
            unreal.load_class(None, native_classes["character"]),
        )
        report["transient_checks"]["blueprint_factory"] = {
            "available": True,
            "parent_class": str(blueprint_factory.get_editor_property("parent_class")),
        }
    except Exception as error:
        report["transient_checks"]["blueprint_factory"] = {
            "available": False,
            "error": str(error),
        }

    try:
        transient_context = unreal.InputMappingContext()
        transient_action = unreal.InputAction()
        transient_action.set_editor_property(
            "value_type",
            unreal.InputActionValueType.AXIS2D,
        )
        key = unreal.Key()
        key_imported = key.import_text("W")
        mapping = transient_context.map_key(transient_action, key)
        negate = unreal.new_object(unreal.InputModifierNegate, outer=transient_context)
        swizzle = unreal.new_object(unreal.InputModifierSwizzleAxis, outer=transient_context)
        swizzle.set_editor_property("order", unreal.InputAxisSwizzle.YXZ)
        mapping.set_editor_property("modifiers", [negate, swizzle])
        default_mappings = transient_context.get_editor_property("default_key_mappings")
        stored_mappings = default_mappings.get_editor_property("mappings")
        report["transient_checks"]["enhanced_input"] = {
            "available": True,
            "key_imported": key_imported,
            "action_value_type": str(transient_action.get_editor_property("value_type")),
            "key": mapping.get_editor_property("key").export_text(),
            "key_dict": {
                str(field_name): str(field_value)
                for field_name, field_value in mapping.get_editor_property("key").to_dict().items()
            },
            "key_tuple": [
                str(field_value)
                for field_value in mapping.get_editor_property("key").to_tuple()
            ],
            "modifier_classes": [
                modifier.get_class().get_name()
                for modifier in mapping.get_editor_property("modifiers")
            ],
            "mapping_count": len(stored_mappings),
        }
    except Exception as error:
        report["transient_checks"]["enhanced_input"] = {
            "available": False,
            "error": str(error),
        }

    for label, class_path in native_classes.items():
        try:
            loaded_class = unreal.load_class(None, class_path)
            report["native_classes"][label] = {
                "path": class_path,
                "available": loaded_class is not None,
                "name": loaded_class.get_name() if loaded_class else None,
                "attributes": (
                    sorted(name for name in dir(loaded_class) if not name.startswith("_"))
                    if loaded_class
                    else []
                ),
            }
        except Exception as error:  # pragma: no cover - exercised inside Unreal
            report["native_classes"][label] = {
                "path": class_path,
                "available": False,
                "error": str(error),
            }

    os.makedirs(os.path.dirname(os.path.abspath(REPORT_PATH)), exist_ok=True)
    with open(REPORT_PATH, "w", encoding="utf-8") as report_file:
        json.dump(report, report_file, indent=2, sort_keys=True)
        report_file.write("\n")

    unreal.log(f"TV_PLAYABLE_API_PROBE={os.path.abspath(REPORT_PATH)}")


try:
    main()
except Exception:  # pragma: no cover - last-resort Unreal logging
    unreal.log_error(traceback.format_exc())
    raise
