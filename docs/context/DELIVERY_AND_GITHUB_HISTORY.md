# Delivery and GitHub History

## Generated deliverables

| Version | Deliverable | Purpose |
|---|---|---|
| 0.1 | `TheVeil_CausalSpine_0_1.zip` | Opportunity, routine, access, evidence and audio-arbitration proof |
| 0.2 | `TheVeil_RoundPhase_0_2.zip` | Authoritative phases, GameState, Round Director and checkpoint persistence |
| 0.3 | `TheVeil_MemoryProvenance_0_3.zip` | Events, memories, witnesses, reports, rumours, lies and relationships |
| 0.4 | `TheVeil_ClaimVoting_0_4.zip` | Claims, contradictions, voting profiles, intentions and vote locking |
| 0.5 | `TheVeil_ContextArchive_0_5.zip` | Prompts, design, architecture, roadmap and milestone context |
| 0.6 | `TheVeil_FullProjectArchive_0_6.zip` | Verified portable public project archive |

All package hashes are recorded in `ARTIFACT_MANIFEST.md`. Their substantive contents are represented unpacked by current source and documentation.

## Repository publication history

Repository: `chadtoerien-dev/game-dev`

### Initial repository

- Commit: `c9b660eeaab800d183315986af9b24a45251a9aa`
- Message: `chore: initialise The Veil repository`

### Gameplay foundation

- Branch: `agent/claim-voting-0-4`
- Commits: `3470d6391bed6b2486563bb22509baeed5bbb076`, `a9cc9d65e3c96e32466a027792c36790d28b0fb2`
- Pull request: #1
- Merge commit: `849622f9b0aa40014e0cb808828f678e3d2f91c5`

### Context archive

- Branch: `agent/context-archive`
- Pull request: #2
- Squash merge commit: `3dd58f7ce0b9f6a635e5c6b81ff17c0f2b798f5d`

### Exhaustive audit

- Branch: `agent/full-project-audit-0-6`
- Includes message/output ledgers, recovered transcript context, artifact manifest and final corrections.
- An intermediate branch was merged while audit corrections were still in progress; the final branch reconciles that history and removes the unverified duplicate binary.

## Validation performed

- C++ structural review available without Unreal
- Generated-header placement and source organisation checks
- ZIP integrity and SHA-256 checks locally
- Git branch file-inventory audits
- Markdown internal-link checks for the context archive

## Validation not performed

- Unreal Header Tool compilation
- `TheVeilEditor` build
- Packaged Windows build
- Unreal Automation execution
- Blueprint, map, StateTree, Input or MetaSound validation

These limitations remain part of the project record.
