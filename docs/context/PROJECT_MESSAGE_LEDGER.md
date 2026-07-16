# Project Message Ledger

Audit date: **16 July 2026**

This ledger maps every recoverable project-relevant message to durable repository content. “Verbatim” means the original visible user text is stored without substantive rewriting. “Canonical” means all actionable content is preserved in maintained project documents, rather than duplicated as a fragile second specification. “Recovered summary” means the earlier thread was available through project history and user-context summaries but not as a complete byte-for-byte transcript.

| Sequence | Date | Role | Message or deliverable | Preservation | Repository location |
|---:|---|---|---|---|---|
| 1 | 15 Jul 2026 | User | Critiqued the browser visual as below AAA quality; requested realism, dynamic and fluid play, social deduction, voting, charisma and action-RPG interaction | Recovered summary | `PROJECT_CHAT_HISTORY.md`, `TRANSCRIPT_2026-07-15_BROWSER_TO_UNREAL.md` |
| 2 | 15 Jul 2026 | User | Asked whether Unreal Engine 5 is free and whether a free/open-source equivalent exists | Recovered summary | `TRANSCRIPT_2026-07-15_BROWSER_TO_UNREAL.md`, `DECISION_LOG.md` |
| 3 | 15 Jul 2026 | User | Selected Unreal Engine 5 | Recovered summary | `DECISION_LOG.md` |
| 4 | 15 Jul 2026 | User | Asked about console, PC, Mac, browser, mobile and VR; requested highest-fidelity browser route before proceeding | Recovered summary | `PROJECT_CHAT_HISTORY.md`, `TECHNICAL_ARCHITECTURE.md` |
| 5 | 15 Jul 2026 | Assistant | Recommended UE production build, Windows-first architecture and browser access through Pixel Streaming rather than a browser-native production build | Recovered summary | `DECISION_LOG.md`, `TECHNICAL_ARCHITECTURE.md` |
| 6 | 15 Jul 2026 | User | Requested a reusable master prompt for a new chat | Recovered summary | `MASTER_PROMPT.md` |
| 7 | 15 Jul 2026 | User | Added Hitman and Hitman 2: Silent Assassin as gameplay, atmosphere and music references | Recovered summary | `REFERENCE_MUSIC_ADDENDUM.md`, `REFERENCE_TRANSLATION.md`, `ADAPTIVE_MUSIC_ARCHITECTURE.md` |
| 8 | 16 Jul 2026 | User | Supplied the full master project brief | Verbatim | `MASTER_PROMPT_PART_01.md` to `MASTER_PROMPT_PART_04.md`; raw source in `artifacts/source/Pasted text.txt` |
| 9 | 16 Jul 2026 | Assistant | Product definition, ten risks, exact vertical slice, Unreal architecture, dependency map, roadmap, two sprints and foundation task | Canonical | `PRODUCT_VISION.md`, `VERTICAL_SLICE.md`, `TECHNICAL_ARCHITECTURE.md`, `ROADMAP.md`, `FOUNDATION_IMPLEMENTATION_GUIDE.md` |
| 10 | 16 Jul 2026 | User | Supplied gameplay-reference, systemic night-operation, score, cinematic and Unreal-audio addendum | Verbatim | `REFERENCE_MUSIC_ADDENDUM_PART_01.md` and `PART_02.md` |
| 11 | 16 Jul 2026 | Assistant | Reference translation, Opportunity Graph/Threads, social stealth, dynamic routines, evidence model, Survey Mode, cinematic grammar and adaptive music architecture | Canonical | `REFERENCE_TRANSLATION.md`, `ADAPTIVE_MUSIC_ARCHITECTURE.md` |
| 12 | 16 Jul 2026 | User | “proceed” | Verbatim intent | Milestone 0.1 history |
| 13 | 16 Jul 2026 | Assistant | Causal Spine 0.1 source, integration instructions and test matrix | Package + docs | `docs/milestones/0.1-causal-spine/`, `artifacts/packages/TheVeil_CausalSpine_0_1.zip` |
| 14 | 16 Jul 2026 | User | “proceed” | Verbatim intent | Milestone 0.2 history |
| 15 | 16 Jul 2026 | Assistant | Round and Phase 0.2, persistent run state and checkpoints | Package + docs | `docs/milestones/0.2-round-phase/`, `artifacts/packages/TheVeil_RoundPhase_0_2.zip` |
| 16 | 16 Jul 2026 | User | “proceed” | Verbatim intent | Milestone 0.3 history |
| 17 | 16 Jul 2026 | Assistant | Memory and Provenance 0.3, witnesses, rumours, lies and directed relationships | Package + docs | `docs/milestones/0.3-memory-provenance/`, `artifacts/packages/TheVeil_MemoryProvenance_0_3.zip` |
| 18 | 16 Jul 2026 | User | “proceed”; requested merge and commit to `chadtoerien-dev/game-dev` | Verbatim intent | `DELIVERY_AND_GITHUB_HISTORY.md` |
| 19 | 16 Jul 2026 | Assistant | Claims, contradictions and voting 0.4; repository initialisation; branch audit; PR and merge | Package + source + docs | `docs/milestones/0.4-claim-voting/`, `artifacts/packages/TheVeil_ClaimVoting_0_4.zip`, repository source |
| 20 | 16 Jul 2026 | User | Requested comprehensive chat/context migration as Markdown | Verbatim intent | PR #2 history and context archive |
| 21 | 16 Jul 2026 | Assistant | Added 37-document context archive and merged PR #2 | Canonical + package | `docs/`, `artifacts/packages/TheVeil_ContextArchive_0_5.zip`, `DELIVERY_AND_GITHUB_HISTORY.md` |
| 22 | 16 Jul 2026 | User | Requested another entire-chat pass, all files, messages, prompts, outputs and relevant information; commit and merge to main | Verbatim intent | This 0.6 audit and archive |

## Assistant progress updates

Short progress messages such as “I’m grounding…”, “I’m building…”, and audit status updates were operational commentary rather than specifications. Their substantive results are preserved in the corresponding final deliverables, package documentation and delivery history. They are intentionally not treated as competing design authorities.

## Coverage conclusion

Every recoverable game-project message now has an explicit repository destination. Where a complete earlier-thread transcript was unavailable, the ledger labels the entry as a recovered summary rather than claiming verbatim preservation.
