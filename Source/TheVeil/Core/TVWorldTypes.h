#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "Core/TVGameplayEnums.h"
#include "TVWorldTypes.generated.h"

USTRUCT(BlueprintType)
struct THEVEIL_API FTVAccessContext
{
    GENERATED_BODY()

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Access")
    float Trust = 0.0f;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Access")
    float Fear = 0.0f;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Access")
    bool bHasInvitation = false;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Access")
    bool bHasMatchingDisguise = false;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Access")
    bool bIsRestrictedArea = true;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Access")
    bool bIsNight = false;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Access")
    bool bObserverRecognisesPlayer = true;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Access")
    bool bCarryingRestrictedItem = false;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Access")
    FGameplayTagContainer ContextTags;
};

USTRUCT(BlueprintType)
struct THEVEIL_API FTVAccessResult
{
    GENERATED_BODY()

    UPROPERTY(BlueprintReadOnly, Category="Access")
    ETVAccessDecision Decision = ETVAccessDecision::Questionable;

    UPROPERTY(BlueprintReadOnly, Category="Access")
    float Score = 0.0f;

    UPROPERTY(BlueprintReadOnly, Category="Access")
    TArray<FText> Reasons;
};

USTRUCT(BlueprintType)
struct THEVEIL_API FTVOpportunityRuntimeState
{
    GENERATED_BODY()

    UPROPERTY(EditAnywhere, BlueprintReadWrite, SaveGame, Category="Opportunity")
    FGameplayTag OpportunityTag;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, SaveGame, Category="Opportunity")
    ETVOpportunityState State = ETVOpportunityState::Unknown;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, SaveGame, Category="Opportunity")
    float CurrentTrust = 0.0f;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, SaveGame, Category="Opportunity")
    FGameplayTagContainer SatisfiedContextTags;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, SaveGame, Category="Opportunity")
    FName LastReason = NAME_None;
};

USTRUCT(BlueprintType)
struct THEVEIL_API FTVEvidenceRecord
{
    GENERATED_BODY()

    UPROPERTY(EditAnywhere, BlueprintReadWrite, SaveGame, Category="Evidence")
    FGuid EvidenceId;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, SaveGame, Category="Evidence")
    FGameplayTag EvidenceTag;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, SaveGame, Category="Evidence")
    FGameplayTag LocationTag;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, SaveGame, Category="Evidence")
    FName SourceContestantId = NAME_None;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, SaveGame, Category="Evidence")
    FName AttributedContestantId = NAME_None;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, SaveGame, Category="Evidence", meta=(ClampMin="0.0", ClampMax="1.0"))
    float Reliability = 1.0f;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, SaveGame, Category="Evidence")
    bool bDiscovered = false;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, SaveGame, Category="Evidence")
    bool bPlanted = false;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, SaveGame, Category="Evidence")
    bool bSurfacedAtDawn = false;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, SaveGame, Category="Evidence")
    int32 CreatedRoundIndex = INDEX_NONE;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, SaveGame, Category="Evidence")
    ETheVeilPhase CreatedPhase = ETheVeilPhase::None;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, SaveGame, Category="Evidence")
    FText Notes;
};

USTRUCT(BlueprintType)
struct THEVEIL_API FTVGameplayEventRecord
{
    GENERATED_BODY()

    UPROPERTY(EditAnywhere, BlueprintReadWrite, SaveGame, Category="Event")
    FGuid EventId;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, SaveGame, Category="Event")
    FGameplayTag EventTag;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, SaveGame, Category="Event")
    FName InstigatorContestantId = NAME_None;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, SaveGame, Category="Event")
    FName AttributedContestantId = NAME_None;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, SaveGame, Category="Event")
    FName TargetContestantId = NAME_None;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, SaveGame, Category="Event")
    FGameplayTag LocationTag;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, SaveGame, Category="Event")
    FVector WorldLocation = FVector::ZeroVector;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, SaveGame, Category="Event")
    ETVEventAudience Audience = ETVEventAudience::LocalSenses;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, SaveGame, Category="Event")
    ETVSensoryChannel SensoryChannel = ETVSensoryChannel::Mixed;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, SaveGame, Category="Event", meta=(ClampMin="0.0", ClampMax="2.0"))
    float Loudness = 1.0f;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, SaveGame, Category="Event", meta=(ClampMin="0.0", ClampMax="1.0"))
    float BaseReliability = 1.0f;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, SaveGame, Category="Event", meta=(ClampMin="0.0", ClampMax="1.0"))
    float Salience = 0.5f;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, SaveGame, Category="Event", meta=(ClampMin="0.0"))
    float MaxWitnessDistance = 0.0f;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, SaveGame, Category="Event")
    bool bRevealInstigatorOnVisualWitness = true;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, SaveGame, Category="Event")
    FGameplayTagContainer ContextTags;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, SaveGame, Category="Event")
    FGuid RelatedEvidenceId;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, SaveGame, Category="Event")
    int32 CreatedRoundIndex = INDEX_NONE;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, SaveGame, Category="Event")
    ETheVeilPhase CreatedPhase = ETheVeilPhase::None;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, SaveGame, Category="Event")
    float WorldTimeSeconds = 0.0f;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, SaveGame, Category="Event")
    FText Summary;
};

USTRUCT(BlueprintType)
struct THEVEIL_API FTVContestantStateSnapshot
{
    GENERATED_BODY()

    UPROPERTY(EditAnywhere, BlueprintReadWrite, SaveGame, Category="Contestant")
    FName ContestantId = NAME_None;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, SaveGame, Category="Contestant")
    float TrustToPlayer = 0.0f;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, SaveGame, Category="Contestant")
    float FearOfPlayer = 0.0f;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, SaveGame, Category="Contestant")
    FGameplayTagContainer KnowledgeTags;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, SaveGame, Category="Contestant")
    FGameplayTagContainer AccessGrantTags;
};

USTRUCT(BlueprintType)
struct THEVEIL_API FTVRoutineSnapshot
{
    GENERATED_BODY()

    UPROPERTY(EditAnywhere, BlueprintReadWrite, SaveGame, Category="Routine")
    FName SaveId = NAME_None;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, SaveGame, Category="Routine")
    FGameplayTag CurrentRoutine;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, SaveGame, Category="Routine")
    FName LastChangeReason = NAME_None;
};

USTRUCT(BlueprintType)
struct THEVEIL_API FTVPhaseSnapshot
{
    GENERATED_BODY()

    UPROPERTY(EditAnywhere, BlueprintReadWrite, SaveGame, Category="Phase")
    ETheVeilPhase CurrentPhase = ETheVeilPhase::None;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, SaveGame, Category="Phase")
    int32 RoundIndex = 1;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, SaveGame, Category="Phase")
    int32 TransitionSerial = 0;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, SaveGame, Category="Phase")
    float PhaseDurationSeconds = 0.0f;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, SaveGame, Category="Phase")
    FName LastTransitionReason = NAME_None;
};
