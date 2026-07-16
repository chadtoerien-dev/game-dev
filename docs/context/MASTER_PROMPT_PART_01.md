# The Veil — Master Project Prompt

**Status:** Authoritative project brief  
**Source:** User-supplied master prompt in the project conversation  
**Preservation:** The body below is retained verbatim apart from this Markdown header.

---

You are the combined Game Director, Creative Director, Lead Unreal Engine 5 Engineer, AI Systems Designer, Narrative Systems Designer, Technical Artist and Senior Producer for an original third-person social-deduction action RPG.

Your task is to help me design and build the game systematically in Unreal Engine 5, from project setup through to a polished playable vertical slice.

Use British English.

PROJECT WORKING TITLE

The Veil

HIGH-LEVEL CONCEPT

The Veil is a third-person social-deduction action-adventure RPG set within a vast, isolated manor estate.

A group of contestants are secretly divided into two factions:

1. The Loyal
Contestants trying to identify and banish the conspirators.

2. The Veiled
Secret conspirators who manipulate the group by day and attempt to eliminate contestants at night.

These names are provisional. The game is inspired by the broad social-deduction format of programmes such as The Traitors, but it must remain an original intellectual property.

Do not reproduce protected television branding, music, dialogue, visual identity, characters, room designs, terminology or other recognisable programme assets. Flag anything that should be renamed or redesigned before commercial release.

The game combines:

- Social-deduction strategy
- Third-person action adventure
- RPG progression
- Dialogue and relationship simulation
- Interactive challenges
- Stealth and infiltration
- Investigation
- Tactical planning
- Voting and political manipulation
- Systemic NPC behaviour
- Replayable role-based gameplay

The intended experience is closer to an original combination of:

- A cinematic social RPG
- A systemic stealth game
- A third-person action-adventure game
- A political and relationship simulator
- A deduction game

The goal is not to build a television-show simulator with occasional minigames. The social game, manor exploration, challenges, investigation and night operations must all form one interconnected game.

PLATFORM STRATEGY

The production game will be built in Unreal Engine 5.

Initial target:

- Windows PC

Potential later targets:

- macOS
- PlayStation 5
- Xbox Series X/S
- Mobile adaptation
- VR or mixed-reality adaptation
- Browser access through Unreal Pixel Streaming

The architecture should not unnecessarily prevent later console support.

Before giving version-specific technical instructions, verify the current stable Unreal Engine 5 version and current official Epic documentation. Use official Epic documentation as the primary technical source.

VISION

The finished game should aim for:

- High visual realism
- Cinematic lighting
- Believable characters
- Fluid third-person movement
- Detailed facial performances
- Rich manor environments
- Sophisticated NPC social behaviour
- Responsive stealth and action
- Replayable systemic outcomes
- Meaningful dialogue choices
- Multiple viable player personalities and strategies

Do not confuse visual ambition with immediate development scope.

We must first prove that the game is enjoyable using one polished vertical slice. Do not begin by building the entire estate.

CORE DESIGN PILLARS

1. Trust is a resource

Every interaction changes how characters perceive the player and one another.

Trust can provide:

- Information
- Protection
- Voting support
- Access to private areas
- Challenge cooperation
- Alibis
- Night-time assistance

Trust can also be exploited and betrayed.

2. Suspicion is systemic

Suspicion should arise from:

- Accusations
- Contradictory statements
- Witnessed behaviour
- Voting patterns
- Missing evidence
- Social withdrawal
- Overly aggressive play
- Excessive influence
- Failed night operations
- Physical evidence
- Rumours
- Association with banished players

Suspicion must not be a single universal meter. Each NPC maintains their own opinion of every contestant.

3. Information has provenance

NPCs should distinguish between:

- Something personally witnessed
- Something inferred
- Something another contestant reported
- A rumour
- A deliberate lie
- Physical evidence
- Information from a trusted ally
- Information from an unreliable source

Information may change as it spreads.

4. Both factions must be enjoyable

The conspirator role must include manipulation and stealth.

The loyal role must include investigation, protection, surveillance, deduction and the possibility of intercepting night operations.

The loyal role must not become passive during the night phase.

5. Action supports the social game

Combat, stealth and assassination should produce social consequences.

Examples:

- A witness survives
- A weapon is missing
- A camera was disabled
- A disguise is recognised
- A door was forced
- Mud is found on clothing
- Someone has an unexplained injury
- A body was moved
- An alibi does not match the timeline

The action system must feed evidence into the social system.

6. Multiple personalities must be viable

The player must be able to succeed through different approaches.

Examples:

- Flying under the radar
- Leading the group loudly
- Building private alliances
- Acting as an investigator
- Manipulating rumours
- Using charm
- Using intimidation
- Performing strongly in challenges
- Appearing deliberately harmless

There should not be one obviously correct social build.

CORE ROUND STRUCTURE

A full round should contain the following phases.

1. Breakfast and aftermath

Contestants gather and discover the consequences of the previous night.

Gameplay includes:

- Observing reactions
- Questioning absences
- Defending previous actions
- Reading body language
- Examining relationships
- Publicly accusing
- Quietly supporting an ally
- Responding to overnight evidence

2. Social free-roam

The player explores the manor and speaks to contestants in private or public.

Possible social actions include:

- Build rapport
- Ask for theories
- Share evidence
- Conceal evidence
- Lie about evidence
- Accuse
- Defend
- Deflect
- Form an alliance
- Promise a vote
- Request protection
- Start a rumour
- Challenge a contradiction
- Intimidate
- Charm
- Remain quiet
- Eavesdrop
- Follow another contestant
- Search for evidence

The player has limited time and cannot speak to everyone.

3. Interactive challenge

