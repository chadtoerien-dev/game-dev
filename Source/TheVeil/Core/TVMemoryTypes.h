#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "Core/TVGameplayEnums.h"
#include "TVMemoryTypes.generated.h"

USTRUCT(BlueprintType)
struct THEVEIL_API FTVInformationProvenance
{
    GENERATED_BODY()

    UPROPERTY(EditAnywhere, BlueprintReadWrite, SaveGame, Category="Information")
    FGuid OriginEventId;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, SaveGame, Category="Information")
    FGuid ParentMemoryId;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, SaveGame, Category="Information")
    FName OriginalObserverContestantId = NAME_None;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, SaveGame, Category="Information")
    FName ImmediateSourceContestantId = NAME_None;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, SaveGame, Category="Information")
    ETVInformationAcquisitionMethod AcquisitionMethod = ETVInformationAcquisitionMethod::Witnessed;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, SaveGame, Category="Information")
    int32 TransmissionDepth = 0;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, SaveGame, Category="Information")
    int32 DistortionCount = 0;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, SaveGame, Category="Information")
    bool bDeliberateLie = false;
};

USTRUCT(BlueprintType)
struct THEVEIL_API FTVMemoryRecord
{
    GENERATED_BODY()

    UPROPERTY(EditAnywhere, BlueprintReadWrite, SaveGame, Category="Memory")
    FGuid MemoryId;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, SaveGame, Category="Memory")
    FName OwnerContestantId = NAME_None;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, SaveGame, Category="Memory")
    FGameplayTag EventTag;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, SaveGame, Category="Memory")
    FName SubjectContestantId = NAME_None;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, SaveGame, Category="Memory")
    FName TargetContestantId = NAME_None;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, SaveGame, Category="Memory")
    FGameplayTag LocationTag;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, SaveGame, Category="Memory", meta=(ClampMin="0.0", ClampMax="1.0"))
    float Confidence = 1.0f;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, SaveGame, Category="Memory", meta=(ClampMin="0.0", ClampMax="1.0"))
    float PerceivedReliability = 1.0f;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, SaveGame, Category="Memory", meta=(ClampMin="0.0", ClampMax="1.0"))
    float Salience = 0.5f;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, SaveGame, Category="Memory")
    ETVTruthAssessment TruthAssessment = ETVTruthAssessment::BelievedTrue;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, SaveGame, Category="Memory")
    FTVInformationProvenance Provenance;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, SaveGame, Category="Memory")
    FGameplayTagContainer ContextTags;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, SaveGame, Category="Memory")
    TArray<FGuid> SupportingEvidenceIds;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, SaveGame, Category="Memory")
    TArray<FGuid> ContradictingMemoryIds;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, SaveGame, Category="Memory")
    int32 CreatedRoundIndex = INDEX_NONE;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, SaveGame, Category="Memory")
    ETheVeilPhase CreatedPhase = ETheVeilPhase::None;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, SaveGame, Category="Memory")
    float LearnedWorldTimeSeconds = 0.0f;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, SaveGame, Category="Memory")
    FText Notes;
};

USTRUCT(BlueprintType)
struct THEVEIL_API FTVInformationMutation
{
    GENERATED_BODY()

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Information")
    bool bOverrideEventTag = false;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Information", meta=(EditCondition="bOverrideEventTag"))
    FGameplayTag EventTag;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Information")
    bool bOverrideSubjectContestant = false;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Information", meta=(EditCondition="bOverrideSubjectContestant"))
    FName SubjectContestantId = NAME_None;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Information")
    bool bOverrideTargetContestant = false;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Information", meta=(EditCondition="bOverrideTargetContestant"))
    FName TargetContestantId = NAME_None;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Information", meta=(ClampMin="0.0", ClampMax="2.0"))
    float ConfidenceMultiplier = 1.0f;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Information", meta=(ClampMin="0.0", ClampMax="2.0"))
    float ReliabilityMultiplier = 1.0f;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Information")
    FText AddedNotes;
};

USTRUCT(BlueprintType)
struct THEVEIL_API FTVInformationTransmissionResult
{
    GENERATED_BODY()

    UPROPERTY(BlueprintReadOnly, Category="Information")
    bool bSucceeded = false;

    UPROPERTY(BlueprintReadOnly, Category="Information")
    FTVMemoryRecord Memory;

    UPROPERTY(BlueprintReadOnly, Category="Information")
    FText FailureReason;
};

USTRUCT(BlueprintType)
struct THEVEIL_API FTVRelationshipDelta
{
    GENERATED_BODY()

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Relationship")
    float Trust = 0.0f;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Relationship")
    float Suspicion = 0.0f;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Relationship")
    float Affinity = 0.0f;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Relationship")
    float Respect = 0.0f;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Relationship")
    float Fear = 0.0f;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Relationship")
    float Credibility = 0.0f;
};

USTRUCT(BlueprintType)
struct THEVEIL_API FTVRelationshipRecord
{
    GENERATED_BODY()

    UPROPERTY(EditAnywhere, BlueprintReadWrite, SaveGame, Category="Relationship")
    FName ObserverContestantId = NAME_None;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, SaveGame, Category="Relationship")
    FName SubjectContestantId = NAME_None;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, SaveGame, Category="Relationship", meta=(ClampMin="0.0", ClampMax="100.0"))
    float Trust = 50.0f;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, SaveGame, Category="Relationship", meta=(ClampMin="0.0", ClampMax="100.0"))
    float Suspicion = 0.0f;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, SaveGame, Category="Relationship", meta=(ClampMin="0.0", ClampMax="100.0"))
    float Affinity = 50.0f;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, SaveGame, Category="Relationship", meta=(ClampMin="0.0", ClampMax="100.0"))
    float Respect = 50.0f;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, SaveGame, Category="Relationship", meta=(ClampMin="0.0", ClampMax="100.0"))
    float Fear = 0.0f;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, SaveGame, Category="Relationship", meta=(ClampMin="0.0", ClampMax="100.0"))
    float Credibility = 50.0f;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, SaveGame, Category="Relationship")
    FGuid LastInfluencingMemoryId;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, SaveGame, Category="Relationship")
    FName LastChangeReason = NAME_None;
};

USTRUCT(BlueprintType)
struct THEVEIL_API FTVMemoryComponentSnapshot
{
    GENERATED_BODY()

    UPROPERTY(EditAnywhere, BlueprintReadWrite, SaveGame, Category="Memory")
    FName ContestantId = NAME_None;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, SaveGame, Category="Memory")
    TArray<FTVMemoryRecord> Memories;
};
