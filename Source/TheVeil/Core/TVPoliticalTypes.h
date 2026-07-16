#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "Core/TVGameplayEnums.h"
#include "TVPoliticalTypes.generated.h"

USTRUCT(BlueprintType)
struct THEVEIL_API FTVClaimRecord
{
    GENERATED_BODY()

    UPROPERTY(EditAnywhere, BlueprintReadWrite, SaveGame, Category="Claim")
    FGuid ClaimId;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, SaveGame, Category="Claim")
    FName SpeakerContestantId = NAME_None;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, SaveGame, Category="Claim")
    FName SubjectContestantId = NAME_None;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, SaveGame, Category="Claim")
    FName TargetContestantId = NAME_None;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, SaveGame, Category="Claim")
    FGameplayTag EventTag;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, SaveGame, Category="Claim")
    FGameplayTag TopicTag;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, SaveGame, Category="Claim")
    FGameplayTag ClaimedLocationTag;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, SaveGame, Category="Claim")
    ETheVeilPhase ClaimedPhase = ETheVeilPhase::None;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, SaveGame, Category="Claim")
    ETVClaimStance Stance = ETVClaimStance::Assert;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, SaveGame, Category="Claim")
    ETVClaimVisibility Visibility = ETVClaimVisibility::Private;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, SaveGame, Category="Claim")
    TArray<FName> AudienceContestantIds;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, SaveGame, Category="Claim", meta=(ClampMin="0.0", ClampMax="1.0"))
    float Confidence = 0.5f;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, SaveGame, Category="Claim")
    TArray<FGuid> SourceMemoryIds;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, SaveGame, Category="Claim")
    TArray<FGuid> SupportingEvidenceIds;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, SaveGame, Category="Claim")
    bool bDeliberateLie = false;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, SaveGame, Category="Claim")
    bool bWithdrawn = false;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, SaveGame, Category="Claim")
    int32 CreatedRoundIndex = INDEX_NONE;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, SaveGame, Category="Claim")
    ETheVeilPhase CreatedPhase = ETheVeilPhase::None;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, SaveGame, Category="Claim")
    float CreatedWorldTimeSeconds = 0.0f;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, SaveGame, Category="Claim")
    FText StatementSummary;
};

USTRUCT(BlueprintType)
struct THEVEIL_API FTVContradictionRecord
{
    GENERATED_BODY()

    UPROPERTY(EditAnywhere, BlueprintReadWrite, SaveGame, Category="Contradiction")
    FGuid ContradictionId;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, SaveGame, Category="Contradiction")
    FGuid ClaimAId;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, SaveGame, Category="Contradiction")
    FGuid ClaimBId;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, SaveGame, Category="Contradiction")
    FName AtFaultSpeakerContestantId = NAME_None;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, SaveGame, Category="Contradiction")
    ETVContradictionSeverity Severity = ETVContradictionSeverity::None;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, SaveGame, Category="Contradiction", meta=(ClampMin="0.0", ClampMax="1.0"))
    float Confidence = 0.0f;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, SaveGame, Category="Contradiction")
    TArray<FName> KnownByContestantIds;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, SaveGame, Category="Contradiction")
    bool bResolved = false;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, SaveGame, Category="Contradiction")
    FName ResolutionReason = NAME_None;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, SaveGame, Category="Contradiction")
    FText Explanation;
};

USTRUCT(BlueprintType)
struct THEVEIL_API FTVVoteCommitment
{
    GENERATED_BODY()

    UPROPERTY(EditAnywhere, BlueprintReadWrite, SaveGame, Category="Voting")
    FName VoterContestantId = NAME_None;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, SaveGame, Category="Voting")
    FName CandidateContestantId = NAME_None;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, SaveGame, Category="Voting", meta=(ClampMin="0.0", ClampMax="1.0"))
    float Strength = 0.0f;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, SaveGame, Category="Voting")
    FName PromisedToContestantId = NAME_None;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, SaveGame, Category="Voting")
    bool bPublicCommitment = false;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, SaveGame, Category="Voting")
    FName Reason = NAME_None;
};

USTRUCT(BlueprintType)
struct THEVEIL_API FTVVoteScoreFactor
{
    GENERATED_BODY()

    UPROPERTY(EditAnywhere, BlueprintReadWrite, SaveGame, Category="Voting")
    FName FactorName = NAME_None;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, SaveGame, Category="Voting")
    float RawValue = 0.0f;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, SaveGame, Category="Voting")
    float Weight = 0.0f;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, SaveGame, Category="Voting")
    float Contribution = 0.0f;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, SaveGame, Category="Voting")
    FText Explanation;
};

USTRUCT(BlueprintType)
struct THEVEIL_API FTVVoteIntention
{
    GENERATED_BODY()

    UPROPERTY(EditAnywhere, BlueprintReadWrite, SaveGame, Category="Voting")
    FName VoterContestantId = NAME_None;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, SaveGame, Category="Voting")
    FName CandidateContestantId = NAME_None;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, SaveGame, Category="Voting")
    ETVVoteIntentStatus Status = ETVVoteIntentStatus::Undecided;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, SaveGame, Category="Voting")
    float TotalScore = 0.0f;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, SaveGame, Category="Voting")
    TArray<FTVVoteScoreFactor> Factors;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, SaveGame, Category="Voting")
    FName LastEvaluationReason = NAME_None;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, SaveGame, Category="Voting")
    int32 RoundIndex = INDEX_NONE;
};
