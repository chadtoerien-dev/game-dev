#include "Social/TVOpportunitySubsystem.h"
#include "Social/TVOpportunityDefinition.h"

bool UTVOpportunitySubsystem::RegisterOpportunity(const UTVOpportunityDefinition* Definition)
{
    if (!IsValid(Definition) || !Definition->OpportunityTag.IsValid())
    {
        UE_LOG(LogTemp, Warning, TEXT("RegisterOpportunity failed: invalid definition or tag."));
        return false;
    }
    if (RuntimeStates.Contains(Definition->OpportunityTag)) return true;
    FTVOpportunityRuntimeState NewState;
    NewState.OpportunityTag = Definition->OpportunityTag;
    NewState.State = ETVOpportunityState::Unknown;
    NewState.LastReason = TEXT("Registered");
    RuntimeStates.Add(Definition->OpportunityTag, NewState);
    return true;
}
ETVOpportunityState UTVOpportunitySubsystem::EvaluateFromSocialState(const UTVOpportunityDefinition* Definition, const float CurrentTrust, const FGameplayTagContainer& ContextTags, const FName Reason)
{
    if (!RegisterOpportunity(Definition)) return ETVOpportunityState::Unknown;
    FTVOpportunityRuntimeState& RuntimeState = RuntimeStates.FindChecked(Definition->OpportunityTag);
    RuntimeState.CurrentTrust = CurrentTrust;
    RuntimeState.SatisfiedContextTags = ContextTags;
    RuntimeState.LastReason = Reason;
    const bool bTrustMet = CurrentTrust >= Definition->RequiredTrust;
    const bool bTagsMet = ContextTags.HasAll(Definition->RequiredContextTags);
    if (bTrustMet && bTagsMet && RuntimeState.State < ETVOpportunityState::Available) SetState(Definition->OpportunityTag, ETVOpportunityState::Available, Reason);
    else if ((!bTrustMet || !bTagsMet) && RuntimeState.State == ETVOpportunityState::Unknown) SetState(Definition->OpportunityTag, ETVOpportunityState::Hinted, Reason);
    return RuntimeStates.FindChecked(Definition->OpportunityTag).State;
}
bool UTVOpportunitySubsystem::MarkPrepared(const FGameplayTag Tag, const FName Reason) { return SetState(Tag, ETVOpportunityState::Prepared, Reason); }
bool UTVOpportunitySubsystem::MarkCompromised(const FGameplayTag Tag, const FName Reason) { return SetState(Tag, ETVOpportunityState::Compromised, Reason); }
bool UTVOpportunitySubsystem::MarkCompleted(const FGameplayTag Tag, const FName Reason) { return SetState(Tag, ETVOpportunityState::Completed, Reason); }
bool UTVOpportunitySubsystem::MarkAbandoned(const FGameplayTag Tag, const FName Reason) { return SetState(Tag, ETVOpportunityState::Abandoned, Reason); }
bool UTVOpportunitySubsystem::GetOpportunityState(const FGameplayTag Tag, FTVOpportunityRuntimeState& OutState) const
{
    if (const FTVOpportunityRuntimeState* Found = RuntimeStates.Find(Tag)) { OutState = *Found; return true; }
    return false;
}
TArray<FTVOpportunityRuntimeState> UTVOpportunitySubsystem::GetAllOpportunityStates() const { TArray<FTVOpportunityRuntimeState> Results; RuntimeStates.GenerateValueArray(Results); return Results; }
void UTVOpportunitySubsystem::RestoreOpportunityStates(const TArray<FTVOpportunityRuntimeState>& States, const bool bBroadcastChanges)
{
    RuntimeStates.Reset();
    for (const FTVOpportunityRuntimeState& State : States)
    {
        if (!State.OpportunityTag.IsValid()) continue;
        RuntimeStates.Add(State.OpportunityTag, State);
        if (bBroadcastChanges) OnOpportunityStateChanged.Broadcast(State.OpportunityTag, State.State, TEXT("CheckpointRestore"));
    }
}
void UTVOpportunitySubsystem::ResetOpportunities() { RuntimeStates.Reset(); }
bool UTVOpportunitySubsystem::SetState(const FGameplayTag Tag, const ETVOpportunityState NewState, const FName Reason)
{
    FTVOpportunityRuntimeState* RuntimeState = RuntimeStates.Find(Tag);
    if (RuntimeState == nullptr)
    {
        UE_LOG(LogTemp, Warning, TEXT("SetState failed: opportunity %s is not registered."), *Tag.ToString());
        return false;
    }
    if (RuntimeState->State == NewState) { RuntimeState->LastReason = Reason; return true; }
    RuntimeState->State = NewState;
    RuntimeState->LastReason = Reason;
    OnOpportunityStateChanged.Broadcast(Tag, NewState, Reason);
    UE_LOG(LogTemp, Log, TEXT("Opportunity %s changed to %d. Reason: %s"), *Tag.ToString(), static_cast<int32>(NewState), *Reason.ToString());
    return true;
}
