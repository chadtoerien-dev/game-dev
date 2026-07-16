#include "Core/TVPhaseSubsystem.h"

#include "Core/TheVeilGameState.h"
#include "Claims/TVClaimSubsystem.h"
#include "Core/TVRunSaveGame.h"
#include "Evidence/TVEvidenceSubsystem.h"
#include "Events/TVGameplayEventSubsystem.h"
#include "Memory/TVMemoryComponent.h"
#include "Social/TVContestantStateComponent.h"
#include "Social/TVOpportunitySubsystem.h"
#include "Social/TVRelationshipSubsystem.h"
#include "World/TVTargetRoutineComponent.h"
#include "Voting/TVVotingSubsystem.h"
#include "Engine/Engine.h"
#include "Engine/GameInstance.h"
#include "EngineUtils.h"
#include "Kismet/GameplayStatics.h"

void UTVPhaseSubsystem::StartNewRun(const int32 RunSeed)
{
    ActiveRunSave = NewObject<UTVRunSaveGame>(this);
    ActiveRunSave->RunSeed = RunSeed;
    ActiveRunSave->SavedAtUtc = FDateTime::UtcNow();
}

bool UTVPhaseSubsystem::CaptureCheckpoint(
    UObject* WorldContextObject,
    ATheVeilGameState* GameState,
    const FName CheckpointLabel,
    const bool bWriteToDisk)
{
    if (!IsValid(GameState))
    {
        UE_LOG(LogTemp, Warning, TEXT("CaptureCheckpoint failed: invalid GameState."));
        return false;
    }

    UWorld* World = GEngine != nullptr
        ? GEngine->GetWorldFromContextObject(WorldContextObject, EGetWorldErrorMode::ReturnNull)
        : nullptr;
    if (World == nullptr)
    {
        UE_LOG(LogTemp, Warning, TEXT("CaptureCheckpoint failed: no world."));
        return false;
    }

    UTVRunSaveGame* Save = EnsureActiveRun();
    Save->SaveVersion = UTVRunSaveGame::CurrentSaveVersion;
    Save->BuildIdentifier = TEXT("ClaimVoting-0.4");
    Save->CheckpointLabel = CheckpointLabel;
    Save->SavedAtUtc = FDateTime::UtcNow();
    Save->Phase = GameState->CreatePhaseSnapshot();

    if (UTVOpportunitySubsystem* S = World->GetSubsystem<UTVOpportunitySubsystem>()) Save->Opportunities = S->GetAllOpportunityStates();
    if (UTVEvidenceSubsystem* S = World->GetSubsystem<UTVEvidenceSubsystem>()) Save->Evidence = S->GetAllEvidence();
    if (UTVGameplayEventSubsystem* S = World->GetSubsystem<UTVGameplayEventSubsystem>()) Save->GameplayEvents = S->GetAllGameplayEvents();
    if (UTVRelationshipSubsystem* S = World->GetSubsystem<UTVRelationshipSubsystem>()) Save->Relationships = S->GetAllRelationships();
    if (UTVClaimSubsystem* S = World->GetSubsystem<UTVClaimSubsystem>())
    {
        Save->Claims = S->GetAllClaims();
        Save->Contradictions = S->GetAllContradictions();
    }
    if (UTVVotingSubsystem* S = World->GetSubsystem<UTVVotingSubsystem>())
    {
        Save->VoteCommitments = S->GetCommitments();
        Save->VoteIntentions = S->GetAllIntentions();
    }

    Save->Contestants.Reset();
    Save->Routines.Reset();
    Save->Memories.Reset();

    for (TActorIterator<AActor> It(World); It; ++It)
    {
        AActor* Actor = *It;
        if (!IsValid(Actor)) continue;

        if (UTVContestantStateComponent* Component = Actor->FindComponentByClass<UTVContestantStateComponent>())
        {
            const FTVContestantStateSnapshot Snapshot = Component->CreateSnapshot();
            if (!Snapshot.ContestantId.IsNone()) Save->Contestants.Add(Snapshot);
        }
        if (UTVTargetRoutineComponent* Component = Actor->FindComponentByClass<UTVTargetRoutineComponent>())
        {
            const FTVRoutineSnapshot Snapshot = Component->CreateSnapshot();
            if (!Snapshot.SaveId.IsNone()) Save->Routines.Add(Snapshot);
        }
        if (UTVMemoryComponent* Component = Actor->FindComponentByClass<UTVMemoryComponent>())
        {
            const FTVMemoryComponentSnapshot Snapshot = Component->CreateSnapshot();
            if (!Snapshot.ContestantId.IsNone()) Save->Memories.Add(Snapshot);
        }
    }

    OnCheckpointCaptured.Broadcast(CheckpointLabel);
    UE_LOG(LogTemp, Log, TEXT("Checkpoint %s captured: %d contestants, %d memories, %d relationships, %d events, %d evidence."),
        *CheckpointLabel.ToString(), Save->Contestants.Num(), Save->Memories.Num(), Save->Relationships.Num(), Save->GameplayEvents.Num(), Save->Evidence.Num());

    return bWriteToDisk ? SaveActiveRunAsync() : true;
}

bool UTVPhaseSubsystem::RestoreCheckpoint(
    UObject* WorldContextObject,
    ATheVeilGameState* GameState,
    const bool bRestorePhaseState,
    const bool bBroadcastRestoredState)
{
    if (!IsValid(ActiveRunSave) || !IsValid(GameState))
    {
        UE_LOG(LogTemp, Warning, TEXT("RestoreCheckpoint failed: no valid run or GameState."));
        return false;
    }
    if (ActiveRunSave->SaveVersion > UTVRunSaveGame::CurrentSaveVersion)
    {
        UE_LOG(LogTemp, Error, TEXT("RestoreCheckpoint rejected a newer save version."));
        return false;
    }

    UWorld* World = GEngine != nullptr
        ? GEngine->GetWorldFromContextObject(WorldContextObject, EGetWorldErrorMode::ReturnNull)
        : nullptr;
    if (World == nullptr)
    {
        UE_LOG(LogTemp, Warning, TEXT("RestoreCheckpoint failed: no world."));
        return false;
    }

    if (UTVOpportunitySubsystem* S = World->GetSubsystem<UTVOpportunitySubsystem>()) S->RestoreOpportunityStates(ActiveRunSave->Opportunities, bBroadcastRestoredState);
    if (UTVEvidenceSubsystem* S = World->GetSubsystem<UTVEvidenceSubsystem>()) S->RestoreEvidenceRecords(ActiveRunSave->Evidence, bBroadcastRestoredState);
    if (UTVGameplayEventSubsystem* S = World->GetSubsystem<UTVGameplayEventSubsystem>()) S->RestoreGameplayEvents(ActiveRunSave->GameplayEvents, false);
    if (UTVRelationshipSubsystem* S = World->GetSubsystem<UTVRelationshipSubsystem>()) S->RestoreRelationships(ActiveRunSave->Relationships, bBroadcastRestoredState);
    if (UTVClaimSubsystem* S = World->GetSubsystem<UTVClaimSubsystem>()) S->RestoreState(ActiveRunSave->Claims, ActiveRunSave->Contradictions, bBroadcastRestoredState);
    if (UTVVotingSubsystem* S = World->GetSubsystem<UTVVotingSubsystem>()) S->RestoreState(ActiveRunSave->VoteCommitments, ActiveRunSave->VoteIntentions, bBroadcastRestoredState);

    TMap<FName, FTVContestantStateSnapshot> ContestantSnapshots;
    for (const FTVContestantStateSnapshot& Snapshot : ActiveRunSave->Contestants) if (!Snapshot.ContestantId.IsNone()) ContestantSnapshots.Add(Snapshot.ContestantId, Snapshot);
    TMap<FName, FTVRoutineSnapshot> RoutineSnapshots;
    for (const FTVRoutineSnapshot& Snapshot : ActiveRunSave->Routines) if (!Snapshot.SaveId.IsNone()) RoutineSnapshots.Add(Snapshot.SaveId, Snapshot);
    TMap<FName, FTVMemoryComponentSnapshot> MemorySnapshots;
    for (const FTVMemoryComponentSnapshot& Snapshot : ActiveRunSave->Memories) if (!Snapshot.ContestantId.IsNone()) MemorySnapshots.Add(Snapshot.ContestantId, Snapshot);

    for (TActorIterator<AActor> It(World); It; ++It)
    {
        AActor* Actor = *It;
        if (!IsValid(Actor)) continue;
        if (UTVContestantStateComponent* C = Actor->FindComponentByClass<UTVContestantStateComponent>())
            if (const FTVContestantStateSnapshot* Snapshot = ContestantSnapshots.Find(C->GetContestantId())) C->RestoreSnapshot(*Snapshot, bBroadcastRestoredState);
        if (UTVTargetRoutineComponent* C = Actor->FindComponentByClass<UTVTargetRoutineComponent>())
            if (const FTVRoutineSnapshot* Snapshot = RoutineSnapshots.Find(C->GetSaveId())) C->RestoreSnapshot(*Snapshot, bBroadcastRestoredState);
        if (UTVMemoryComponent* C = Actor->FindComponentByClass<UTVMemoryComponent>())
            if (const FTVMemoryComponentSnapshot* Snapshot = MemorySnapshots.Find(C->ResolveOwnerContestantId())) C->RestoreSnapshot(*Snapshot, bBroadcastRestoredState);
    }

    if (bRestorePhaseState)
    {
        const FTVPhaseSnapshot& Phase = ActiveRunSave->Phase;
        GameState->ApplyPhaseState(Phase.CurrentPhase, Phase.RoundIndex, Phase.PhaseDurationSeconds, TEXT("CheckpointRestore"));
    }

    OnCheckpointRestored.Broadcast(ActiveRunSave->CheckpointLabel);
    UE_LOG(LogTemp, Log, TEXT("Checkpoint %s restored."), *ActiveRunSave->CheckpointLabel.ToString());
    return true;
}

bool UTVPhaseSubsystem::SaveActiveRunAsync()
{
    if (!IsValid(ActiveRunSave)) return false;
    if (bSaveInProgress)
    {
        bSaveQueued = true;
        return true;
    }

    PendingSaveObject = DuplicateObject<UTVRunSaveGame>(ActiveRunSave, this);
    if (!IsValid(PendingSaveObject)) return false;
    bSaveInProgress = true;
    const FAsyncSaveGameToSlotDelegate Delegate = FAsyncSaveGameToSlotDelegate::CreateUObject(this, &UTVPhaseSubsystem::HandleAsyncSaveComplete);
    UGameplayStatics::AsyncSaveGameToSlot(PendingSaveObject, SaveSlotName, UserIndex, Delegate);
    return true;
}

bool UTVPhaseSubsystem::LoadActiveRunAsync()
{
    const FAsyncLoadGameFromSlotDelegate Delegate = FAsyncLoadGameFromSlotDelegate::CreateUObject(this, &UTVPhaseSubsystem::HandleAsyncLoadComplete);
    UGameplayStatics::AsyncLoadGameFromSlot(SaveSlotName, UserIndex, Delegate);
    return true;
}

void UTVPhaseSubsystem::ClearActiveRun(const bool bDeleteDiskSave)
{
    ActiveRunSave = nullptr;
    PendingSaveObject = nullptr;
    bSaveInProgress = false;
    bSaveQueued = false;
    if (bDeleteDiskSave && UGameplayStatics::DoesSaveGameExist(SaveSlotName, UserIndex)) UGameplayStatics::DeleteGameInSlot(SaveSlotName, UserIndex);
}

UTVRunSaveGame* UTVPhaseSubsystem::EnsureActiveRun()
{
    if (!IsValid(ActiveRunSave)) StartNewRun(FMath::Rand());
    return ActiveRunSave;
}

void UTVPhaseSubsystem::HandleAsyncSaveComplete(const FString& SlotName, const int32 CompletedUserIndex, const bool bSuccess)
{
    bSaveInProgress = false;
    PendingSaveObject = nullptr;
    UE_LOG(LogTemp, bSuccess ? Log : Error, TEXT("Save %s for slot %s, user %d."), bSuccess ? TEXT("completed") : TEXT("failed"), *SlotName, CompletedUserIndex);
    OnSaveCompleted.Broadcast(bSuccess);
    if (bSaveQueued)
    {
        bSaveQueued = false;
        SaveActiveRunAsync();
    }
}

void UTVPhaseSubsystem::HandleAsyncLoadComplete(const FString& SlotName, const int32 CompletedUserIndex, USaveGame* LoadedGameData)
{
    UTVRunSaveGame* LoadedRun = Cast<UTVRunSaveGame>(LoadedGameData);
    const bool bSuccess = IsValid(LoadedRun) && LoadedRun->SaveVersion <= UTVRunSaveGame::CurrentSaveVersion;
    if (bSuccess) ActiveRunSave = LoadedRun;
    UE_LOG(LogTemp, bSuccess ? Log : Error, TEXT("Load %s for slot %s, user %d."), bSuccess ? TEXT("completed") : TEXT("failed"), *SlotName, CompletedUserIndex);
    OnLoadCompleted.Broadcast(bSuccess);
}
