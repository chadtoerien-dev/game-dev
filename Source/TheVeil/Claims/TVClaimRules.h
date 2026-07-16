#pragma once
#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "Core/TVGameplayTypes.h"
#include "TVClaimRules.generated.h"
UCLASS()
class THEVEIL_API UTVClaimRules : public UBlueprintFunctionLibrary
{
    GENERATED_BODY()
public:
    UFUNCTION(BlueprintPure, Category="The Veil|Claims") static FTVContradictionRecord EvaluateContradiction(const FTVClaimRecord& ClaimA, const FTVClaimRecord& ClaimB);
    UFUNCTION(BlueprintPure, Category="The Veil|Claims") static float GetSeverityScalar(ETVContradictionSeverity Severity);
private:
    static bool ClaimsShareTopic(const FTVClaimRecord& ClaimA, const FTVClaimRecord& ClaimB);
    static TArray<FName> FindSharedAudience(const FTVClaimRecord& ClaimA, const FTVClaimRecord& ClaimB);
};
