#include "Memory/TVMemoryComponent.h"
#include "Social/TVContestantStateComponent.h"
#include "Social/TVRelationshipSubsystem.h"
#include "Social/TVSocialInterpretationProfile.h"
#include "Engine/World.h"
#include "GameFramework/Actor.h"

UTVMemoryComponent::UTVMemoryComponent() { PrimaryComponentTick.bCanEverTick = false; }
FGuid UTVMemoryComponent::AddMemory(FTVMemoryRecord Memory, const bool bApplyInterpretation, const bool bBroadcast)
{
    const FName OwnerId = ResolveOwnerContestantId();
    if (OwnerId.IsNone())
    {
        UE_LOG(LogTemp, Warning, TEXT("AddMemory rejected because %s has no contestant ID."), *GetNameSafe(GetOwner()));
        return FGuid();
    }
    if (!Memory.EventTag.IsValid())
    {
        UE_LOG(LogTemp, Warning, TEXT("AddMemory rejected invalid event tag."));
        return FGuid();
    }
    if (!Memory.MemoryId.IsValid()) Memory.MemoryId = FGuid::NewGuid();
    else if (Memories.ContainsByPredicate([&Memory](const FTVMemoryRecord& Existing){ return Existing.MemoryId == Memory.MemoryId; })) return Memory.MemoryId;
    Memory.OwnerContestantId = OwnerId;
    Memory.Confidence = FMath::Clamp(Memory.Confidence, 0.0f, 1.0f);
    Memory.PerceivedReliability = FMath::Clamp(Memory.PerceivedReliability, 0.0f, 1.0f);
    Memory.Salience = FMath::Clamp(Memory.Salience, 0.0f, 1.0f);
    if (const UWorld* World = GetWorld()) Memory.LearnedWorldTimeSeconds = World->GetTimeSeconds();
    Memories.Add(Memory);
    TrimToMaximum();
    if (bApplyInterpretation && IsValid(InterpretationProfile) && GetWorld() != nullptr)
        if (UTVRelationshipSubsystem* Relationships = GetWorld()->GetSubsystem<UTVRelationshipSubsystem>()) Relationships->ApplyMemoryInterpretation(Memories.Last(), InterpretationProfile);
    if (bBroadcast) OnMemoryAdded.Broadcast(Memories.Last());
    UE_LOG(LogTemp, Log, TEXT("%s learned memory %s about %s via method %d."), *OwnerId.ToString(), *Memory.EventTag.ToString(), *Memory.SubjectContestantId.ToString(), static_cast<int32>(Memory.Provenance.AcquisitionMethod));
    return Memory.MemoryId;
}
bool UTVMemoryComponent::FindMemory(const FGuid MemoryId, FTVMemoryRecord& OutMemory) const
{
    if (const FTVMemoryRecord* Found = Memories.FindByPredicate([&MemoryId](const FTVMemoryRecord& Existing){ return Existing.MemoryId == MemoryId; })) { OutMemory = *Found; return true; }
    return false;
}
TArray<FTVMemoryRecord> UTVMemoryComponent::GetMemoriesAbout(const FName Subject) const
{
    TArray<FTVMemoryRecord> Result;
    for (const FTVMemoryRecord& Memory : Memories) if (Memory.SubjectContestantId == Subject) Result.Add(Memory);
    return Result;
}
TArray<FTVMemoryRecord> UTVMemoryComponent::GetMemoriesForEvent(const FGuid EventId) const
{
    TArray<FTVMemoryRecord> Result;
    for (const FTVMemoryRecord& Memory : Memories) if (Memory.Provenance.OriginEventId == EventId) Result.Add(Memory);
    return Result;
}
bool UTVMemoryComponent::SetTruthAssessment(const FGuid MemoryId, const ETVTruthAssessment Assessment, const FName Reason)
{
    for (FTVMemoryRecord& Memory : Memories) if (Memory.MemoryId == MemoryId)
    {
        Memory.TruthAssessment = Assessment;
        if (!Reason.IsNone()) Memory.Notes = FText::FromString(Memory.Notes.ToString() + FString::Printf(TEXT(" [%s]"), *Reason.ToString()));
        OnMemoryChanged.Broadcast(Memory);
        return true;
    }
    return false;
}
bool UTVMemoryComponent::AddSupportingEvidence(const FGuid MemoryId, const FGuid EvidenceId)
{
    if (!EvidenceId.IsValid()) return false;
    for (FTVMemoryRecord& Memory : Memories) if (Memory.MemoryId == MemoryId) { Memory.SupportingEvidenceIds.AddUnique(EvidenceId); OnMemoryChanged.Broadcast(Memory); return true; }
    return false;
}
bool UTVMemoryComponent::AddContradiction(const FGuid MemoryId, const FGuid OtherId)
{
    if (!OtherId.IsValid() || MemoryId == OtherId) return false;
    for (FTVMemoryRecord& Memory : Memories) if (Memory.MemoryId == MemoryId)
    {
        Memory.ContradictingMemoryIds.AddUnique(OtherId);
        Memory.TruthAssessment = ETVTruthAssessment::Questioned;
        OnMemoryChanged.Broadcast(Memory);
        return true;
    }
    return false;
}
FTVMemoryComponentSnapshot UTVMemoryComponent::CreateSnapshot() const
{
    FTVMemoryComponentSnapshot Snapshot;
    Snapshot.ContestantId = ResolveOwnerContestantId();
    Snapshot.Memories = Memories;
    return Snapshot;
}
bool UTVMemoryComponent::RestoreSnapshot(const FTVMemoryComponentSnapshot& Snapshot, const bool bBroadcast)
{
    const FName OwnerId = ResolveOwnerContestantId();
    if (OwnerId.IsNone() || Snapshot.ContestantId != OwnerId) return false;
    Memories = Snapshot.Memories;
    TrimToMaximum();
    if (bBroadcast) for (const FTVMemoryRecord& Memory : Memories) OnMemoryAdded.Broadcast(Memory);
    return true;
}
void UTVMemoryComponent::ResetMemories() { Memories.Reset(); }
FName UTVMemoryComponent::ResolveOwnerContestantId() const
{
    if (!ContestantIdOverride.IsNone()) return ContestantIdOverride;
    if (const UTVContestantStateComponent* State = GetOwner() != nullptr ? GetOwner()->FindComponentByClass<UTVContestantStateComponent>() : nullptr) return State->GetContestantId();
    return NAME_None;
}
void UTVMemoryComponent::TrimToMaximum()
{
    MaximumMemories = FMath::Clamp(MaximumMemories, 16, 2048);
    while (Memories.Num() > MaximumMemories) Memories.RemoveAt(0);
}
