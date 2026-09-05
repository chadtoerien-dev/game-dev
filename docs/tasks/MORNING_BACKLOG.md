# The Veil — Morning Human Backlog

Last updated: 6 September 2026

This file records human-only checkpoints accumulated during autonomous work. Automated agents must not mark an item complete without the stated human evidence.

## Milestone 0.6 — Interaction and Camera Feel

Status: waiting for Product Owner Editor/PIE acceptance.

Partial Product Owner evidence, 6 September 2026:

- PASS: the inspectable object and test door appeared automatically in `L_Dev_Sandbox`.
- PASS: the contextual interaction prompt appeared.
- PASS: the corrected edge-hinged door opened and closed through interaction.
- NOT YET CLAIMED: controller parity, explicit out-of-range rejection, real-wall occlusion rejection, test-object activation, extended camera comfort and the two-minute clean PIE exit.

### Required Editor setup

1. Open `C:\devprojects\game-dev\TheVeil.uproject` in Unreal Engine 5.8.
2. Open `/Game/Maps/L_Dev_Sandbox`.
3. Confirm one `ATheVeilTestDoor` and one `ATheVeilInspectableTestObject` appear automatically near `PlayerStart`. Do not place or save duplicate actors in the map.

### Required PIE checks

1. Approach each test actor and confirm the development prompt is readable and switches cleanly between targets. Prompt appearance is partially accepted; target switching still needs explicit confirmation.
2. Press `E` once and confirm exactly one activation. Repeat with the controller face-left input and confirm parity.
3. Attempt interaction outside the configured range and confirm it is rejected.
4. Put a visibility-blocking wall between the player and target and confirm interaction is rejected.
5. Toggle the test door open and closed and activate the test object through the same interaction path. Door toggle passed on 6 September 2026; test-object activation still needs explicit confirmation.
6. Walk close to walls and around both actors; confirm camera retraction is comfortable, does not clip and does not visibly judder.
7. Run PIE continuously for at least two minutes, then exit with `Esc` and confirm there is no crash, fatal error or `LogPIE: Error` entry.

### Evidence to retain

- Product Owner pass/fail decision for every check above.
- A short video or screenshots showing prompt focus, keyboard and controller activation, range rejection, occlusion rejection and camera behaviour, if practical.
- Any legitimate `.umap`, external actor/object or HLOD changes produced by accepted Editor placement, reviewed explicitly before commit.
- Exact Output Log errors or warnings if any check fails.

## Merge checkpoint

Do not merge to `main` until the milestone 0.6 independent review verdict is recorded, the Editor/PIE checks above pass, and Chad Toerien explicitly approves the merge.
