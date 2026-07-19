# The Veil — Decision Log

This file records accepted decisions that should survive across chats, contributors and implementation sessions.

## Product and scope

| Decision | Rationale | Status |
|---|---|---|
| Build a third-person social-deduction action RPG, not a television-show simulator | Social, exploration, challenge, investigation, voting and night operations must be one causal game | Accepted |
| Use **The Veil** as the working title | Provides an original working identity; commercial clearance remains required | Provisional |
| Target Windows PC first | Provides the clearest path to a high-fidelity UE5 vertical slice | Accepted |
| Preserve later console viability | Avoid architecture that depends unnecessarily on PC-only assumptions | Accepted |
| Treat browser access as Pixel Streaming, not the main native build | Highest-fidelity browser experience without compromising the production renderer | Accepted |
| Build one 25–30-minute round before expanding the estate | Proves the core game and controls scope | Accepted |
| Base the bounded vertical-slice estate on a maximum-fidelity, one-to-one-scale reconstruction of King’s College, Cambridge | Establishes a concrete architectural and spatial target while preserving milestone scope; unverified or unauthorised elements remain documented interpretations | Accepted by Chad, 19 July 2026 |
| Use eight contestants: two Veiled and six Loyal | Small enough for a polished slice; large enough for social structure | Accepted |
| Support either player faction | Both Loyal and Veiled must have active day and night gameplay | Accepted |

## Distinctive gameplay

| Decision | Rationale | Status |
|---|---|---|
| Trust is a strategic resource | It grants information, access, protection, voting support and vulnerability | Accepted |
| Suspicion is directed, not global | Every NPC maintains their own opinion of every contestant | Accepted |
| Information retains provenance | NPCs distinguish witnessed, heard, reported, inferred, rumoured, physical and deceptive information | Accepted |
| Night operations begin during the day | Social engineering creates physical opportunities | Accepted |
| Physical actions generate social evidence | Stealth and action must affect later dialogue and voting | Accepted |
| Loyal night play is active | Protection, surveillance, following, traps and interception prevent passive play | Accepted |
| A clean operation leaves weak or ambiguous evidence, not necessarily zero evidence | Supports investigation and avoids binary perfection | Accepted |
| Firearms are not the default or optimal solution | The game prioritises planning, social access, stealth, sabotage and evidence control | Accepted |

## References and originality

| Decision | Rationale | Status |
|---|---|---|
| Use Hitman, Splinter Cell, Metal Gear Solid and modern action RPGs directionally | They establish quality bars for systems, readability and tone | Accepted |
| Do not copy protected expression | Setting, characters, terminology, missions, UI, score, dialogue and audiovisual identity must remain original | Mandatory |
| Use original faction, ceremony and reward terminology | Reduce confusing similarity with television and game references | In progress |
| Treat “Breakfast”, “Round Table”, “Banishment”, “Murder”, “Protection” and “Prize Fund” as placeholders | These may require replacement before commercial release | Accepted |

## Unreal architecture

| Decision | Rationale | Status |
|---|---|---|
| Use Unreal Engine 5.8 for the current branch | It was the verified stable version when the architecture was established | Accepted; reverify before upgrade |
| Start from a C++ Third Person project with Blueprint integration | Native foundations plus rapid content iteration | Accepted |
| Use Enhanced Input and Gameplay Tags immediately | Core input and categorical state systems | Accepted |
| Use StateTree for high-level NPC state and utility scoring for social/voting choices | Separates broad scheduling from decision utility | Accepted |
| Use AI Perception only as sensory input | Perception should create memories, not act as omniscient knowledge | Accepted |
| Use Primary Data Assets for contestants, opportunities, actions, evidence and music | Data-driven configuration without rewriting core logic | Accepted |
| Use event-driven systems and avoid Level Blueprint authority | Improves testability, persistence and future replication | Accepted |
| Defer Gameplay Ability System | Current slice does not yet justify its complexity | Accepted |
| Do not use Mass Entity for eight contestants | Unnecessary complexity | Accepted |
| Defer multiplayer implementation | Preserve clean authority boundaries, but prove single-player first | Accepted |

## Social simulation

| Decision | Rationale | Status |
|---|---|---|
| Start with Trust, Suspicion, Affinity, Respect, Fear and Credibility | Sufficient behavioural range without unmanageable tuning noise | Accepted |
| Store relationships directionally | `A → B` is independent from `B → A` | Implemented foundation |
| Keep objective truth separate from attributed identity | Enables framing, ambiguity and honest AI ignorance | Implemented foundation |
| Public claims have explicit audiences | “Public” is not omniscient global knowledge | Implemented foundation |
| Contradictions affect only observers who know both claims | Preserves information boundaries | Implemented foundation |
| Voting must expose a development-only score explanation | Prevents apparently random decisions and supports balancing | Implemented foundation |

## Audio and cinematics

| Decision | Rationale | Status |
|---|---|---|
| The soundtrack must be wholly original | Avoids copying and creates a durable identity | Mandatory |
| Use a central Audio Director | Gameplay requests musical meaning; individual actors do not directly own the score | Implemented arbitration foundation |
| Plan MetaSounds, Quartz and Audio Modulation behind the director | Supports layered stems and musical timing | Designed, not implemented |
| Use silence deliberately | Environmental sound should carry high-tension moments | Accepted |
| Use controlled, ceremonial cinematography | Matches aristocratic grandeur and moral tension | Accepted |
| Avoid excessive handheld movement and rapid cutting | Protects clarity and tone | Accepted |

## Production and source control

| Decision | Rationale | Status |
|---|---|---|
| Use Git with Git LFS for Unreal binary assets | Appropriate for the small founding team | Implemented configuration |
| Use small, meaningful branches and commits | Keeps the project auditable | Accepted |
| Consider Perforce when art production and locking become a material problem | Better suited to a larger art-heavy team | Deferred |
| Do not claim completion without a testable implementation | Protects trust and project quality | Mandatory |
| Do not start final art before mechanics are stable | Avoids masking weak gameplay | Mandatory |
