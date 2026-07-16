#include "Core/TVPhaseRules.h"

bool UTVPhaseRules::IsTransitionAllowed(
    const ETheVeilPhase FromPhase,
    const ETheVeilPhase ToPhase,
    const bool bUseShortPrototypeLoop)
{
    return GetNextPhase(FromPhase, bUseShortPrototypeLoop) == ToPhase;
}

ETheVeilPhase UTVPhaseRules::GetNextPhase(
    const ETheVeilPhase CurrentPhase,
    const bool bUseShortPrototypeLoop)
{
    if (bUseShortPrototypeLoop)
    {
        switch (CurrentPhase)
        {
        case ETheVeilPhase::None: return ETheVeilPhase::SocialFreeRoam;
        case ETheVeilPhase::SocialFreeRoam: return ETheVeilPhase::NightPlanning;
        case ETheVeilPhase::NightPlanning: return ETheVeilPhase::NightOperation;
        case ETheVeilPhase::NightOperation: return ETheVeilPhase::DawnAssembly;
        case ETheVeilPhase::DawnAssembly: return ETheVeilPhase::SocialFreeRoam;
        default: return ETheVeilPhase::None;
        }
    }

    switch (CurrentPhase)
    {
    case ETheVeilPhase::None: return ETheVeilPhase::DawnAssembly;
    case ETheVeilPhase::DawnAssembly: return ETheVeilPhase::SocialFreeRoam;
    case ETheVeilPhase::SocialFreeRoam: return ETheVeilPhase::Challenge;
    case ETheVeilPhase::Challenge: return ETheVeilPhase::Lobbying;
    case ETheVeilPhase::Lobbying: return ETheVeilPhase::Tribunal;
    case ETheVeilPhase::Tribunal: return ETheVeilPhase::Vote;
    case ETheVeilPhase::Vote: return ETheVeilPhase::Exile;
    case ETheVeilPhase::Exile: return ETheVeilPhase::NightPlanning;
    case ETheVeilPhase::NightPlanning: return ETheVeilPhase::NightOperation;
    case ETheVeilPhase::NightOperation: return ETheVeilPhase::Aftermath;
    case ETheVeilPhase::Aftermath: return ETheVeilPhase::DawnAssembly;
    default: return ETheVeilPhase::None;
    }
}
