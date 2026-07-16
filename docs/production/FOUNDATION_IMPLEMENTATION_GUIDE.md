# Foundation Implementation Guide

This preserves the first practical implementation task defined in the original project conversation. It establishes the Unreal project, source control, responsive movement, a reusable interaction path and a packaged test build. It intentionally excludes final art and advanced AI.

## Objective

Create a clean project containing:

- Hybrid C++ and Blueprint foundation
- Compact greybox test level
- Responsive third-person movement
- Sprint input
- Reusable interaction interface
- Functioning door
- Development HUD
- Packaged Windows build

## Toolchain

Install the selected Unreal Engine 5 version with core components, templates and Windows target support. Install a supported Visual Studio version with:

- Game development with C++
- Desktop development with C++
- C++ profiling tools
- AddressSanitizer
- Windows SDK
- Visual Studio Tools for Unreal Engine

Record the exact engine version in the repository.

## Create the project

1. Launch Unreal Engine.
2. Select **Games → Third Person**.
3. Use C++.
4. Target Desktop with Maximum quality.
5. Disable Starter Content and ray tracing initially.
6. Name the project `TheVeil`.
7. Use a short local path without spaces.
8. Generate and compile the native project.
9. Build `TheVeilEditor` in Development Editor / Win64.

## Source control

Use Git and Git LFS. Binary Unreal assets, source art, audio and video belong in LFS. Generated folders such as `Binaries`, `Intermediate`, `Saved` and `DerivedDataCache` are ignored.

Commit the clean engine foundation separately before gameplay changes.

## Content structure

```text
Content/TheVeil/
    AI/
    Animation/
    Audio/
    Cameras/
    Challenges/
    Characters/
        Player/
        Contestants/
    Core/
        GameModes/
        GameStates/
        PlayerControllers/
    Data/
    Dialogue/
    Evidence/
    Input/
    Interaction/
    Items/
    Levels/
        Development/
    Missions/
    Narrative/
    Social/
    UI/
        Development/
    Voting/
    World/
        Greybox/
```

## Naming

- Blueprint Actor: `BP_`
- Blueprint Component: `BPC_`
- Blueprint Interface: `BPI_`
- Widget: `WBP_`
- Animation Blueprint: `ABP_`
- Input Action: `IA_`
- Input Mapping Context: `IMC_`
- Data Asset: `DA_`
- Data Table: `DT_`
- StateTree: `ST_`
- Behaviour Tree: `BT_`
- Blackboard: `BB_`
- Enum: `E_`
- Structure: `S_`
- Level: `L_`
- Material: `M_`
- Material Instance: `MI_`
- Niagara System: `NS_`
- Sound: `SFX_`

## Greybox level

Save the template map as:

`Content/TheVeil/Levels/Development/L_Dev_Foundation`

Build:

- Starting area
- Ten-metre acceleration lane
- Ninety-degree corner
- 180-degree turn area
- Stairs
- Low jump obstacle
- Narrow doorway
- Interaction door
- Raised platform

Add a NavMesh Bounds Volume for the later NPC sprint.

## Project Blueprint classes

Create:

- `BP_TVPlayerCharacter`
- `BP_TVPlayerController`
- `BP_TVGameMode`

Set the GameMode’s default pawn and controller. Set the development level as editor and game default map.

## Initial movement values

These are test baselines, not final tuning:

| Character Movement property | Value |
|---|---:|
| Max Walk Speed | 500 |
| Max Acceleration | 2400 |
| Braking Deceleration Walking | 2200 |
| Ground Friction | 8 |
| Braking Friction Factor | 1 |
| Rotation Rate Yaw | 720 |
| Jump Z Velocity | 650 |
| Air Control | 0.35 |
| Orient Rotation to Movement | True |
| Use Controller Desired Rotation | False |

Character:

- Use Controller Rotation Yaw: False

Camera boom:

- Target Arm Length: 350
- Use Pawn Control Rotation: True
- Camera Lag: False during response testing

## Sprint input

Create `IA_Sprint` as a digital action. Bind Left Shift and the gamepad left-stick button in `IMC_Player`.

Blueprint flow:

```text
IA_Sprint Started
→ set Max Walk Speed to 700

IA_Sprint Completed or Cancelled
→ set Max Walk Speed to 500
```

The cancelled path is required to prevent a stuck sprint after an input-context change.

## Interaction interface

Create `BPI_Interactable` with:

- `CanInteract → Boolean`
- `GetInteractionPrompt → Text`
- `Interact(Interactor: Actor)`

Create a custom trace channel named `Interactable`, defaulting to Ignore.

Create `IA_Interact`, bind `E` and gamepad face button bottom.

On input, trace 350 units from the camera. If the hit actor implements `BPI_Interactable`, call `Interact`. Do not perform the trace on Tick in this first implementation.

## Development door

Create `BP_DevDoor`:

```text
DefaultSceneRoot
└── DoorPivot
    └── DoorMesh
```

Variables:

- `bIsOpen`: Boolean, false
- `ClosedYaw`: 0
- `OpenYaw`: 90
- `InteractionDuration`: 0.35

Implement the interface. A Timeline interpolates the pivot yaw from closed to open and reverses when closing.

## Development HUD

Create `WBP_DevHUD` showing movement and interaction controls. Add it from `BP_TVPlayerController` BeginPlay using Game Only input mode.

This is a development overlay, not release UI.

## Debugging

Use temporary sprint state messages and:

```text
stat fps
stat unit
```

Do not leave uncontrolled Print String nodes throughout production graphs.

## Packaging

Package a Windows Development build outside the repository and verify:

- Correct map loads
- Keyboard and mouse work
- Controller works
- Sprint always returns to walk
- Door works
- Build runs without the editor

Suggested commits:

```text
chore: create Unreal project foundation
feat: add playable movement and interaction foundation
```

## Acceptance criteria

- Clean checkout can generate and build.
- No Blueprint compile errors.
- Packaged build launches.
- Movement reacts immediately.
- Stopping has no excessive slide.
- Turning is responsive.
- Sprint never sticks.
- Door interaction works from both sides.
- Controller completes the test course.
- Frame timings are recorded.

## Known limitations

- No crouch, vault or mantle
- No final locomotion animation
- No Motion Matching
- No targeting prompt
- No AI
- No social simulation
- No final UI or environment art
