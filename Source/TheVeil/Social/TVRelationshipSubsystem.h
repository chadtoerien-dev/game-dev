#pragma once
#include "CoreMinimal.h"
#include "Subsystems/WorldSubsystem.h"
#include "Core/TVGameplayTypes.h"
#include "TVRelationshipSubsystem.generated.h"
class UTVSocialInterpretationProfile;
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FTVRelationshipChanged, const FTVRelationshipRecord&, Relationship, const FTVRelationshipDelta&, AppliedDelta);
UCLASS(BlueprintType)
class THEVEIL_API UTVRelationshipSubsystem : public UWorldSubsystem
{
    GENERATED_BODY()
public:
    UPROPERTY(BlueprintAssignable, Category="The Veil|Relationships") FTVRelationshipChanged OnRelationshipChanged;
    UFUNCTION(BlueprintPure, Category="The Veil|Relationships") FTVRelationshipRecord GetRelationship(FName ObserverContestantId, FName SubjectContestantId) const;
    UFUNCTION(BlueprintCallable, Category="The Veil|Relationships") void SetRelationship(const FTVRelationshipRecord& Relationship, FName Reason);
    UFUNCTION(BlueprintCallable, Category="The Veil|Relationships") bool ApplyRelationshipDelta(FName ObserverContestantId, FName SubjectContestantId, const FTVRelationshipDelta& Delta, FGuid InfluencingMemoryId, FName Reason);
    UFUNCTION(BlueprintCallable, Category="The Veil|Relationships") bool ApplyMemoryInterpretation(const FTVMemoryRecord& Memory, const UTVSocialInterpretationProfile* InterpretationProfile);
    UFUNCTION(BlueprintPure, Category="The Veil|Relationships") TArray<FTVRelationshipRecord> GetAllRelationships() const { return Relationships; }
    UFUNCTION(BlueprintCallable, Category="The Veil|Relationships") void RestoreRelationships(const TArray<FTVRelationshipRecord>& Records, bool bBroadcastRestoredRelationships);
    UFUNCTION(BlueprintCallable, Category="The Veil|Relationships") void ResetRelationships();
private:
    UPROPERTY() TArray<FTVRelationshipRecord> Relationships;
    FTVRelationshipRecord* FindMutableRelationship(FName ObserverContestantId, FName SubjectContestantId);
    static void ClampRelationship(FTVRelationshipRecord& Relationship);
};
