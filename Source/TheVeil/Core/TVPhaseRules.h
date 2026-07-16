#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "Core/TVGameplayTypes.h"
#include "TVPhaseRules.generated.h"

UCLASS()
class THEVEIL_API UTVPhaseRules : public UBlueprintFunctionLibrary
{
    GENERATED_BODY()

public:
    UFUNCTION(BlueprintPure, Category="The Veil|Phase")
    static bool IsTransitionAllowed(
        ETheVeilPhase FromPhase,
        ETheVeilPhase ToPhase,
        bool bUseShortPrototypeLoop);

    UFUNCTION(BlueprintPure, Category="The Veil|Phase")
    static ETheVeilPhase GetNextPhase(
        ETheVeilPhase CurrentPhase,
        bool bUseShortPrototypeLoop);
};
