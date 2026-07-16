#include "Core/TheVeilGameState.h"

#include "Net/UnrealNetwork.h"

ATheVeilGameState::ATheVeilGameState()
{
    bReplicates = true;
}

bool ATheVeilGameState::ApplyPhaseState(
    const ETheVeilPhase NewPhase,
    const int32 NewRoundIndex,
    const float NewDurationSeconds,
    const FName Reason)
{
    if (!HasAuthority())
    {
        UE_LOG(LogTemp, Warning, TEXT("ApplyPhaseState rejected: GameState has no authority."));
        return false;
    }

    if (NewRoundIndex < 1)
    {
        UE_LOG(LogTemp, Warning, TEXT("ApplyPhaseState rejected an invalid round index."));
        return false;
    }

    const ETheVeilPhase PreviousPhase = CurrentPhase;
    CurrentPhase = NewPhase;
    RoundIndex = NewRoundIndex;
    PhaseDurationSeconds = FMath::Max(0.0f, NewDurationSeconds);
    PhaseStartServerTime = GetServerWorldTimeSeconds();
    LastTransitionReason = Reason;
    ++TransitionSerial;
    BroadcastPhaseChange(PreviousPhase);
    ForceNetUpdate();

    UE_LOG(LogTemp, Log, TEXT("Phase changed from %d to %d. Round %d. Reason: %s"),
        static_cast<int32>(PreviousPhase), static_cast<int32>(CurrentPhase), RoundIndex, *Reason.ToString());
    return true;
}

float ATheVeilGameState::GetRemainingPhaseTime() const
{
    if (PhaseDurationSeconds <= 0.0f)
    {
        return 0.0f;
    }

    const float Elapsed = GetServerWorldTimeSeconds() - PhaseStartServerTime;
    return FMath::Max(0.0f, PhaseDurationSeconds - Elapsed);
}

FTVPhaseSnapshot ATheVeilGameState::CreatePhaseSnapshot() const
{
    FTVPhaseSnapshot Snapshot;
    Snapshot.CurrentPhase = CurrentPhase;
    Snapshot.RoundIndex = RoundIndex;
    Snapshot.TransitionSerial = TransitionSerial;
    Snapshot.PhaseDurationSeconds = GetRemainingPhaseTime();
    Snapshot.LastTransitionReason = LastTransitionReason;
    return Snapshot;
}

void ATheVeilGameState::OnRep_CurrentPhase()
{
    BroadcastPhaseChange(LastBroadcastPhase);
}

void ATheVeilGameState::BroadcastPhaseChange(const ETheVeilPhase PreviousPhase)
{
    OnPhaseChanged.Broadcast(PreviousPhase, CurrentPhase, RoundIndex, LastTransitionReason);
    LastBroadcastPhase = CurrentPhase;
}

void ATheVeilGameState::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
    Super::GetLifetimeReplicatedProps(OutLifetimeProps);
    DOREPLIFETIME(ATheVeilGameState, CurrentPhase);
    DOREPLIFETIME(ATheVeilGameState, RoundIndex);
    DOREPLIFETIME(ATheVeilGameState, TransitionSerial);
    DOREPLIFETIME(ATheVeilGameState, PhaseStartServerTime);
    DOREPLIFETIME(ATheVeilGameState, PhaseDurationSeconds);
    DOREPLIFETIME(ATheVeilGameState, LastTransitionReason);
}
