#pragma once
#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Core/TVGameplayTypes.h"
#include "TVMemoryComponent.generated.h"
class UTVSocialInterpretationProfile;
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FTVMemoryAdded, const FTVMemoryRecord&, Memory);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FTVMemoryChanged, const FTVMemoryRecord&, Memory);
UCLASS(ClassGroup=(TheVeil), meta=(BlueprintSpawnableComponent))
class THEVEIL_API UTVMemoryComponent : public UActorComponent
{
    GENERATED_BODY()
public:
    UTVMemoryComponent();
    UPROPERTY(BlueprintAssignable, Category="The Veil|Memory") FTVMemoryAdded OnMemoryAdded;
    UPROPERTY(BlueprintAssignable, Category="The Veil|Memory") FTVMemoryChanged OnMemoryChanged;
    UFUNCTION(BlueprintCallable, Category="The Veil|Memory") FGuid AddMemory(FTVMemoryRecord Memory, bool bApplyRelationshipInterpretation, bool bBroadcastMemory);
    UFUNCTION(BlueprintPure, Category="The Veil|Memory") bool FindMemory(FGuid MemoryId, FTVMemoryRecord& OutMemory) const;
    UFUNCTION(BlueprintPure, Category="The Veil|Memory") TArray<FTVMemoryRecord> GetAllMemories() const { return Memories; }
    UFUNCTION(BlueprintPure, Category="The Veil|Memory") TArray<FTVMemoryRecord> GetMemoriesAbout(FName SubjectContestantId) const;
    UFUNCTION(BlueprintPure, Category="The Veil|Memory") TArray<FTVMemoryRecord> GetMemoriesForEvent(FGuid OriginEventId) const;
    UFUNCTION(BlueprintCallable, Category="The Veil|Memory") bool SetTruthAssessment(FGuid MemoryId, ETVTruthAssessment NewAssessment, FName Reason);
    UFUNCTION(BlueprintCallable, Category="The Veil|Memory") bool AddSupportingEvidence(FGuid MemoryId, FGuid EvidenceId);
    UFUNCTION(BlueprintCallable, Category="The Veil|Memory") bool AddContradiction(FGuid MemoryId, FGuid ContradictingMemoryId);
    UFUNCTION(BlueprintPure, Category="The Veil|Memory") FTVMemoryComponentSnapshot CreateSnapshot() const;
    UFUNCTION(BlueprintCallable, Category="The Veil|Memory") bool RestoreSnapshot(const FTVMemoryComponentSnapshot& Snapshot, bool bBroadcastRestoredMemories);
    UFUNCTION(BlueprintCallable, Category="The Veil|Memory") void ResetMemories();
    UFUNCTION(BlueprintPure, Category="The Veil|Memory") FName ResolveOwnerContestantId() const;
protected:
    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Memory") FName ContestantIdOverride = NAME_None;
    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Memory") TObjectPtr<UTVSocialInterpretationProfile> InterpretationProfile = nullptr;
    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Memory", meta=(ClampMin="16", ClampMax="2048")) int32 MaximumMemories = 256;
private:
    UPROPERTY() TArray<FTVMemoryRecord> Memories;
    void TrimToMaximum();
};
