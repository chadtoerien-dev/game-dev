# Task Brief and Agent Prompt Templates

Copy this file into a GitHub issue, Linear task or a temporary task document. Complete every required field before asking an implementation agent to edit code.

---

## Task brief

### Title

`<verb + bounded outcome>`

### Player or production value

`Why this work matters now.`

### Objective

`One observable outcome. Avoid combining unrelated features.`

### Current behaviour

`What happens today, including exact errors or limitations.`

### Required behaviour

`What must happen after the change.`

### Read first

- `AGENTS.md`
- `docs/context/AGENT_HANDOFF.md`
- `docs/production/ROADMAP.md`
- `<relevant design/architecture/integration docs>`
- `<relevant source and test files>`

### In scope

- `<system/file/behaviour>`
- `<system/file/behaviour>`

### Out of scope

- `<explicit non-goal>`
- `<explicit non-goal>`

### Constraints

- Unreal Engine 5.8
- Windows x64
- MSVC toolchain 14.50.35717
- British English
- Preserve save compatibility unless explicitly approved otherwise
- Preserve NPC information boundaries
- Do not hand-edit Unreal binary assets
- No unrelated refactors

### Expected files to change

- `<path>`
- `<path>`

Any additional file requires an explanation before editing.

### Acceptance criteria

1. `<observable criterion>`
2. `<observable criterion>`
3. `<failure case handled>`
4. `<debug or explainability requirement>`

### Verification

```powershell
powershell -ExecutionPolicy Bypass -File ".\scripts\Build-TheVeil.ps1"
```

```powershell
powershell -ExecutionPolicy Bypass -File ".\scripts\Run-TheVeilTests.ps1"
```

Additional test or editor procedure:

`<exact command or step-by-step test>`

### Risks

- `<technical risk>`
- `<content or scope risk>`

### Rollback

`How to restore the previous working state.`

### Agent routing

- Planner: `<model>`
- Implementer: `<model>`
- Independent reviewer: `<different model/provider>`
- Human acceptance owner: Chad Toerien

---

## Planner prompt

```text
You are the planning agent for The Veil, an Unreal Engine 5.8 project.

Read AGENTS.md, docs/context/AGENT_HANDOFF.md, docs/production/ROADMAP.md, this task brief, and the referenced source/tests.

Do not edit files and do not run destructive commands.

Produce:
1. a concise diagnosis of the current state;
2. assumptions and missing information;
3. the smallest coherent implementation plan;
4. exact files expected to change and why;
5. Unreal-specific lifecycle, reflection, save and content risks;
6. tests to add or update;
7. exact build and verification commands;
8. rollback steps.

Flag any conflict with the task brief or project authority documents. Do not expand scope.
```

## Implementer prompt

```text
You are the implementation agent for The Veil.

Read AGENTS.md, docs/context/AGENT_HANDOFF.md, the approved task brief and the approved plan. Work on the assigned branch/worktree only.

Before editing:
- run git rev-parse --show-toplevel and confirm this repository;
- run git status --short;
- inspect every file listed in scope;
- restate the acceptance criteria.

Implement only the approved plan. Keep edits minimal and coherent. Do not hand-edit .uasset, .umap, World Partition external actor/object files, or generated Unreal directories.

Add or update tests where practical. Then run the required build and test commands. Do not claim success unless you inspected the actual output.

Return:
1. summary of behaviour changed;
2. files changed;
3. build result;
4. test result;
5. manual editor steps still required;
6. known limitations;
7. exact diff or commit ready for independent review.
```

## Independent reviewer prompt

```text
You are the independent reviewer for The Veil. You did not implement this change.

Read AGENTS.md, the task brief, approved plan, relevant architecture documents, the diff, build output and test output.

Review for:
- compliance with acceptance criteria;
- unintended scope or file changes;
- Unreal reflection, object lifetime, subsystem and save risks;
- deterministic behaviour where required;
- NPC information leaks or use of hidden simulation truth;
- test coverage and failure handling;
- performance and content-pipeline implications;
- originality and licensing boundaries;
- accuracy of the claimed build/test evidence.

Do not edit files in the first pass.

Return one verdict: APPROVE, REQUEST CHANGES, or INSUFFICIENT EVIDENCE.
For every issue, cite the file and line or the exact missing evidence. Distinguish blockers from non-blocking suggestions.
```

## Visual and editor review prompt

```text
You are the visual and Unreal Editor review agent for The Veil.

Use the task brief, screenshots/video, map name, performance data and project visual direction. Do not infer success from source code alone.

Assess:
- camera readability and responsiveness;
- player navigation and collision;
- lighting and visual hierarchy;
- animation continuity and foot sliding;
- interaction affordances;
- UI legibility;
- frame pacing and obvious performance issues;
- whether the result is original rather than a copy of a reference work.

Return a prioritised list with: blocker, important, polish. Include precise editor checks the human should perform next.
```

## Pull-request evidence block

Paste this into every non-trivial PR:

```markdown
## Scope
- [ ] Matches the approved task brief
- [ ] No unrelated refactors
- [ ] No unexpected binary assets

## Verification
- Build command:
- Build result:
- Tests run:
- Test result:
- PIE/manual test:
- Screenshots/video/logs:

## Review
- Implementing model/agent:
- Independent reviewing model/agent:
- Review verdict:

## Risks and limitations
-

## Documentation
- [ ] Handover updated if project state changed
- [ ] Roadmap updated if milestone status changed
- [ ] Integration notes updated if setup changed
```
