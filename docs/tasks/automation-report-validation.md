# Validate automation evidence before reporting success

Date: 16 September 2026
Status: implemented and independently approved; merge approval pending

## Objective and production value

Prevent a successful editor process exit from being mistaken for a complete passing test run. The existing runner does not inspect the exported report.

## Read first

AGENTS.md, playable-level-delivery.md, AI_AGENT_WORKFLOW.md, AGENT_DELEGATION_PROTOCOL.md, TASK_BRIEF_TEMPLATE.md and the existing UE 5.8 index.json reports.

## Required behaviour and acceptance

Require a newly created report, a non-empty test array, exact expected test names derived from current literal simple C++ registrations, successful states, zero warnings/errors/skips/in-process tests and consistent summary counts. Reject missing, malformed, stale, duplicate, incomplete and inconsistent reports. Target filters must match at least one registered test; every requested filter term must match. Preserve substring, + unions, StartsWith: and ^/$ anchors. Fail explicitly for unsupported group filters or registration forms rather than claiming coverage.

## Scope and files

Only scripts/Run-TheVeilTests.ps1, scripts/Automation/validate_report.py, scripts/Automation/test_validate_report.py and this brief. No gameplay, build configuration, binary assets or save changes. Use Python 3 standard library, available locally as Python 3.12.10. Work on codex/playable-level-delivery; no commit or merge.

## Verification

Run `python -m unittest discover -s scripts/Automation -p test_validate_report.py`, then `powershell -ExecutionPolicy Bypass -File scripts/Run-TheVeilTests.ps1`, inspect its report and run `git diff --check`. Parent owns build verification and independent review. No new editor acceptance is required for this tooling change; Chad retains merge authority.

## Risks and rollback

Source discovery intentionally supports literal IMPLEMENT_SIMPLE_AUTOMATION_TEST registrations, not dynamic/complex tests. New registration styles need a deliberate discovery extension. A Python 3 interpreter is required; the runner exposes PythonExecutable. No fixed suite count is used. Revert only this bounded diff to roll back.

## Routing

Bounded tooling implementation by current Codex agent, parent coordinating an independent review agent. Current agent has local PowerShell, Python and UE access. Other preferred workflow tools are not available in this session. Human checkpoint: review evidence and approve any subsequent merge.

## Verification evidence

- Python regression suite: seven methods with parameterised failure cases passed on 16 September 2026. Includes malformed/missing/duplicate JSON, empty and duplicate results, coverage mismatch, stale timestamps, unsuccessful states, warning/error entries, summary inconsistencies and invalid filters.
- Full hardened runner exited 0: Saved/AutomationReports/20260916-224218-314-fd30271d/index.json; validator confirmed 21 successes, zero warnings, failures or skips.
- Targeted `^TheVeil.Claims.DirectContradiction$` runner exited 0: Saved/AutomationReports/20260916-224310-748-c8a35f84/index.json; exactly one success, zero warnings, failures or skips.
- `TheVeil.DoesNotExist` runner exited 1 before launching Unreal. Console evidence: Saved/AutomationReports/runner-zero-match-console.log.
- Independent review: parent reports review_baseline_tooling APPROVE with no blocking findings; reviewer independently reran the seven regression methods and checked filter semantics against installed UE source.
- Build evidence is owned by the parent compiler-selection task. This task changes no C++ or assets and makes no PIE, rendering or packaged-build acceptance claim.
- Known limitation: registration discovery is intentionally strict and source-based; it is not a C++ preprocessor. Conditional/complex registrations or configured Group: filters need explicit discovery support before using this runner. Runtime/plugin tests outside Source are not supported by its coverage contract. No fixed test count is embedded.
