# Project Context Archive

This directory preserves the design and development context that originally existed only in the ChatGPT project conversation. The repository, rather than any single chat thread, is the project's durable memory.

## Authority order

When documents conflict, use this order:

1. [`MASTER_PROMPT.md`](MASTER_PROMPT.md) — authoritative original project brief.
2. [`REFERENCE_MUSIC_ADDENDUM.md`](REFERENCE_MUSIC_ADDENDUM.md) — authoritative gameplay-reference and score addendum.
3. [`DECISION_LOG.md`](DECISION_LOG.md) — accepted project decisions made after the prompts.
4. [`AGENT_HANDOFF.md`](AGENT_HANDOFF.md) — current verified development state and operating boundary.
5. [`../production/ROADMAP.md`](../production/ROADMAP.md) — approved delivery order and milestone gates.
6. Current code, tests, config and integration documentation.
7. [`PROJECT_CHAT_HISTORY.md`](PROJECT_CHAT_HISTORY.md) — historical context explaining how decisions emerged; it is not itself a specification.

Root [`../../AGENTS.md`](../../AGENTS.md) defines cross-agent rules. Multi-model routing is defined in [`../production/AI_AGENT_WORKFLOW.md`](../production/AI_AGENT_WORKFLOW.md).

## Current handover and migration

- [`AGENT_HANDOFF.md`](AGENT_HANDOFF.md): verified UE5.8 state, toolchain, build repairs, content status, limitations and immediate next milestone.
- [`CURSOR_MIGRATION_CHECKLIST.md`](CURSOR_MIGRATION_CHECKLIST.md): safe reconciliation of local commits, sandbox content, GitHub `main` and Cursor startup.
- [`../production/AI_AGENT_WORKFLOW.md`](../production/AI_AGENT_WORKFLOW.md): division of labour across GPT, Claude, Cursor Composer, Gemini and DeepSeek.
- [`../production/TASK_BRIEF_TEMPLATE.md`](../production/TASK_BRIEF_TEMPLATE.md): reusable prompts for planning, implementation, independent review and visual review.

## Core context files

- [`MASTER_PROMPT.md`](MASTER_PROMPT.md): full preserved master prompt.
- [`REFERENCE_MUSIC_ADDENDUM.md`](REFERENCE_MUSIC_ADDENDUM.md): full reference, night-operation, music and cinematic addendum.
- [`PROJECT_CHAT_HISTORY.md`](PROJECT_CHAT_HISTORY.md): chronological project conversation and assistant deliverables.
- [`DECISION_LOG.md`](DECISION_LOG.md): durable design, architecture, production and IP decisions.
- [`MIGRATION_AUDIT.md`](MIGRATION_AUDIT.md): requirement-to-repository coverage and outstanding historical gaps.

## Exhaustive second-pass audit

- [`FULL_PROJECT_CONTEXT_INDEX.md`](FULL_PROJECT_CONTEXT_INDEX.md)
- [`PROJECT_MESSAGE_LEDGER.md`](PROJECT_MESSAGE_LEDGER.md)
- [`ASSISTANT_OUTPUTS_ARCHIVE.md`](ASSISTANT_OUTPUTS_ARCHIVE.md)
- [`DELIVERY_AND_GITHUB_HISTORY.md`](DELIVERY_AND_GITHUB_HISTORY.md)
- [`ARTIFACT_MANIFEST.md`](ARTIFACT_MANIFEST.md)
- [`TRANSCRIPT_2026-07-15_BROWSER_TO_UNREAL.md`](TRANSCRIPT_2026-07-15_BROWSER_TO_UNREAL.md)
- [`TRANSCRIPT_2026-07-16_BUILD_SESSION.md`](TRANSCRIPT_2026-07-16_BUILD_SESSION.md)

Historical generated files are preserved under [`../../artifacts/`](../../artifacts/). Current unpacked source and maintained Markdown are authoritative over historical ZIP wrappers.

## Design and implementation documents

- [`../design/PRODUCT_VISION.md`](../design/PRODUCT_VISION.md)
- [`../design/VERTICAL_SLICE.md`](../design/VERTICAL_SLICE.md)
- [`../design/REFERENCE_TRANSLATION.md`](../design/REFERENCE_TRANSLATION.md)
- [`../audio/ADAPTIVE_MUSIC_ARCHITECTURE.md`](../audio/ADAPTIVE_MUSIC_ARCHITECTURE.md)
- [`../technical/TECHNICAL_ARCHITECTURE.md`](../technical/TECHNICAL_ARCHITECTURE.md)
- [`../production/ROADMAP.md`](../production/ROADMAP.md)
- [`../production/FOUNDATION_IMPLEMENTATION_GUIDE.md`](../production/FOUNDATION_IMPLEMENTATION_GUIDE.md)
- [`../production/IMPLEMENTATION_HISTORY.md`](../production/IMPLEMENTATION_HISTORY.md)
- [`../milestones/`](../milestones/) — preserved version-specific package documentation.

## Privacy boundary

Only game-project material from the visible conversation is preserved. System messages, hidden reasoning, unrelated personal context, credentials and account data are deliberately excluded. The repository is public, so future additions must follow the same rule.
