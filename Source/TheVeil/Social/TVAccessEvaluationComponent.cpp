#include "Social/TVAccessEvaluationComponent.h"

UTVAccessEvaluationComponent::UTVAccessEvaluationComponent()
{
    PrimaryComponentTick.bCanEverTick = false;
}

FTVAccessResult UTVAccessEvaluationComponent::EvaluateAccess(const FTVAccessContext& Context) const
{
    FTVAccessResult Result;
    float Score = Context.Trust * TrustScale;
    if (Context.bHasInvitation)
    {
        Score += InvitationBonus;
        Result.Reasons.Add(FText::FromString(TEXT("A valid invitation supports access.")));
    }
    if (Context.bHasMatchingDisguise)
    {
        Score += DisguiseBonus;
        Result.Reasons.Add(FText::FromString(TEXT("Clothing makes the access claim plausible.")));
        if (Context.bObserverRecognisesPlayer)
        {
            Score -= RecognisedDisguisePenalty;
            Result.Reasons.Add(FText::FromString(TEXT("The observer recognises the player beneath the disguise.")));
        }
    }
    if (Context.bIsRestrictedArea)
    {
        Score -= RestrictedAreaPenalty;
        Result.Reasons.Add(FText::FromString(TEXT("The area is restricted.")));
    }
    if (Context.bIsNight)
    {
        Score -= NightPenalty;
        Result.Reasons.Add(FText::FromString(TEXT("Night-time access requires stronger justification.")));
    }
    if (Context.bCarryingRestrictedItem)
    {
        Score -= RestrictedItemPenalty;
        Result.Reasons.Add(FText::FromString(TEXT("The carried item undermines the access claim.")));
    }
    Score -= Context.Fear * FearPenaltyScale;
    Result.Score = Score;
    if (Score >= 50.0f) Result.Decision = ETVAccessDecision::Authorised;
    else if (Score >= 25.0f) Result.Decision = ETVAccessDecision::Tolerated;
    else if (Score >= 5.0f) Result.Decision = ETVAccessDecision::Unusual;
    else if (Score >= -20.0f) Result.Decision = ETVAccessDecision::Questionable;
    else if (Score >= -50.0f) Result.Decision = ETVAccessDecision::Trespassing;
    else Result.Decision = ETVAccessDecision::Hostile;
    return Result;
}
