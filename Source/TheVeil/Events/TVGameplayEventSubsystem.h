#pragma once

#include "CoreMinimal.h"
#include "Subsystems/WorldSubsystem.h"
#include "Core/TVGameplayTypes.h"
#include "TVGameplayEventSubsystem.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(
    FTVGameplayEventEmitted,
    const FTVGameplayEventRecord&,
    GameplayEvent);

UCLASS(BlueprintType)
class THEVEIL_API UTVGameplayEventSubsystem : public UWorldSubsystem
{
    GENERATED_BODY()

public:
    UPROPERTY(BlueprintAssignable, Category="The Veil|Events")
    FTVGameplayEventEmitted OnGameplayEventEmitted;

    UFUNCTION(BlueprintCallable, Category="The Veil|Events")
    FGuid EmitGameplayEvent(FTVGameplayEventRecord GameplayEvent);

    UFUNCTION(BlueprintPure, Category="The Veil|Events")
    bool FindGameplayEvent(FGuid EventId, FTVGameplayEventRecord& OutEvent) const;

    UFUNCTION(BlueprintPure, Category="The Veil|Events")
    TArray<FTVGameplayEventRecord> GetAllGameplayEvents() const { return GameplayEvents; }

    UFUNCTION(BlueprintCallable, Category="The Veil|Events")
    void RestoreGameplayEvents(
        const TArray<FTVGameplayEventRecord>& Records,
        bool bBroadcastRestoredEvents);

    UFUNCTION(BlueprintCallable, Category="The Veil|Events")
    void ResetGameplayEvents();

private:
    UPROPERTY()
    TArray<FTVGameplayEventRecord> GameplayEvents;
};
