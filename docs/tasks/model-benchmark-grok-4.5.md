# Task Brief — Benchmark Grok 4.5 on Playable Foundation 0.5

## Objective

Evaluate Grok 4.5 on one real, bounded The Veil integration task before changing the permanent model-routing hierarchy.

## Recommended benchmark task

After the Playable Foundation 0.5 implementation branch exists, ask Grok 4.5 to perform a non-destructive integration and evidence audit.

It must:

- read the approved task brief and plan
- inspect the full branch diff
- compare changed files with the approved scope
- inspect build and automation outputs
- identify missing editor steps, tests or documentation
- flag Unreal reflection, lifecycle, input and camera risks
- produce a ready-to-paste prompt for the next specialist when changes are required
- not edit files during the first pass

## Comparison

Run the same audit prompt through one other capable model, preferably GPT-5.6 or Claude Sonnet 5 in review-only mode.

Record:

- correct issues found
- false positives
- missed acceptance criteria
- quality of repository and terminal reasoning
- quality of Unreal-specific analysis
- clarity of delegation prompts
- unnecessary proposed changes
- time and usage cost

## Acceptance

The benchmark is complete when:

1. both model outputs are preserved in the task or PR;
2. Chad records which findings were valid;
3. the routing decision is updated in `docs/production/GROK_4_5_ROUTING.md` when evidence justifies a change;
4. Grok 4.5 is not used to independently approve work it implemented.

## Prompt

```text
DESTINATION AGENT: Cursor Grok 4.5
ROLE: independent integration auditor
TASK: Audit the Playable Foundation 0.5 branch against its approved brief

Read:
- AGENTS.md
- docs/context/AGENT_HANDOFF.md
- docs/production/ROADMAP.md
- docs/production/AI_AGENT_WORKFLOW.md
- docs/production/AGENT_DELEGATION_PROTOCOL.md
- docs/production/GROK_4_5_ROUTING.md
- docs/tasks/0.5-playable-foundation.md
- the approved implementation plan
- the branch diff
- build and test logs

Do not edit files in the first pass.

Return:
1. mandatory routing response;
2. acceptance-criteria coverage table;
3. blockers with exact file/line or missing evidence;
4. Unreal-specific lifecycle, reflection, Enhanced Input, movement and camera risks;
5. unintended files or scope drift;
6. missing tests, editor steps or documentation;
7. verdict: APPROVE, REQUEST CHANGES, or INSUFFICIENT EVIDENCE;
8. a complete ready-to-paste prompt for the correct implementation agent when changes are required.
```
