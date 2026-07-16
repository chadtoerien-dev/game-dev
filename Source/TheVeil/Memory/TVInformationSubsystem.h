#pragma once
#include "CoreMinimal.h"
#include "Subsystems/WorldSubsystem.h"
#include "Core/TVGameplayTypes.h"
#include "TVInformationSubsystem.generated.h"
DECLARE_DYNAMIC_MULTICAST_DELEGATE_ThreeParams(FTVInformationTransmitted, FName, SourceContestantId, FName, RecipientContestantId, const FTVMemoryRecord&, TransmittedMemory);
UCLASS(BlueprintType)
class THEVEIL_API UTVInformationSubsystem : public UWorldSubsystem
{
    GENERATED_BODY()
public:
    UPROPERTY(BlueprintAssignable, Category="The Veil|Information") FTVInformationTransmitted OnInformationTransmitted;
    UFUNCTION(BlueprintCallable, Category="The Veil|Information") FTVInformationTransmissionResult TransmitMemory(AActor* SourceActor, AActor* RecipientActor, FGuid SourceMemoryId, ETVInformationTransmissionMode TransmissionMode, const FTVInformationMutation& Mutation, int32 DistortionSeed);
    UFUNCTION(BlueprintCallable, Category="The Veil|Information") TArray<FTVInformationTransmissionResult> BroadcastMemory(AActor* SourceActor, const TArray<AActor*>& RecipientActors, FGuid SourceMemoryId, ETVInformationTransmissionMode TransmissionMode, const FTVInformationMutation& Mutation, int32 BaseDistortionSeed);
};
