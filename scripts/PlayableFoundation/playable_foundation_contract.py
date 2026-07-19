"""Shared, Unreal-independent contract checks for Playable Foundation tooling."""

from __future__ import annotations

from typing import Any


EXPECTED_ASSET_PATHS = frozenset(
    {
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
)

EXPECTED_PLACEHOLDER_VISUAL = {
    "component_name": "PlaceholderVisual",
    "component_class": "/Script/Engine.StaticMeshComponent",
    "attach_to": "CapsuleComponent",
    "mesh": "/Engine/BasicShapes/Sphere.Sphere",
    "relative_location": {"x": 0.0, "y": 0.0, "z": 0.0},
    "relative_rotation": {"pitch": 0.0, "yaw": 0.0, "roll": 0.0},
    "relative_scale": [0.84, 0.84, 1.92],
    "collision_profile": "NoCollision",
    "can_ever_affect_navigation": False,
}

CURRENT_BLUEPRINT_STATUSES = frozenset(
    {
        "UP_TO_DATE",
        "UP_TO_DATE_WITH_WARNINGS",
        "BS_UP_TO_DATE",
        "BS_UP_TO_DATE_WITH_WARNINGS",
    }
)


def enum_name(value: Any) -> str:
    """Return a stable reflected-enum name across Unreal Python representations."""

    return str(
        getattr(value, "name", str(value).split(".")[-1].split(":")[0].strip(" <>"))
    ).upper()


def blueprint_status_is_current(value: Any) -> bool:
    return enum_name(value) in CURRENT_BLUEPRINT_STATUSES


def validate_manifest_contract(manifest: dict) -> None:
    """Reject any manifest outside the exact authorised Playable Foundation scope."""

    if manifest.get("schema_version") != 1:
        raise ValueError("Only Playable Foundation manifest schema version 1 is supported")

    action_definitions = manifest.get("input_actions")
    mapping_context = manifest.get("mapping_context")
    blueprint_definitions = manifest.get("blueprints")
    map_definition = manifest.get("map")
    if not isinstance(action_definitions, list):
        raise ValueError("Manifest input_actions must be a list")
    if not isinstance(mapping_context, dict):
        raise ValueError("Manifest mapping_context must be an object")
    if not isinstance(blueprint_definitions, dict):
        raise ValueError("Manifest blueprints must be an object")
    if not isinstance(map_definition, dict):
        raise ValueError("Manifest map must be an object")

    if set(blueprint_definitions) != {"character", "controller", "game_mode"}:
        raise ValueError(
            "Manifest must define exactly character, controller and game_mode Blueprints"
        )

    action_paths = [definition["asset"] for definition in action_definitions]
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

    mappings = mapping_context.get("mappings")
    if not isinstance(mappings, list) or len(mappings) != 13:
        raise ValueError("Manifest must define exactly thirteen player mappings")
    action_path_set = set(action_paths)
    if any(mapping.get("action") not in action_path_set for mapping in mappings):
        raise PermissionError("Every mapping must reference a manifest-owned Input Action")
    mapping_pairs = [(mapping.get("action"), mapping.get("key")) for mapping in mappings]
    if len(set(mapping_pairs)) != len(mapping_pairs):
        raise ValueError("Manifest action/key mapping pairs must be unique")

    supported_modifiers = {"negate", "swizzle", "dead_zone", "scalar"}
    for mapping in mappings:
        modifiers = mapping.get("modifiers")
        if not isinstance(modifiers, list):
            raise ValueError("Every mapping must define a modifier list")
        for modifier in modifiers:
            if modifier.get("type") not in supported_modifiers:
                raise ValueError(
                    f"Unsupported input modifier type: {modifier.get('type')}"
                )

    if map_definition.get("asset") != "/Game/Maps/L_Dev_Sandbox":
        raise PermissionError("Only L_Dev_Sandbox may be inspected by this workflow")
    if map_definition.get("game_mode_override") != blueprint_definitions["game_mode"][
        "asset"
    ]:
        raise PermissionError("Sandbox GameMode override must use the manifest GameMode Blueprint")
    if map_definition.get("player_start_count") != 1:
        raise PermissionError("Sandbox contract requires exactly one PlayerStart")
    if map_definition.get("placed_auto_possessed_pawn_count") != 0:
        raise PermissionError("Sandbox contract forbids placed auto-possessed pawns")

    placeholder = blueprint_definitions["character"].get("placeholder_visual", {})
    if placeholder != EXPECTED_PLACEHOLDER_VISUAL:
        raise PermissionError(
            "Character placeholder visual differs from the authorised contract"
        )
