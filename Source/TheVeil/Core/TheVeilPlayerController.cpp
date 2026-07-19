#include "Core/TheVeilPlayerController.h"

#include "Characters/TheVeilCharacter.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "Engine/LocalPlayer.h"
#include "InputAction.h"
#include "InputActionValue.h"
#include "InputMappingContext.h"

DEFINE_LOG_CATEGORY_STATIC(LogTheVeilPlayerController, Log, All);

void ATheVeilPlayerController::BeginPlay()
{
    Super::BeginPlay();
    ApplyInputMappingContext();
}

void ATheVeilPlayerController::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
    ResetCharacterTransientState(GetPawn());
    RemoveOwnedInputMappingContext();
    BoundInputComponent.Reset();
    Super::EndPlay(EndPlayReason);
}

void ATheVeilPlayerController::SetupInputComponent()
{
    Super::SetupInputComponent();
    ApplyInputMappingContext();
    BindConfiguredInputActions(InputComponent);
}

void ATheVeilPlayerController::BindConfiguredInputActions(UInputComponent* InputComponentToBind)
{
    if (BoundInputComponent.Get() == InputComponentToBind)
    {
        return;
    }

    UEnhancedInputComponent* EnhancedInputComponent = Cast<UEnhancedInputComponent>(InputComponentToBind);
    if (EnhancedInputComponent == nullptr)
    {
        UE_LOG(LogTheVeilPlayerController, Error,
            TEXT("%s requires an Enhanced Input component; no playable input was bound."), *GetNameSafe(this));
        return;
    }

    if (MoveAction != nullptr)
    {
        EnhancedInputComponent->BindAction(MoveAction, ETriggerEvent::Triggered, this, &ATheVeilPlayerController::HandleMove);
    }
    else
    {
        UE_LOG(LogTheVeilPlayerController, Warning, TEXT("MoveAction is not assigned on %s."), *GetNameSafe(this));
    }

    if (LookAction != nullptr)
    {
        EnhancedInputComponent->BindAction(LookAction, ETriggerEvent::Triggered, this, &ATheVeilPlayerController::HandleLook);
    }
    else
    {
        UE_LOG(LogTheVeilPlayerController, Warning, TEXT("LookAction is not assigned on %s."), *GetNameSafe(this));
    }

    if (JumpAction != nullptr)
    {
        EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Started, this, &ATheVeilPlayerController::HandleJumpStarted);
        EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Completed, this, &ATheVeilPlayerController::HandleJumpStopped);
        EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Canceled, this, &ATheVeilPlayerController::HandleJumpStopped);
    }
    else
    {
        UE_LOG(LogTheVeilPlayerController, Warning, TEXT("JumpAction is not assigned on %s."), *GetNameSafe(this));
    }

    if (SprintAction != nullptr)
    {
        EnhancedInputComponent->BindAction(SprintAction, ETriggerEvent::Started, this, &ATheVeilPlayerController::HandleSprintStarted);
        EnhancedInputComponent->BindAction(SprintAction, ETriggerEvent::Completed, this, &ATheVeilPlayerController::HandleSprintStopped);
        EnhancedInputComponent->BindAction(SprintAction, ETriggerEvent::Canceled, this, &ATheVeilPlayerController::HandleSprintStopped);
    }
    else
    {
        UE_LOG(LogTheVeilPlayerController, Warning, TEXT("SprintAction is not assigned on %s."), *GetNameSafe(this));
    }

    if (CrouchAction != nullptr)
    {
        EnhancedInputComponent->BindAction(CrouchAction, ETriggerEvent::Started, this, &ATheVeilPlayerController::HandleCrouchStarted);
    }
    else
    {
        UE_LOG(LogTheVeilPlayerController, Warning, TEXT("CrouchAction is not assigned on %s."), *GetNameSafe(this));
    }

    BoundInputComponent = InputComponentToBind;
}

void ATheVeilPlayerController::FlushPressedKeys()
{
    Super::FlushPressedKeys();
    ResetCharacterTransientState(GetPawn());
}

void ATheVeilPlayerController::SetPawn(APawn* InPawn)
{
    APawn* PreviousPawn = GetPawn();
    if (PreviousPawn != InPawn)
    {
        ResetCharacterTransientState(PreviousPawn);
    }

    Super::SetPawn(InPawn);

    if (PreviousPawn != InPawn)
    {
        ResetCharacterTransientState(InPawn);
    }
}

#if WITH_DEV_AUTOMATION_TESTS
void ATheVeilPlayerController::ConfigureInputActionsForAutomationTests(
    UInputAction* InMoveAction,
    UInputAction* InLookAction,
    UInputAction* InJumpAction,
    UInputAction* InSprintAction,
    UInputAction* InCrouchAction)
{
    MoveAction = InMoveAction;
    LookAction = InLookAction;
    JumpAction = InJumpAction;
    SprintAction = InSprintAction;
    CrouchAction = InCrouchAction;
}

void ATheVeilPlayerController::BindInputComponentForAutomationTests(
    UInputComponent* TestInputComponent)
{
    BindConfiguredInputActions(TestInputComponent);
}

void ATheVeilPlayerController::ConfigureMappingContextForAutomationTests(
    UInputMappingContext* InMappingContext)
{
    PlayerMappingContext = InMappingContext;
}

void ATheVeilPlayerController::ApplyInputMappingContextForAutomationTests()
{
    ApplyInputMappingContext();
}

void ATheVeilPlayerController::RemoveOwnedInputMappingContextForAutomationTests()
{
    RemoveOwnedInputMappingContext();
}
#endif

void ATheVeilPlayerController::ApplyInputMappingContext()
{
    if (!IsLocalPlayerController())
    {
        return;
    }

    if (PlayerMappingContext == nullptr)
    {
        UE_LOG(LogTheVeilPlayerController, Warning, TEXT("PlayerMappingContext is not assigned on %s."), *GetNameSafe(this));
        return;
    }

    ULocalPlayer* LocalPlayer = GetLocalPlayer();
    UEnhancedInputLocalPlayerSubsystem* InputSubsystem =
        ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(LocalPlayer);
    if (InputSubsystem == nullptr)
    {
        UE_LOG(LogTheVeilPlayerController, Warning,
            TEXT("No Enhanced Input local-player subsystem is available for %s."), *GetNameSafe(this));
        return;
    }

    if (bAddedMappingContext &&
        RegisteredInputSubsystem.Get() == InputSubsystem &&
        AddedMappingContext.Get() == PlayerMappingContext &&
        InputSubsystem->HasMappingContext(PlayerMappingContext))
    {
        return;
    }

    if (bAddedMappingContext)
    {
        RemoveOwnedInputMappingContext();
    }

    RegisteredInputSubsystem = InputSubsystem;
    if (!InputSubsystem->HasMappingContext(PlayerMappingContext))
    {
        InputSubsystem->AddMappingContext(PlayerMappingContext, 0);
        AddedMappingContext = PlayerMappingContext;
        bAddedMappingContext = true;
    }
}

void ATheVeilPlayerController::RemoveOwnedInputMappingContext()
{
    UEnhancedInputLocalPlayerSubsystem* InputSubsystem = RegisteredInputSubsystem.Get();
    UInputMappingContext* MappingContext = AddedMappingContext.Get();

    if (bAddedMappingContext && InputSubsystem != nullptr && MappingContext != nullptr &&
        InputSubsystem->HasMappingContext(MappingContext))
    {
        InputSubsystem->RemoveMappingContext(MappingContext);
    }

    bAddedMappingContext = false;
    AddedMappingContext.Reset();
    RegisteredInputSubsystem.Reset();
}

void ATheVeilPlayerController::ResetCharacterTransientState(APawn* PawnToReset) const
{
    if (ATheVeilCharacter* VeilCharacter = Cast<ATheVeilCharacter>(PawnToReset))
    {
        VeilCharacter->ResetTransientMovementState();
    }
}

void ATheVeilPlayerController::HandleMove(const FInputActionValue& Value)
{
    if (ATheVeilCharacter* VeilCharacter = Cast<ATheVeilCharacter>(GetPawn()))
    {
        VeilCharacter->Move(Value.Get<FVector2D>());
    }
}

void ATheVeilPlayerController::HandleLook(const FInputActionValue& Value)
{
    if (ATheVeilCharacter* VeilCharacter = Cast<ATheVeilCharacter>(GetPawn()))
    {
        VeilCharacter->Look(Value.Get<FVector2D>());
    }
}

void ATheVeilPlayerController::HandleJumpStarted()
{
    if (ATheVeilCharacter* VeilCharacter = Cast<ATheVeilCharacter>(GetPawn()))
    {
        VeilCharacter->StartJump();
    }
}

void ATheVeilPlayerController::HandleJumpStopped()
{
    if (ATheVeilCharacter* VeilCharacter = Cast<ATheVeilCharacter>(GetPawn()))
    {
        VeilCharacter->StopJump();
    }
}

void ATheVeilPlayerController::HandleSprintStarted()
{
    if (ATheVeilCharacter* VeilCharacter = Cast<ATheVeilCharacter>(GetPawn()))
    {
        VeilCharacter->StartSprint();
    }
}

void ATheVeilPlayerController::HandleSprintStopped()
{
    if (ATheVeilCharacter* VeilCharacter = Cast<ATheVeilCharacter>(GetPawn()))
    {
        VeilCharacter->StopSprint();
    }
}

void ATheVeilPlayerController::HandleCrouchStarted()
{
    if (ATheVeilCharacter* VeilCharacter = Cast<ATheVeilCharacter>(GetPawn()))
    {
        VeilCharacter->ToggleCrouch();
    }
}
