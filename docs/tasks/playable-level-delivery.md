# Single playable level delivery contract

Date: 16 September 2026
Status: active; completion unproven

## Objective and player value

Deliver one complete, replayable Windows PC level of The Veil according to the authoritative project documents. Prioritise quality over speed. The user requests realistic AAA presentation, with 007 First Light as a quality reference, and a visual realism acceptance score of at least 8/10. Reference quality does not authorise copying assets, locations, characters, UI or music.

## Read first

- AGENTS.md
- docs/context/MASTER_PROMPT.md and its ordered parts
- docs/context/REFERENCE_MUSIC_ADDENDUM.md and its ordered parts
- docs/context/DECISION_LOG.md
- docs/context/AGENT_HANDOFF.md
- docs/production/ROADMAP.md
- docs/technical/TECHNICAL_ARCHITECTURE.md
- docs/design/VERTICAL_SLICE.md
- docs/design/KINGS_COLLEGE_CAMBRIDGE_REPLICA_BRIEF.md
- docs/production/AI_AGENT_WORKFLOW.md
- docs/production/AGENT_DELEGATION_PROTOCOL.md
- docs/production/VISUAL_REALISM_ACCEPTANCE.md

## Verified starting state

The checkout inspected on 16 September is based on 75cd57d. The existing 6 September automation report records 21 successes and no failures, warnings or skipped tests. This is historical evidence, not a fresh run. Milestone 0.5 has human acceptance; the latest 0.6 and 0.7a evidence leaves manual checks outstanding. An untracked production-pack ZIP belongs to the existing workspace and must not be deleted or silently incorporated.

## Scope and required behaviour

- One complete 25–30-minute round plus the next Dawn consequences.
- Eight contestants including the player; two Veiled and six Loyal; either player faction playable.
- Responsive third-person controls, camera and keyboard/controller interaction.
- Bounded connected environment following the approved King's College footprint and fidelity/deviation rules.
- Social choices change relationships, information, routines and access.
- Observer-specific memories, reports, lies, claims and contradictions; no hidden-truth leakage.
- One Reliquary Run challenge, interactive Tribunal, private vote and exile.
- One archive night scenario, including documented approaches and active Loyal counterplay.
- Persistent consequences, checkpoint save/load, onboarding, restart and completion flow.
- Original, coherent audiovisual presentation, accessibility baseline and measured performance.
- Distributable Windows build with clean-machine and external-playtest evidence.
- Visual realism score at least 8/10 under the linked rubric.

## Non-goals

Preserve existing exclusions: full estate, multiplayer, other platform production, procedural dialogue/voice, extensive progression and additional challenge families. Intermediate greyboxes are permissible but do not satisfy final visual acceptance.

## Delivery sequence

1. Rebuild and rerun baseline verification; close outstanding interaction/PIE acceptance against an exact commit. Attempt packaging early.
2. Correct demonstrated information-boundary and restore-lifecycle defects with regression tests.
3. Prove a playable social-action → access/routine → physical action → evidence → informed reaction → vote consequence chain with checkpoint restoration.
4. Complete eight-contestant identity, navigation, routines, conversations and personality configuration.
5. Complete Tribunal, eligible private votes, tally/tie handling and exile consequences.
6. Complete archive operation and Loyal counterplay, then the challenge using shared mechanics.
7. Integrate and pace the entire round; resolve the documented timing/order discrepancies explicitly before content lock.
8. Produce and assess environment, characters, animation, lighting, audio and UI against gameplay and visual gates.
9. Package, profile, test externally, fix release blockers and repeat full acceptance.

Each implementation increment requires a bounded task brief, dedicated branch/worktree, appropriate tests and independent review. Do not replace this objective with completion of one increment.

## Acceptance and evidence ledger

All items below are currently open for this delivery goal:

| Requirement | Evidence required |
|---|---|
| Complete level | Recorded beginning-to-end packaged gameplay, both factions, no developer intervention |
| Scope compliance | Requirement-by-requirement comparison with authoritative slice documents |
| Social integrity | Tests showing informed/uninformed divergence, provenance retention and framing without truth leaks |
| Voting | Eligible participants, one private vote each, player choice, ties and exile tested |
| Persistence | Save/quit/load at every supported boundary; no duplicated or missing consequences |
| Stability | Ten consecutive complete internal runs covering both factions; no crashes or unresolved deadlocks |
| Replayability | Three materially different strategic outcomes with causal evidence |
| Timing | Measured 25–30-minute intended round; onboarding/travel accounted for |
| Performance | Named hardware/settings and frame-time, memory and loading captures against the documented target |
| Visual realism | Captures, weighted score, category scores, independent review and Chad acceptance under the rubric |
| Distribution | Fresh-machine installation and playthrough; reproducible package instructions |
| Build reproducibility | Clean clone generates, builds, tests and packages cooked Development and Shipping configurations |
| Save compatibility | Save migration test and explicit supported content/save versions |
| Diagnostics | Crash logging verified in the distributed build and usable issue-report instructions |
| Hardware coverage | Performance evidence on selected minimum and recommended hardware |
| External testing | At least five testers, recorded assistance/comprehension and triaged feedback |
| Release | Independent review, known limitations, current documentation and explicit human merge acceptance |

The scope comparison must enumerate every room, all four archive approaches, all persistent consequences in VERTICAL_SLICE.md, and the replica brief's source/rights register, measurement manifest and deviation register. A generic claim of scope compliance is insufficient.

## Verification

Run scripts/Build-TheVeil.ps1 and scripts/Run-TheVeilTests.ps1; inspect actual reports and expected test discovery. Use the milestone-specific PIE checklist, git diff --check, packaged gameplay and visual captures. Never infer runtime or visual acceptance from source tests.

## Risks and rollback

Major risks are disconnected gameplay, hidden knowledge leaks, save lifecycle, navigation/conversation deadlocks, content scope, animation quality and unmeasured performance. Keep bounded commits and preserve binary companions. Roll back a failed increment through its reviewed change; never discard unrelated workspace files. Assets require provenance and the existing human/editor review process.

## Routing

Lead planner: current available GPT-6 Codex agent under the workflow's lead-reasoning role. Implementation and independent review are assigned per bounded task and available tools; do not claim an unavailable model performed work. Chad owns scope, visual acceptance, required editor checks and merges.

## Next task

Close outstanding 0.6/0.7a human acceptance and attempt early packaging. Compiler selection and automated report validation are being hardened first. A successful build alone does not close those gates. See docs/evidence/2026-09-16-delivery-baseline.md for actual recompilation and the missing approved compiler.

## 16 September verification update

- Build script returned success: Saved/BuildLogs/TheVeilEditor-Development-20260916-223650.log. Target was up to date, with zero compile actions; this does not prove a clean rebuild.
- Environment drift: Unreal selected compiler 14.51.36256 (toolchain 14.51.36231) despite the script's requested 14.50 baseline. It warned about the newer compiler and missing Visual Studio SDK integration. Resolve or explicitly validate this environment before claiming reproducible compilation.
- Fresh automation report Saved/AutomationReports/20260916-223701/index.json: 21 succeeded, zero test warnings, zero failures, zero not run. Process exited 0. These results do not establish visual, PIE or packaged acceptance.
- No gameplay or binary content was changed in this planning increment. Outstanding human acceptance remains open.
