#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "TVVotingProfile.generated.h"

UCLASS(BlueprintType)
class THEVEIL_API UTVVotingProfile : public UPrimaryDataAsset
{
    GENERATED_BODY()

public:
    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Relationship Weights")
    float SuspicionWeight = 1.0f;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Relationship Weights")
    float DistrustWeight = 0.4f;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Relationship Weights")
    float LowAffinityWeight = 0.15f;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Relationship Weights")
    float FearWeight = 0.15f;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Relationship Weights")
    float ThreatWeight = 0.12f;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Information Weights")
    float AccusationClaimWeight = 24.0f;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Information Weights")
    float DefenceClaimWeight = 18.0f;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Information Weights")
    float ContradictionWeight = 22.0f;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Political Weights")
    float CommitmentWeight = 35.0f;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Political Weights")
    float GroupPressureWeight = 20.0f;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Decision")
    float LeaningThreshold = 35.0f;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Decision")
    float CommitmentThreshold = 75.0f;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Decision")
    float TieBreakerScale = 0.01f;
};
