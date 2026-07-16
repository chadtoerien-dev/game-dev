#pragma once
#include "CoreMinimal.h"
#include "Subsystems/WorldSubsystem.h"
#include "Core/TVGameplayTypes.h"
#include "TVOpportunitySubsystem.generated.h"
class UTVOpportunityDefinition;
DECLARE_DYNAMIC_MULTICAST_DELEGATE_ThreeParams(FTVOpportunityStateChanged, FGameplayTag, OpportunityTag, ETVOpportunityState, NewState, FName, Reason);
UCLASS(BlueprintType)
class THEVEIL_API UTVOpportunitySubsystem : public UWorldSubsystem
{
    GENERATED_BODY()
public:
    UPROPERTY(BlueprintAssignable, Category="The Veil|Opportunity") FTVOpportunityStateChanged OnOpportunityStateChanged;
    UFUNCTION(BlueprintCallable, Category="The Veil|Opportunity") bool RegisterOpportunity(const UTVOpportunityDefinition* Definition);
    UFUNCTION(BlueprintCallable, Category="The Veil|Opportunity") ETVOpportunityState EvaluateFromSocialState(const UTVOpportunityDefinition* Definition, float CurrentTrust, const FGameplayTagContainer& ContextTags, FName Reason);
    UFUNCTION(BlueprintCallable, Category="The Veil|Opportunity") bool MarkPrepared(FGameplayTag OpportunityTag, FName Reason);
    UFUNCTION(BlueprintCallable, Category="The Veil|Opportunity") bool MarkCompromised(FGameplayTag OpportunityTag, FName Reason);
    UFUNCTION(BlueprintCallable, Category="The Veil|Opportunity") bool MarkCompleted(FGameplayTag OpportunityTag, FName Reason);
    UFUNCTION(BlueprintCallable, Category="The Veil|Opportunity") bool MarkAbandoned(FGameplayTag OpportunityTag, FName Reason);
    UFUNCTION(BlueprintPure, Category="The Veil|Opportunity") bool GetOpportunityState(FGameplayTag OpportunityTag, FTVOpportunityRuntimeState& OutState) const;
    UFUNCTION(BlueprintPure, Category="The Veil|Opportunity") TArray<FTVOpportunityRuntimeState> GetAllOpportunityStates() const;
    UFUNCTION(BlueprintCallable, Category="The Veil|Opportunity") void RestoreOpportunityStates(const TArray<FTVOpportunityRuntimeState>& States, bool bBroadcastChanges);
    UFUNCTION(BlueprintCallable, Category="The Veil|Opportunity") void ResetOpportunities();
private:
    UPROPERTY() TMap<FGameplayTag, FTVOpportunityRuntimeState> RuntimeStates;
    bool SetState(FGameplayTag OpportunityTag, ETVOpportunityState NewState, FName Reason);
};
