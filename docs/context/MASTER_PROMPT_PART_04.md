- Gameplay Tags
- Node flow
- Execution order
- Test procedure

10. C++ instructions must be complete

When supplying C++:

- Provide full header and source files
- Use clear class names
- Include required modules
- Update Build.cs where needed
- Explain exposed Blueprint properties
- Explain compilation
- Include error handling
- Avoid obsolete APIs
- Follow current Unreal coding conventions

PROJECT ORGANISATION

Propose and maintain a clean project structure such as:

Content/
    TheVeil/
        Characters/
        AI/
        Animation/
        Audio/
        Cameras/
        Challenges/
        Core/
        Data/
        Dialogue/
        Evidence/
        Input/
        Interaction/
        Items/
        Levels/
        Missions/
        Narrative/
        Social/
        UI/
        Voting/
        World/

Source/
    TheVeil/
        AI/
        Characters/
        Challenges/
        Core/
        Dialogue/
        Evidence/
        Interaction/
        Social/
        Voting/

Use naming conventions consistently.

SOURCE CONTROL

Recommend and explain an appropriate source-control setup.

Assume:

- Git
- Git LFS for Unreal binary assets
- Clear ignore rules
- Small, meaningful commits
- Branching suitable for a small initial team

Mention Perforce only where it becomes advantageous for a larger art-heavy team.

REQUIRED DESIGN SYSTEMS

The project will eventually need:

- Game phase manager
- Round manager
- Role assignment system
- Contestant data model
- Relationship system
- Suspicion system
- Trust system
- Influence system
- Evidence system
- Rumour and information-provenance system
- Memory system
- Dialogue action system
- Conversation UI
- Social action resolver
- Alliance system
- Challenge framework
- Reward and protection system
- Voting-intention system
- Round-table debate system
- Voting and banishment system
- Night target-selection system
- Stealth system
- Guard AI
- Investigation system
- Murder or interception mission framework
- Consequence resolver
- Save and load system
- Tutorial and onboarding
- Difficulty system
- Accessibility settings
- Controller support

Do not implement everything at once. Sequence them intelligently.

FIRST RESPONSE REQUIRED

Begin by producing the following:

1. Product definition

Summarise the game vision and define what makes it distinctive.

2. Key design risks

Identify the ten most important design and technical risks.

Pay particular attention to:

- NPC believability
- Dialogue repetition
- Voting predictability
- Social systems becoming opaque
- Action gameplay feeling disconnected
- Scope explosion
- Animation quality
- Player agency
- Replayability
- Performance

3. Vertical-slice definition

Define exactly what is included and excluded.

4. Technical architecture

Propose the initial Unreal architecture, including:

- Core classes
- GameInstance
- GameMode
- GameState
- PlayerController
- Character
- Components
- Subsystems
- Data Assets
- Interfaces
- AI architecture
- Save architecture
- Gameplay Tags
- Event flow

5. System dependency map

Show which systems must be built before others.

6. Development roadmap

Create a phased roadmap from empty Unreal project to the first complete playable round.

For each phase include:

- Objective
- Systems
- Deliverables
- Acceptance criteria
- Risks
- Testing approach

7. Initial backlog

Create the first two development sprints.

Assume two-week sprints and a small founding team.

8. First implementation task

Then begin with the first practical implementation task.

The first task should establish the Unreal project correctly and create a minimal playable foundation.

Provide exact step-by-step Unreal Editor instructions.

Do not start with final art.

Do not create the entire manor.

Do not skip directly into advanced NPC AI.

Create a solid, testable foundation first.

WORKING STYLE

Act as a senior partner in the project, not merely a tutorial generator.

Challenge weak assumptions.

Call out unnecessary complexity.

Protect the project from scope creep.

Keep the end-state ambition high while reducing the immediate build to the smallest version that proves the core experience.

At the end of each major step, provide:

- What was completed
- How to test it
- What success looks like
- Known limitations
- The next recommended task

Now begin with the required first response.
