#pragma once
#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "Core/TVGameplayTypes.h"
#include "TVSocialInterpretationProfile.generated.h"

USTRUCT(BlueprintType)
struct THEVEIL_API FTVInterpretationRule
{
    GENERATED_BODY()
    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Interpretation") FGameplayTag EventTag;
    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Interpretation", meta=(ClampMin="0.0", ClampMax="1.0")) float MinimumConfidence = 0.0f;
    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Interpretation") bool bScaleBySalience = true;
    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Interpretation") FTVRelationshipDelta BaseDelta;
    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Interpretation") FGameplayTagContainer RequiredContextTags;
    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Interpretation") FGameplayTagContainer BlockedContextTags;
};

UCLASS(BlueprintType)
class THEVEIL_API UTVSocialInterpretationProfile : public UPrimaryDataAsset
{
    GENERATED_BODY()
public:
    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Interpretation") TArray<FTVInterpretationRule> Rules;
    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Provenance", meta=(ClampMin="0.0", ClampMax="2.0")) float WitnessedMultiplier = 1.0f;
    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Provenance", meta=(ClampMin="0.0", ClampMax="2.0")) float HeardMultiplier = 0.75f;
    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Provenance", meta=(ClampMin="0.0", ClampMax="2.0")) float InferredMultiplier = 0.55f;
    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Provenance", meta=(ClampMin="0.0", ClampMax="2.0")) float ReportedMultiplier = 0.65f;
    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Provenance", meta=(ClampMin="0.0", ClampMax="2.0")) float RumourMultiplier = 0.4f;
    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Provenance", meta=(ClampMin="0.0", ClampMax="2.0")) float PhysicalEvidenceMultiplier = 1.1f;
    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Provenance", meta=(ClampMin="0.0", ClampMax="2.0")) float PublicAnnouncementMultiplier = 0.8f;
    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Provenance", meta=(ClampMin="0.0", ClampMax="2.0")) float FabricatedMultiplier = 0.6f;
    const FTVInterpretationRule* FindBestRule(const FGameplayTag& EventTag, const FGameplayTagContainer& ContextTags) const;
    float GetAcquisitionMultiplier(ETVInformationAcquisitionMethod AcquisitionMethod) const;
};
