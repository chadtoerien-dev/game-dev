#pragma once

#include "CoreMinimal.h"
#include "Subsystems/WorldSubsystem.h"
#include "Core/TVGameplayTypes.h"
#include "TVVotingSubsystem.generated.h"

class UTVVotingProfile;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FTVVoteIntentionChanged, const FTVVoteIntention&, Intention);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FTVVoteLocked, FName, VoterContestantId, FName, CandidateContestantId);

UCLASS(BlueprintType)
class THEVEIL_API UTVVotingSubsystem : public UWorldSubsystem
{
    GENERATED_BODY()

public:
    UPROPERTY(BlueprintAssignable, Category="The Veil|Voting") FTVVoteIntentionChanged OnVoteIntentionChanged;
    UPROPERTY(BlueprintAssignable, Category="The Veil|Voting") FTVVoteLocked OnVoteLocked;

    UFUNCTION(BlueprintCallable, Category="The Veil|Voting") void SetVoteCommitment(const FTVVoteCommitment& Commitment);
    UFUNCTION(BlueprintCallable, Category="The Veil|Voting") bool RemoveVoteCommitment(FName VoterContestantId, FName CandidateContestantId);
    UFUNCTION(BlueprintPure, Category="The Veil|Voting") TArray<FTVVoteCommitment> GetCommitments() const { return Commitments; }

    UFUNCTION(BlueprintCallable, Category="The Veil|Voting")
    FTVVoteIntention EvaluateCandidate(FName VoterContestantId, FName CandidateContestantId, const UTVVotingProfile* Profile, float GroupPressureRatio, int32 RoundIndex, FName Reason) const;

    UFUNCTION(BlueprintCallable, Category="The Veil|Voting")
    void RecalculateAllIntentions(const TArray<FName>& VoterContestantIds, const TArray<FName>& CandidateContestantIds, const UTVVotingProfile* Profile, int32 RoundIndex, FName Reason);

    UFUNCTION(BlueprintCallable, Category="The Veil|Voting")
    TArray<FTVVoteIntention> LockVotes(const TArray<FName>& VoterContestantIds, const TArray<FName>& CandidateContestantIds, const UTVVotingProfile* Profile, int32 RoundIndex, FName Reason);

    UFUNCTION(BlueprintPure, Category="The Veil|Voting") TArray<FTVVoteIntention> GetAllIntentions() const { return Intentions; }
    UFUNCTION(BlueprintPure, Category="The Veil|Voting") TArray<FTVVoteIntention> GetIntentionsForVoter(FName VoterContestantId) const;
    UFUNCTION(BlueprintPure, Category="The Veil|Voting") bool GetLeadingIntention(FName VoterContestantId, FTVVoteIntention& OutIntention) const;
    UFUNCTION(BlueprintCallable, Category="The Veil|Voting") void RestoreState(const TArray<FTVVoteCommitment>& SavedCommitments, const TArray<FTVVoteIntention>& SavedIntentions, bool bBroadcastRestoredState);
    UFUNCTION(BlueprintCallable, Category="The Veil|Voting") void ResetVoting();

private:
    UPROPERTY() TArray<FTVVoteCommitment> Commitments;
    UPROPERTY() TArray<FTVVoteIntention> Intentions;

    static void AddFactor(FTVVoteIntention& Intention, FName FactorName, float RawValue, float Weight, const FText& Explanation);
    float GetCommitmentStrength(FName VoterContestantId, FName CandidateContestantId) const;
    float CalculateClaimPressure(FName VoterContestantId, FName CandidateContestantId, const UTVVotingProfile* Profile, FTVVoteIntention& Intention) const;
    float CalculateContradictionPressure(FName VoterContestantId, FName CandidateContestantId, const UTVVotingProfile* Profile, FTVVoteIntention& Intention) const;
    static float MakeDeterministicTieBreaker(FName VoterContestantId, FName CandidateContestantId, int32 RoundIndex, float Scale);
};
