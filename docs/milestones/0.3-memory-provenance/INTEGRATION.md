# Integration guide — Memory and Information Provenance 0.3

## 1. Replace 0.2

This package is cumulative. Replace the supplied 0.2 source files rather than
copying only the new folders.

Copy:

- `Source/TheVeil/`
- `Config/Tags/TheVeil_PhaseTags.ini`

Keep project-specific files that are not present in the package.

Regenerate project files and build `TheVeilEditor` before editing Blueprints.

## 2. Add knowledge components to contestants

For Eleanor and two additional placeholder contestants, for example Marcus and
Beatrice, add:

- `TVContestantStateComponent`
- `TVMemoryComponent`
- `TVWitnessComponent`

Contestant IDs:

- Eleanor
- Marcus
- Beatrice

The memory component obtains its owner ID from `TVContestantStateComponent`.
Leave `Contestant Id Override` empty unless the actor cannot carry the state
component.

Initial witness settings:

- Automatically Observe Gameplay Events: true
- Visual Range: 1800
- Hearing Range: 1400
- Require Line Of Sight: true
- Occluded Hearing Confidence Multiplier: 0.65
- Minimum Salience: 0.05

## 3. Create the interpretation profile

Create folder:

`Content/TheVeil/Data/Social`

Create a Data Asset based on:

`TVSocialInterpretationProfile`

Name:

`DA_SocialInterpretation_Default`

Add these rules.

### Archive sabotage

- Event Tag: `Event.Operation.ArchiveSabotage`
- Minimum Confidence: 0.20
- Scale By Salience: true
- Base Delta:
  - Trust: -20
  - Suspicion: +30
  - Affinity: -5
  - Respect: 0
  - Fear: +8
  - Credibility: -5

### Questionable access

- Event Tag: `Event.Access.Questionable`
- Minimum Confidence: 0.25
- Base Delta:
  - Trust: -4
  - Suspicion: +10
  - Fear: +2

### Public evidence

- Event Tag: `Event.Evidence.Surfaced`
- Minimum Confidence: 0.20
- Base Delta:
  - Trust: -8
  - Suspicion: +16
  - Credibility: -4

This rule has no effect when the memory has no `SubjectContestantId`. That is
intentional: unattributed evidence creates knowledge but should not magically
assign guilt.

### Contradiction

- Event Tag: `Event.Statement.Contradiction`
- Minimum Confidence: 0.30
- Base Delta:
  - Trust: -10
  - Suspicion: +15
  - Credibility: -18

Assign `DA_SocialInterpretation_Default` to the Interpretation Profile property
on each contestant's `TVMemoryComponent`.

Later, create different profiles for trusting, paranoid, analytical and
self-interested personalities.

## 4. Initialise directed relationships

Create an Actor Blueprint:

`BP_DevSocialBootstrap`

Place one instance in `L_Dev_Foundation`. Do not use the Level Blueprint.

On BeginPlay:

1. Get World Subsystem: `TVRelationshipSubsystem`.
2. Construct and set these `FTVRelationshipRecord` values:

### Eleanor observes Player

- Observer: Eleanor
- Subject: Player
- Trust: 35
- Suspicion: 5
- Affinity: 50
- Respect: 50
- Fear: 10
- Credibility: 50

### Marcus observes Player

- Observer: Marcus
- Subject: Player
- Trust: 50
- Suspicion: 0
- Affinity: 50
- Respect: 50
- Fear: 0
- Credibility: 50

### Beatrice observes Player

Use the same neutral starting values as Marcus.

The matrix is directed. `Eleanor -> Player` is not the same record as
`Player -> Eleanor`.

## 5. Migrate the social trust action

The relationship matrix should now be authoritative for social simulation.
`TVContestantStateComponent.TrustToPlayer` remains only for 0.2 save
compatibility and should not drive new logic.

Eleanor interaction flow:

`Get World Subsystem: TVRelationshipSubsystem`
→ `Get Relationship(Observer=Eleanor, Subject=Player)`
→ read Trust
→ construct `FTVRelationshipDelta` with Trust +15
→ `Apply Relationship Delta`
   - Observer: Eleanor
   - Subject: Player
   - Influencing Memory Id: invalid GUID
   - Reason: `SocialTrustAction`
→ `Get Relationship` again
→ pass returned Trust into `TVOpportunitySubsystem.Evaluate From Social State`

Keep the existing knowledge-tag and opportunity requirements.

Archive access should also read:

- Trust: `GetRelationship(Eleanor, Player).Trust`
- Fear: `GetRelationship(Eleanor, Player).Fear`

This ensures witnessed behaviour can alter later access.

## 6. Emit the archive sabotage event

In `BP_ArchiveSabotagePoint`, after the operation succeeds and evidence is
created, construct `FTVGameplayEventRecord`:

- Event Tag: `Event.Operation.ArchiveSabotage`
- Instigator Contestant Id: Player
- Attributed Contestant Id: None
- Target Contestant Id: Eleanor
- Location Tag: `Location.Archive`
- World Location: `GetActorLocation`
- Audience: Local Senses
- Sensory Channel: Mixed
- Loudness: 0.8
- Base Reliability: 0.95
- Salience: 0.9
- Max Witness Distance: 0
- Reveal Instigator On Visual Witness: true
- Context Tags:
  - `Context.Event.Illegal`
  - `Context.Event.Private`
- Related Evidence Id: result returned by `AddEvidence`
- Summary: `Someone tampered with the archive mechanism.`

Call:

`Get World Subsystem: TVGameplayEventSubsystem`
→ `Emit Gameplay Event`

Do not add memories directly from the sabotage Blueprint. The event is the
single source and each witness decides independently whether they perceived it.

## 7. Configure the evidence record

For the initial direct-witness test:

- Source Contestant Id: Player
- Attributed Contestant Id: None
- Evidence Tag: `Evidence.Physical.ToolMarks`
- Location: `Location.Archive`
- Reliability: 0.8

`Source Contestant Id` is hidden objective simulation data. Dawn Assembly does
not publish it.

For a later frame test, set:

- Attributed Contestant Id: Marcus

All Dawn memories will then associate the public evidence with Marcus even
though the objective source remains Player.

## 8. Position the witness test

Place Eleanor inside the archive with direct line of sight to the sabotage
point.

Place Marcus outside the room, close enough to hear but unable to see.

Expected after sabotage:

- Eleanor creates a first-hand witnessed memory about Player.
- Eleanor's suspicion of Player rises strongly.
- Marcus may create a heard memory, but its Subject is None because sound alone
  did not identify the instigator.
- Marcus's relationship with Player does not change from that unidentified
  sound memory.

This difference is essential. Hearing an event is not the same as knowing who
caused it.

## 9. Test reporting

Create development input action:

`IA_DebugReportMemory` bound to `T`.

PlayerController debug flow:

1. Get Eleanor's `TVMemoryComponent`.
2. Get Memories About `Player`.
3. Select the newest archive-sabotage memory.
4. Get World Subsystem: `TVInformationSubsystem`.
5. Call `Transmit Memory`:
   - Source Actor: Eleanor
   - Recipient Actor: Marcus
   - Source Memory Id: selected memory ID
   - Mode: Report
   - Mutation: defaults
   - Distortion Seed: 12345

Expected:

- Marcus receives a new reported memory.
- Original observer remains Eleanor.
- Immediate source is Eleanor.
- Transmission depth becomes 1.
- Confidence and reliability are lower than Eleanor's first-hand memory.
- Marcus's suspicion of Player rises, but less than Eleanor's.

## 10. Test rumour spread

Create development action:

`IA_DebugSpreadRumour` bound to `Y`.

Use Marcus's reported memory as the source and transmit it to Beatrice using:

- Mode: Rumour
- Distortion Seed: 67890

Expected:

- Transmission depth becomes 2.
- Distortion count increases.
- Confidence drops again.
- Beatrice's suspicion increase is weaker than Marcus's.

## 11. Test a deliberate lie

Construct `FTVInformationMutation`:

- Override Subject Contestant: true
- Subject Contestant Id: Marcus
- Added Notes: `Eleanor claimed Marcus performed the sabotage.`

Transmit Eleanor's source memory to Beatrice using:

- Mode: Deliberate Lie

Expected:

- Beatrice believes the report concerns Marcus.
- The hidden provenance flag records that it was deliberately altered.
- Beatrice does not automatically know it is a lie.
- The original event ID remains traceable for future contradiction systems.

## 12. Dawn Assembly integration

No additional Blueprint is required for event conversion.

When the Round Director enters Dawn Assembly:

1. Evidence is surfaced.
2. A public `Event.Evidence.Surfaced` is emitted for each new record.
3. Every active witness component creates a public-announcement memory.
4. Only `AttributedContestantId` becomes the memory subject.
5. The AtDawn checkpoint captures the resulting memories and relationships.

Do not display `SourceContestantId` in player-facing Dawn UI.

## 13. Extend the debug HUD

Display:

- Total gameplay events
- Eleanor memory count
- Marcus memory count
- Beatrice memory count
- Eleanor -> Player suspicion
- Marcus -> Player suspicion
- Beatrice -> Player suspicion
- Newest memory acquisition method
- Original observer
- Immediate source
- Transmission depth
- Distortion count
- Confidence
- Perceived reliability

Update by timer around four times per second, not Tick bindings.

## 14. Checkpoint test

1. Create a direct witness memory.
2. Report it to Marcus.
3. Capture a checkpoint.
4. Add a rumour to Beatrice.
5. Restore the checkpoint.

Expected:

- Eleanor and Marcus retain their saved memories.
- Beatrice's post-checkpoint rumour disappears.
- Relationship values return to checkpoint values.
- Historical gameplay events restore without re-emitting.
- No duplicate memories are generated during restore.

## 15. Run Automation tests

Open:

`Tools -> Test Automation`

Run:

- `TheVeil.Information.ProvenanceTransmission`
- `TheVeil.Information.InterpretationRuleSelection`
- `TheVeil.Phase.ShortLoop`

Expected: all assertions pass.

## Acceptance criteria

- A gameplay action emits one typed event.
- Individual contestants form different memories from the same event.
- Visual witnesses may identify an instigator; sound-only witnesses do not by
  default.
- Memories preserve provenance.
- Reports and rumours reduce information quality.
- A deliberate lie can change the alleged culprit without leaking its falsity.
- Relationships are directed between every observer and subject.
- Memory effects are data-driven.
- Public evidence never exposes hidden objective culprit data.
- Event, memory and relationship state survives checkpoints.
- Restoring a checkpoint creates no duplicate memories.
- No memory, transmission or relationship decision runs on Tick.
