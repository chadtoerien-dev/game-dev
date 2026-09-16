# Playable level baseline verification

Date: 16 September 2026
Branch: codex/playable-level-delivery
Gameplay source baseline: 75cd57d; this increment changes tooling and delivery documentation only.

## Compiler selection diagnosis

Installed UE 5.8 UEBuildWindows.cs binds -CompilerVersion to the compiler version. It describes -VCToolchainVersion as the supporting MSVC version for a non-MSVC compiler. VCEnvironment.cs selects MSVC using CompilerVersion.

The machine exposes 14.51.36231 (compiler 14.51.36256). An explicit -CompilerVersion=14.50.35717 build returned "Unable to find valid 14.50.35717 C++ toolchain for VisualStudio2026 x64". Therefore earlier up-to-date build success did not verify the documented compiler pin.

The build helper now passes -CompilerVersion while preserving its ToolchainVersion parameter and default. Verification through the changed helper:

- Default invocation: failed as expected; requested compiler unavailable. Log: Saved/BuildLogs/TheVeilEditor-Development-20260916-224126.log.
- Explicit -ToolchainVersion 14.51.36231: succeeded, up to date. Log: Saved/BuildLogs/TheVeilEditor-Development-20260916-224128.log.

## Actual recompilation

Command (diagnostic override, not a baseline approval):

```powershell
& 'C:/Program Files/Epic Games/UE_5.8/Engine/Build/BatchFiles/Build.bat' TheVeilEditor Win64 Development '-Project=C:/devprojects/game-dev/TheVeil.uproject' -WaitMutex -2026 '-CompilerVersion=14.51.36231' -architecture=x64 -Rebuild -MaxParallelActions=1 -NoUBA
```

Log: Saved/BuildLogs/TheVeilEditor-installed-compiler-rebuild-20260916.log.

Result: succeeded in 87.71 seconds. Six actions included project module compilation, library and DLL linking, resource compilation and target metadata. The log identifies compiler 14.51.36256 and Windows SDK 10.0.22621.0.

Warnings remain: newer/non-preferred compiler, unavailable Visual Studio SDK editor integration and deprecated APIs in engine headers. This is not a warning-free release certification or a clean-clone build. No engine headers were modified.

## Automation evidence validation

The runner now derives expected names from current literal simple-test registrations and checks a unique, fresh report for exact coverage, successful states, zero test warnings/errors and consistent summary counts. Missing, empty, malformed, stale, duplicate or incomplete reports fail the command. Python 3 is required; unsupported registration/filter forms fail explicitly until discovery is extended.

Seven Python regression methods with parameterised rejection cases passed, including independent reviewer execution. The full runner returned exit 0 and validated 21 tests in Saved/AutomationReports/20260916-224218-314-fd30271d/index.json. The report contains 21 successes, zero warnings, zero failures and zero not run. Engine startup diagnostics are not represented by these test-warning totals.

Independent runner review: approved for the current registration scope, no blocking findings. No new gameplay or visual acceptance is inferred from these tests.

## Review and acceptance boundary

Independent build-tooling review approved the one-line flag correction with no findings. The previous delivery contract and visual rubric also received independent planning approval after corrections. These approvals do not accept gameplay, visuals or merge.

The approved 14.50 compiler must be restored or a separately validated baseline change accepted before claiming compiler reproducibility. Human 0.6/0.7a interaction checks, full level, packaged execution, performance and visual realism remain outstanding. No visual score is claimed.
