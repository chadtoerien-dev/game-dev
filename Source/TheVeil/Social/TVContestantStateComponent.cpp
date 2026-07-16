#include "Social/TVContestantStateComponent.h"

UTVContestantStateComponent::UTVContestantStateComponent() { PrimaryComponentTick.bCanEverTick = false; }
void UTVContestantStateComponent::SetTrustToPlayer(const float NewValue, const FName Reason) { TrustToPlayer = FMath::Clamp(NewValue, 0.0f, 100.0f); BroadcastChange(Reason); }
void UTVContestantStateComponent::AddTrustToPlayer(const float Delta, const FName Reason) { SetTrustToPlayer(TrustToPlayer + Delta, Reason); }
void UTVContestantStateComponent::SetFearOfPlayer(const float NewValue, const FName Reason) { FearOfPlayer = FMath::Clamp(NewValue, 0.0f, 100.0f); BroadcastChange(Reason); }
void UTVContestantStateComponent::SetKnowledgeTag(const FGameplayTag Tag, const bool bEnabled, const FName Reason)
{
    if (!Tag.IsValid()) return;
    if (bEnabled) KnowledgeTags.AddTag(Tag); else KnowledgeTags.RemoveTag(Tag);
    BroadcastChange(Reason);
}
void UTVContestantStateComponent::SetAccessGrant(const FGameplayTag Tag, const bool bEnabled, const FName Reason)
{
    if (!Tag.IsValid()) return;
    if (bEnabled) AccessGrantTags.AddTag(Tag); else AccessGrantTags.RemoveTag(Tag);
    BroadcastChange(Reason);
}
bool UTVContestantStateComponent::HasKnowledgeTag(const FGameplayTag Tag) const { return Tag.IsValid() && KnowledgeTags.HasTagExact(Tag); }
bool UTVContestantStateComponent::HasAccessGrant(const FGameplayTag Tag) const { return Tag.IsValid() && AccessGrantTags.HasTagExact(Tag); }
FTVContestantStateSnapshot UTVContestantStateComponent::CreateSnapshot() const
{
    FTVContestantStateSnapshot Snapshot;
    Snapshot.ContestantId = ContestantId;
    Snapshot.TrustToPlayer = TrustToPlayer;
    Snapshot.FearOfPlayer = FearOfPlayer;
    Snapshot.KnowledgeTags = KnowledgeTags;
    Snapshot.AccessGrantTags = AccessGrantTags;
    return Snapshot;
}
bool UTVContestantStateComponent::RestoreSnapshot(const FTVContestantStateSnapshot& Snapshot, const bool bBroadcastChange)
{
    if (ContestantId.IsNone() || Snapshot.ContestantId != ContestantId)
    {
        UE_LOG(LogTemp, Warning, TEXT("Contestant state restore rejected for %s."), *GetNameSafe(GetOwner()));
        return false;
    }
    TrustToPlayer = FMath::Clamp(Snapshot.TrustToPlayer, 0.0f, 100.0f);
    FearOfPlayer = FMath::Clamp(Snapshot.FearOfPlayer, 0.0f, 100.0f);
    KnowledgeTags = Snapshot.KnowledgeTags;
    AccessGrantTags = Snapshot.AccessGrantTags;
    if (bBroadcastChange) BroadcastChange(TEXT("CheckpointRestore"));
    return true;
}
void UTVContestantStateComponent::BroadcastChange(const FName Reason) { OnContestantStateChanged.Broadcast(Reason); }
