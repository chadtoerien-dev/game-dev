#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "TVInteractable.generated.h"

UINTERFACE(BlueprintType)
class THEVEIL_API UTVInteractable : public UInterface
{
    GENERATED_BODY()
};

class THEVEIL_API ITVInteractable
{
    GENERATED_BODY()

public:
    UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category="The Veil|Interaction")
    FText GetInteractionPrompt(const AActor* Interactor) const;

    UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category="The Veil|Interaction")
    bool CanInteract(const AActor* Interactor, FText& OutFailureReason) const;

    UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category="The Veil|Interaction")
    bool Interact(AActor* Interactor);

    UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category="The Veil|Interaction")
    FText GetInteractionResultText(const AActor* Interactor) const;
};
