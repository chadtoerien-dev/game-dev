#pragma once
#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "TVVotingProfile.generated.h"
UCLASS(BlueprintType)
class THEVEIL_API UTVVotingProfile : public UPrimaryDataAsset
{
    GENERATED_BODY()
public:
    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Relationship Weights") float SuspicionWeight = 1.0f;
    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Relationship Weights") float DistrustWeight = 0.4f;
    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Relationship Weights") float LowAffinityWeight = 