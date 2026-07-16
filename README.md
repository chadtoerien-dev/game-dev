# The Veil

Unreal Engine 5 foundation for an original third-person social-deduction action RPG.

The game’s causal spine is:

```text
social action
→ routine and access change
→ night operation
→ evidence and witnessed events
→ individual memories and rumours
→ claims and contradictions
→ explainable voting intentions
→ Exile and persistent consequences
```

## Current milestone: 0.4 Claim, Contradiction and Voting

Implemented native foundations:

- Round and phase authority
- Checkpoint save and restoration
- Opportunity Threads
- Routine and contextual access evaluation
- Evidence persistence and Dawn surfacing
- Gameplay events, witnessing and individual memory
- Information provenance, reports, rumours and lies
- Directed NPC-to-NPC relationships
- Public and private claims
- Contradiction detection and credibility consequences
- Data-driven, explainable voting intentions
- Deterministic vote locking
- Adaptive-audio state arbitration foundation

## Project context

The original prompts, design decisions and project-chat history are preserved in [`docs/context/README.md`](docs/context/README.md).

Start with:

1. [`docs/context/MASTER_PROMPT.md`](docs/context/MASTER_PROMPT.md)
2. [`docs/context/REFERENCE_MUSIC_ADDENDUM.md`](docs/context/REFERENCE_MUSIC_ADDENDUM.md)
3. [`docs/context/DECISION_LOG.md`](docs/context/DECISION_LOG.md)
4. [`docs/design/VERTICAL_SLICE.md`](docs/design/VERTICAL_SLICE.md)
5. [`docs/technical/TECHNICAL_ARCHITECTURE.md`](docs/technical/TECHNICAL_ARCHITECTURE.md)
6. [`INTEGRATION.md`](INTEGRATION.md)

The comprehensive context migration status is recorded in [`docs/context/MIGRATION_AUDIT.md`](docs/context/MIGRATION_AUDIT.md). The exhaustive message, output and file audit starts at [`docs/context/FULL_PROJECT_CONTEXT_INDEX.md`](docs/context/FULL_PROJECT_CONTEXT_INDEX.md), with historical deliverables under [`artifacts/`](artifacts/).

## Local setup

1. Install the selected Unreal Engine 5 version and supported Visual Studio C++ workload.
2. Clone the repository with Git LFS installed.
3. Generate project files from `TheVeil.uproject`.
4. Build `TheVeilEditor` in Development Editor / Win64.
5. Read [`INTEGRATION.md`](INTEGRATION.md).
6. Run tests under `TheVeil.*` in Unreal’s Test Automation window.

## Important status

The native source has been structurally reviewed, but it was not compiled in a local Unreal Engine installation from the originating assistant environment. Blueprint assets, maps, StateTrees, animation assets, MetaSounds and final audio are not yet included.

## Originality boundary

Hitman, Splinter Cell, Metal Gear Solid and modern action RPGs are directional references only. All setting, terminology, characters, missions, dialogue, music, UI and audiovisual expression must remain original.
