#pragma once

#include "CoreMinimal.h"
#include "Subsystems/WorldSubsystem.h"
#include "Core/TVGameplayTypes.h"
#include "TVEvidenceSubsystem.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(
    FTVEvidenceAdded,
    const FTVEvidenceRecord&,
    Evidence);

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(
    FTVEvidenceSurfacedAtDawn,
    const FTVEvidenceRecord&,
    Evidence);

UCLASS(BlueprintType)
class THEVEIL_API UTVEvidenceSubsystem : public UWorldSubsystem
{
    GENERATED_BODY()

public:
    UPROPERTY(BlueprintAssignable, Category="The Veil|Evidence")
    FTVEvidenceAdded OnEvidenceAdded;

    UPROPERTY(BlueprintAssignable, Category="The Veil|Evidence")
    FTVEvidenceSurfacedAtDawn OnEvidenceSurfacedAtDawn;

    UFUNCTION(BlueprintCallable, Category="The Veil|Evidence")
    FGuid AddEvidence(FTVEvidenceRecord Evidence);

    UFUNCTION(BlueprintCallable, Category="The Veil|Evidence")
    bool SetEvidenceDiscovered(FGuid EvidenceId, bool bDiscovered);

    UFUNCTION(BlueprintCallable, Category="The Veil|Evidence")
    TArray<FTVEvidenceRecord> SurfaceEvidenceForDawn(int32 CurrentRoundIndex);

    UFUNCTION(BlueprintPure, Category="The Veil|Evidence")
    TArray<FTVEvidenceRecord> GetAllEvidence() const { return EvidenceRecords; }

    UFUNCTION(BlueprintPure, Category="The Veil|Evidence")
    TArray<FTVEvidenceRecord> GetLastDawnEvidence() const { return LastDawnEvidence; }

    UFUNCTION(BlueprintCallable, Category="The Veil|Evidence")
    void RestoreEvidenceRecords(
        const TArray<FTVEvidenceRecord>& Records,
        bool bBroadcastAdded);

    UFUNCTION(BlueprintCallable, Category="The Veil|Evidence")
    void ResetEvidence();

private:
    UPROPERTY()
    TArray<FTVEvidenceRecord> EvidenceRecords;

    UPROPERTY()
    TArray<FTVEvidenceRecord> LastDawnEvidence;
};
