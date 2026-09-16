# Archive causal encounter: implementation dependency plan

Date: 16 September 2026
Status: specialist plan; implementation not started

## Objective

Prove an ordinary player action changing access, physical evidence, acquired knowledge and a voting intention in the sandbox. This is an intermediate integration test for the complete level, not a replacement for the eight-contestant round, Tribunal, challenge, night approaches or visual acceptance.

## Code-grounded gaps

- Player character has no contestant-state or memory component.
- Development greetings only set a local boolean; they do not change social state.
- Access evaluation is not consumed by a secured door; routine tags do not drive the current patrol.
- Directed relationship Trust and contestant TrustToPlayer are separate stores with different defaults. Use the directed relationship as canonical access input.
- Development memories have no configured interpretation profile.
- Public witness events bypass observer-specific recipients; attributed subjects count as participants.
- Voting group pressure uses private provisional preferences.
- Contradictions match broad topics without establishing a shared incident.
- Restore can broadcast historical consequence delegates before all state has been restored.
- Door/object/greeting presentation is not persisted. Reconstruct it from authoritative saved gameplay state rather than duplicating booleans.

## Dependency order

1. **Witness boundaries:** explicit recipients/participants in TVWorldTypes, witness handling and producer updates in claims/Dawn. Empty recipients must not silently mean everyone; attribution is not participation. Visual events need facing, range and obstruction checks. Test absent, framed, blocked and behind-facing observers with actual world traces.
2. **Known group pressure:** voting reads only each observer's known statements. Deduplicate speakers and ignore withdrawn/unknown claims. Test that another voter's private suspicion cannot change the observer's pressure score.
3. **Incident-scoped contradiction:** carry origin-event identity into claims; different meetings/rounds must not contradict merely because their topics match. Own statements count as knowledge only when the other claim is also known. Define old-save behaviour explicitly before implementation.
4. **Restore lifecycle:** restore records silently, rehydrate presentation once and have the Round Director cancel/resume timers without replaying phase-entry effects. Test repeated restore, live timers, night restart and stable event/evidence/vote counts.
5. **Player identity and encounter:** stable player state/memory, bounded dialogue choices, real access control and physical tampering, knowledge-derived reactions and checkpoint continuation.

Each numbered item needs its own bounded brief, tests, implementation owner and independent review. Do not make a single broad refactor across these systems. Preserve supported saves or obtain the existing required compatibility decision.

## Proposed player sequence

Request access from Eleanor through a bounded rapport/invitation choice. Success authorises a real secured passage. Tampering with an archive mechanism changes visible state and emits one evidence/event result. Marcus witnesses through actual perception; Beatrice is outside sight/range or obstructed. Marcus's acquired memory supports an accusation and vote change. Beatrice remains unaffected until receiving a report with provenance. Save, quit and restore at the designated boundary without duplicating rewards, evidence, claims or penalties.

Use provisional development dialogue, not new accepted narrative canon. A floating door in open space does not prove access control: require enclosing collision and an actual route through authorised content/editor work.

## Acceptance

- Refused access blocks traversal; invitation permits it.
- Repeated rapport/tampering cannot accumulate duplicate rewards or consequences.
- Informed and uninformed observers differ without consulting objective culprit data.
- Report transmission retains origin and immediate source.
- Ordinary scoring changes at least one leading vote intention; no forced vote assignment.
- Save/quit/load preserves access, physical presentation and learned consequences.
- Keyboard/controller choices work without console commands.
- Fresh editor/game build, full automation and recorded encounter evidence; independent review and human acceptance remain required.

## Routing and next step

Read-only specialist planner completed the dependency analysis. No product decision is needed to begin bounded correctness fixes under the existing goal. After packaging verification, prepare the witness-boundary brief first. Chad's later acceptance must cover both the informed encounter and its uninformed control case, not just successful input activation.
