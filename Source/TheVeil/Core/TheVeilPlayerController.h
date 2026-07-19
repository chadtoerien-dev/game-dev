#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "TheVeilPlayerController.generated.h"

class UEnhancedInputLocalPlayerSubsystem;
class UInputAction;
class UInputComponent;
class UInputMappingContext;
struct FInputActionValue;

UCLASS(Blueprintable)
class THEVEIL_API ATheVeilPlayerController : public APlayerController
{
    GENERATED_BODY()

public:
    virtual void FlushPressedKeys() override;
    virtual void SetPawn(APawn* InPawn) override;

#if WITH_DEV_AUTOMATION_TESTS
    void ConfigureInputActionsForAutomationTests(
        UInputAction* InMoveAction,
        UInputAction* InLookAction,
        UInputAction* InJumpAction,
        UInputAction* InSprintAction,
        UInputAction* InCrouchAction);
    void ConfigureMappingContextForAutomationTests(UInputMappingContext* InMappingContext);
    void ApplyInputMappingContextForAutomationTests();
    void BindInputComponentForAutomationTests(UInputComponent* TestInputComponent);
    void RemoveOwnedInputMappingContextForAutomationTests();
#endif

protected:
    virtual void BeginPlay() override;
    virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;
    virtual void SetupInputComponent() override;

    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="The Veil|Input")
    TObjectPtr<UInputMappingContext> PlayerMappingContext;

    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="The Veil|Input")
    TObjectPtr<UInputAction> MoveAction;

    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="The Veil|Input")
    TObjectPtr<UInputAction> LookAction;

    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="The Veil|Input")
    TObjectPtr<UInputAction> JumpAction;

    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="The Veil|Input")
    TObjectPtr<UInputAction> SprintAction;

    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="The Veil|Input")
    TObjectPtr<UInputAction> CrouchAction;

private:
    void ApplyInputMappingContext();
    void BindConfiguredInputActions(UInputComponent* InputComponentToBind);
    void RemoveOwnedInputMappingContext();
    void ResetCharacterTransientState(APawn* PawnToReset) const;

    void HandleMove(const FInputActionValue& Value);
    void HandleLook(const FInputActionValue& Value);
    void HandleJumpStarted();
    void HandleJumpStopped();
    void HandleSprintStarted();
    void HandleSprintStopped();
    void HandleCrouchStarted();

    TWeakObjectPtr<UInputComponent> BoundInputComponent;
    TWeakObjectPtr<UEnhancedInputLocalPlayerSubsystem> RegisteredInputSubsystem;
    TWeakObjectPtr<UInputMappingContext> AddedMappingContext;
    bool bAddedMappingContext = false;
};
