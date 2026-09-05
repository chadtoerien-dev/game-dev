#include "UI/TVDevelopmentPromptHUD.h"

#include "Engine/Canvas.h"
#include "GameFramework/Pawn.h"
#include "Interaction/TVInteractionComponent.h"

void ATVDevelopmentPromptHUD::DrawHUD()
{
    Super::DrawHUD();

    if (Canvas == nullptr || PlayerOwner == nullptr)
    {
        return;
    }

    const APawn* Pawn = PlayerOwner->GetPawn();
    const UTVInteractionComponent* InteractionComponent =
        Pawn != nullptr ? Pawn->FindComponentByClass<UTVInteractionComponent>() : nullptr;
    if (InteractionComponent == nullptr || !InteractionComponent->HasFocusedInteractable())
    {
        return;
    }

    const FString Prompt = FString::Printf(
        TEXT("Press E / face-left: %s"),
        *InteractionComponent->GetCurrentPrompt().ToString());

    const float PromptX = Canvas->ClipX * PromptScreenX;
    const float PromptY = Canvas->ClipY * PromptScreenY;
    float PromptWidth = 0.0f;
    float PromptHeight = 0.0f;
    GetTextSize(Prompt, PromptWidth, PromptHeight);
    DrawRect(
        PromptBackdropColour,
        PromptX - PromptPadding,
        PromptY - PromptPadding,
        PromptWidth + PromptPadding * 2.0f,
        PromptHeight + PromptPadding * 2.0f);
    DrawText(
        Prompt,
        PromptColour.ToFColor(true),
        PromptX,
        PromptY);
}
