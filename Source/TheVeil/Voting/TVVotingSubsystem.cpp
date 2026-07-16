#include "Voting/TVVotingSubsystem.h"

#include "Claims/TVClaimRules.h"
#include "Claims/TVClaimSubsystem.h"
#include "Social/TVRelationshipSubsystem.h"
#include "Voting/TVVotingProfile.h"
#include "Engine/World.h"

void UTVVotingSubsystem::SetVoteCommitment(const FTVVoteCommitment& Commitment)
{
    if (Commitment.VoterContestantId.IsNone() || Commitment.CandidateContestantId.IsNone() || Commitment.VoterContestantId == Commitment.CandidateContestantId)
    {
        return;
    }

    FTVVoteCommitment Sanitised = Commitment;
    Sanitised.Strength = FMath::Clamp(Sanitised.Strength, 0.0f, 1.0f);
    if (FTVVoteCommitment* Existing = Commitments.FindByPredicate([&Sanitised](const FTVVoteCommitment& Item)
    {
        return Item.VoterContestantId == Sanitised.VoterContestantId && Item.CandidateContestantId == Sanitised.CandidateContestantId;
    }))
    {
        *Existing = Sanitised;
        return;
    }
    Commitments.Add(Sanitised);
}

bool UTVVotingSubsystem::RemoveVoteCommitment(const FName VoterContestantId, const FName CandidateContestantId)
{
    return Commitments.RemoveAll([VoterContestantId, CandidateContestantId](const FTVVoteCommitment& Item)
    {
        return Item.VoterContestantId == VoterContestantId && Item.CandidateContestantId == CandidateContestantId;
    }) > 0;
}

FTVVoteIntention UTVVotingSubsystem::EvaluateCandidate(const FName VoterContestantId, const FName CandidateContestantId, const UTVVotingProfile* Profile, const float GroupPressureRatio, const int32 RoundIndex, const FName Reason) const
{
    FTVVoteIntention Result;
    Result.VoterContestantId = VoterContestantId;
    Result.CandidateContestantId = CandidateContestantId;
    Result.RoundIndex = RoundIndex;
    Result.LastEvaluationReason = Reason;

    if (!IsValid(Profile) || VoterContestantId.IsNone() || CandidateContestantId.IsNone() || VoterContestantId == CandidateContestantId || GetWorld() == nullptr)
    {
        return Result;
    }

    const UTVRelationshipSubsystem* Relationships = GetWorld()->GetSubsystem<UTVRelationshipSubsystem>();
    if (!IsValid(Relationships))
    {
        return Result;
    }

    const FTVRelationshipRecord Relationship = Relationships->GetRelationship(VoterContestantId, CandidateContestantId);
    AddFactor(Result, TEXT("Suspicion"), Relationship.Suspicion, Profile->SuspicionWeight, FText::FromString(TEXT("Personal suspicion.")));
    AddFactor(Result, TEXT("Distrust"), 100.0f - Relationship.Trust, Profile->DistrustWeight, FText::FromString(TEXT("Low trust.")));
    AddFactor(Result, TEXT("LowAffinity"), 100.0f - Relationship.Affinity, Profile->LowAffinityWeight, FText::FromString(TEXT("Weak affinity.")));
    AddFactor(Result, TEXT("Fear"), Relationship.Fear, Profile->FearWeight, FText::FromString(TEXT("Defensive fear.")));
    AddFactor(Result, TEXT("PerceivedThreat"), 0.5f * (Relationship.Respect + Relationship.Credibility), Profile->ThreatWeight, FText::FromString(TEXT("Strategic threat.")));

    CalculateClaimPressure(VoterContestantId, CandidateContestantId, Profile, Result);
    CalculateContradictionPressure(VoterContestantId, CandidateContestantId, Profile, Result);
    AddFactor(Result, TEXT("Commitment"), GetCommitmentStrength(VoterContestantId, CandidateContestantId), Profile->CommitmentWeight, FText::FromString(TEXT("Voting promise.")));
    AddFactor(Result, TEXT("GroupPressure"), FMath::Clamp(GroupPressureRatio, 0.0f, 1.0f), Profile->GroupPressureWeight, FText::FromString(TEXT("Visible group pressure.")));
    AddFactor(Result, TEXT("TieBreaker"), 1.0f, MakeDeterministicTieBreaker(VoterContestantId, CandidateContestantId, RoundIndex, Profile->TieBreakerScale), FText::FromString(TEXT("Stable tie-breaker.")));

    for (const FTVVoteScoreFactor& Factor : Result.Factors)
    {
        Result.TotalScore += Factor.Contribution;
    }
    if (Result.TotalScore >= Profile->CommitmentThreshold) Result.Status = ETVVoteIntentStatus::Committed;
    else if (Result.TotalScore >= Profile->LeaningThreshold) Result.Status = ETVVoteIntentStatus::Leaning;
    return Result;
}

void UTVVotingSubsystem::RecalculateAllIntentions(const TArray<FName>& VoterContestantIds, const TArray<FName>& CandidateContestantIds, const UTVVotingProfile* Profile, const int32 RoundIndex, const FName Reason)
{
    Intentions.Reset();
    if (!IsValid(Profile)) return;

    TMap<FName, int32> ProvisionalSupport;
    for (const FName Voter : VoterContestantIds)
    {
        TArray<FTVVoteIntention> Evaluations;
        for (const FName Candidate : CandidateContestantIds)
        {
            if (Candidate != Voter) Evaluations.Add(EvaluateCandidate(Voter, Candidate, Profile, 0.0f, RoundIndex, TEXT("InitialVoteEvaluation")));
        }
        Evaluations.Sort([](const FTVVoteIntention& A, const FTVVoteIntention& B) { return A.TotalScore > B.TotalScore; });
        if (Evaluations.Num() > 0) ProvisionalSupport.FindOrAdd(Evaluations[0].CandidateContestantId)++;
    }

    const float VoterCount = static_cast<float>(FMath::Max(1, VoterContestantIds.Num()));
    for (const FName Voter : VoterContestantIds)
    {
        for (const FName Candidate : CandidateContestantIds)
        {
            if (Candidate == Voter) continue;
            const float Pressure = static_cast<float>(ProvisionalSupport.FindRef(Candidate)) / VoterCount;
            Intentions.Add(EvaluateCandidate(Voter, Candidate, Profile, Pressure, RoundIndex, Reason));
            OnVoteIntentionChanged.Broadcast(Intentions.Last());
        }
    }
}

TArray<FTVVoteIntention> UTVVotingSubsystem::LockVotes(const TArray<FName>& VoterContestantIds, const TArray<FName>& CandidateContestantIds, const UTVVotingProfile* Profile, const int32 RoundIndex, const FName Reason)
{
    RecalculateAllIntentions(VoterContestantIds, CandidateContestantIds, Profile, RoundIndex, Reason);
    TArray<FTVVoteIntention> Locked;
    for (const FName Voter : VoterContestantIds)
    {
        FTVVoteIntention Leading;
        if (!GetLeadingIntention(Voter, Leading)) continue;
        for (FTVVoteIntention& Intention : Intentions)
        {
            if (Intention.VoterContestantId == Voter && Intention.CandidateContestantId == Leading.CandidateContestantId)
            {
                Intention.Status = ETVVoteIntentStatus::Locked;
                Intention.LastEvaluationReason = Reason;
                Leading = Intention;
                break;
            }
        }
        Locked.Add(Leading);
        OnVoteLocked.Broadcast(Voter, Leading.CandidateContestantId);
    }
    return Locked;
}

TArray<FTVVoteIntention> UTVVotingSubsystem::GetIntentionsForVoter(const FName VoterContestantId) const
{
    return Intentions.FilterByPredicate([VoterContestantId](const FTVVoteIntention& Intention) { return Intention.VoterContestantId == VoterContestantId; });
}

bool UTVVotingSubsystem::GetLeadingIntention(const FName VoterContestantId, FTVVoteIntention& OutIntention) const
{
    const TArray<FTVVoteIntention> VoterIntentions = GetIntentionsForVoter(VoterContestantId);
    if (VoterIntentions.Num() == 0) return false;
    OutIntention = VoterIntentions[0];
    for (const FTVVoteIntention& Intention : VoterIntentions)
    {
        if (Intention.TotalScore > OutIntention.TotalScore) OutIntention = Intention;
    }
    return true;
}

void UTVVotingSubsystem::RestoreState(const TArray<FTVVoteCommitment>& SavedCommitments, const TArray<FTVVoteIntention>& SavedIntentions, const bool bBroadcastRestoredState)
{
    Commitments = SavedCommitments;
    Intentions = SavedIntentions;
    if (bBroadcastRestoredState)
    {
        for (const FTVVoteIntention& Intention : Intentions)
        {
            OnVoteIntentionChanged.Broadcast(Intention);
            if (Intention.Status == ETVVoteIntentStatus::Locked) OnVoteLocked.Broadcast(Intention.VoterContestantId, Intention.CandidateContestantId);
        }
    }
}

void UTVVotingSubsystem::ResetVoting()
{
    Commitments.Reset();
    Intentions.Reset();
}

void UTVVotingSubsystem::AddFactor(FTVVoteIntention& Intention, const FName FactorName, const float RawValue, const float Weight, const FText& Explanation)
{
    FTVVoteScoreFactor Factor;
    Factor.FactorName = FactorName;
    Factor.RawValue = RawValue;
    Factor.Weight = Weight;
    Factor.Contribution = RawValue * Weight;
    Factor.Explanation = Explanation;
    Intention.Factors.Add(Factor);
}

float UTVVotingSubsystem::GetCommitmentStrength(const FName VoterContestantId, const FName CandidateContestantId) const
{
    float Strength = 0.0f;
    for (const FTVVoteCommitment& Commitment : Commitments)
    {
        if (Commitment.VoterContestantId == VoterContestantId && Commitment.CandidateContestantId == CandidateContestantId) Strength = FMath::Max(Strength, Commitment.Strength);
    }
    return Strength;
}

float UTVVotingSubsystem::CalculateClaimPressure(const FName VoterContestantId, const FName CandidateContestantId, const UTVVotingProfile* Profile, FTVVoteIntention& Intention) const
{
    if (GetWorld() == nullptr) return 0.0f;
    const UTVClaimSubsystem* Claims = GetWorld()->GetSubsystem<UTVClaimSubsystem>();
    const UTVRelationshipSubsystem* Relationships = GetWorld()->GetSubsystem<UTVRelationshipSubsystem>();
    if (!IsValid(Claims) || !IsValid(Relationships)) return 0.0f;

    float Pressure = 0.0f;
    for (const FTVClaimRecord& Claim : Claims->GetClaimsKnownBy(VoterContestantId))
    {
        if (Claim.bWithdrawn || Claim.SubjectContestantId != CandidateContestantId) continue;
        const FTVRelationshipRecord SpeakerRelationship = Relationships->GetRelationship(VoterContestantId, Claim.SpeakerContestantId);
        const float ClaimStrength = Claim.Confidence * FMath::Clamp(SpeakerRelationship.Credibility / 100.0f, 0.0f, 1.0f);
        if (Claim.Stance == ETVClaimStance::Accuse || Claim.Stance == ETVClaimStance::Assert) Pressure += ClaimStrength;
        else if (Claim.Stance == ETVClaimStance::Defend || Claim.Stance == ETVClaimStance::Deny) Pressure -= ClaimStrength * (Profile->DefenceClaimWeight / FMath::Max(1.0f, Profile->AccusationClaimWeight));
    }
    AddFactor(Intention, TEXT("KnownClaims"), Pressure, Profile->AccusationClaimWeight, FText::FromString(TEXT("Known claims scaled by speaker credibility.")));
    return Pressure;
}

float UTVVotingSubsystem::CalculateContradictionPressure(const FName VoterContestantId, const FName CandidateContestantId, const UTVVotingProfile* Profile, FTVVoteIntention& Intention) const
{
    if (GetWorld() == nullptr) return 0.0f;
    const UTVClaimSubsystem* Claims = GetWorld()->GetSubsystem<UTVClaimSubsystem>();
    if (!IsValid(Claims)) return 0.0f;

    float Pressure = 0.0f;
    for (const FTVContradictionRecord& Contradiction : Claims->GetContradictionsKnownBy(VoterContestantId))
    {
        if (!Contradiction.bResolved && Contradiction.AtFaultSpeakerContestantId == CandidateContestantId)
        {
            Pressure += UTVClaimRules::GetSeverityScalar(Contradiction.Severity) * Contradiction.Confidence;
        }
    }
    AddFactor(Intention, TEXT("Contradictions"), Pressure, Profile->ContradictionWeight, FText::FromString(TEXT("Known unresolved contradictions.")));
    return Pressure;
}

float UTVVotingSubsystem::MakeDeterministicTieBreaker(const FName VoterContestantId, const FName CandidateContestantId, const int32 RoundIndex, const float Scale)
{
    const uint32 Hash = HashCombine(HashCombine(GetTypeHash(VoterContestantId), GetTypeHash(CandidateContestantId)), GetTypeHash(RoundIndex));
    return static_cast<float>(Hash % 1000) / 1000.0f * Scale;
}
