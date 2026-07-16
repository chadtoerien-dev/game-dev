- Mantle
- Contextual climb
- Cover interaction
- Shoulder switching
- Soft target lock
- Contextual takedowns
- Door interaction
- Surface-dependent footsteps
- Noise generation
- Hit reactions
- Animation blending
- Fluid transitions between exploration and action

Avoid excessive animation lock and input delay.

Use third-person as the primary camera.

TACTICAL CAMERA

The player can temporarily transition from third-person into a higher tactical planning camera.

Tactical mode should allow:

- Patrol observation
- Target marking
- Route planning
- Sound-source identification
- Known secret-passage display
- Danger-zone display
- Ally command options where applicable
- Equipment placement
- Slowed time rather than necessarily pausing the game

The transition should be smooth and retain the player as the visual anchor.

The tactical camera should support the action game rather than turning it into a distant strategy game.

WORLD VISION

The eventual estate may include:

- Main castle or manor
- Entrance hall
- Dining halls
- Drawing rooms
- Lounges
- Libraries
- Galleries
- Kitchens
- Servants’ corridors
- Guest rooms
- Private suites
- Round-table chamber
- Towers
- Cellars
- Dungeons
- Crypts
- Hidden rooms
- Secret passages
- Hidden doors
- Trapdoors
- Courtyards
- Pools
- Fountains
- Forests
- Lakes
- Islands
- Boathouses
- Stables
- Gardens
- Hedge mazes
- Ruins
- Underground tunnels
- Security facilities

Do not build all of this for the first milestone.

FIRST PLAYABLE VERTICAL SLICE

Build one complete 20–30 minute round.

The initial vertical slice should contain:

- Eight contestants
- Two conspirators
- Six loyal contestants
- Player-selected or randomly assigned role
- One polished manor wing
- One courtyard or garden
- One underground route
- One secret passage
- One breakfast sequence
- One social free-roam phase
- One interactive challenge
- One pre-vote lobbying phase
- One interactive round table
- One banishment
- One night operation
- Consequences at the next breakfast

The initial level should include:

- Entrance hall
- Dining room
- Drawing room
- Library
- Round-table room
- Guest corridor
- Several bedrooms
- Kitchen
- Servants’ corridor
- Courtyard
- One underground chamber
- One secret route
- One guarded night-operation location

The initial challenge should test:

- Third-person movement
- Cooperation
- Prize collection
- Protection reward
- Voting advantage
- Optional sabotage
- Social consequences

UNREAL ENGINE APPROACH

Start with the current Unreal Engine 5 Third Person template unless there is a stronger justified starting point.

Prefer:

- Blueprints for rapid prototyping
- C++ when required for performance, extensibility, maintainability or complex systems
- Enhanced Input
- Data Assets or Data Tables for contestant configuration
- Gameplay Tags for state and relationship categorisation
- State Trees, Behaviour Trees, utility AI or a justified combination
- AI Perception for relevant sight and hearing systems
- Smart Objects where useful
- Navigation meshes
- Sequencer for controlled cinematic moments
- SaveGame architecture for persistent state
- An event-driven architecture rather than tightly coupled Blueprint references

Evaluate whether the Gameplay Ability System is appropriate for:

- Skills
- Status effects
- Equipment
- Protection
- Social buffs and penalties
- Action abilities

Do not introduce the Gameplay Ability System merely because it is powerful. Explain the trade-offs.

Evaluate current official Unreal features for:

- Motion Matching
- Character Movement
- Animation Blueprints
- Control Rig
- MetaHuman
- Mass Entity
- State Trees
- Smart Objects
- Common UI
- Modular Gameplay
- Dialogue systems
- Multiplayer readiness

Use official current documentation before making version-sensitive recommendations.

VISUAL DIRECTION

The final aspiration is high realism, but the prototype should use a controlled art scope.

Potential tools and assets may include:

- Unreal Marketplace or Fab assets
- Quixel Megascans where licensing permits
- MetaHuman characters
- High-quality manor environment assets
- Motion-captured animation packs
- Control Rig
- Niagara effects
- Lumen
- Nanite
- Virtual Shadow Maps
- Physically based materials
- Cinematic colour grading
- Volumetric fog
- Strong interior lighting
- Spatial audio
- Footstep and material audio

Confirm current licensing and platform implications before recommending specific assets.

The first vertical slice should favour:

- One highly detailed environment
- Strong lighting
- Believable character scale
- Good animations
- Readable gameplay
- Stable frame rate

Do not hide weak visuals by making the environment excessively dark.

PERFORMANCE TARGETS

Initial PC target:

- 1080p
- Stable 60 frames per second on a defined mid-range target machine
- Scalable settings
- Controller and keyboard/mouse support

Define realistic performance budgets for:

- Characters
- Skeletal meshes
- Materials
- Texture memory
- Draw calls
- Lighting
- AI update frequency
- Navigation
- Animation
- Post-processing

DEVELOPMENT PRINCIPLES

1. Mechanics before estate size

Do not build a huge empty castle before proving the round structure.

2. Playable increments

Every development phase must end in something testable.

3. Data-driven systems

Contestants, relationships, dialogue actions, evidence and voting rules should be configurable without rewriting core logic.

4. Persistent consequences

Actions in one phase must affect later phases.

5. No fake implementation

Do not claim systems are complete unless they are genuinely implemented and testable.

6. Explain assumptions

Clearly label:

- Confirmed Unreal capability
- Recommended architecture
- Design assumption
- Prototype simplification
- Future production requirement

7. Avoid unnecessary questions

Make sensible assumptions and proceed.

Ask questions only when the answer would materially change architecture, scope or cost.

8. No giant code dumps without context

When providing implementation guidance:

- Explain where the file belongs
- Explain what it does
- Include dependencies
- Include compile instructions
- Include Blueprint integration steps
- Include expected results
- Include common failure cases

9. Blueprint instructions must be reproducible

For Blueprint implementation, specify:

- Blueprint class
- Parent class
- Component list
- Variable names
- Variable types
- Default values
- Functions
- Events
- Interfaces
- Delegates
