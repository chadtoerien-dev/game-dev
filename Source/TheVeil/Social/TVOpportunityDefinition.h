#pragma once
#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "GameplayTagContainer.h"
#include "TVOpportunityDefinition.generated.h"

UCLASS(BlueprintType)
class THEVEIL_API UTVOpportunityDefinition : public UPrimaryDataAsset
{
    GENERATED_BODY()
public:
    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Opportunity") FGameplayTag OpportunityTag;
    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Opportunity") FText DisplayName;
    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Opportunity", meta=(ClampMin="0.0", ClampMax="100.0")) float RequiredTrust = 60.0f;
    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Opportunity") FGameplayTagContainer RequiredContextTags;
    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Opportunity") FGameplayTag TargetRoutineTag;
    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Opportunity") FGameplayTag TargetLocationTag;
    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Opportunity") FGameplayTag PreparedMusicStateTag;
    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Opportunity") FGameplayTag OperationMusicStateTag;
    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Opportunity") FGameplayTag EvidenceTagOnCompletion;
};
