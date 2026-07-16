# Integration guide

## 1. Set the GameState

Open `BP_TVGameMode`.

Set:

- **Game State Class:** `ATheVeilGameState`

If you prefer a Blueprint child:

1. Create `BP_TVGameState` based on `ATheVeilGameState`.
2. Set **Game State Class** to `BP_TVGameState`.

The project will not function correctly if the map still uses the template GameState.

## 2. Place the Round Director

Create a Blueprint child:

`BP_TVRoundDirector` based on `ATheVeilRoundDirector`.

Place exactly one in `L_Dev_Foundation`.

Initial settings:

- Auto Start: true
- Use Short Prototype Loop: true
- Use Automatic Phase Timers: false
- Write Checkpoints to Disk: true
- Restore Social Checkpoint Before Night: true
- Initial Phase: Social Free Roam

Do not place phase logic in the Level Blueprint.

## 3. Replace Eleanor's loose state variables

On `BP_Contestant_Eleanor`, add:

`TVContestantStateComponent`

Rename it:

`ContestantState`

Set:

- Contestant Id: `Eleanor`
- Trust To Player: `35`
- Fear Of Player: `10`
- Knowledge Tags: `Knowledge.Target.FearsAccusation`

On the existing `TVTargetRoutineComponent`, set:

- Save Id: `Eleanor`
- Current Routine: `Routine.Day.Social`

Delete or stop using:

- `TrustToPlayer`
- `FearOfPlayer`
- `bPlayerInvitedToArchive`
- `KnowledgeTags`

The component is now the authoritative state.

## 4. Update the social interaction

Use this Blueprint flow on Eleanor:

`Interact`
→ `ContestantState.AddTrustToPlayer(15, SocialTrustAction)`
→ `ContestantState.GetTrustToPlayer`
→ `ContestantState.GetKnowledgeTags`
→ `Get World Subsystem: TVOpportunitySubsystem`
→ `Evaluate From Social State`

If the returned state is `Available`:

1. `ContestantState.SetAccessGrant`
   - Tag: `Access.Invitation.Archive`
   - Enabled: true
   - Reason: `PrivateArchiveInvitation`
2. `RoutineComponent.SetRoutine`
   - New Routine: `Routine.Night.ArchiveMeeting`
   - Instigator Contestant Id: `Player`
   - Reason: `PrivateArchiveInvitation`
3. `OpportunitySubsystem.MarkPrepared`
4. Submit the target-isolated music request.

## 5. Update archive access

Build `FTVAccessContext` from the contestant component:

- Trust: `ContestantState.GetTrustToPlayer`
- Fear: `ContestantState.GetFearOfPlayer`
- Has Invitation: `ContestantState.HasAccessGrant(Access.Invitation.Archive)`
- Is Restricted Area: true
- Is Night: compare `ATheVeilGameState.GetCurrentPhase` with `NightPlanning` or `NightOperation`

Delete `bNightTestActive`.

The phase state is now authoritative.

## 6. Update the operation point

When constructing `FTVEvidenceRecord`, leave:

- Created Round Index: `-1`
- Created Phase: `None`

`UTVEvidenceSubsystem.AddEvidence` fills them from `ATheVeilGameState`.

Keep the existing duplicate-operation guard.

## 7. Add debug controls

Create two development-only input actions.

### Next phase

`IA_DebugNextPhase` bound to `N`.

PlayerController flow:

`IA_DebugNextPhase Started`
→ `Get Actor Of Class: BP_TVRoundDirector`
→ `Advance Phase`
→ Reason: `DeveloperAdvance`

### Restart night from checkpoint

`IA_DebugRestoreNight` bound to `R`.

Flow:

`IA_DebugRestoreNight Started`
→ `Get Actor Of Class: BP_TVRoundDirector`
→ `Restart Night From Social Checkpoint`

Remove both bindings before a public build.

## 8. Dawn UI

Create `WBP_DawnEvidenceDebug`.

Bind to `TVEvidenceSubsystem.OnEvidenceSurfacedAtDawn`.

For each record, display:

- Evidence Tag
- Location Tag
- Source Contestant Id
- Reliability
- Notes

Also bind to `RoundDirector.OnDawnConsequencesReady` to display the count.

This is a debug presentation only. The final Dawn Assembly should convert evidence into NPC memories and public claims rather than showing raw data.

## 9. Test procedure

1. Start PIE.
2. Confirm current phase is Social Free Roam.
3. Interact with Eleanor until trust reaches 65.
4. Confirm:
   - archive access grant exists
   - opportunity is Prepared
   - routine is Archive Meeting
5. Press `N`.
6. Confirm:
   - `AfterSocial` checkpoint is captured
   - phase becomes Night Planning
   - social state is restored from checkpoint
7. Press `N`.
8. Confirm phase becomes Night Operation.
9. Complete archive sabotage.
10. Confirm one evidence record is created with:
    - current round
    - Night Operation phase
11. Press `N`.
12. Confirm:
    - `AfterNight` checkpoint is captured
    - phase becomes Dawn Assembly
    - evidence is marked surfaced and discovered
    - Dawn UI receives one record
13. Press `N`.
14. Confirm:
    - phase returns to Social Free Roam
    - round index increments

## 10. Run the Automation test

In Unreal Editor:

`Tools -> Test Automation`

Search:

`TheVeil.Phase.ShortLoop`

Run the test.

Expected: all assertions pass.

## Acceptance criteria

- No Blueprint owns the canonical current phase.
- The GameState exposes the current phase and round.
- Social state survives the Social-to-Night checkpoint.
- Invitations use Gameplay Tags rather than a standalone Boolean.
- The target routine survives checkpoint restoration.
- Opportunity state survives checkpoint restoration.
- Night evidence records their creation phase and round.
- Dawn surfaces evidence once only.
- Returning to Social increments the round.
- Invalid phase jumps are rejected.
- No phase decision runs on Tick.
