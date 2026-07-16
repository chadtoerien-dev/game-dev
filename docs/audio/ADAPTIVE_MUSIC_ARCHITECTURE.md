# Adaptive Music Architecture

## Creative direction

The score should be cinematic, operatic, psychologically tense, elegant, restrained and dangerous. It should communicate sacred grandeur, institutional power, moral consequence, isolation, tragedy and controlled menace without copying any existing soundtrack.

The musical language may combine:

- Full orchestra
- Low strings, solo cello and contrabass
- French horns, bass trombones and restrained brass
- Timpani, frame drums and deep ceremonial percussion
- Bells, prepared piano and organ textures
- Wordless choir and solo vocal textures
- Subtle electronic pulses
- Processed estate clocks, thunder, breathing, footsteps, doors and locks

Avoid generic trailer writing and constant bombast.

## Core motifs

- **Estate:** grand, decaying, historical and watchful.
- **Veiled:** concealed, ritualistic and fragmented.
- **Loyal:** human, fragile and uncertain.
- **Player:** one motif transformed by compassion, manipulation, fear, leadership or isolation.
- **Exile:** judgement, finality and doubt.
- **Nightfall:** a darker transformation of the Estate.
- **Discovery:** restrained connection and realisation.
- **Elimination:** dread, inevitability and consequence rather than triumph.

## Architectural rule

Gameplay systems request musical meaning. They do not directly play tracks.

```text
gameplay event or state
→ Gameplay Tag or continuous parameter request
→ central Audio Director
→ priority and transition resolution
→ Quartz-scheduled transition
→ MetaSound and Audio Modulation response
```

## Core classes

### `UTVAudioDirectorSubsystem`

Recommended parent: `UWorldSubsystem`.

Responsibilities:

- Receive and withdraw music-state requests
- Track phase, alert state and narrative priority
- Resolve competing states
- Own the Quartz clock
- Control music Audio Components
- Schedule transitions and stingers
- Apply stem targets
- Enforce silence rules
- Publish debug data

### `ATVAudioDirectorProxy`

Visible editor actor containing Audio Components for:

- Base
- Low strings
- Pulse
- Percussion
- Brass
- Choir
- Texture
- Stinger
- Transition

Authority remains in the subsystem.

### `UTVMusicStateComponent`

Optional sustained request source for objectives, targets, pursuits, Tribunal state and other world actors.

## Data assets

### `DA_MusicCueDefinition`

Stores:

- Cue ID and state tag
- BPM and time signature
- Loop length
- Entry and exit quantisation
- Minimum play bars
- Stem references
- Entry and exit stingers
- Allowed next states
- Fallback cue
- Silence support
- Memory estimate

### `DA_MusicTransitionDefinition`

Stores:

- From and to tags
- Transition type
- Quantisation boundary
- Crossfade
- Transition cue
- Minimum bars
- Interruptibility
- Priority
- Fallback

### Runtime requests

A music request contains state tag, priority, intensity, tension, urgency, duration, expiry, silence flag, interruptibility and context tags.

## Gameplay Tags and parameters

Use tags for categorical state and continuous parameters for strength.

Example tags:

```text
Music.Phase.Dawn
Music.Phase.Social
Music.Phase.Challenge
Music.Phase.Tribunal
Music.Phase.Vote
Music.Phase.Exile
Music.Phase.Night
Music.Phase.Aftermath

Music.State.Safe
Music.State.Uneasy
Music.State.Suspicion.Low
Music.State.Suspicion.Medium
Music.State.Suspicion.High

Music.State.Discovery.Clue
Music.State.Discovery.Contradiction
Music.State.Discovery.Secret

Music.State.Stealth.ObservingTarget
Music.State.Stealth.Trespassing
Music.State.Stealth.Curiosity
Music.State.Stealth.Investigation
Music.State.Stealth.Challenged
Music.State.Stealth.Compromised
Music.State.Stealth.Pursuit
Music.State.Stealth.Lockdown
Music.State.Stealth.Escape

Music.State.Operation.TargetIsolated
Music.State.Operation.Opportunity
Music.State.Operation.Irreversible
Music.State.Operation.Committed
Music.State.Operation.Failed
Music.State.Operation.Aborted

Music.State.Silence.Eavesdropping
Music.State.Silence.IntimateDialogue
Music.State.Silence.Hiding
Music.State.Silence.PreElimination
Music.State.Silence.CriticalDecision
```

Continuous parameters:

- Intensity
- Tension
- Suspicion pressure
- Social exposure
- Security escalation

## Arbitration

Priority order:

1. Ceremonial or irreversible narrative event
2. Pursuit, lockdown or immediate danger
3. Operation committed or body discovered
4. Security investigation
5. Opportunity or discovery
6. Phase music
7. Location ambience

Use hysteresis, minimum activation time, minimum hold time, cooldown and separate escalation/de-escalation thresholds to prevent musical flicker.

## MetaSounds

### `MS_MusicStemPlayer`

Inputs include wave asset, play/stop, seek time, gain, fades, filters, looping and loop boundaries. Outputs include stereo audio, playback triggers, position and error status.

All stems in one cue must share exact start sample, sample rate, duration, tempo, bars and loop boundary.

### `MS_MusicTexture`

Supports restrained procedural clock, lock, metallic, breath and drone textures. It supplements composition rather than replacing it.

### `MS_TransitionStinger`

Supports contradiction, vote lock, body discovery, operation commitment and Exile reveal.

## Quartz

Create one primary clock such as `VeilMusicClock`.

Prototype default:

- 72 BPM
- 4/4

Supported boundaries:

- Immediate
- Next beat
- Half bar
- Next bar
- Two-bar phrase
- Four-bar phrase
- End of section

Example rules:

- Mild suspicion: next bar
- Trespass: next beat or half bar
- Investigation: next beat
- Pursuit: immediate stinger, layer change next beat
- Hiding: fast orchestration reduction
- Irreversible action: immediate sting, transition next bar
- Vote lock: next bar
- Exile: dedicated linear cue

Do not implement continuous time-stretching in the vertical slice.

## Audio Modulation and routing

Suggested buses:

- Master
- Music
- Base
- Low strings
- Pulse
- Percussion
- Brass
- Choir
- Texture
- Ambience
- Dialogue
- SFX
- UI

Suggested submixes:

```text
Master
├── Music
│   ├── Orchestra
│   ├── Choir
│   ├── Pulse
│   └── Stingers
├── Ambience
├── SFX
├── Dialogue
└── UI
```

Dialogue ducking should be contextual. Do not mute the entire world whenever someone speaks.

## Adaptive method

Use a hybrid:

- Horizontal sections for phase and major narrative change
- Vertical stems for intensity within a section

Night-operation sections may include exploration, investigation, opportunity, commitment, escape and aftermath.

## Silence

Silence is a controlled state. During eavesdropping, close hiding, waiting, critical dialogue, pre-elimination and contradiction discovery, music may withdraw while clocks, weather, breathing, clothing, footsteps, mechanisms and distant voices remain.

## Phase cues

- **Dawn:** Estate motif, outcome reveal and unresolved transition.
- **Social:** chamber texture, light harmony and suspicion layers without revealing objective guilt.
- **Challenge:** rhythmic strings, frame drums and controlled momentum.
- **Tribunal:** ceremonial foundation, accusation pressure, evidence interruption and silence before the result.
- **Night:** ambience, low strings, pulse, percussion, brass, choir and consequence layers.
- **Exile:** dedicated judgement cue, never a game-show victory sting.
- **Aftermath:** reflects correctness, loss, evidence, cohesion and player moral direction.

## Vertical-slice package

- Estate theme
- Social-suspicion cue
- Challenge cue
- Tribunal cue
- Adaptive night-infiltration cue
- Exile cue
- Aftermath cue

Prototype order:

1. Night infiltration
2. Social suspicion
3. Tribunal
4. Estate
5. Exile
6. Aftermath
7. Challenge

Start with original MIDI mock-ups and aligned stems. Do not commission full orchestral production until the state machine survives gameplay testing.

## Testing

Create a dedicated audio test map and debug HUD showing requested states, winning request, cue, section, bar, beat, intensity, active stems, silence and pending transition.

Test:

- State priority
- Request expiry
- Stem alignment over ten minutes
- Click-free loops
- Dialogue ducking
- Save/load and world restart
- Rapid random state cycling
- Clean, compromised, aborted and intercepted operations
- Blind listening for safety, unease, investigation, opportunity, irreversibility, escape and consequence
- Voice count, audio thread, streaming, memory and submix cost

## Rights governance

Every music asset must record composer, performer, recording owner, composition owner, agreement, licence scope, territory, duration, platform, marketing, adaptation, stem rights, AI disclosure and source location.

No reference soundtrack may be committed, distributed in builds or used in public material.
