#if WITH_DEV_AUTOMATION_TESTS
#include "Misc/AutomationTest.h"
#include "Memory/TVInformationRules.h"
#include "Social/TVSocialInterpretationProfile.h"

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FTVInformationProvenanceTest, "TheVeil.Information.ProvenanceTransmission", EAutomationTestFlags::EditorContext | EAutomationTestFlags::EngineFilter)
bool FTVInformationProvenanceTest::RunTest(const FString& Parameters)
{
    const FGameplayTag SabotageTag = FGameplayTag::RequestGameplayTag(FName(TEXT("Event.Operation.ArchiveSabotage")), false);
    TestTrue(TEXT("Sabotage tag is registered"), SabotageTag.IsValid());
    if (!SabotageTag.IsValid()) return false;

    FTVMemoryRecord Source;
    Source.MemoryId = FGuid::NewGuid();
    Source.OwnerContestantId = TEXT("Eleanor");
    Source.EventTag = SabotageTag;
    Source.SubjectContestantId = TEXT("Player");
    Source.TargetContestantId = TEXT("Eleanor");
    Source.Confidence = 0.95f;
    Source.PerceivedReliability = 0.9f;
    Source.Salience = 0.9f;
    Source.Provenance.OriginEventId = FGuid::NewGuid();
    Source.Provenance.OriginalObserverContestantId = TEXT("Eleanor");
    Source.Provenance.AcquisitionMethod = ETVInformationAcquisitionMethod::Witnessed;

    const FTVInformationMutation NoMutation;
    const FTVInformationTransmissionResult Report = UTVInformationRules::BuildTransmittedMemory(Source, TEXT("Eleanor"), TEXT("Marcus"), ETVInformationTransmissionMode::Report, NoMutation, 12345);
    TestTrue(TEXT("Report succeeds"), Report.bSucceeded);
    TestEqual(TEXT("Report owner is recipient"), Report.Memory.OwnerContestantId, FName(TEXT("Marcus")));
    TestEqual(TEXT("Immediate source is Eleanor"), Report.Memory.Provenance.ImmediateSourceContestantId, FName(TEXT("Eleanor")));
    TestEqual(TEXT("Report depth increments"), Report.Memory.Provenance.TransmissionDepth, 1);
    TestTrue(TEXT("Report confidence is below source confidence"), Report.Memory.Confidence < Source.Confidence);

    const FTVInformationTransmissionResult Rumour = UTVInformationRules::BuildTransmittedMemory(Report.Memory, TEXT("Marcus"), TEXT("Beatrice"), ETVInformationTransmissionMode::Rumour, NoMutation, 67890);
    TestTrue(TEXT("Rumour succeeds"), Rumour.bSucceeded);
    TestEqual(TEXT("Rumour depth increments again"), Rumour.Memory.Provenance.TransmissionDepth, 2);
    TestTrue(TEXT("Rumour has at least one distortion"), Rumour.Memory.Provenance.DistortionCount >= 1);
    TestTrue(TEXT("Rumour confidence is below report confidence"), Rumour.Memory.Confidence < Report.Memory.Confidence);

    FTVInformationMutation LieMutation;
    LieMutation.bOverrideSubjectContestant = true;
    LieMutation.SubjectContestantId = TEXT("Marcus");
    LieMutation.AddedNotes = FText::FromString(TEXT("The speaker changed the alleged culprit."));
    const FTVInformationTransmissionResult Lie = UTVInformationRules::BuildTransmittedMemory(Source, TEXT("Eleanor"), TEXT("Beatrice"), ETVInformationTransmissionMode::DeliberateLie, LieMutation, 24680);
    TestTrue(TEXT("Deliberate lie succeeds"), Lie.bSucceeded);
    TestEqual(TEXT("Lie changes the alleged subject"), Lie.Memory.SubjectContestantId, FName(TEXT("Marcus")));
    TestTrue(TEXT("Lie is marked internally as deliberate"), Lie.Memory.Provenance.bDeliberateLie);
    TestTrue(TEXT("Lie mutation increases distortion count"), Lie.Memory.Provenance.DistortionCount >= 2);
    return true;
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FTVInterpretationRuleSelectionTest, "TheVeil.Information.InterpretationRuleSelection", EAutomationTestFlags::EditorContext | EAutomationTestFlags::EngineFilter)
bool FTVInterpretationRuleSelectionTest::RunTest(const FString& Parameters)
{
    UTVSocialInterpretationProfile* Profile = NewObject<UTVSocialInterpretationProfile>();
    const FGameplayTag OperationTag = FGameplayTag::RequestGameplayTag(FName(TEXT("Event.Operation")), false);
    const FGameplayTag SabotageTag = FGameplayTag::RequestGameplayTag(FName(TEXT("Event.Operation.ArchiveSabotage")), false);
    TestTrue(TEXT("Operation parent tag is registered"), OperationTag.IsValid());
    TestTrue(TEXT("Sabotage child tag is registered"), SabotageTag.IsValid());
    if (!OperationTag.IsValid() || !SabotageTag.IsValid()) return false;

    FTVInterpretationRule GeneralRule;
    GeneralRule.EventTag = OperationTag;
    GeneralRule.BaseDelta.Suspicion = 10.0f;
    FTVInterpretationRule SpecificRule;
    SpecificRule.EventTag = SabotageTag;
    SpecificRule.BaseDelta.Suspicion = 30.0f;
    Profile->Rules.Add(GeneralRule);
    Profile->Rules.Add(SpecificRule);
    const FTVInterpretationRule* Found = Profile->FindBestRule(SabotageTag, FGameplayTagContainer());
    TestNotNull(TEXT("A matching rule is found"), Found);
    if (Found != nullptr) TestEqual(TEXT("Most specific rule wins"), Found->BaseDelta.Suspicion, 30.0f);
    return true;
}
#endif
