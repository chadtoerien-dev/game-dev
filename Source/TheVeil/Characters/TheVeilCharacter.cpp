#include "Characters/TheVeilCharacter.h"

#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/Controller.h"
#include "GameFramework/SpringArmComponent.h"

ATheVeilCharacter::ATheVeilCharacter()
{
    GetCapsuleComponent()->InitCapsuleSize(42.0f, 96.0f);

    bUseControllerRotationPitch = false;
    bUseControllerRotationYaw = false;
    bUseControllerRotationRoll = false;

    UCharacterMovementComponent* MovementComponent = GetCharacterMovement();
    MovementComponent->bOrientRotationToMovement = true;
    MovementComponent->GetNavAgentPropertiesRef().bCanCrouch = true;

    CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
    CameraBoom->SetupAttachment(RootComponent);
    CameraBoom->bUsePawnControlRotation = true;
    CameraBoom->bDoCollisionTest = true;
    CameraBoom->ProbeChannel = ECC_Camera;
    CameraBoom->bEnableCameraLag = true;
    CameraBoom->bUseCameraLagSubstepping = true;
    CameraBoom->bEnableCameraRotationLag = false;

    FollowCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("FollowCamera"));
    FollowCamera->SetupAttachment(CameraBoom, USpringArmComponent::SocketName);
    FollowCamera->bUsePawnControlRotation = false;

    ApplyTuning();
}

void ATheVeilCharacter::PostInitializeComponents()
{
    Super::PostInitializeComponents();
    ApplyTuning();
}

void ATheVeilCharacter::Move(const FVector2D& MovementInput)
{
    const AController* CharacterController = GetController();
    if (CharacterController == nullptr)
    {
        return;
    }

    const FRotator ControlRotation = CharacterController->GetControlRotation();
    const FRotator YawRotation(0.0f, ControlRotation.Yaw, 0.0f);
    const FVector ForwardDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
    const FVector RightDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);

    AddMovementInput(ForwardDirection, MovementInput.Y);
    AddMovementInput(RightDirection, MovementInput.X);
}

void ATheVeilCharacter::Look(const FVector2D& LookInput)
{
    AddControllerYawInput(LookInput.X);
    AddControllerPitchInput(LookInput.Y);
}

void ATheVeilCharacter::StartSprint()
{
    const UCharacterMovementComponent* MovementComponent = GetCharacterMovement();
    if (bIsCrouched || MovementComponent == nullptr || MovementComponent->bWantsToCrouch)
    {
        return;
    }

    bWantsToSprint = true;
    UpdateMovementSpeed();
}

void ATheVeilCharacter::StopSprint()
{
    bWantsToSprint = false;
    UpdateMovementSpeed();
}

void ATheVeilCharacter::ToggleCrouch()
{
    UCharacterMovementComponent* MovementComponent = GetCharacterMovement();
    if (MovementComponent == nullptr)
    {
        return;
    }

    if (bIsCrouched || MovementComponent->bWantsToCrouch)
    {
        UnCrouch();
        return;
    }

    StopSprint();
    Crouch();
}

void ATheVeilCharacter::StartJump()
{
    Jump();
}

void ATheVeilCharacter::StopJump()
{
    StopJumping();
}

void ATheVeilCharacter::ResetTransientMovementState()
{
    bWantsToSprint = false;
    StopJumping();
    UnCrouch();
    UpdateMovementSpeed();
}

void ATheVeilCharacter::Restart()
{
    Super::Restart();
    ResetTransientMovementState();
}

void ATheVeilCharacter::PawnClientRestart()
{
    Super::PawnClientRestart();
    ResetTransientMovementState();
}

void ATheVeilCharacter::OnStartCrouch(const float HeightAdjust, const float ScaledHeightAdjust)
{
    StopSprint();
    Super::OnStartCrouch(HeightAdjust, ScaledHeightAdjust);
}

void ATheVeilCharacter::ApplyTuning()
{
    UCharacterMovementComponent* MovementComponent = GetCharacterMovement();
    if (MovementComponent != nullptr)
    {
        MovementComponent->MaxAcceleration = MovementAcceleration;
        MovementComponent->BrakingDecelerationWalking = WalkingBrakingDeceleration;
        MovementComponent->RotationRate = FRotator(0.0f, YawRotationRate, 0.0f);
        MovementComponent->GroundFriction = GroundFriction;
        MovementComponent->AirControl = AirControl;
        MovementComponent->MaxWalkSpeedCrouched = CrouchedSpeed;
        UpdateMovementSpeed();
    }

    if (CameraBoom != nullptr)
    {
        CameraBoom->TargetArmLength = CameraBoomLength;
        CameraBoom->ProbeSize = CameraCollisionProbeSize;
        CameraBoom->CameraLagSpeed = CameraPositionLagSpeed;
    }
}

void ATheVeilCharacter::UpdateMovementSpeed()
{
    UCharacterMovementComponent* MovementComponent = GetCharacterMovement();
    if (MovementComponent == nullptr)
    {
        return;
    }

    const bool bCanSprint = bWantsToSprint && !bIsCrouched && !MovementComponent->bWantsToCrouch;
    MovementComponent->MaxWalkSpeed = bCanSprint ? SprintSpeed : WalkSpeed;
    MovementComponent->MaxWalkSpeedCrouched = CrouchedSpeed;
}
