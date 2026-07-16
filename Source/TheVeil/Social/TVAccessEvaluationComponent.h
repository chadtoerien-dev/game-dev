#pragma once
#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Core/TVGameplayTypes.h"
#include "TVAccessEvaluationComponent.generated.h"

UCLASS(ClassGroup=(TheVeil), meta=(BlueprintSpawnableComponent))
class THEVEIL_API UTVAccessEvaluationComponent : public UActorComponent
{
    GENERATED_BODY()
public:
    UTVAccessEvaluationComponent();
    UFUNCTION(BlueprintPure, Category="The Veil|Access")
    FTVAccessResult EvaluateAccess(const FTVAccessContext& Context) const;
protected:
    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Access|Weights") float InvitationBonus = 55.0f;
    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Access|Weights") float DisguiseBonus = 25.0f;
    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Access|Weights") float TrustScale = 0.45f;
    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Access|Weights") float FearPenaltyScale = 0.25f;
    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Access|Weights") float RestrictedAreaPenalty = 35.0f;
    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Access|Weights") float NightPenalty = 15.0f;
    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Access|Weights") float RecognisedDisguisePenalty = 30.0f;
    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Access|Weights") float RestrictedItemPenalty = 40.0f;
};
