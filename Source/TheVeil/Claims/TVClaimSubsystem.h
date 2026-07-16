#pragma once
#include "CoreMinimal.h"
#include "Subsystems/WorldSubsystem.h"
#include "Core/TVGameplayTypes.h"
#include "TVClaimSubsystem.generated.h"
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FTVClaimRegistered, const FTVClaimRecord&, Claim);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FTVContradictionDetected, const FTVContradictionRecord&, Contradiction);
UCLASS(BlueprintType)
class THEVEIL_API UTVClaimSubsystem : public UWorldSubsystem
{
    GENERATED_BODY()
public:
    UPROPERTY(BlueprintAssignable, Category="The Veil|Claims") FTVClaimRegistered OnClaimRegistered;
    UPROPERTY(BlueprintAssignable, Category="The Veil|Claims") FTVContradictionDetected OnContradictionDetected;
    UFUNCTION(BlueprintCallable, Category="The Veil|Claims") FGuid RegisterClaim(FTVClaimRecord Claim, bool bEvaluateContradictions);
    UFUNCTION(BlueprintCallable, Category="The Veil|Claims") FGuid CreateClaimFromMemory(AActor* SpeakerActor, FGuid MemoryId, ETVClaimStance Stance, ETVClaimVisibility Visibility, const TArray<FName>& AudienceContestantIds, FGameplayTag TopicTag, FText StatementSummary, bool bDeliberateLie);
    UFUNCTION(BlueprintCallable, Category="The Veil|Claims") bool WithdrawClaim(FGuid ClaimId, FName Reason);
    UFUNCTION(BlueprintPure, Category="The Veil|Claims") bool FindClaim(FGuid ClaimId, FTVClaimRecord& OutClaim) const;
    UFUNCTION(BlueprintPure, Category="The Veil|Claims") TArray<FTVClaimRecord> GetAllClaims() const { return Claims; }
    UFUNCTION(BlueprintPure, Category="The Veil|Claims") TArray<FTVClaimRecord> GetClaimsAbout(FName SubjectContestantId) const;
    UFUNCTION(BlueprintPure, Category="The Veil|Claims") TArray<FTVClaimRecord> GetClaimsKnownBy(FName ContestantId) const;
    UFUNCTION(BlueprintPure, Category="The Veil|Claims") TArray<FTVContradictionRecord> GetAllContradictions() const { return Contradictions; }
    UFUNCTION(BlueprintPure, Category="The Veil|Claims") TArray<FTVContradictionRecord> GetContradictionsKnownBy(FName ContestantId) const;
    UFUNCTION(BlueprintCallable, Category="The Veil|Claims") void RestoreState(const TArray<FTVClaimRecord>& ClaimRecords, const TArray<FTVContradictionRecord>& ContradictionRecords, bool bBroadcastRestoredState);
    UFUNCTION(BlueprintCallable, Category="The Veil|Claims") void ResetClaims();
private:
    UPROPERTY() TArray<FTVClaimRecord> Claims;
    UPROPERTY() TArray<FTVContradictionRecord> Contradictions;
    void EvaluateClaimAgainstExisting(const FTVClaimRecord& NewClaim);
    void ApplyContradictionConsequences(const FTVContradictionRecord& Contradiction);
    void EmitContradictionEvent(const FTVContradictionRecord& Contradiction);
};
