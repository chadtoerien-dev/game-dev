#include "World/TVTargetRoutineComponent.h"

UTVTargetRoutineComponent::UTVTargetRoutineComponent()
{
    PrimaryComponentTick.bCanEverTick = false;
}

bool UTVTargetRoutineComponent::SetRoutine(
    const FGameplayTag NewRoutine,
    const FName InstigatorContestantId,
    const FName Reason)
{
    if (!NewRoutine.IsValid())
    {
        UE_LOG(LogTemp, Warning, TEXT("SetRoutine rejected an invalid routine tag."));
        return false;
    }

    if (CurrentRoutine == NewRoutine)
    {
        LastChangeReason = Reason;
        return true;
    }

    const FGameplayTag PreviousRoutine = CurrentRoutine;
    CurrentRoutine = NewRoutine;
    LastChangeReason = Reason;
    OnRoutineChanged.Broadcast(PreviousRoutine, CurrentRoutine, InstigatorContestantId, Reason);

    UE_LOG(LogTemp, Log, TEXT("Routine changed from %s to %s. Reason: %s"),
        *PreviousRoutine.ToString(), *CurrentRoutine.ToString(), *Reason.ToString());
    return true;
}

FTVRoutineSnapshot UTVTargetRoutineComponent::CreateSnapshot() const
{
    FTVRoutineSnapshot Snapshot;
    Snapshot.SaveId = SaveId;
    Snapshot.CurrentRoutine = CurrentRoutine;
    Snapshot.LastChangeReason = LastChangeReason;
    return Snapshot;
}

bool UTVTargetRoutineComponent::RestoreSnapshot(
    const FTVRoutineSnapshot& Snapshot,
    const bool bBroadcastChange)
{
    if (SaveId.IsNone() || Snapshot.SaveId != SaveId || !Snapshot.CurrentRoutine.IsValid())
    {
        UE_LOG(
            LogTemp,
            Warning,
            TEXT("Routine restore rejected for %s."),
            *GetNameSafe(GetOwner()));
        return false;
    }

    const FGameplayTag PreviousRoutine = CurrentRoutine;
    CurrentRoutine = Snapshot.CurrentRoutine;
    LastChangeReason = Snapshot.LastChangeReason;

    if (bBroadcastChange && PreviousRoutine != CurrentRoutine)
    {
        OnRoutineChanged.Broadcast(
            PreviousRoutine,
            CurrentRoutine,
            NAME_None,
            TEXT("CheckpointRestore"));
    }

    return true;
}
