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
    if (InteractionComponent == nullptr)
    {
        return;
    }

    if (InteractionComponent->HasVisibleInteractionFeedback())
    {
        const FString Feedback = InteractionComponent->GetLastInteractionFeedback().ToString();
        const float FeedbackX = Canvas->ClipX * PromptScreenX;
        const float FeedbackY = Canvas->ClipY * FeedbackScreenY;
        float FeedbackWidth = 0.0f;
        float FeedbackHeight = 0.0f;
        GetTextSize(Feedback, FeedbackWidth, FeedbackHeight);
        DrawRect(
            PromptBackdropColour,
            FeedbackX - PromptPadding,
            FeedbackY - PromptPadding,
            FeedbackWidth + PromptPadding * 2.0f,
            FeedbackHeight + PromptPadding * 2.0f);
        DrawText(Feedback, FeedbackColour.ToFColor(true), FeedbackX, FeedbackY);
    }

    if (!InteractionComponent->HasFocusedInteractable())
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
