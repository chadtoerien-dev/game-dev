#pragma once

#include "CoreMinimal.h"
#include "GameFramework/HUD.h"
#include "TVDevelopmentPromptHUD.generated.h"

UCLASS()
class THEVEIL_API ATVDevelopmentPromptHUD : public AHUD
{
    GENERATED_BODY()

public:
    virtual void DrawHUD() override;

protected:
    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="The Veil|Interaction")
    FLinearColor PromptColour = FLinearColor::White;

    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="The Veil|Interaction")
    FLinearColor PromptBackdropColour = FLinearColor(0.01f, 0.01f, 0.015f, 0.78f);

    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="The Veil|Interaction", meta=(ClampMin="0.0"))
    float PromptPadding = 10.0f;

    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="The Veil|Interaction", meta=(ClampMin="0.0", ClampMax="1.0"))
    float PromptScreenX = 0.08f;

    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="The Veil|Interaction", meta=(ClampMin="0.0", ClampMax="1.0"))
    float PromptScreenY = 0.78f;
};
