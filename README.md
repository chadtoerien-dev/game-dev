# The Veil

Unreal Engine 5.8 foundation for an original third-person social-deduction action RPG.

The game's causal loop is:

```text
social action
→ routine and access change
→ night operation or challenge
→ evidence and witnessed events
→ individual memories, reports, rumours and lies
→ claims and contradictions
→ explainable voting intentions
→ private vote lock and exile
→ persistent consequences
```

## Current position

**Native foundation:** milestone 0.4 — claims, contradictions and voting

**Next playable milestone:** 0.5 — third-person character, camera, input and interaction foundation

Verified locally on Windows with Unreal Engine 5.8 on 17 July 2026:

- `TheVeilEditor` Development/Win64 built and linked successfully
- five `TheVeil.*` automation tests passed with zero failures and zero skips
- Unreal Editor opened successfully
- `/Game/Maps/L_Dev_Sandbox` loaded
- Play-in-Editor started and exited without a crash or fatal error

The social-simulation code is ahead of the playable shell. The immediate priority is responsive action-RPG control and interaction, followed by embodied contestants and an interactive Tribunal.

## Implemented native foundations

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

## Start here

1. [`AGENTS.md`](AGENTS.md) — cross-agent operating rules
2. [`docs/context/AGENT_HANDOFF.md`](docs/context/AGENT_HANDOFF.md) — current verified state and next task
3. [`docs/production/ROADMAP.md`](docs/production/ROADMAP.md) — gated delivery roadmap
4. [`docs/production/AI_AGENT_WORKFLOW.md`](docs/production/AI_AGENT_WORKFLOW.md) — model routing and multi-agent workflow
5. [`docs/production/TASK_BRIEF_TEMPLATE.md`](docs/production/TASK_BRIEF_TEMPLATE.md) — reusable planning, implementation and review prompts
6. [`docs/context/CURSOR_MIGRATION_CHECKLIST.md`](docs/context/CURSOR_MIGRATION_CHECKLIST.md) — final local/remote reconciliation
7. [`docs/context/MASTER_PROMPT.md`](docs/context/MASTER_PROMPT.md) — authoritative original brief
8. [`docs/context/REFERENCE_MUSIC_ADDENDUM.md`](docs/context/REFERENCE_MUSIC_ADDENDUM.md) — gameplay-reference and score addendum
9. [`docs/technical/TECHNICAL_ARCHITECTURE.md`](docs/technical/TECHNICAL_ARCHITECTURE.md)
10. [`INTEGRATION.md`](INTEGRATION.md)

The exhaustive project-message, output and file audit begins at [`docs/context/FULL_PROJECT_CONTEXT_INDEX.md`](docs/context/FULL_PROJECT_CONTEXT_INDEX.md).

## Local development

Requirements:

- Windows x64
- Unreal Engine 5.8
- Visual Studio Community 2026 or compatible installation
- MSVC toolchain `14.50.35717`
- Windows SDK `10.0.22621.0`
- Git LFS

From the repository root:

```powershell
powershell -ExecutionPolicy Bypass -File ".\scripts\Build-TheVeil.ps1"
```

```powershell
powershell -ExecutionPolicy Bypass -File ".\scripts\Run-TheVeilTests.ps1"
```

```powershell
powershell -ExecutionPolicy Bypass -File ".\scripts\Launch-TheVeil.ps1"
```

Before any Git write, run `git rev-parse --show-toplevel` and confirm the result is the game-dev repository rather than the user's home directory.

## Project context

The original prompts, decisions, project history and assistant deliverables are preserved in [`docs/context/README.md`](docs/context/README.md). Current repository source and maintained documents are authoritative over historical archives.

## Originality boundary

Hitman, Splinter Cell, Metal Gear Solid, social-deduction television formats and modern action RPGs are directional references only. All setting, terminology, characters, missions, dialogue, music, UI and audiovisual expression must remain original.
