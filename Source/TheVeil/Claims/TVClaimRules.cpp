#include "Claims/TVClaimRules.h"

namespace
{
float GetClaimStrength(const FTVClaimRecord& Claim)
{
    const float EvidenceBonus = FMath::Min(0.35f, Claim.SupportingEvidenceIds.Num() * 0.08f);
    const float MemoryBonus = FMath::Min(0.15f, Claim.SourceMemoryIds.Num() * 0.03f);
    return FMath::Clamp(Claim.Confidence + EvidenceBonus + MemoryBonus, 0.0f, 1.5f);
}
bool IsPositiveAssertion(const ETVClaimStance Stance)
{
    return Stance == ETVClaimStance::Assert || Stance == ETVClaimStance::Accuse || Stance == ETVClaimStance::Support;
}
}

FTVContradictionRecord UTVClaimRules::EvaluateContradiction(const FTVClaimRecord& A, const FTVClaimRecord& B)
{
    FTVContradictionRecord Result;
    if (!A.ClaimId.IsValid() || !B.ClaimId.IsValid() || A.ClaimId == B.ClaimId || A.bWithdrawn || B.bWithdrawn || !ClaimsShareTopic(A, B)) return Result;
    bool bContradiction = false;
    ETVContradictionSeverity Severity = ETVContradictionSeverity::None;
    FText Explanation;
    const bool bOpposing = (IsPositiveAssertion(A.Stance) && B.Stance == ETVClaimStance::Deny) || (IsPositiveAssertion(B.Stance) && A.Stance == ETVClaimStance::Deny);
    if (bOpposing)
    {
        bContradiction = true; Severity = ETVContradictionSeverity::Critical;
        Explanation = FText::FromString(TEXT("The claims directly assert and deny the same material event."));
    }
    else if (IsPositiveAssertion(A.Stance) && IsPositiveAssertion(B.Stance) && A.ClaimedLocationTag.IsValid() && B.ClaimedLocationTag.IsValid() && A.ClaimedLocationTag != B.ClaimedLocationTag)
    {
        bContradiction = true; Severity = ETVContradictionSeverity::Material;
        Explanation = FText::FromString(TEXT("The claims place the same subject and event in different locations."));
    }
    else if (IsPositiveAssertion(A.Stance) && IsPositiveAssertion(B.Stance) && A.ClaimedPhase != ETheVeilPhase::None && B.ClaimedPhase != ETheVeilPhase::None && A.ClaimedPhase != B.ClaimedPhase)
    {
        bContradiction = true; Severity = ETVContradictionSeverity::Material;
        Explanation = FText::FromString(TEXT("The claims place the same event in incompatible phases."));
    }
    else if (A.SpeakerContestantId == B.SpeakerContestantId && A.Stance == ETVClaimStance::Retract && B.Stance != ETVClaimStance::Retract)
    {
        bContradiction = true; Severity = ETVContradictionSeverity::Minor;
        Explanation = FText::FromString(TEXT("The speaker has retracted an earlier material statement."));
    }
    if (!bContradiction) return Result;
    Result.ContradictionId = FGuid::NewGuid();
    Result.ClaimAId = A.ClaimId;
    Result.ClaimBId = B.ClaimId;
    Result.Severity = Severity;
    Result.Explanation = Explanation;
    Result.KnownByContestantIds = FindSharedAudience(A, B);
    const float StrengthA = GetClaimStrength(A);
    const float StrengthB = GetClaimStrength(B);
    Result.Confidence = FMath::Clamp(0.5f * FMath::Min(A.Confidence, B.Confidence) + 0.25f * FMath::Abs(StrengthA - StrengthB) + 0.15f * GetSeverityScalar(Severity), 0.0f, 1.0f);
    if (A.SpeakerContestantId == B.SpeakerContestantId) Result.AtFaultSpeakerContestantId = A.SpeakerContestantId;
    else if (StrengthA + 0.15f < StrengthB) Result.AtFaultSpeakerContestantId = A.SpeakerContestantId;
    else if (StrengthB + 0.15f < StrengthA) Result.AtFaultSpeakerContestantId = B.SpeakerContestantId;
    return Result;
}
float UTVClaimRules::GetSeverityScalar(const ETVContradictionSeverity Severity)
{
    switch (Severity)
    {
    case ETVContradictionSeverity::Minor: return 0.35f;
    case ETVContradictionSeverity::Material: return 0.7f;
    case ETVContradictionSeverity::Critical: return 1.0f;
    default: return 0.0f;
    }
}
bool UTVClaimRules::ClaimsShareTopic(const FTVClaimRecord& A, const FTVClaimRecord& B)
{
    if (A.SubjectContestantId.IsNone() || A.SubjectContestantId != B.SubjectContestantId) return false;
    if (A.TopicTag.IsValid() && B.TopicTag.IsValid()) return A.TopicTag == B.TopicTag;
    return A.EventTag.IsValid() && A.EventTag == B.EventTag;
}
TArray<FName> UTVClaimRules::FindSharedAudience(const FTVClaimRecord& A, const FTVClaimRecord& B)
{
    TArray<FName> Shared;
    for (const FName Id : A.AudienceContestantIds) if (!Id.IsNone() && B.AudienceContestantIds.Contains(Id)) Shared.AddUnique(Id);
    if (A.SpeakerContestantId == B.SpeakerContestantId && !A.SpeakerContestantId.IsNone()) Shared.AddUnique(A.SpeakerContestantId);
    return Shared;
}
