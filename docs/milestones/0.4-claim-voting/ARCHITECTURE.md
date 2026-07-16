# Architecture

## Causal system flow

```text
Objective gameplay event
→ observer-specific memory
→ report, rumour or lie
→ claim made to a defined audience
→ contradiction detection
→ directed credibility and suspicion changes
→ candidate utility calculation
→ explainable intention
→ locked private vote
```

## Authority boundaries

- `ATheVeilGameState`: public round and phase state.
- `ATheVeilRoundDirector`: validates and coordinates phase transitions.
- `UTVPhaseSubsystem`: persistent run/checkpoint ownership.
- `UTVGameplayEventSubsystem`: objective event history.
- `UTVMemoryComponent`: what one contestant believes they know.
- `UTVInformationSubsystem`: report, rumour and lie transmission.
- `UTVRelationshipSubsystem`: directed social opinions.
- `UTVClaimSubsystem`: statements and contradictions known to explicit audiences.
- `UTVVotingSubsystem`: political commitments, score breakdowns and locked votes.

## Anti-omniscience rules

- NPCs do not read objective culprit fields from evidence or events.
- Public claims still have explicit audiences.
- Contradictions affect only contestants who know both statements.
- Voting uses only the evaluating NPC's relationships and known claims.
- Deterministic tie-breaking avoids random-looking vote changes while retaining reproducibility.
