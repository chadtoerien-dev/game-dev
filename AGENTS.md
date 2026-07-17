# The Veil — Agent Instructions

Last updated: 17 July 2026

## Mission

Build **The Veil**, an original third-person social-deduction action RPG in Unreal Engine 5.8, towards a polished 25–30 minute vertical slice. The experience combines responsive action-adventure movement, social manipulation, observer-specific knowledge, claims, contradictions, voting, and active night operations.

## Development model

The Veil uses **agent-led development with human-in-the-loop supervision**.

Every agent must first classify the task, identify the correct specialist role and decide whether it should proceed or delegate. When another agent is better suited, stop before implementation and give Chad a complete copy/paste handoff prompt. Follow `docs/production/AGENT_DELEGATION_PROTOCOL.md`.

Chad Toerien is Product Owner, Game Director and final merge authority. Agents lead planning, implementation, review, verification and handoffs; Chad supervises scope, runs required local/editor actions, accepts visible behaviour and approves merges.

## Read before changing anything

Use this authority order when documents conflict:

1. `docs/context/MASTER_PROMPT.md`
2. `docs/context/REFERENCE_MUSIC_ADDENDUM.md`
3. `docs/context/DECISION_LOG.md`
4. `docs/context/AGENT_HANDOFF.md`
5. `docs/production/ROADMAP.md`
6. `docs/technical/TECHNICAL_ARCHITECTURE.md`
7. Current code, tests, config and `INTEGRATION.md`
8. Historical chat and milestone archives

For multi-agent routing, delegation and review rules, read:

- `docs/production/AI_AGENT_WORKFLOW.md`
- `docs/production/AGENT_DELEGATION_PROTOCOL.md`
- `docs/production/TASK_BRIEF_TEMPLATE.md`

## Mandatory routing response

Before planning or editing, state:

1. task classification;
2. recommended role;
3. best available model/agent;
4. why that agent fits;
5. whether you will proceed or delegate;
6. the exact human checkpoint or evidence required.

Do not continue simply because you can answer. Continue only when you are the correct role and have the required tools and context.

When delegating, provide the destination model/tool, bounded objective, current verified state, documents/files to read, scope, non-goals, acceptance criteria, verification commands, risks, expected return format and a ready-to-paste prompt. Tell Chad exactly where to run it.

## Verified development baseline

- Engine: Unreal Engine 5.8 installed at `C:\Program Files\Epic Games\UE_5.8`
- Platform: Windows x64
- Project: `TheVeil.uproject`
- Local repository: `C:\devprojects\game-dev`
- MSVC toolchain: `14.50.35717`
- Compiler reported by UE: Visual Studio `14.50.35737`
- Windows SDK: `10.0.22621.0`
- Development map: `/Game/Maps/L_Dev_Sandbox`
- Native editor build: verified successful on 17 July 2026
- Automation: five `TheVeil.*` tests passed with zero failures and zero skips
- Play-in-Editor smoke test: verified; the sandbox started and exited without a crash or fatal error

Run `git rev-parse --show-toplevel` before any Git write. It must resolve to the project repository, never the user's home directory.

## Non-negotiable engineering rules

- Use British English in documentation and user-facing copy.
- Preserve NPC information boundaries. An NPC may only reason from memories, claims, evidence and sources it has actually learned.
- Keep objective simulation truth separate from attributed public knowledge.
- Keep consequential AI decisions explainable through development-only score breakdowns and logs.
- Prefer data assets, Gameplay Tags and explicit state transitions over hidden Blueprint coupling.
- Keep C++ interfaces small and Blueprint-facing APIs deliberate.
- Do not make broad refactors while implementing a feature unless the task explicitly authorises them.
- Do not claim a build, test, package or editor result that was not actually run.
- Never hand-edit `.uasset` or `.umap` files. Treat World Partition files under `Content/__ExternalActors__` and `Content/__ExternalObjects__` as required parts of their map.
- Do not delete HLOD or external-actor assets merely because their names are opaque.
- Do not commit `Binaries/`, `DerivedDataCache/`, `Intermediate/`, `Saved/`, `.vs/`, generated `.sln` or generated `.slnx` files.
- Do not copy characters, dialogue, levels, music, UI, names or other protected expression from reference games or television programmes. References are directional only.
- Do not imitate a living artist or a protected soundtrack. Create an original dramatic stealth and social-suspense score.
- Do not expand to multiplayer, browser, mobile, VR or console production until the PC vertical slice is stable.

## Standard task workflow

1. Read the relevant authoritative documents and code.
2. Perform the mandatory routing check.
3. Either issue the correct delegation prompt or restate the task, assumptions, files in scope and acceptance criteria.
4. Create a dedicated branch or worktree. Never implement directly on `main`.
5. Write or update tests before or with the implementation where practical.
6. Make the smallest coherent change that satisfies the task.
7. Build the editor target.
8. Run the relevant automation tests.
9. Review the diff for unintended files, especially binary assets and config churn.
10. Use an independent review agent or model; the implementing agent does not self-approve.
11. Update documentation and the roadmap when behaviour, architecture or milestone status changes.
12. Open a pull request with evidence of build and test results.
13. Merge only after human approval.

Use `docs/production/TASK_BRIEF_TEMPLATE.md` for every non-trivial task.

## Commands

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

The manual editor test route is `Tools -> Session Frontend -> Automation`, then run the `TheVeil` group.

## Definition of done

A task is not done until it includes:

- the intended behaviour
- all required source, config and content files
- build evidence
- relevant automated or manual test evidence
- no unexpected warnings or unrelated file changes
- an independent review verdict
- updated integration notes where needed
- known limitations
- a clear next task or delegation prompt

## Current production priority

The social-simulation foundation is ahead of the playable shell. The next priority is a responsive third-person character, camera, input, GameMode, PlayerStart and interaction loop in `L_Dev_Sandbox`, followed by embodied NPCs and an interactive Tribunal. Do not jump to final art before these gates pass.
