# Agent and Contributor Handoff

## Mission

Continue designing and implementing **The Veil**, an original third-person social-deduction action RPG in Unreal Engine 5, towards a polished 25–30-minute vertical slice.

## Read first

1. [`MASTER_PROMPT.md`](MASTER_PROMPT.md)
2. [`REFERENCE_MUSIC_ADDENDUM.md`](REFERENCE_MUSIC_ADDENDUM.md)
3. [`DECISION_LOG.md`](DECISION_LOG.md)
4. [`../design/VERTICAL_SLICE.md`](../design/VERTICAL_SLICE.md)
5. [`../technical/TECHNICAL_ARCHITECTURE.md`](../technical/TECHNICAL_ARCHITECTURE.md)
6. Root [`INTEGRATION.md`](../../INTEGRATION.md)
7. Root [`CHANGELOG.md`](../../CHANGELOG.md)

## Working style

- Use British English.
- Act as a senior game-development partner, not only a tutorial generator.
- Protect the project from scope expansion.
- End each milestone with something testable.
- State assumptions and limitations explicitly.
- Use official current Epic documentation for version-sensitive Unreal guidance.
- Keep Blueprint instructions reproducible.
- Provide complete C++ headers, sources, module dependencies, compile steps and integration notes.
- Do not claim local Unreal compilation unless it was actually performed.
- Keep NPC reasoning explainable through development tools.
- Preserve information boundaries; NPCs must not read simulation truth they have not learned.
- Do not copy protected audiovisual expression from reference games or television programmes.

## Current repository state

The native architecture is at milestone **0.4**:

```text
objective event
→ observer-specific memory
→ report, rumour or lie
→ audience-specific claim
→ contradiction
→ directed credibility and suspicion changes
→ explainable vote intention
→ locked vote
```

The repository has been structurally reviewed but has not been compiled in a local UE5.8 environment from the originating assistant session.

## Immediate next milestone

Implement the playable Tribunal and Exile loop:

```text
claims presented
→ player interrupts, defends or challenges
→ NPC speaking priorities and reactions
→ live vote-intention changes
→ private vote lock
→ tie handling
→ exile
→ optional faction reveal
→ persistent credibility and alliance consequences
```

Do not expand the estate, add final art or commission final music before this loop is playable.

## Definition of done for a milestone

Every milestone must state:

- What was completed
- Where each file belongs
- How to compile or integrate it
- How to test it
- What success looks like
- Known limitations
- The next recommended task
