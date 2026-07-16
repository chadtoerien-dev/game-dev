#if WITH_AUTOMATION_TESTS
#include "Core/TVPhaseRules.h"
#include "Misc/AutomationTest.h"
IMPLEMENT_SIMPLE_AUTOMATION_TEST(FTVShortPhaseLoopTest, "TheVeil.Phase.ShortLoop", EAutomationTestFlags::EditorContext | EAutomationTestFlags::EngineFilter)
bool FTVShortPhaseLoopTest::RunTest(const FString& Parameters)
{
    TestEqual(TEXT("None starts Social"), static_cast<uint8>(UTVPhaseRules::GetNextPhase(ETheVeilPhase::None, true)), static_cast<uint8>(ETheVeilPhase::SocialFreeRoam));
    TestEqual(TEXT("Social moves to Night Planning"), static_cast<uint8>(UTVPhaseRules::GetNextPhase(ETheVeilPhase::SocialFreeRoam, true)), static_cast<uint8>(ETheVeilPhase::NightPlanning));
    TestEqual(TEXT("Night Planning moves to Night Operation"), static_cast<uint8>(UTVPhaseRules::GetNextPhase(ETheVeilPhase::NightPlanning, true)), static_cast<uint8>(ETheVeilPhase::NightOperation));
    TestEqual(TEXT("Night Operation moves to Dawn"), static_cast<uint8>(UTVPhaseRules::GetNextPhase(ETheVeilPhase::NightOperation, true)), static_cast<uint8>(ETheVeilPhase::DawnAssembly));
    TestEqual(TEXT("Dawn starts next Social phase"), static_cast<uint8>(UTVPhaseRules::GetNextPhase(ETheVeilPhase::DawnAssembly, true)), static_cast<uint8>(ETheVeilPhase::SocialFreeRoam));
    TestFalse(TEXT("Social cannot jump directly to Dawn"), UTVPhaseRules::IsTransitionAllowed(ETheVeilPhase::SocialFreeRoam, ETheVeilPhase::DawnAssembly, true));
    return true;
}
#endif
