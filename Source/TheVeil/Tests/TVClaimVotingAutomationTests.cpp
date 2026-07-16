#if WITH_DEV_AUTOMATION_TESTS

#include "Misc/AutomationTest.h"
#include "Claims/TVClaimRules.h"

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FTVDirectClaimContradictionTest, "TheVeil.Claims.DirectContradiction", EAutomationTestFlags::EditorContext | EAutomationTestFlags::EngineFilter)
bool FTVDirectClaimContradictionTest::RunTest(const FString& Parameters)
{
    const FGameplayTag EventTag = FGameplayTag::RequestGameplayTag(FName(TEXT("Event.Operation.ArchiveSabotage")), false);
    const FGameplayTag TopicTag = FGameplayTag::RequestGameplayTag(FName(TEXT("Claim.Topic.ArchiveSabotage")), false);
    TestTrue(TEXT("Operation event tag exists"), EventTag.IsValid());
    TestTrue(TEXT("Claim topic tag exists"), TopicTag.IsValid());
    if (!EventTag.IsValid() || !TopicTag.IsValid()) return false;

    FTVClaimRecord Accusation;
    Accusation.ClaimId = FGuid::NewGuid();
    Accusation.SpeakerContestantId = TEXT("Eleanor");
    Accusation.SubjectContestantId = TEXT("Player");
    Accusation.EventTag = EventTag;
    Accusation.TopicTag = TopicTag;
    Accusation.Stance = ETVClaimStance::Accuse;
    Accusation.Confidence = 0.9f;
    Accusation.AudienceContestantIds = { TEXT("Marcus"), TEXT("Beatrice") };

    FTVClaimRecord Denial;
    Denial.ClaimId = FGuid::NewGuid();
    Denial.SpeakerContestantId = TEXT("Player");
    Denial.SubjectContestantId = TEXT("Player");
    Denial.EventTag = EventTag;
    Denial.TopicTag = TopicTag;
    Denial.Stance = ETVClaimStance::Deny;
    Denial.Confidence = 0.75f;
    Denial.AudienceContestantIds = { TEXT("Marcus"), TEXT("Beatrice") };

    const FTVContradictionRecord Contradiction = UTVClaimRules::EvaluateContradiction(Accusation, Denial);
    TestEqual(TEXT("Direct assertion and denial is critical"), Contradiction.Severity, ETVContradictionSeverity::Critical);
    TestTrue(TEXT("Contradiction is known by shared audience"), Contradiction.KnownByContestantIds.Contains(TEXT("Marcus")) && Contradiction.KnownByContestantIds.Contains(TEXT("Beatrice")));
    TestTrue(TEXT("Confidence is bounded"), Contradiction.Confidence >= 0.0f && Contradiction.Confidence <= 1.0f);
    return true;
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FTVLocationClaimContradictionTest, "TheVeil.Claims.LocationContradiction", EAutomationTestFlags::EditorContext | EAutomationTestFlags::EngineFilter)
bool FTVLocationClaimContradictionTest::RunTest(const FString& Parameters)
{
    const FGameplayTag EventTag = FGameplayTag::RequestGameplayTag(FName(TEXT("Event.Social.PrivateMeeting")), false);
    const FGameplayTag TopicTag = FGameplayTag::RequestGameplayTag(FName(TEXT("Claim.Topic.Alibi")), false);
    const FGameplayTag Archive = FGameplayTag::RequestGameplayTag(FName(TEXT("Location.Archive")), false);
    const FGameplayTag Corridor = FGameplayTag::RequestGameplayTag(FName(TEXT("Location.GuestCorridor")), false);
    if (!EventTag.IsValid() || !TopicTag.IsValid() || !Archive.IsValid() || !Corridor.IsValid())
    {
        AddError(TEXT("Required claim test tags are not registered."));
        return false;
    }

    FTVClaimRecord First;
    First.ClaimId = FGuid::NewGuid();
    First.SpeakerContestantId = TEXT("Marcus");
    First.SubjectContestantId = TEXT("Marcus");
    First.EventTag = EventTag;
    First.TopicTag = TopicTag;
    First.ClaimedLocationTag = Archive;
    First.Stance = ETVClaimStance::Assert;
    First.Confidence = 0.8f;
    First.AudienceContestantIds = { TEXT("Eleanor") };

    FTVClaimRecord Second = First;
    Second.ClaimId = FGuid::NewGuid();
    Second.ClaimedLocationTag = Corridor;
    const FTVContradictionRecord Contradiction = UTVClaimRules::EvaluateContradiction(First, Second);
    TestEqual(TEXT("Conflicting alibi locations are material"), Contradiction.Severity, ETVContradictionSeverity::Material);
    TestEqual(TEXT("Self-contradiction assigns the same speaker at fault"), Contradiction.AtFaultSpeakerContestantId, FName(TEXT("Marcus")));
    return true;
}
#endif
