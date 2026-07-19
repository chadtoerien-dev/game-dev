#if WITH_DEV_AUTOMATION_TESTS

#include "Misc/AutomationTest.h"

#include "Camera/CameraComponent.h"
#include "Characters/TheVeilCharacter.h"
#include "Core/TheVeilGameMode.h"
#include "Core/TheVeilGameState.h"
#include "Core/TheVeilPlayerController.h"
#include "Engine/World.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/SpringArmComponent.h"

namespace
{
class FTVScopedTestWorld
{
public:
    FTVScopedTestWorld()
    {
        World = UWorld::CreateWorld(EWorldType::Game, false);
    }

    ~FTVScopedTestWorld()
    {
        if (World != nullptr)
        {
            World->DestroyWorld(false);
            World->RemoveFromRoot();
        }
    }

    UWorld* Get() const { return World; }

private:
    UWorld* World = nullptr;
};

template <typename TActor>
TActor* SpawnTestActor(UWorld* World)
{
    if (World == nullptr)
    {
        return nullptr;
    }

    FActorSpawnParameters SpawnParameters;
    SpawnParameters.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
    return World->SpawnActor<TActor>(TActor::StaticClass(), FTransform::Identity, SpawnParameters);
}
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(
    FTVPlayableCharacterDefaultsTest,
    "TheVeil.PlayableFoundation.CharacterDefaults",
    EAutomationTestFlags::EditorContext | EAutomationTestFlags::EngineFilter)

bool FTVPlayableCharacterDefaultsTest::RunTest(const FString& Parameters)
{
    const ATheVeilCharacter* Character = GetDefault<ATheVeilCharacter>();
    TestNotNull(TEXT("The Veil character can be constructed"), Character);
    if (Character == nullptr)
    {
        return false;
    }

    const UCharacterMovementComponent* MovementComponent = Character->GetCharacterMovement();
    const USpringArmComponent* CameraBoom = Character->GetCameraBoom();
    const UCameraComponent* FollowCamera = Character->GetFollowCamera();

    TestNotNull(TEXT("Character movement component exists"), MovementComponent);
    TestNotNull(TEXT("Camera boom exists"), CameraBoom);
    TestNotNull(TEXT("Follow camera exists"), FollowCamera);
    if (MovementComponent == nullptr || CameraBoom == nullptr || FollowCamera == nullptr)
    {
        return false;
    }

    TestFalse(TEXT("Controller pitch does not rotate the character"), Character->bUseControllerRotationPitch);
    TestFalse(TEXT("Controller yaw does not rotate the character"), Character->bUseControllerRotationYaw);
    TestFalse(TEXT("Controller roll does not rotate the character"), Character->bUseControllerRotationRoll);
    TestTrue(TEXT("Character orients to movement"), MovementComponent->bOrientRotationToMovement);
    TestTrue(TEXT("Character can crouch"), MovementComponent->GetNavAgentPropertiesRef().bCanCrouch);

    TestEqual(TEXT("Walk speed uses the approved default"), MovementComponent->MaxWalkSpeed, 500.0f);
    TestEqual(TEXT("Crouched speed uses the approved default"), MovementComponent->MaxWalkSpeedCrouched, 300.0f);
    TestEqual(TEXT("Maximum acceleration uses the approved default"), MovementComponent->MaxAcceleration, 2200.0f);
    TestEqual(TEXT("Walking braking uses the approved default"), MovementComponent->BrakingDecelerationWalking, 2000.0f);
    TestEqual(TEXT("Yaw rotation rate uses the approved default"), MovementComponent->RotationRate.Yaw, 600.0);
    TestEqual(TEXT("Ground friction uses the approved default"), MovementComponent->GroundFriction, 8.0f);
    TestEqual(TEXT("Air control uses the approved default"), MovementComponent->AirControl, 0.35f);

    TestEqual(TEXT("Camera boom uses the approved arm length"), CameraBoom->TargetArmLength, 340.0f);
    TestTrue(TEXT("Camera collision testing is enabled"), CameraBoom->bDoCollisionTest);
    TestEqual(TEXT("Camera uses the camera collision channel"), CameraBoom->ProbeChannel.GetValue(), ECC_Camera);
    TestEqual(TEXT("Camera collision probe uses the approved size"), CameraBoom->ProbeSize, 12.0f);
    TestTrue(TEXT("Camera position lag is enabled"), CameraBoom->bEnableCameraLag);
    TestEqual(TEXT("Camera lag speed uses the approved default"), CameraBoom->CameraLagSpeed, 18.0f);
    TestTrue(TEXT("Camera lag substepping is enabled"), CameraBoom->bUseCameraLagSubstepping);
    TestFalse(TEXT("Camera rotation lag is disabled"), CameraBoom->bEnableCameraRotationLag);
    TestTrue(TEXT("Camera boom uses pawn control rotation"), CameraBoom->bUsePawnControlRotation);
    TestFalse(TEXT("Follow camera does not independently use pawn control rotation"), FollowCamera->bUsePawnControlRotation);

    return true;
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(
    FTVPlayableGameModeDefaultsTest,
    "TheVeil.PlayableFoundation.GameModeDefaults",
    EAutomationTestFlags::EditorContext | EAutomationTestFlags::EngineFilter)

bool FTVPlayableGameModeDefaultsTest::RunTest(const FString& Parameters)
{
    const ATheVeilGameMode* GameMode = GetDefault<ATheVeilGameMode>();
    TestNotNull(TEXT("The Veil GameMode can be constructed"), GameMode);
    if (GameMode == nullptr)
    {
        return false;
    }

    TestTrue(TEXT("GameMode uses The Veil character"), GameMode->DefaultPawnClass.Get() == ATheVeilCharacter::StaticClass());
    TestTrue(TEXT("GameMode uses The Veil player controller"), GameMode->PlayerControllerClass.Get() == ATheVeilPlayerController::StaticClass());
    TestTrue(TEXT("GameMode preserves The Veil GameState"), GameMode->GameStateClass.Get() == ATheVeilGameState::StaticClass());
    return true;
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(
    FTVPlayableCharacterStateTransitionsTest,
    "TheVeil.PlayableFoundation.CharacterStateTransitions",
    EAutomationTestFlags::EditorContext | EAutomationTestFlags::EngineFilter)

bool FTVPlayableCharacterStateTransitionsTest::RunTest(const FString& Parameters)
{
    FTVScopedTestWorld TestWorld;
    ATheVeilCharacter* Character = SpawnTestActor<ATheVeilCharacter>(TestWorld.Get());
    TestNotNull(TEXT("Character spawns in a transient test world"), Character);
    if (Character == nullptr)
    {
        return false;
    }

    UCharacterMovementComponent* MovementComponent = Character->GetCharacterMovement();
    TestNotNull(TEXT("Spawned character has movement"), MovementComponent);
    if (MovementComponent == nullptr)
    {
        return false;
    }

    Character->StartSprint();
    TestTrue(TEXT("Sprint enters held state"), Character->IsSprinting());
    TestEqual(TEXT("Sprint applies sprint speed"), MovementComponent->MaxWalkSpeed, 720.0f);

    Character->ToggleCrouch();
    TestFalse(TEXT("Entering crouch cancels sprint"), Character->IsSprinting());
    TestTrue(TEXT("Crouch requests the standard movement transition"), MovementComponent->bWantsToCrouch);

    Character->StartSprint();
    TestFalse(TEXT("Sprint cannot start while crouch is pending"), Character->IsSprinting());
    TestEqual(TEXT("Pending crouch keeps walk speed"), MovementComponent->MaxWalkSpeed, 500.0f);

    Character->ResetTransientMovementState();
    TestFalse(TEXT("Reset clears sprint"), Character->IsSprinting());
    TestFalse(TEXT("Reset clears crouch intent"), MovementComponent->bWantsToCrouch);
    TestEqual(TEXT("Reset restores walk speed"), MovementComponent->MaxWalkSpeed, 500.0f);
    return true;
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(
    FTVPlayableMovementBasisTest,
    "TheVeil.PlayableFoundation.MovementBasis",
    EAutomationTestFlags::EditorContext | EAutomationTestFlags::EngineFilter)

bool FTVPlayableMovementBasisTest::RunTest(const FString& Parameters)
{
    FTVScopedTestWorld TestWorld;
    ATheVeilCharacter* Character = SpawnTestActor<ATheVeilCharacter>(TestWorld.Get());
    ATheVeilPlayerController* Controller = SpawnTestActor<ATheVeilPlayerController>(TestWorld.Get());
    TestNotNull(TEXT("Character spawns for movement-basis test"), Character);
    TestNotNull(TEXT("Controller spawns for movement-basis test"), Controller);
    if (Character == nullptr || Controller == nullptr)
    {
        return false;
    }

    Controller->Possess(Character);
    Controller->SetControlRotation(FRotator(37.0f, 90.0f, 18.0f));

    Character->Move(FVector2D(0.0f, 1.0f));
    const FVector ForwardInput = Character->ConsumeMovementInputVector();
    TestTrue(
        TEXT("Forward movement uses controller yaw and ignores pitch and roll"),
        ForwardInput.Equals(FVector(0.0f, 1.0f, 0.0f), KINDA_SMALL_NUMBER));

    Character->Move(FVector2D(1.0f, 0.0f));
    const FVector RightInput = Character->ConsumeMovementInputVector();
    TestTrue(
        TEXT("Right movement uses the yaw-relative basis"),
        RightInput.Equals(FVector(-1.0f, 0.0f, 0.0f), KINDA_SMALL_NUMBER));
    return true;
}

#endif
