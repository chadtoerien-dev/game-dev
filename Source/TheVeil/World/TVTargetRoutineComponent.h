#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "GameplayTagContainer.h"
#include "Core/TVGameplayTypes.h"
#include "TVTargetRoutineComponent.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_FourParams(
    FTVRoutineChanged,
    FGameplayTag, PreviousRoutine,
    FGameplayTag, NewRoutine,
    FName, InstigatorContestantId,
    FName, Reason);

UCLASS(ClassGroup=(TheVeil), meta=(BlueprintSpawnableComponent))
class THEVEIL_API UTVTargetRoutineComponent : public UActorComponent
{
    GENERATED_BODY()

public:
    UTVTargetRoutineComponent();

    UPROPERTY(BlueprintAssignable, Category="The Veil|Routine")
    FTVRoutineChanged OnRoutineChanged;

    UFUNCTION(BlueprintCallable, Category="The Veil|Routine")
    bool SetRoutine(FGameplayTag NewRoutine, FName InstigatorContestantId, FName Reason);

    UFUNCTION(BlueprintPure, Category="The Veil|Routine")
    FGameplayTag GetCurrentRoutine() const { return CurrentRoutine; }

    UFUNCTION(BlueprintPure, Category="The Veil|Routine")
    FName GetLastChangeReason() const { return LastChangeReason; }

    UFUNCTION(BlueprintPure, Category="The Veil|Routine")
    FName GetSaveId() const { return SaveId; }

    UFUNCTION(BlueprintPure, Category="The Veil|Routine")
    FTVRoutineSnapshot CreateSnapshot() const;

    UFUNCTION(BlueprintCallable, Category="The Veil|Routine")
    bool RestoreSnapshot(const FTVRoutineSnapshot& Snapshot, bool bBroadcastChange);

protected:
    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Routine")
    FName SaveId = NAME_None;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Routine")
    FGameplayTag CurrentRoutine;

    UPROPERTY(BlueprintReadOnly, Category="Routine")
    FName LastChangeReason = NAME_None;
};
