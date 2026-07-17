# Agent-Led Delegation Protocol

Last updated: 17 July 2026

## Operating model

The Veil uses **agent-led development with human-in-the-loop supervision**.

AI agents are expected to do more than answer the immediate request. Every agent must first identify the type of work, judge whether it is the right agent for that work, and either proceed within its assigned role or hand the work to the correct specialist with a ready-to-use prompt.

The human owner, Chad Toerien, supervises scope, approves irreversible choices, performs Unreal Editor work that requires binary assets, validates visible behaviour, and authorises merges to `main`. The human should not have to manually invent the next agent prompt or decide routing from scratch.

## Mandatory routing check

Before planning or editing, every agent must state:

1. **Task classification** — architecture, Unreal C++, bounded implementation, build/debugging, editor/binary asset, visual, audio, documentation, review, or production decision.
2. **Recommended role** — planner, implementer, reviewer, visual reviewer, or human editor operator.
3. **Best available agent/model** — selected from the routing matrix in `AI_AGENT_WORKFLOW.md`.
4. **Why** — one or two sentences based on task complexity, tool access, blast radius and evidence required.
5. **Proceed or delegate** — whether the current agent should continue.
6. **Human checkpoint** — the exact decision, command, editor step or evidence the human must provide.

An agent must not continue merely because it can produce an answer. It should continue only when it is the correct role for the task and has the required tools and context.

## Mandatory delegation behaviour

When another agent is better suited, the current agent must stop before implementation and provide a complete handoff package containing:

- destination agent/model
- role: planner, implementer or reviewer
- objective and player/production value
- current verified state
- authoritative documents to read
- files and systems in scope
- explicit non-goals
- acceptance criteria
- verification commands
- risks and rollback expectations
- expected output format
- a copy/paste prompt for the destination agent
- the evidence that must return to the originating task record

The current agent should also tell Chad exactly where to run the prompt, for example Cursor Agent, Claude Code, ChatGPT, Gemini, or an independent review thread.

## Delegation prompt format

Use this structure:

```text
DESTINATION AGENT: <model/tool>
ROLE: <planner | implementer | reviewer | visual reviewer>
TASK: <bounded task title>

You are working on The Veil, an Unreal Engine 5.8 project.

Read first:
- AGENTS.md
- docs/context/AGENT_HANDOFF.md
- docs/production/ROADMAP.md
- docs/production/AI_AGENT_WORKFLOW.md
- docs/production/AGENT_DELEGATION_PROTOCOL.md
- <task-specific documents and files>

Verified starting state:
<facts only; include build/test/map status where relevant>

Objective:
<one observable outcome>

In scope:
- ...

Out of scope:
- ...

Acceptance criteria:
1. ...

Required verification:
- powershell -ExecutionPolicy Bypass -File ".\scripts\Build-TheVeil.ps1"
- powershell -ExecutionPolicy Bypass -File ".\scripts\Run-TheVeilTests.ps1"
- <manual or visual evidence>

Constraints:
- Work on a dedicated branch/worktree.
- Do not edit main directly.
- Do not hand-edit Unreal binary assets.
- Do not broaden scope.
- Do not claim success without actual evidence.

Return:
1. diagnosis or implementation summary;
2. files changed or proposed;
3. build/test evidence;
4. manual steps for Chad;
5. risks and limitations;
6. exact material for independent review.
```

## Self-routing examples

### Social AI or voting logic

- Planner: GPT-5.6 Sol
- Implementer: Claude Sonnet 5
- Reviewer: Gemini or DeepSeek V4
- Human: inspect debug explanation and approve behavioural intent

A Composer agent receiving this task should not redesign the architecture. It should generate the GPT-5.6 planning prompt and tell Chad to run that first.

### Multi-file Unreal C++ feature

- Planner: GPT-5.6 Sol or Claude Opus
- Implementer: Claude Sonnet 5
- Reviewer: GPT-5.6 or Gemini
- Human: run editor/PIE checks and approve merge

### Small bounded fix with known root cause

- Planner/implementer: Cursor Composer 2.5
- Reviewer: Claude Sonnet or GPT-5.6
- Human: run the supplied verification commands

### Blueprint, map or World Partition work

- Planner: GPT-5.6 or Gemini
- Operator: Chad in Unreal Editor
- Reviewer: Gemini using screenshots/video

The planning agent must provide exact editor steps. It must not pretend that text edits can replace required `.uasset`, `.umap`, external actor or HLOD changes.

### Visual or motion concept

- Brief and approval: Chad
- Generation/previsualisation: current Gemini Image, Gemini Omni or Veo model
- Review: Gemini analysis plus GPT-5.6 or human originality review

### Documentation, scripts and tooling

- Planner: GPT-5.6, Gemini or Claude
- Implementer: Cursor Composer 2.5
- Reviewer: a different model family

## Human-in-the-loop checkpoints

Human approval is required before:

- changing product scope, narrative canon or terminology
- committing to a visual, audio or character direction
- changing architecture across subsystems
- modifying save compatibility
- editing or deleting maps, World Partition assets or other binary content
- accepting a model-generated media asset for production use
- merging any pull request to `main`

The agent must ask for the smallest possible human decision. Do not return a vague request such as “please review”. State the exact choice or evidence required.

## Agent-to-agent evidence loop

The destination agent must return its output to the durable task record, not only to a chat window. At minimum preserve:

- approved plan
- implementation commit or diff
- build and test outputs
- manual PIE or editor evidence
- independent review verdict
- human acceptance decision

After merge, update the roadmap, handover and integration notes when the project state changed.

## Failure and escalation

An agent must stop and delegate when:

- it lacks repository or editor access required to complete the task
- the task belongs to a different specialist role
- the same grounded error survives two attempts
- the requested work exceeds the approved task brief
- evidence is unavailable
- requirements conflict
- an irreversible asset change is proposed

The correct output in these cases is a precise delegation prompt and human checkpoint, not speculative implementation.
