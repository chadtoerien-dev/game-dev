# The Veil Development Roadmap

Last updated: 17 July 2026

## Product destination

Deliver an original, polished 25–30 minute PC vertical slice of a third-person social-deduction action RPG in Unreal Engine 5.8.

The playable causal loop is:

```text
social action
→ relationship, routine and access change
→ challenge or night operation
→ evidence and witnessed events
→ individual memories, reports, rumours and lies
→ public claims and contradictions
→ explainable vote movement
→ private vote lock and exile
→ persistent consequences
```

The project already contains substantial native social logic, but the playable shell is behind. The roadmap therefore prioritises movement, interaction and embodied NPCs before expanding content or presentation.

## Status legend

- **Complete:** built, tested and merged
- **Verified locally:** built and tested on the owner's machine; confirm the corresponding commit is on `main`
- **In progress:** approved current work
- **Next:** ready to start after the preceding gate
- **Later:** deliberately deferred

## Non-negotiable production principles

1. One complete round before more content.
2. Mechanics and readability before final art.
3. PC vertical slice before browser, mobile, console or VR production.
4. Every milestone ends in build and test evidence.
5. NPC decisions remain explainable and respect information boundaries.
6. Originality is mandatory; references guide function and tone, not copied expression.
7. No concurrent agents edit the same Unreal binary assets or source files.
8. The human owner approves scope, canon, visual direction and merges.

---

# Foundation already delivered

## Milestone 0.1 — Causal Spine

**Status:** Complete

Proved:

```text
daytime trust action
→ invitation
→ target routine change
→ contextual access
→ operation
→ evidence
→ music-state request
```

## Milestone 0.2 — Round and Phase Authority

**Status:** Complete

Delivered:

- authoritative phase transitions
- Round Director and GameState foundation
- persistent contestant state
- checkpoint save and restoration
- Dawn surfacing

## Milestone 0.3 — Memory and Provenance

**Status:** Complete

Delivered:

- objective gameplay events
- observer-specific memories
- visual and auditory witnessing
- reports, rumours and deliberate lies
- information-provenance tracking
- directed relationships
- data-driven social interpretation

## Milestone 0.4 — Claims, Contradictions and Voting

**Status:** Verified locally; final GitHub reconciliation required

Delivered:

- public and private claims
- audience-specific knowledge
- direct and location contradictions
- credibility and suspicion consequences
- explainable vote scoring
- commitments and group pressure
- deterministic vote locking
- five passing automation tests
- successful UE5.8 editor build
- successful editor launch and PIE smoke test

The GitHub migration checklist must confirm the local UE5.8 repair commit and development map are on `main`.

---

# Playable production sequence

## Milestone 0.5 — Playable Foundation

**Status:** In progress — native source and automation verified locally; human Editor/PIE gate pending

**Objective:** turn `L_Dev_Sandbox` into a controllable third-person game rather than an editor-only map.

### Deliverables

- production C++ player character or pawn foundation
- PlayerController
- GameMode
- PlayerStart
- Enhanced Input actions and mapping context
- camera-relative movement
- acceleration and braking tuning
- sprint
- crouch
- jump
- mouse and controller look
- stable spring-arm camera
- development movement telemetry

### Acceptance criteria

- the correct GameMode loads with the sandbox
- the player spawns at a deliberate PlayerStart
- WASD and controller movement are camera-relative
- sprint cannot become stuck
- crouch and jump transition cleanly
- camera movement is smooth and does not visibly judder
- collision prevents falling through the map
- PIE starts and exits without a crash
- editor target builds successfully
- the five pre-existing automation tests remain green
- the four playable-foundation movement/default tests remain green

### Explicit non-goals

- final animation set
- parkour or climbing
- combat
- multiplayer replication
- final character art

## Milestone 0.6 — Interaction and Camera Feel

**Status:** Next after 0.5

**Objective:** prove a responsive action-adventure interaction loop.

### Deliverables

- reusable interaction interface
- camera-centred or character-centred interaction trace
- focus and range validation
- test door
- one inspectable or collectible test object
- clear development prompt
- input buffering where necessary
- camera collision and occlusion handling
- interaction debug visualisation

### Acceptance criteria

- the player can approach, focus and activate a test object
- interaction cannot trigger through walls or outside range
- keyboard and controller have parity
- prompts update correctly as targets change
- no repeated interaction from one input press
- the result remains responsive under normal editor frame pacing

## Milestone 0.7 — Embodied Contestants

**Status:** Later

**Objective:** place visible placeholder contestants into the sandbox and connect them to the native state model.

### Deliverables

- eight stable contestant identities
- placeholder character actors
- contestant state, memory and witness components
- navigation mesh and basic navigation
- simple StateTree or equivalent routine control
- visible location and phase state
- development relationship and memory overlay

### Acceptance criteria

- all contestants spawn with stable IDs
- no duplicate identity or missing state
- contestants move without navigation deadlocks
- witness components produce expected memories
- directed relationships remain asymmetric
- ten unattended short cycles complete without deadlock

## Milestone 0.8 — Social Free-Roam Loop

**Status:** Later

**Objective:** make player social actions change observable NPC behaviour and access.

### Deliverables

- conversation participation and reservation
- Build Rapport action
- accuse, defend, question and withdraw actions
- charisma or influence presentation
- Opportunity Thread activation
- target routine changes
- contextual invitations and access
- private versus public audience handling
- development explanation panel

### Acceptance criteria

- the same social event produces different reactions by personality
- a successful relationship change unlocks one concrete opportunity
- NPCs do not use knowledge they have not acquired
- rumours visibly lose confidence and can distort
- the player can understand why an NPC's behaviour changed

## Milestone 0.9 — Interactive Tribunal and Exile

**Status:** Later

**Objective:** convert the existing claim and voting foundation into a playable public decision.

### Deliverables

- ceremonial Tribunal staging
- speaking priority
- player interruption, defence and challenge actions
- evidence presentation
- live but non-numeric vote-intention feedback
- private vote lock
- tie handling
- exile presentation
- optional faction reveal rule
- persistent alliance and credibility consequences

### Acceptance criteria

- the player can legitimately move at least two NPC votes
- only informed voters react to a claim or defence
- contradictions are visible without exposing hidden simulation truth
- every NPC locks exactly one eligible candidate
- save and restore preserves claims, contradictions and votes
- at least three materially different Tribunal outcomes are possible

## Milestone 1.0 — Active Night Operation

**Status:** Later

**Objective:** prove that both factions have active, systemic night gameplay.

### Deliverables

- one original archive or equivalent operation space
- target selection
- social access route
- secret or risky physical route
- guard and witness perception
- protection and interception
- sabotage, contamination or prevention actions
- evidence generation by approach
- adaptive tension-state requests

### Acceptance criteria

- at least three approaches are viable
- each approach produces different evidence or witness risk
- a Loyal player can actively prevent or mitigate the operation
- a Veiled player can execute it without a forced cutscene solution
- at least three distinct outcomes reach the next Dawn

## Milestone 1.1 — Complete Round Integration

**Status:** Later

**Objective:** run the entire round without developer intervention.

### Required sequence

```text
Social Free Roam
→ Night Planning
→ Night Operation
→ Dawn Assembly
→ Challenge
→ Social Fallout
→ Tribunal
→ Vote
→ Exile
→ checkpoint or next round
```

### Acceptance criteria

- three consecutive complete runs
- stable phase transitions
- no duplicated or lost state
- save and restore at phase boundaries
- no deadlocked conversations or navigation
- no omniscient NPC decisions
- all consequential vote factors have a development explanation

## Milestone 1.2 — Reliquary Run Challenge

**Status:** Later

**Objective:** connect physical performance and cooperation to social consequence.

### Deliverables

- team assignment
- carrying and coordination
- shortcuts and role specialisation
- cooperation and limited sabotage
- reward or access assignment
- observed acts that enter the social memory system

### Acceptance criteria

- challenge performance changes later social state
- at least two strategic approaches exist
- sabotage is readable and produces evidence risk
- the challenge fits the full round time budget

## Milestone 1.3 — Vertical Slice Content Lock

**Status:** Later

**Objective:** lock one complete, replayable 25–30 minute slice.

### Content limit

- one estate or compact location set
- eight contestants
- two Veiled and six Loyal roles
- one challenge
- one night-operation location
- one Tribunal and Exile sequence
- one complete round
- limited but meaningful dialogue content

### Acceptance criteria

- beginning-to-end playthrough without developer tools
- at least three distinct strategic routes
- role seed and social state create replay variation
- onboarding is understandable to a first-time player
- no placeholder system blocks completion

## Milestone 1.4 — AAA Presentation Pass

**Status:** Later

**Objective:** raise fidelity only after the complete loop is stable.

### Workstreams

- locomotion animation quality
- motion matching or selected locomotion solution
- facial and conversation animation
- cinematic camera grammar
- lighting and atmosphere
- materials and environment art
- original adaptive score and sound design
- MetaSounds, Quartz and Audio Modulation implementation
- final UI and accessibility
- VFX and interaction feedback

### Acceptance criteria

- consistent visual direction
- original audiovisual identity
- no obvious foot sliding or camera discontinuity
- readable social and stealth states
- stable frame pacing on target hardware
- accessibility baseline for subtitles, controls, colour and motion

## Milestone 1.5 — Optimisation, Packaging and External Test

**Status:** Later

**Objective:** produce a distributable Windows vertical-slice build.

### Deliverables

- cooked Development and Shipping builds
- performance budgets and captures
- crash logging
- save migration test
- clean-machine build instructions
- external playtest package
- issue triage and release checklist

### Acceptance criteria

- clean clone can generate, build, test and package
- packaged build completes the full round
- no fatal errors in repeated sessions
- frame-time targets are met on selected minimum and recommended hardware
- feedback from at least five external playtesters is triaged

---

# Near-term delivery plan

## Sprint A — Repository and playable baseline

**Goal:** complete migration and begin 0.5.

- reconcile local commits with GitHub `main`
- commit complete sandbox World Partition content
- add launch, build and test scripts
- verify Cursor rules and agent documents
- create the 0.5 task brief
- implement GameMode, controller, character and spawn

## Sprint B — Movement quality

**Goal:** responsive movement worth building on.

- tune acceleration, braking and rotation
- add sprint, crouch and jump
- stabilise camera and collision
- verify controller parity
- add development telemetry
- record movement video for independent review

## Sprint C — Interaction

**Goal:** one reusable interaction loop.

- interaction interface and trace
- test door and test object
- focus, range and occlusion rules
- prompt and debug feedback
- automated and PIE tests

## Sprint D — First contestants

**Goal:** three embodied contestants prove the event-to-memory pipeline in the map.

- Eleanor, Marcus and Beatrice placeholders
- navigation and routines
- witness and memory components
- archive sabotage test event
- relationship and memory debug overlay

---

# Quality gates for every milestone

A milestone may move to complete only when all applicable gates pass:

1. **Scope gate:** task matches the approved brief.
2. **Source gate:** files are in the correct folders and generated files are excluded.
3. **Build gate:** `TheVeilEditor` builds successfully.
4. **Automation gate:** relevant tests pass.
5. **PIE gate:** player-facing behaviour works in the editor without a fatal error.
6. **Explainability gate:** AI decisions have development-visible reasons.
7. **Save gate:** persistent changes survive save and restore where applicable.
8. **Performance gate:** no unexplained regression in frame time or memory.
9. **Originality gate:** no protected expression has been copied.
10. **Review gate:** an independent agent and the human owner have reviewed the result.
11. **Documentation gate:** handover, integration notes and roadmap are current.

# Deferred platform strategy

Unreal Engine can target additional platforms, but development remains focused on the Windows PC vertical slice.

Deferred until after 1.5:

- macOS production support
- Linux production support
- console certification
- mobile adaptation
- browser or Pixel Streaming delivery
- VR or spatial-computing adaptation
- user-generated metaverse deployment

Each future platform requires a separate feasibility, performance, control and commercial assessment. It is not a free export step.
