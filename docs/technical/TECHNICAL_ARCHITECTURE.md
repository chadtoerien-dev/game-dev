# Technical Architecture

## Engine strategy

- Unreal Engine 5.8 is the current project baseline.
- Reverify the stable engine version and official Epic documentation before any upgrade or version-sensitive recommendation.
- Start from a C++ Third Person project.
- Use Blueprints for rapid content integration and C++ for durable systems, performance and maintainability.
- Initial target: Windows PC, 1080p, stable 60 fps on a defined mid-range machine.

## Feature decisions

| Feature | Decision |
|---|---|
| Enhanced Input | Use immediately |
| Gameplay Tags | Use immediately |
| StateTree | Use for high-level NPC phase and schedule state |
| Utility scoring | Use for social goals, debate and voting |
| AI Perception | Use as sight/hearing input into memory |
| Smart Objects | Use selectively for seats, conversations and activities |
| Behaviour Trees | Reserve for guard/tactical behaviour if useful |
| Motion Matching | Evaluate after responsive movement is proved |
| MetaHuman | Later selected visual tests, not initial foundation |
| Common UI | Introduce with conversation and menu navigation |
| Gameplay Ability System | Deferred pending overlapping skills, equipment and status needs |
| Mass Entity | Do not use for eight contestants |
| Modular Gameplay | Defer during vertical slice |
| Multiplayer | Preserve authority boundaries, do not implement yet |

## Runtime authority

### Core classes

- `UTheVeilGameInstance`: application bootstrap and persistent profile settings.
- `ATheVeilGameMode`: authoritative transition requests and spawning.
- `ATheVeilGameState`: public round, phase, timer and result state.
- `ATheVeilPlayerState`: contestant identity, role, attributes and public voting history.
- `ATheVeilPlayerController`: input contexts, HUD, conversation focus and camera mode.
- `ATheVeilCharacter`: movement, camera-facing state and physical interaction.
- `ATheVeilAIController`: navigation, perception and StateTree execution.
- `ATheVeilContestantCharacter`: shared contestant physical base.
- `ATheVeilRoundDirector`: time-boxed phase coordination.
- `ATheVeilChallengeDirector`: challenge rules and rewards.
- `ATheVeilTribunalDirector`: debate, claims and transition to vote.
- `ATheVeilNightDirector`: target selection, operation configuration and consequence capture.

GameMode authorises; dedicated directors run phase-specific logic. Avoid a god object.

## Components

- `UTVInteractionComponent`
- `UTVSocialAgentComponent`
- `UTVContestantStateComponent`
- `UTVRelationshipComponent` or central relationship authority
- `UTVMemoryComponent`
- `UTVEvidenceInventoryComponent`
- `UTVNoiseComponent`
- `UTVMovementStateComponent`
- `UTVConversationComponent`
- `UTVWitnessComponent`
- `UTVInventoryComponent`
- `UTVTargetRoutineComponent`
- `UTVAccessEvaluationComponent`

The complete relationship matrix has one authoritative runtime representation rather than being copied into every character Blueprint.

## Subsystems

- `UTVSimulationSubsystem`: persistent contestant and relationship simulation.
- `UTVPhaseSubsystem`: phase and checkpoint lifecycle.
- `UTVSaveSubsystem` or phase-save responsibility: save versioning and restoration.
- `UTVGameplayEventSubsystem`: objective event registry.
- `UTVEvidenceSubsystem`: evidence state and Dawn surfacing.
- `UTVInformationSubsystem`: reports, rumours and deliberate lies.
- `UTVRelationshipSubsystem`: directed social values.
- `UTVClaimSubsystem`: audience-specific statements and contradictions.
- `UTVVotingSubsystem`: commitments, score breakdowns and vote locking.
- `UTVOpportunitySubsystem`: Opportunity Thread state.
- `UTVDialogueSubsystem`: contextual line and action resolution.
- `UTVAudioDirectorSubsystem`: music-state arbitration.
- `UTVDebugSubsystem`: relationship, memory, voting and event inspection.

## Data assets

### Contestant profile

Contains contestant ID, name, portrait, persona tags, hidden traits, attributes, decision weights, communication style, risk tolerance, starting relationships, voice set, role eligibility and character references.

### Social action

Contains action ID, label, requirements, blocked tags, attribute, difficulty, public/private scope, relationship effects, memory event, claim output, dialogue intent, cooldown and AI utility factors.

### Evidence definition

Contains category, physical/testimonial status, reliability, discoverability, interpretations, location, contamination and tags.

### Opportunity definition

Contains required trust, knowledge, target routine, location, preparation state, music state, evidence output, expiry and risk.

### Voting and interpretation profiles

Contain personality-specific weights rather than hard-coded universal behaviour.

## Information model

An information or claim record should retain:

- Unique ID
- Subject, predicate and object/value
- Objective origin where simulation-authoritative
- Attributed identity where visible
- Original observer
- Immediate source
- Acquisition method
- First-hand/report/rumour/lie state
- Confidence and perceived reliability
- Timestamp, phase and location
- Supporting evidence
- Known contradictions
- Transmission depth and distortion

NPC decisions use only the information available to that NPC.

## AI architecture

```text
round phase
→ StateTree selects broad state
→ utility evaluator selects goal
→ movement, Smart Object or conversation task
→ perception and events create memories
→ relationships and information change
→ next utility evaluation uses changed state
```

StateTree broad states include assembly, socialise, seek, avoid, investigate, challenge, lobby, Tribunal and night behaviour.

Utility choices include talking to an ally, verifying a rumour, confronting a suspect, influencing an undecided voter, avoiding a threat and searching for evidence.

AI Perception supplies observations. It must not directly assign guilt.

## Voting model

For each voter and candidate, calculate a development-visible breakdown including:

- Suspicion
- Distrust
- Low affinity
- Fear
- Perceived threat
- Known claims
- Known contradictions
- Commitments
- Group pressure
- Deterministic tie-breaker

Different personalities weight factors differently. A vote may be strategic rather than sincere.

## Save architecture

Layers:

- Profile settings
- Run state
- Phase checkpoint

Persist:

- Save format and content version
- Round and phase
- Random seed and role assignments
- Contestant states
- Relationships
- Memories and information provenance
- Evidence
- Claims and contradictions
- Voting history, commitments and intentions
- Opportunity state
- Challenge result
- Night consequences

Historical objective events restore without rebroadcasting and creating duplicate memories.

## Event flow

```text
GameMode requests phase transition
→ GameState validates/publishes
→ Round Director activates phase director
→ NPC StateTrees receive phase event
→ UI changes context
→ gameplay emits typed events
→ memory, evidence, relationships, claims and voting consume events
→ phase director evaluates completion
→ checkpoint save
→ next phase
```

## Performance principles

- Eight active contestants and at most two guards in the slice.
- Social decisions are staggered and event-driven.
- Voting and relationship changes never run on Tick.
- Limit simultaneous detailed faces.
- Profile lighting, animation, AI and audio from greybox stage.
- Use scalable settings and controller plus keyboard/mouse support.

## Source layout

```text
Content/TheVeil/
    AI/
    Animation/
    Audio/
    Cameras/
    Challenges/
    Characters/
    Core/
    Data/
    Dialogue/
    Evidence/
    Input/
    Interaction/
    Items/
    Levels/
    Missions/
    Narrative/
    Social/
    UI/
    Voting/
    World/

Source/TheVeil/
    AI/
    Audio/
    Characters/
    Challenges/
    Claims/
    Core/
    Dialogue/
    Events/
    Evidence/
    Interaction/
    Memory/
    Social/
    Voting/
    World/
```

## Source control

Use Git and Git LFS for Unreal binary assets, clear ignore rules, meaningful commits and short-lived branches. Consider Perforce when a larger art team makes locking and large-binary workflows materially difficult.
