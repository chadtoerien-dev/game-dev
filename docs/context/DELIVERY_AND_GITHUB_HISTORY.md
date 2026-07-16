# Delivery and GitHub History

## Generated deliverables

| Version | Deliverable | Purpose |
|---|---|---|
| 0.1 | `TheVeil_CausalSpine_0_1.zip` | Opportunity, routine, access, evidence and audio-arbitration proof |
| 0.2 | `TheVeil_RoundPhase_0_2.zip` | Authoritative phases, GameState, Round Director and checkpoint persistence |
| 0.3 | `TheVeil_MemoryProvenance_0_3.zip` | Events, memories, witnesses, reports, rumours, lies and relationships |
| 0.4 | `TheVeil_ClaimVoting_0_4.zip` | Claims, contradictions, voting profiles, intentions and vote locking |
| 0.5 | `TheVeil_ContextArchive_0_5.zip` | Prompts, design, architecture, roadmap and milestone context |
| 0.6 | `TheVeil_FullProjectArchive_0_6.zip` | Complete public project-context and artifact bundle from this audit |

The 0.1–0.5 files are preserved byte-for-byte inside the directly committed 0.6 bundle.

## Repository publication history

Repository: `chadtoerien-dev/game-dev`

### Initial repository

- Commit: `c9b660eeaab800d183315986af9b24a45251a9aa`
- Message: `chore: initialise The Veil repository`

### Gameplay foundation branch

- Branch: `agent/claim-voting-0-4`
- Commits:
  - `3470d6391bed6b2486563bb22509baeed5bbb076`
  - `a9cc9d65e3c96e32466a027792c36790d28b0fb2`
- Pull request: `#1 Add The Veil 0.4 claim, contradiction and voting foundation`
- Merge commit: `849622f9b0aa40014e0cb808828f678e3d2f91c5`

### Context archive branch

- Branch: `agent/context-archive`
- Pull request: `#2 Preserve The Veil project prompts and chat context`
- Squash merge commit: `3dd58f7ce0b9f6a635e5c6b81ff17c0f2b798f5d`

### Exhaustive project audit branch

- Branch: `agent/full-project-audit-0-6`
- Adds the original prompt file, historical package ZIPs, message ledger, assistant-output index, artifact manifest and Full Project Archive 0.6.
- The final PR and merge identifiers are recorded in Git history after merge.

## Validation actually performed

- C++ structural checks available without Unreal
- Generated-header placement checks
- Source organisation review
- ZIP integrity checks
- Branch file-inventory audits
- Markdown internal-link checks for the 0.5 archive
- SHA-256 calculation for available source and package files

## Validation not performed

- Unreal Header Tool compilation
- `TheVeilEditor` build
- Packaged Windows build
- Unreal Automation test execution
- Blueprint/map/StateTree/Input/MetaSound validation

These limitations must remain visible in future project reporting.
