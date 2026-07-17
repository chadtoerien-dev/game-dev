# Grok 4.5 Routing for The Veil

Last updated: 17 July 2026

## Status

Grok 4.5 is available to the project through Cursor and is part of the active model roster.

Cursor describes it as a larger first-party model designed for difficult, long-running tasks that require creative tool use across software engineering and broader computer work. Cursor also states that Grok 4.5 and Composer 2.5 are different model weight classes. The Veil therefore uses them for different jobs rather than treating one as a universal replacement for the other.

This document extends `AI_AGENT_WORKFLOW.md`. When a Cursor agent sees a suitable task, it must identify Grok 4.5 explicitly during the mandatory routing check.

## Assigned role

**Primary role:** autonomous integration agent and long-running repository investigator.

Use Grok 4.5 for:

- multi-step investigations that require repeated repository search, terminal use, log inspection and verification
- end-to-end integration work after the architecture and task boundaries are approved
- cross-document and cross-subsystem consistency audits
- branch readiness checks before independent review
- coordinating build, test and evidence loops inside Cursor
- tracing intermittent or causally unclear bugs across code, config and tooling
- producing precise handoffs when a task must move to another specialist
- comparing implementation evidence against an approved task brief
- broad but bounded project-management or engineering tasks that involve more than code generation

Grok 4.5 may implement code when the task is bounded, the approved plan is clear and the file ownership is exclusive. It is not automatically the architecture authority merely because it is a larger model.

## What Grok 4.5 does not replace by default

- **GPT-5.6 Sol remains lead architect** for social simulation, NPC knowledge boundaries, voting logic, save authority and high-consequence design reasoning.
- **Claude Sonnet 5 remains the default Unreal C++ implementer** for sustained reflection, object-lifecycle and engine-integration work until project evidence shows another model is more reliable.
- **Cursor Composer 2.5 remains the fast bounded implementer** for scripts, tests, mechanical migrations and small or medium changes with a known plan.
- **Gemini remains the primary multimodal and visual reviewer** for screenshots, video, level presentation, motion concepts and visual bugs.
- **The human owner remains the Unreal Editor operator** for `.uasset`, `.umap`, World Partition, HLOD and irreversible content changes.

Grok 4.5 must never review its own implementation as the independent reviewer.

## Routing matrix additions

| Task | Recommended Grok 4.5 role | Other required roles |
|---|---|---|
| Long-running multi-step repository investigation | Primary investigator | GPT-5.6 or Claude reviews conclusions when architecture is affected |
| Cross-subsystem integration after an approved plan | Integration implementer or coordinator | Original planner remains separate; independent reviewer required |
| Branch readiness and evidence audit | Reviewer, only when Grok did not implement | Human checks the visible result and PR file list |
| Intermittent build or tooling failure | Investigator after two bounded Composer/Sonnet attempts | Escalate architecture questions to GPT-5.6 or Opus |
| Documentation, migration and workflow orchestration | Planner or implementer | Different model family reviews |
| Social AI, memory, claims or voting architecture | Secondary critic or test adversary | GPT-5.6 plans; Claude implements |
| Unreal C++ feature with reflection/lifecycle risk | Plan critic or integration reviewer | Claude Sonnet implements unless a controlled benchmark promotes Grok |
| Visual, animation or cinematic review | Not primary | Gemini and human review |
| Map or World Partition asset editing | Not operator | Human edits in Unreal Editor; Gemini reviews captures |

## Standard Grok 4.5 handoff prompt

```text
DESTINATION AGENT: Cursor Grok 4.5
ROLE: autonomous integration agent and repository investigator
TASK: <bounded task title>

You are working on The Veil, an Unreal Engine 5.8 project using agent-led development with human-in-the-loop supervision.

Read first:
- AGENTS.md
- docs/context/AGENT_HANDOFF.md
- docs/production/ROADMAP.md
- docs/production/AI_AGENT_WORKFLOW.md
- docs/production/AGENT_DELEGATION_PROTOCOL.md
- docs/production/GROK_4_5_ROUTING.md
- <approved task brief and relevant source/tests>

Before editing, return the mandatory routing response:
1. task classification;
2. recommended role;
3. best model/agent;
4. why;
5. proceed or delegate;
6. exact human checkpoint.

Objective:
<one observable outcome>

Constraints:
- Work on a dedicated branch or worktree.
- Do not edit main directly.
- Do not hand-edit Unreal binary assets.
- Do not broaden scope.
- Do not overwrite another agent's file ownership.
- Do not claim build, test or PIE success without actual evidence.
- Stop and delegate architecture, visual or binary-asset work to the correct specialist when required.

Required verification:
- powershell -ExecutionPolicy Bypass -File ".\scripts\Build-TheVeil.ps1"
- powershell -ExecutionPolicy Bypass -File ".\scripts\Run-TheVeilTests.ps1"
- git diff --check
- git status --short
- <task-specific manual or visual evidence>

Return:
1. investigation or implementation summary;
2. files changed or proposed;
3. actual commands run and results;
4. unresolved risks;
5. precise human editor steps;
6. a ready-to-paste prompt for the next specialist agent when needed;
7. exact material for independent review.
```

## Use on Playable Foundation 0.5

For milestone 0.5, the recommended sequence is:

1. GPT-5.6 Sol: architecture and acceptance plan.
2. Claude Sonnet 5: primary Unreal C++ implementation.
3. Grok 4.5: integration pass across the approved branch, build/test evidence, missing-step detection and handoff preparation. Grok must not rewrite the architecture without approval.
4. Gemini: screenshot or video review of camera feel, movement and collision.
5. Chad: Unreal Editor asset creation, PIE validation and merge approval.

Composer 2.5 may handle bounded follow-up fixes, test scaffolding and scripts after the root cause and interface are known.

## Controlled benchmark before promotion

Grok 4.5 is provisionally assigned rather than declared universally superior. During milestone 0.5, run one representative comparison against Claude Sonnet 5 or Composer 2.5 using the same bounded task and record:

- correctness
- iterations required
- unnecessary file changes
- Unreal-specific mistakes
- build and test success
- quality of terminal/tool use
- quality of self-verification
- latency and usage cost
- clarity of delegation and human instructions

Promote Grok 4.5 to a primary role only when The Veil's own evidence supports it.

## Source basis

- Cursor product page: `https://cursor.com/grok-4-5`
- Cursor announcement: `https://cursor.com/blog/grok-4-5`

These sources describe current availability and intended capability. Project-specific role assignments above are The Veil production decisions and must be updated after controlled benchmarks.