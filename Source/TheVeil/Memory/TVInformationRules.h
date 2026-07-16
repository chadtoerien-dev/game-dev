#pragma once
#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "Core/TVGameplayTypes.h"
#include "TVInformationRules.generated.h"
UCLASS()
class THEVEIL_API UTVInformationRules : public UBlueprintFunctionLibrary
{
    GENERATED_BODY()
public:
    UFUNCTION(BlueprintPure, Category="The Veil|Information")
    static FTVInformationTransmissionResult BuildTransmittedMemory(const FTVMemoryRecord& SourceMemory, FName SourceContestantId, FName RecipientContestantId, ETVInformationTransmissionMode TransmissionMode, const FTVInformationMutation& Mutation, int32 DistortionSeed);
};
