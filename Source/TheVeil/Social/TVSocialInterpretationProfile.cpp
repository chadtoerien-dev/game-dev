#include "Social/TVSocialInterpretationProfile.h"

const FTVInterpretationRule* UTVSocialInterpretationProfile::FindBestRule(const FGameplayTag& EventTag, const FGameplayTagContainer& ContextTags) const
{
    const FTVInterpretationRule* BestRule = nullptr;
    int32 BestSpecificity = INDEX_NONE;
    for (const FTVInterpretationRule& Rule : Rules)
    {
        if (!Rule.EventTag.IsValid() || !EventTag.MatchesTag(Rule.EventTag) || !ContextTags.HasAll(Rule.RequiredContextTags) || ContextTags.HasAny(Rule.BlockedContextTags)) continue;
        int32 Specificity = 1;
        for (const TCHAR Character : Rule.EventTag.ToString()) if (Character == TEXT('.')) ++Specificity;
        if (BestRule == nullptr || Specificity > BestSpecificity) { BestRule = &Rule; BestSpecificity = Specificity; }
    }
    return BestRule;
}
float UTVSocialInterpretationProfile::GetAcquisitionMultiplier(const ETVInformationAcquisitionMethod Method) const
{
    switch (Method)
    {
    case ETVInformationAcquisitionMethod::Witnessed: return WitnessedMultiplier;
    case ETVInformationAcquisitionMethod::Heard: return HeardMultiplier;
    case ETVInformationAcquisitionMethod::Inferred: return InferredMultiplier;
    case ETVInformationAcquisitionMethod::Reported: return ReportedMultiplier;
    case ETVInformationAcquisitionMethod::Rumour: return RumourMultiplier;
    case ETVInformationAcquisitionMethod::PhysicalEvidence: return PhysicalEvidenceMultiplier;
    case ETVInformationAcquisitionMethod::PublicAnnouncement: return PublicAnnouncementMultiplier;
    case ETVInformationAcquisitionMethod::Fabricated: return FabricatedMultiplier;
    default: return 1.0f;
    }
}
