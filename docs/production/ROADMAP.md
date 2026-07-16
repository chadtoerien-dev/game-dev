# Development Roadmap and Initial Backlog

## Phase 0 — Production foundation

**Objective:** reproducible project and build process.

Deliver a UE project, source-control rules, naming, folders and packaged Development build. A clean machine must be able to clone, generate, build, open and package.

## Phase 1 — Movement and interaction

**Objective:** prove responsive third-person control.

Systems:

- Camera-relative movement
- Acceleration and braking
- Sprint, crouch and jump
- Camera
- Interaction trace
- Reusable door
- Development HUD

Acceptance: immediate movement, no stuck sprint, controller parity and stable packaged build.

## Phase 2 — Round and contestant foundation

**Objective:** represent eight contestants and authoritative phases.

Systems:

- GameState and Round Director
- Phase transitions and timers
- Contestant profiles
- Role assignment
- Directed relationship matrix
- Phase checkpoint

Acceptance: deterministic, logged, saveable transitions.

## Phase 3 — Social simulation

**Objective:** prove persistent NPC-to-NPC opinion change.

Systems:

- Relationships
- Memories
- Information provenance
- Typed events
- Social interpretation profiles

Acceptance: the same event produces different reactions across personalities and every major decision has a debug explanation.

## Phase 4 — NPC schedules and conversation

**Objective:** make free-roam contestants purposeful.

Systems:

- StateTree
- Utility goals
- Navigation
- Smart Objects
- Conversation participation and reservation
- Contextual dialogue actions

Acceptance: NPCs seek others for intelligible reasons, communicate while the player is absent and avoid deadlocks.

## Phase 5 — Challenge

**Objective:** connect physical performance to social consequence.

Build the Reliquary Run with teams, carrying, shortcuts, cooperation, sabotage and reward assignment.

## Phase 6 — Tribunal and voting

**Objective:** turn social state into a playable public decision.

Systems:

- Claims
- Contradictions
- Speaking priority
- Player interruptions, defence and evidence
- Live voting intentions
- Vote lock
- Exile

Acceptance: the player can legitimately move at least two NPC votes; outcomes are explainable without feeling deterministic.

## Phase 7 — Night operation

**Objective:** prove active dual-faction night gameplay.

Systems:

- Target selection
- Social access
- Secret route
- Guard perception
- Protection and interception
- Sabotage and contamination
- Evidence and witnesses

Acceptance: at least three distinct outcomes reach the following Dawn Assembly.

## Phase 8 — Complete round integration

**Objective:** run the full round without developer intervention.

Acceptance:

- Three consecutive complete runs
- Stable phase transitions
- No duplicated or lost state
- Save and restore at phase boundaries

## Phase 9 — Polish and optimisation

**Objective:** raise presentation only after mechanics work.

Systems:

- Animation and locomotion quality
- Audio and adaptive score
- Lighting
- UI
- Accessibility
- Performance
- Onboarding

## Sprint 1 — Playable foundation

Two-week goal: a clean UE project that can be cloned, built, packaged and played with responsive movement and interaction.

Priority work:

- Pin engine and toolchain
- Create C++ Third Person project
- Configure Git and LFS
- Create folders and naming rules
- Greybox movement course
- Tune movement
- Interaction interface and door
- Development HUD
- Package and record performance baseline
- Verify controller

## Sprint 2 — Round and contestant skeleton

Two-week goal: eight placeholders inside a phase-controlled simulation.

Priority work:

- Native phase state
- Round Director
- Contestant Primary Data Asset
- Eight stable contestant IDs
- Two-Veiled/six-Loyal seeded assignment
- Directed relationships
- Basic navigation
- Minimal StateTree
- One Build Rapport action
- Relationship debug overlay
- Checkpoint snapshot
- Ten unattended cycles without deadlock

## Current milestone position

Milestones 0.1–0.4 implemented the native causal architecture ahead of the full playable content sequence. The next production task is Phase 6’s interactive Tribunal and Exile presentation, followed by the actual movement/level/Blueprint content required to play it in UE5.

## Scope-control rules

- Mechanics before estate size.
- One challenge, one night location and one round.
- No final art before the loop is stable.
- No procedural dialogue generation in the slice.
- No multiplayer, mobile, VR or browser deployment yet.
- Every phase ends in something testable.
