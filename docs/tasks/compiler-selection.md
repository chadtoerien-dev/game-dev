# Enforce the requested MSVC compiler

Date: 16 September 2026

## Objective and value

Make the build helper select the compiler it reports, so a successful build cannot silently certify a different compiler baseline.

## Current and required behaviour

The helper passes -VCToolchainVersion. Installed UE 5.8 source UEBuildWindows.cs documents that option as supporting MSVC tools when the compiler is not MSVC. VCEnvironmentParameters selects the MSVC compiler using CompilerVersion. The actual machine has only 14.51.36231 (compiler 14.51.36256); an explicit -CompilerVersion=14.50.35717 diagnostic fails with "Unable to find valid 14.50.35717 C++ toolchain".

Use -CompilerVersion for the existing ToolchainVersion parameter. Preserve its default 14.50.35717 and caller compatibility. Missing requested compilers must fail; no silent fallback or approved-baseline change.

## Read first and scope

Read AGENTS.md, docs/context/AGENT_HANDOFF.md, docs/production/AI_AGENT_WORKFLOW.md, docs/tasks/playable-level-delivery.md and scripts/Build-TheVeil.ps1. Changes: this brief and the single compiler-selection argument in scripts/Build-TheVeil.ps1. No gameplay, engine, installer, global configuration or binary asset edits.

## Acceptance criteria

1. The helper passes -CompilerVersion with the caller's value.
2. Default invocation fails clearly when the documented compiler is unavailable.
3. Explicit installed-version override is honoured and verified in build output.
4. Do not claim an experimental compiler build changes the approved project baseline.

## Verification

Run the helper with its default, then with -ToolchainVersion 14.51.36231. Inspect actual compiler output and exit codes. Parent diagnostic clean rebuild uses the installed version explicitly; record warning and compile/link evidence separately. Run relevant automation after compilation and git diff --check. No new behaviour-mirroring unit test is needed for a single flag; actual UBT selection is the meaningful check.

## Risks, rollback and routing

The corrected default exposes the missing installation as a failure rather than continuing with a different compiler. Restore/install the approved compiler or obtain a separately verified baseline decision before claiming release reproducibility. Revert only this argument change if required, retaining evidence of why the former success was misleading.

Planner/implementer: current lead Codex agent for this bounded diagnosed tooling correction. Independent reviewer: separate agent. Human acceptance: compiler-baseline decision if changing from the approved version, and eventual merge approval.
