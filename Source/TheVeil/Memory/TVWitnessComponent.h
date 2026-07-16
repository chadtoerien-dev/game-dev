#pragma once
#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Core/TVGameplayTypes.h"
#include "TVWitnessComponent.generated.h"
class UTVMemoryComponent;
class UTVGameplayEventSubsystem;
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FTVGameplayEventWitnessed, const FTVGameplayEventRecord&, GameplayEvent, const FTVMemoryRecord&, CreatedMemory);
UCLASS(ClassGroup=(TheVeil), meta=(BlueprintSpawnableComponent))
class THEVEIL_API UTVWitnessComponent : public UActorComponent
{
    GENERATED_BODY()
public:
    UTVWitnessComponent();
    UPROPERTY(BlueprintAssignable, Category="The Veil|Witness") FTVGameplayEventWitnessed OnGameplayEventWitnessed;
    UFUNCTION(BlueprintCallable, Category="The Veil|Witness") bool TryWitnessGameplayEvent(const FTVGameplayEventRecord& GameplayEvent, FTVMemoryRecord& OutMemory);
protected:
    virtual void BeginPlay() override;
    virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;
    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Witness") bool bAutomaticallyObserveGameplayEvents = true;
    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Witness", meta=(ClampMin="0.0")) float VisualRange = 1800.0f;
    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Witness", meta=(ClampMin="0.0")) float HearingRange = 1400.0f;
    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Witness") bool bRequireLineOfSightForVisualEvents = true;
    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Witness", meta=(ClampMin="0.0", ClampMax="1.0")) float OccludedHearingConfidenceMultiplier = 0.65f;
    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Witness", meta=(ClampMin="0.0", ClampMax="1.0")) float MinimumSalience = 0.05f;
    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Witness") FGameplayTagContainer IgnoredEventTags;
private:
    UPROPERTY() TObjectPtr<UTVMemoryComponent> MemoryComponent = nullptr;
    UPROPERTY() TObjectPtr<UTVGameplayEventSubsystem> EventSubsystem = nullptr;
    UFUNCTION() void HandleGameplayEventEmitted(const FTVGameplayEventRecord& GameplayEvent);
    bool CanReceiveDirectEvent(const FTVGameplayEventRecord& GameplayEvent, FName OwnerContestantId) const;
    bool EvaluateVisualWitness(const FTVGameplayEventRecord& GameplayEvent, float& OutConfidence) const;
    bool EvaluateAuditoryWitness(const FTVGameplayEventRecord& GameplayEvent, float& OutConfidence) const;
    FTVMemoryRecord BuildMemory(const FTVGameplayEventRecord& GameplayEvent, FName OwnerContestantId, ETVInformationAcquisitionMethod AcquisitionMethod, float Confidence) const;
};
