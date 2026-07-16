#pragma once

#include "CoreMinimal.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "TVPhaseSubsystem.generated.h"

class ATheVeilGameState;
class UTVRunSaveGame;
class USaveGame;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FTVCheckpointEvent, FName, CheckpointLabel);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FTVSaveOperationCompleted, bool, bSucceeded);

UCLASS(BlueprintType)
class THEVEIL_API UTVPhaseSubsystem : public UGameInstanceSubsystem
{
    GENERATED_BODY()

public:
    UPROPERTY(BlueprintAssignable, Category="The Veil|Save")
    FTVCheckpointEvent OnCheckpointCaptured;

    UPROPERTY(BlueprintAssignable, Category="The Veil|Save")
    FTVCheckpointEvent OnCheckpointRestored;

    UPROPERTY(BlueprintAssignable, Category="The Veil|Save")
    FTVSaveOperationCompleted OnSaveCompleted;

    UPROPERTY(BlueprintAssignable, Category="The Veil|Save")
    FTVSaveOperationCompleted OnLoadCompleted;

    UFUNCTION(BlueprintCallable, Category="The Veil|Save")
    void StartNewRun(int32 RunSeed);

    UFUNCTION(BlueprintCallable, Category="The Veil|Save", meta=(WorldContext="WorldContextObject"))
    bool CaptureCheckpoint(UObject* WorldContextObject, ATheVeilGameState* GameState, FName CheckpointLabel, bool bWriteToDisk);

    UFUNCTION(BlueprintCallable, Category="The Veil|Save", meta=(WorldContext="WorldContextObject"))
    bool RestoreCheckpoint(UObject* WorldContextObject, ATheVeilGameState* GameState, bool bRestorePhaseState, bool bBroadcastRestoredState);

    UFUNCTION(BlueprintCallable, Category="The Veil|Save")
    bool SaveActiveRunAsync();

    UFUNCTION(BlueprintCallable, Category="The Veil|Save")
    bool LoadActiveRunAsync();

    UFUNCTION(BlueprintCallable, Category="The Veil|Save")
    void ClearActiveRun(bool bDeleteDiskSave);

    UFUNCTION(BlueprintPure, Category="The Veil|Save")
    bool HasActiveRun() const { return ActiveRunSave != nullptr; }

    UFUNCTION(BlueprintPure, Category="The Veil|Save")
    UTVRunSaveGame* GetActiveRun() const { return ActiveRunSave; }

    UFUNCTION(BlueprintPure, Category="The Veil|Save")
    FString GetSaveSlotName() const { return SaveSlotName; }

protected:
    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Save")
    FString SaveSlotName = TEXT("TV_Run_Auto");

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Save")
    int32 UserIndex = 0;

private:
    UPROPERTY()
    TObjectPtr<UTVRunSaveGame> ActiveRunSave = nullptr;

    UPROPERTY()
    TObjectPtr<UTVRunSaveGame> PendingSaveObject = nullptr;

    bool bSaveInProgress = false;
    bool bSaveQueued = false;

    UTVRunSaveGame* EnsureActiveRun();
    void HandleAsyncSaveComplete(const FString& SlotName, int32 CompletedUserIndex, bool bSuccess);
    void HandleAsyncLoadComplete(const FString& SlotName, int32 CompletedUserIndex, USaveGame* LoadedGameData);
};
