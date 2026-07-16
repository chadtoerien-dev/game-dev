# Memory and provenance architecture

## Authority layers

### Objective simulation

- `UTVGameplayEventSubsystem`
- `UTVEvidenceSubsystem`
- phase and round state

These systems know what physically happened. NPC decision logic must not read
objective culprit data directly.

### Individual knowledge

- one `UTVMemoryComponent` per contestant
- one `UTVWitnessComponent` per contestant

Each memory records what that contestant believes, how they learned it and how
confident they are.

### Information transmission

- `UTVInformationSubsystem`
- `UTVInformationRules`

Reports retain provenance but reduce confidence. Rumours reduce confidence
further and increase distortion. Deliberate lies can mutate the alleged event,
subject or target without telling the receiving NPC that the claim is false.

### Social interpretation

- `UTVSocialInterpretationProfile`
- `UTVRelationshipSubsystem`

The same memory can affect different personalities differently because each
contestant can use a different interpretation profile.

## Event flow

1. Gameplay emits `FTVGameplayEventRecord`.
2. Witness components evaluate audience, range, line of sight and hearing.
3. A perceived event becomes `FTVMemoryRecord`.
4. The memory's profile selects the most specific matching rule.
5. Confidence, reliability, salience and provenance scale the rule.
6. The directed observer-to-subject relationship changes.
7. Information may be transmitted to another contestant.
8. All event, memory and relationship state is saved at checkpoints.

## Scaling limit

The automatic witness approach broadcasts each event to all witness components.
With eight contestants this is simple and appropriate. It should be replaced or
partitioned before simulating large crowds. AI Perception can later feed the
same memory API, but it should not become the knowledge or truth database.
