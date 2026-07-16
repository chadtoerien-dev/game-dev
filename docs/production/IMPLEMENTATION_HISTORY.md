# Implementation History

## Milestone 0.1 — Causal Spine

Purpose: prove that a daytime social action can create a night opportunity and evidence.

Added:

- Opportunity subsystem and data asset
- Target routine component
- Contextual access evaluation
- Evidence registry
- Audio-state arbitration foundation

Proof chain:

```text
trust action
→ archive invitation
→ target routine change
→ contextual access
→ sabotage
→ tool-mark evidence
→ music-state request
```

Limitations: one target, one opportunity, one evidence record, placeholder audio, no real phases or save.

## Milestone 0.2 — Round and Phase

Purpose: cross real phase boundaries without losing state.

Added:

- GameState
- Round Director
- Phase subsystem and rules
- Run SaveGame
- Contestant state component
- Opportunity, routine and evidence snapshots
- Dawn evidence surfacing
- Phase automation test

Temporary Blueprint Booleans were replaced by persistent Gameplay Tags and run state.

## Milestone 0.3 — Memory and Provenance

Purpose: prevent omniscient NPCs and create individual knowledge.

Added:

- Objective gameplay events
- Witness components
- Individual memory components
- Reports, rumours and deliberate lies
- Transmission depth and distortion
- Directed relationships
- Interpretation profiles
- Memory and information persistence
- Provenance automation tests

Key rule: evidence stores hidden objective source separately from publicly attributed identity.

## Milestone 0.4 — Claims, Contradictions and Voting

Purpose: turn knowledge into political behaviour.

Added:

- Public/private claims with explicit audiences
- Claims generated from memory
- Direct, location, phase and retraction contradictions
- Observer-specific credibility effects
- Voting profiles
- Per-factor score breakdowns
- Commitments and group pressure
- Deterministic vote lock
- Save persistence
- Claim and voting automation coverage

GitHub merge:

- Repository: `chadtoerien-dev/game-dev`
- Pull request: `#1`
- Merge commit: `849622f9b0aa40014e0cb808828f678e3d2f91c5`

## Validation truth

The code and archives were structurally checked. The originating environment did not contain a local Unreal Engine installation, so Unreal Header Tool generation, C++ compilation, Blueprint integration and in-engine Automation execution remain unverified.

## Current gaps

- No committed Unreal `.uasset` Blueprint classes
- No development map
- No playable movement implementation in the repository
- No StateTree assets
- No conversation UI
- No Tribunal UI or speaker system
- No challenge content
- No guard AI
- No actual night operation map
- No MetaSound, Quartz or music assets
- No final animation, characters, lighting or art
- No local UE5.8 compilation evidence

## Next milestone — Interactive Tribunal and Exile

Implement:

```text
claim selection
→ speaker priority
→ accusation/defence/challenge actions
→ audience memories and reactions
→ live vote recalculation
→ vote lock
→ tie handling
→ exile
→ faction-reveal configuration
→ persistent consequences
```

Required outputs:

- Tribunal Director
- Debate-state model
- Turn and interruption rules
- Player Tribunal UI
- NPC speech utility
- Exile resolution
- Automation and playable test instructions
