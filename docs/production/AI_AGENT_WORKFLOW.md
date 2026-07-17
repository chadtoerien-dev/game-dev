# AI Agent Workflow and Model Routing

Last updated: 17 July 2026

## Purpose

This document defines how The Veil uses multiple AI coding and creative agents without turning the repository into a collection of conflicting edits. Model names change quickly, so assignments are expressed as durable capability roles with the current preferred models listed as a dated snapshot.

The governing rule is simple:

> One agent plans, one agent implements, a different agent reviews, the human owner approves, and automated evidence decides whether the work is mergeable.

No model is trusted because of brand, geography or a benchmark alone. Route work by task type, blast radius, tool access, current reliability and cost.

## Human authority

**Chad Toerien is Product Owner, Game Director and final merge authority.**

Only the human owner may approve:

- scope changes
- visual direction
- narrative canon
- player-facing terminology
- irreversible map or asset changes
- final music and voice direction
- pull-request merge to `main`

AI agents may recommend but must not silently redefine these areas.

## Current model roster

This is a routing snapshot, not a permanent ranking. Use the newest equivalent model when a listed model is retired, and update this section after a controlled comparison on real The Veil tasks.

### 1. Lead architect and difficult reasoning

**Primary:** OpenAI GPT-5.6 Sol at High, XHigh or Max reasoning

Use for:

- gameplay-system architecture
- social-simulation and NPC reasoning rules
- information-provenance logic
- save-state and phase-authority design
- difficult root-cause debugging
- reviewing large architectural diffs
- acceptance criteria and adversarial edge cases
- synthesising product, technical and production constraints

Do not spend this model on repetitive renames or boilerplate.

### 2. Primary Unreal C++ implementer

**Primary:** Claude Sonnet 5 in Cursor or Claude Code

**Escalation:** Claude Opus 4.8 for the hardest brownfield refactors, subtle engine integration bugs or very long autonomous investigations

Use for:

- sustained multi-file C++ implementation
- Unreal module and reflection work
- refactoring within an explicitly bounded subsystem
- tracing existing code paths before editing
- writing tests alongside implementation
- debugging compiler and linker failures
- careful changes in mature or unfamiliar code

Sonnet is the default implementation model. Escalate to Opus only when the task justifies the cost or Sonnet has failed with a reproducible blocker.

### 3. Fast implementation and iteration

**Primary:** Cursor Composer 2.5

Use for:

- small and medium bounded features
- test scaffolding
- repetitive but coherent cross-file edits
- build scripts and developer tooling
- documentation updates
- mechanical API migrations
- quick build-fix loops after the root cause is understood
- implementing an already approved plan

Composer is the default for interactive speed, not the final architecture authority. For high-blast-radius work, require a prior GPT-5.6 or Claude plan and an independent review.

### 4. Long-context and multimodal reviewer

**Primary:** Gemini 3.5 Flash or the latest Gemini Pro model available in the chosen tool

Use for:

- loading large portions of the repository and documentation at once
- cross-document consistency checks
- reviewing screenshots, video captures, audio references and PDFs
- analysing level-editor screenshots and visual bugs
- finding omissions across long specifications
- independent test-case generation
- performance-log and trace summarisation

Gemini is a reviewer and visual-analysis specialist by default, not the sole author of core gameplay architecture.

### 5. Visual concept and previsualisation

**Primary tools:** current Gemini Image models for still concepts; Gemini Omni Flash or Veo 3.1 for motion concepts and cinematic previsualisation

Use for:

- mood boards
- original environment and costume exploration
- shot planning
- lighting studies
- UI look-development
- motion and camera previsualisation
- storyboards and trailer concepts

Generated media is reference material only unless the human owner explicitly clears it for production use. It must not reproduce protected characters, locations, music or distinctive audiovisual expression from reference works.

### 6. Cost-efficient independent second opinion

**Primary:** DeepSeek V4 Pro or V4 Flash

Use for:

- alternative implementation proposals
- code review on a clean diff
- test expansion
- large-context summarisation
- low-cost repetitive analysis
- checking whether another agent made an unjustified assumption

DeepSeek must not be the only reviewer for security-sensitive, architecture-wide or binary-asset changes. Treat vendor model identifiers as volatile and update them when deprecated.

## Task routing matrix

| Task | Planner | Implementer | Independent reviewer | Human evidence |
|---|---|---|---|---|
| Social AI, voting, memory or provenance logic | GPT-5.6 Sol | Claude Sonnet 5 | Gemini or DeepSeek V4 | Test output and debug explanation |
| UE C++ feature across several files | GPT-5.6 Sol or Claude Opus | Claude Sonnet 5 | GPT-5.6 or Gemini | Successful build and targeted tests |
| Small bounded C++ change | Composer 2.5 or Sonnet | Composer 2.5 | Sonnet or GPT-5.6 | Build and relevant test |
| Compiler/linker/build failure | Sonnet or Composer diagnosis | Composer or Sonnet | GPT-5.6 if unresolved | Full command output |
| Build scripts, CI and developer tooling | GPT-5.6 or Sonnet | Composer 2.5 | DeepSeek or Sonnet | Clean run on Windows |
| Blueprint/editor procedure | GPT-5.6 or Gemini plan | Human in Unreal Editor | Gemini screenshot review | Screenshots and PIE result |
| Map, World Partition or binary asset work | GPT-5.6 plan | Human in Unreal Editor | Gemini visual review | Saved assets, map load and smoke test |
| Visual concept stills | Human art brief | Gemini Image | GPT-5.6 and human art review | Originality and usability check |
| Motion concept/previs | Human shot brief | Gemini Omni Flash or Veo 3.1 | Gemini analysis plus human review | Shot list and source prompts |
| Documentation and handover | Gemini or GPT-5.6 | Composer 2.5 | GPT-5.6 or Claude | Link and consistency check |
| Roadmap and scope decisions | GPT-5.6 | Human owner | Claude or Gemini critique | Human approval |

## Standard delivery pipeline

### Stage 1 — Task brief

Create a task from `TASK_BRIEF_TEMPLATE.md`. It must contain:

- objective
- business or player value
- files and systems in scope
- files and systems explicitly out of scope
- acceptance criteria
- required build and test commands
- primary implementer and independent reviewer
- risks and rollback plan

A vague task is not ready for an agent.

### Stage 2 — Plan-only pass

The planning agent must not edit files. It should:

1. read the authoritative documents
2. inspect the relevant code and tests
3. identify dependencies and risks
4. propose the smallest coherent implementation
5. list exact files expected to change
6. define verification and rollback

The human owner approves or corrects the plan before implementation when the task changes architecture, content scope or more than five files.

### Stage 3 — Isolated implementation

- Create a branch such as `feature/player-locomotion` or `fix/save-load-logging`.
- Prefer a separate Git worktree for each concurrent agent.
- Give one agent ownership of a file set. Two agents must not edit the same files in parallel.
- Commit small, logically complete checkpoints.
- Keep generated Unreal folders out of Git.
- Never let an agent hand-edit `.uasset`, `.umap`, external-actor or external-object files.

### Stage 4 — Local verification

The implementing agent must run, or provide the human with exact commands to run:

1. `scripts/Build-TheVeil.ps1`
2. relevant automation tests, normally `scripts/Run-TheVeilTests.ps1`
3. a Play-in-Editor smoke test for player-facing work
4. `git diff --check`
5. `git status --short`

Save logs for failures. Do not summarise away the first compiler or test error.

### Stage 5 — Independent review

Use a different provider or model family from the implementer where practical. The reviewer receives:

- task brief
- approved plan
- diff
- build output
- test output
- any screenshots or video

The reviewer checks:

- correctness against acceptance criteria
- hidden information leaks in NPC logic
- Unreal lifecycle and reflection risks
- save compatibility
- deterministic behaviour where required
- performance and memory implications
- unrelated edits or config churn
- missing tests
- originality and licensing boundaries

The reviewer may approve, request changes or state that evidence is insufficient. It must not merge.

### Stage 6 — Human acceptance and merge

The human owner:

1. reviews the visible behaviour
2. checks the PR file list
3. confirms build and test evidence
4. confirms no unintended binary assets were changed
5. approves the scope outcome
6. merges to `main`

After merge, update the handover, roadmap and changelog when the project state changed materially.

## Parallel-agent rules

Parallel work is allowed only when workstreams have clean boundaries.

Good parallel split:

- Agent A: player movement C++
- Agent B: movement automation tests
- Agent C: camera design review
- Human: Blueprint and editor setup

Bad parallel split:

- two agents editing the same character class
- one agent changing interfaces while another implements against the old interface
- multiple agents modifying `DefaultEngine.ini`
- simultaneous map edits

For dependent streams, merge the shared interface first, then rebase each implementation branch.

## Context-window discipline

Do not recreate a giant permanent chat. Repository files are the memory system.

For every new task:

1. start a fresh agent thread
2. attach `AGENTS.md`
3. attach the task brief
4. attach only the relevant architecture and source files
5. ask the agent to identify missing context before coding
6. keep decisions in Git, not only in chat
7. close the thread when the PR is merged

Long-context models may audit the whole repository, but implementation prompts should remain bounded.

## Escalation rules

Escalate from Composer to Sonnet when:

- the same build error survives two grounded attempts
- the change crosses subsystem boundaries
- Unreal reflection, lifetime or replication behaviour is unclear
- the task requires sustained investigation rather than mechanical edits

Escalate from Sonnet to GPT-5.6 or Opus when:

- architecture is disputed
- save compatibility could break
- AI knowledge boundaries may be violated
- the diff exceeds the approved scope
- the bug is intermittent or causally unclear

Stop and return to the human owner when:

- the task requires final art or narrative canon
- a map or binary asset may be lost
- the model proposes copying a reference work
- requirements conflict
- build or test evidence cannot be obtained

## Model evaluation policy

Model routing is provisional. Once per major milestone, run the same small, representative task through candidate models and record:

- correctness
- number of iterations
- unnecessary file changes
- build/test success
- latency
- token or request cost
- quality of explanation

Update this document based on The Veil's real results, not marketing claims alone.
