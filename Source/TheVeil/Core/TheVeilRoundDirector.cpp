#include "Core/TheVeilRoundDirector.h"

#include "Core/TheVeilGameState.h"
#include "Core/TVPhaseRules.h"
#include "Core/TVPhaseSubsystem.h"
#include "Evidence/TVEvidenceSubsystem.h"
#include "Events/TVGameplayEventSubsystem.h"
#include "Voting/TVVotingProfile.h"
#include "Voting/TVVotingSubsystem.h"
#include "GameplayTagContainer.h"
#include "Engine/World.h"
#include "Engine/GameInstance.h"
#include "TimerManager.h"

ATheVeilRoundDirector::ATheVeilRoundDirector()
{
    PrimaryActorTick.bCanEverTick = false;
    bReplicates = true;
    ConfigureDefaultDurations();
}

void ATheVeilRoundDirector::BeginPlay()
{
    Super::BeginPlay();
    if (HasAuthority() && bAutoStart) StartRoundFlow();
}

bool ATheVeilRoundDirector::StartRoundFlow()
{
    if (!HasAuthority()) return false;
    ATheVeilGameState* GameState = GetTheVeilGameState();
    if (!IsValid(GameState))
    {
        UE_LOG(LogTemp, Error, TEXT("RoundDirector requires ATheVeilGameState."));
        return false;
    }
    if (GameState->GetCurrentPhase() != ETheVeilPhase::None) return true;
    if (UTVPhaseSubsystem* PhaseSubsystem = GetPhaseSubsystem())
        if (!PhaseSubsystem->HasActiveRun()) PhaseSubsystem->StartNewRun(FMath::Rand());
    return RequestPhaseTransition(InitialPhase, TEXT("RoundFlowStarted"));
}

bool ATheVeilRoundDirector::AdvancePhase(const FName Reason)
{
    const ATheVeilGameState* GameState = GetTheVeilGameState();
    if (!IsValid(GameState)) return false;
    const ETheVeilPhase NextPhase = UTVPhaseRules::GetNextPhase(GameState->GetCurrentPhase(), bUseShortPrototypeLoop);
    return NextPhase != ETheVeilPhase::None && RequestPhaseTransition(NextPhase, Reason);
}

bool ATheVeilRoundDirector::RequestPhaseTransition(const ETheVeilPhase NewPhase, const FName Reason)
{
    if (!HasAuthority()) return false;
    ATheVeilGameState* GameState = GetTheVeilGameState();
    if (!IsValid(GameState)) return false;
    const ETheVeilPhase PreviousPhase = GameState->GetCurrentPhase();
    const bool bStartingFromNone = PreviousPhase == ETheVeilPhase::None && NewPhase == InitialPhase;
    if (!bStartingFromNone && !UTVPhaseRules::IsTransitionAllowed(PreviousPhase, NewPhase, bUseShortPrototypeLoop))
    {
        UE_LOG(LogTemp, Warning, TEXT("Rejected phase transition from %d to %d."), static_cast<int32>(PreviousPhase), static_cast<int32>(NewPhase));
        return false;
    }

    GetWorldTimerManager().ClearTimer(PhaseTimerHandle);
    HandlePhaseExit(PreviousPhase, NewPhase);
    int32 NewRoundIndex = GameState->GetRoundIndex();
    if (PreviousPhase == ETheVeilPhase::DawnAssembly && NewPhase == ETheVeilPhase::SocialFreeRoam) ++NewRoundIndex;
    if (!GameState->ApplyPhaseState(NewPhase, NewRoundIndex, GetConfiguredDuration(NewPhase), Reason)) return false;
    HandlePhaseEnter(NewPhase);
    ScheduleAutomaticAdvance(NewPhase);
    return true;
}

bool ATheVeilRoundDirector::RestartNightFromSocialCheckpoint()
{
    if (!HasAuthority()) return false;
    ATheVeilGameState* GameState = GetTheVeilGameState();
    UTVPhaseSubsystem* PhaseSubsystem = GetPhaseSubsystem();
    if (!IsValid(GameState) || !IsValid(PhaseSubsystem)) return false;
    if (!PhaseSubsystem->RestoreCheckpoint(this, GameState, false, true)) return false;
    if (GameState->GetCurrentPhase() == ETheVeilPhase::SocialFreeRoam)
        return RequestPhaseTransition(ETheVeilPhase::NightPlanning, TEXT("NightRestartedFromSocialCheckpoint"));
    return GameState->ApplyPhaseState(ETheVeilPhase::NightPlanning, GameState->GetRoundIndex(), GetConfiguredDuration(ETheVeilPhase::NightPlanning), TEXT("NightRestartedFromSocialCheckpoint"));
}

ATheVeilGameState* ATheVeilRoundDirector::GetTheVeilGameState() const
{
    return GetWorld() != nullptr ? GetWorld()->GetGameState<ATheVeilGameState>() : nullptr;
}

float ATheVeilRoundDirector::GetConfiguredDuration(const ETheVeilPhase Phase) const
{
    if (const float* Found = PhaseDurations.Find(Phase)) return FMath::Max(0.0f, *Found);
    return 0.0f;
}

void ATheVeilRoundDirector::ConfigureDefaultDurations()
{
    PhaseDurations.Add(ETheVeilPhase::DawnAssembly, 90.0f);
    PhaseDurations.Add(ETheVeilPhase::SocialFreeRoam, 360.0f);
    PhaseDurations.Add(ETheVeilPhase::Challenge, 300.0f);
    PhaseDurations.Add(ETheVeilPhase::Lobbying, 180.0f);
    PhaseDurations.Add(ETheVeilPhase::Tribunal, 300.0f);
    PhaseDurations.Add(ETheVeilPhase::Vote, 60.0f);
    PhaseDurations.Add(ETheVeilPhase::Exile, 90.0f);
    PhaseDurations.Add(ETheVeilPhase::NightPlanning, 90.0f);
    PhaseDurations.Add(ETheVeilPhase::NightOperation, 300.0f);
    PhaseDurations.Add(ETheVeilPhase::Aftermath, 90.0f);
}

void ATheVeilRoundDirector::ScheduleAutomaticAdvance(const ETheVeilPhase Phase)
{
    if (!bUseAutomaticPhaseTimers) return;
    const float Duration = GetConfiguredDuration(Phase);
    if (Duration <= 0.0f) return;
    GetWorldTimerManager().SetTimer(PhaseTimerHandle, this, &ATheVeilRoundDirector::HandlePhaseTimerExpired, Duration, false);
}

void ATheVeilRoundDirector::HandlePhaseTimerExpired()
{
    AdvancePhase(TEXT("PhaseTimerExpired"));
}

void ATheVeilRoundDirector::HandlePhaseExit(const ETheVeilPhase Phase, const ETheVeilPhase DestinationPhase)
{
    OnPhaseExited.Broadcast(Phase, TEXT("PhaseTransition"));
    UTVPhaseSubsystem* PhaseSubsystem = GetPhaseSubsystem();
    ATheVeilGameState* GameState = GetTheVeilGameState();
    if (!IsValid(PhaseSubsystem) || !IsValid(GameState)) return;

    if (Phase == ETheVeilPhase::SocialFreeRoam && DestinationPhase == ETheVeilPhase::NightPlanning)
        PhaseSubsystem->CaptureCheckpoint(this, GameState, TEXT("AfterSocial"), bWriteCheckpointsToDisk);
    if (Phase == ETheVeilPhase::NightOperation && (DestinationPhase == ETheVeilPhase::DawnAssembly || DestinationPhase == ETheVeilPhase::Aftermath))
        PhaseSubsystem->CaptureCheckpoint(this, GameState, TEXT("AfterNight"), bWriteCheckpointsToDisk);
    if (Phase == ETheVeilPhase::Tribunal && DestinationPhase == ETheVeilPhase::Vote)
        PhaseSubsystem->CaptureCheckpoint(this, GameState, TEXT("AfterTribunal"), bWriteCheckpointsToDisk);
    if (Phase == ETheVeilPhase::Vote && DestinationPhase == ETheVeilPhase::Exile)
        PhaseSubsystem->CaptureCheckpoint(this, GameState, TEXT("AfterVote"), bWriteCheckpointsToDisk);
}

void ATheVeilRoundDirector::HandlePhaseEnter(const ETheVeilPhase Phase)
{
    ATheVeilGameState* GameState = GetTheVeilGameState();

    if ((Phase == ETheVeilPhase::Tribunal || Phase == ETheVeilPhase::Vote) && VotingContestantIds.Num() > 0 && VotingCandidateIds.Num() > 0 && IsValid(VotingProfile))
    {
        if (UTVVotingSubsystem* VotingSubsystem = GetWorld()->GetSubsystem<UTVVotingSubsystem>())
        {
            const int32 RoundIndex = GameState != nullptr ? GameState->GetRoundIndex() : 1;
            if (Phase == ETheVeilPhase::Tribunal)
                VotingSubsystem->RecalculateAllIntentions(VotingContestantIds, VotingCandidateIds, VotingProfile, RoundIndex, TEXT("TribunalEntered"));
            else
                VotingSubsystem->LockVotes(VotingContestantIds, VotingCandidateIds, VotingProfile, RoundIndex, TEXT("VotePhaseEntered"));
        }
    }

    if (Phase == ETheVeilPhase::NightPlanning && bRestoreSocialCheckpointBeforeNight)
        if (UTVPhaseSubsystem* PhaseSubsystem = GetPhaseSubsystem()) PhaseSubsystem->RestoreCheckpoint(this, GameState, false, true);

    if (Phase == ETheVeilPhase::DawnAssembly)
    {
        if (UTVEvidenceSubsystem* EvidenceSubsystem = GetWorld()->GetSubsystem<UTVEvidenceSubsystem>())
        {
            const TArray<FTVEvidenceRecord> Surfaced = EvidenceSubsystem->SurfaceEvidenceForDawn(GameState != nullptr ? GameState->GetRoundIndex() : 1);
            OnDawnConsequencesReady.Broadcast(Surfaced.Num());
            if (UTVGameplayEventSubsystem* EventSubsystem = GetWorld()->GetSubsystem<UTVGameplayEventSubsystem>())
            {
                const FGameplayTag Tag = FGameplayTag::RequestGameplayTag(FName(TEXT("Event.Evidence.Surfaced")), false);
                if (Tag.IsValid())
                {
                    for (const FTVEvidenceRecord& Evidence : Surfaced)
                    {
                        FTVGameplayEventRecord Event;
                        Event.EventTag = Tag;
                        Event.AttributedContestantId = Evidence.AttributedContestantId;
                        Event.LocationTag = Evidence.LocationTag;
                        Event.Audience = ETVEventAudience::PublicBroadcast;
                        Event.SensoryChannel = ETVSensoryChannel::Public;
                        Event.BaseReliability = Evidence.Reliability;
                        Event.Salience = 0.85f;
                        Event.RelatedEvidenceId = Evidence.EvidenceId;
                        Event.ContextTags.AddTag(Evidence.EvidenceTag);
                        Event.Summary = Evidence.Notes;
                        EventSubsystem->EmitGameplayEvent(Event);
                    }
                }
            }
            if (UTVPhaseSubsystem* PhaseSubsystem = GetPhaseSubsystem()) PhaseSubsystem->CaptureCheckpoint(this, GameState, TEXT("AtDawn"), bWriteCheckpointsToDisk);
        }
    }

    OnPhaseEntered.Broadcast(Phase, TEXT("PhaseEntered"));
}

UTVPhaseSubsystem* ATheVeilRoundDirector::GetPhaseSubsystem() const
{
    UGameInstance* GameInstance = GetWorld() != nullptr ? GetWorld()->GetGameInstance() : nullptr;
    return GameInstance != nullptr ? GameInstance->GetSubsystem<UTVPhaseSubsystem>() : nullptr;
}
