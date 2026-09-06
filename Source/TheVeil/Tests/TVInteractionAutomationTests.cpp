#if WITH_DEV_AUTOMATION_TESTS

#include "Misc/AutomationTest.h"

#include "Characters/TheVeilCharacter.h"
#include "Camera/CameraComponent.h"
#include "Components/BoxComponent.h"
#include "Core/TheVeilPlayerController.h"
#include "EnhancedInputComponent.h"
#include "Engine/World.h"
#include "InputAction.h"
#include "Interaction/TVInteractionComponent.h"
#include "Interaction/TVInteractable.h"
#include "Interaction/TVInteractionTestActors.h"

namespace
{
class FTVScopedInteractionTestWorld
{
public:
    FTVScopedInteractionTestWorld()
    {
        World = UWorld::CreateWorld(EWorldType::Game, false);
    }

    ~FTVScopedInteractionTestWorld()
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
TActor* SpawnInteractionTestActor(UWorld* World, const FVector& Location = FVector::ZeroVector)
{
    if (World == nullptr)
    {
        return nullptr;
    }

    FActorSpawnParameters SpawnParameters;
    SpawnParameters.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
    return World->SpawnActor<TActor>(TActor::StaticClass(), FTransform(Location), SpawnParameters);
}

AActor* SpawnVisibilityBlocker(UWorld* World, const FVector& Location)
{
    AActor* Blocker = SpawnInteractionTestActor<AActor>(World, Location);
    if (Blocker == nullptr)
    {
        return nullptr;
    }

    UBoxComponent* Box = NewObject<UBoxComponent>(Blocker, TEXT("VisibilityBlocker"));
    Blocker->SetRootComponent(Box);
    Box->InitBoxExtent(FVector(35.0f, 80.0f, 80.0f));
    Box->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
    Box->SetCollisionObjectType(ECC_WorldStatic);
    Box->SetCollisionResponseToAllChannels(ECR_Ignore);
    Box->SetCollisionResponseToChannel(ECC_Visibility, ECR_Block);
    Box->RegisterComponent();
    return Blocker;
}
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(
    FTVInteractionThirdPersonCameraReachTest,
    "TheVeil.Interaction.ThirdPersonCameraTraceReachesPawnRange",
    EAutomationTestFlags::EditorContext | EAutomationTestFlags::EngineFilter)

bool FTVInteractionThirdPersonCameraReachTest::RunTest(const FString& Parameters)
{
    FTVScopedInteractionTestWorld TestWorld;
    ATheVeilCharacter* Character = SpawnInteractionTestActor<ATheVeilCharacter>(TestWorld.Get());
    TestNotNull(TEXT("Character spawns for camera reach regression"), Character);
    if (Character == nullptr)
    {
        return false;
    }

    UTVInteractionComponent* InteractionComponent = Character->GetInteractionComponent();
    UCameraComponent* Camera = Character->FindComponentByClass<UCameraComponent>();
    ATheVeilInspectableTestObject* Target = SpawnInteractionTestActor<ATheVeilInspectableTestObject>(
        TestWorld.Get(), FVector(200.0f, 0.0f, 70.0f));
    TestNotNull(TEXT("Character camera exists"), Camera);
    TestNotNull(TEXT("A pawn-range target exists"), Target);
    if (InteractionComponent == nullptr || Camera == nullptr || Target == nullptr)
    {
        return false;
    }

    Camera->SetWorldLocationAndRotation(FVector(-420.0f, 0.0f, 120.0f), FRotator::ZeroRotator);
    FVector TraceStart;
    FVector TraceEnd;
    TestTrue(TEXT("Third-person interaction trace can be built"), InteractionComponent->BuildTraceForAutomationTests(TraceStart, TraceEnd));
    const float CameraToPawnDistance = FVector::Distance(TraceStart, Character->GetActorLocation());
    TestTrue(TEXT("Regression setup has a meaningful third-person camera offset"), CameraToPawnDistance > 400.0f);
    TestTrue(
        TEXT("Trace includes camera-to-pawn separation before the pawn-relative interaction range"),
        FVector::Distance(TraceStart, TraceEnd) >= CameraToPawnDistance + 319.0f);
    InteractionComponent->UpdateFocusedInteractable();
    TestEqual(TEXT("Actual camera trace focuses a target inside pawn-relative range"), InteractionComponent->GetFocusedActor(), Cast<AActor>(Target));
    return true;
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(
    FTVInteractionComponentFocusTest,
    "TheVeil.Interaction.FocusPromptAndActivation",
    EAutomationTestFlags::EditorContext | EAutomationTestFlags::EngineFilter)

bool FTVInteractionComponentFocusTest::RunTest(const FString& Parameters)
{
    FTVScopedInteractionTestWorld TestWorld;
    ATheVeilCharacter* Character = SpawnInteractionTestActor<ATheVeilCharacter>(TestWorld.Get());
    ATheVeilInspectableTestObject* TestObject =
        SpawnInteractionTestActor<ATheVeilInspectableTestObject>(TestWorld.Get(), FVector(180.0f, 0.0f, 0.0f));
    TestNotNull(TEXT("Character spawns for interaction focus"), Character);
    TestNotNull(TEXT("Inspectable test object spawns"), TestObject);
    if (Character == nullptr || TestObject == nullptr)
    {
        return false;
    }

    UTVInteractionComponent* InteractionComponent = Character->GetInteractionComponent();
    TestNotNull(TEXT("Character owns an interaction component"), InteractionComponent);
    if (InteractionComponent == nullptr)
    {
        return false;
    }

    TestTrue(
        TEXT("Inspectable object advertises the interactable interface"),
        TestObject->GetClass()->ImplementsInterface(UTVInteractable::StaticClass()) ||
            Cast<ITVInteractable>(TestObject) != nullptr);
    TestEqual(
        TEXT("Inspectable object direct prompt is stable"),
        TestObject->GetInteractionPrompt_Implementation(Character).ToString(),
        FString(TEXT("Inspect estate seal")));
    TestTrue(
        TEXT("Inspectable object starts inside default interaction range"),
        FVector::DistSquared(Character->GetActorLocation(), TestObject->GetActorLocation()) <= FMath::Square(320.0f));

    InteractionComponent->SetTraceHitActorForAutomationTests(TestObject);
    InteractionComponent->UpdateFocusedInteractable();

    TestEqual(TEXT("Trace focuses the inspectable object"), InteractionComponent->GetFocusedActor(), Cast<AActor>(TestObject));
    TestEqual(TEXT("Prompt comes from the focused object"), InteractionComponent->GetCurrentPrompt().ToString(), FString(TEXT("Inspect estate seal")));

    const ETVInteractionAttemptResult Result = InteractionComponent->TryInteract();
    TestEqual(TEXT("Focused object interaction succeeds"), Result, ETVInteractionAttemptResult::Succeeded);
    TestEqual(TEXT("A single interaction request activates once"), TestObject->GetInteractionCount(), 1);
    return true;
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(
    FTVInteractionComponentOcclusionTest,
    "TheVeil.Interaction.OcclusionBlocksFocus",
    EAutomationTestFlags::EditorContext | EAutomationTestFlags::EngineFilter)

bool FTVInteractionComponentOcclusionTest::RunTest(const FString& Parameters)
{
    FTVScopedInteractionTestWorld TestWorld;
    ATheVeilCharacter* Character = SpawnInteractionTestActor<ATheVeilCharacter>(TestWorld.Get());
    ATheVeilInspectableTestObject* TestObject =
        SpawnInteractionTestActor<ATheVeilInspectableTestObject>(TestWorld.Get(), FVector(220.0f, 0.0f, 0.0f));
    AActor* Blocker = SpawnVisibilityBlocker(TestWorld.Get(), FVector(100.0f, 0.0f, 0.0f));
    TestNotNull(TEXT("Character spawns for occlusion test"), Character);
    TestNotNull(TEXT("Inspectable object spawns for occlusion test"), TestObject);
    TestNotNull(TEXT("Visibility blocker spawns"), Blocker);
    if (Character == nullptr || TestObject == nullptr || Blocker == nullptr)
    {
        return false;
    }

    UTVInteractionComponent* InteractionComponent = Character->GetInteractionComponent();
    TestNotNull(TEXT("Interaction component exists for occlusion"), InteractionComponent);
    if (InteractionComponent == nullptr)
    {
        return false;
    }

    InteractionComponent->SetTraceHitActorForAutomationTests(Blocker);
    InteractionComponent->UpdateFocusedInteractable();

    TestNull(TEXT("A blocking non-interactable first hit prevents focus through a wall"), InteractionComponent->GetFocusedActor());
    TestEqual(
        TEXT("Blocked interaction does not activate the object"),
        InteractionComponent->TryInteract(),
        ETVInteractionAttemptResult::NoTarget);
    TestEqual(TEXT("Blocked object is untouched"), TestObject->GetInteractionCount(), 0);
    return true;
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(
    FTVInteractionComponentRangeTest,
    "TheVeil.Interaction.RangeRejectsTarget",
    EAutomationTestFlags::EditorContext | EAutomationTestFlags::EngineFilter)

bool FTVInteractionComponentRangeTest::RunTest(const FString& Parameters)
{
    FTVScopedInteractionTestWorld TestWorld;
    ATheVeilCharacter* Character = SpawnInteractionTestActor<ATheVeilCharacter>(TestWorld.Get());
    ATheVeilInspectableTestObject* TestObject =
        SpawnInteractionTestActor<ATheVeilInspectableTestObject>(TestWorld.Get(), FVector(260.0f, 0.0f, 0.0f));
    TestNotNull(TEXT("Character spawns for range test"), Character);
    TestNotNull(TEXT("Inspectable object spawns for range test"), TestObject);
    if (Character == nullptr || TestObject == nullptr)
    {
        return false;
    }

    UTVInteractionComponent* InteractionComponent = Character->GetInteractionComponent();
    TestNotNull(TEXT("Interaction component exists for range"), InteractionComponent);
    if (InteractionComponent == nullptr)
    {
        return false;
    }

    InteractionComponent->SetInteractionRangeForAutomationTests(160.0f);
    InteractionComponent->SetTraceHitActorForAutomationTests(TestObject);
    InteractionComponent->UpdateFocusedInteractable();

    TestNull(TEXT("Object outside interaction range cannot be focused"), InteractionComponent->GetFocusedActor());
    TestEqual(
        TEXT("Out-of-range object is not activated"),
        InteractionComponent->TryInteract(),
        ETVInteractionAttemptResult::NoTarget);
    TestEqual(TEXT("Out-of-range object remains untouched"), TestObject->GetInteractionCount(), 0);
    return true;
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(
    FTVInteractionTestDoorTest,
    "TheVeil.Interaction.TestDoorToggles",
    EAutomationTestFlags::EditorContext | EAutomationTestFlags::EngineFilter)

bool FTVInteractionTestDoorTest::RunTest(const FString& Parameters)
{
    FTVScopedInteractionTestWorld TestWorld;
    ATheVeilTestDoor* TestDoor = SpawnInteractionTestActor<ATheVeilTestDoor>(TestWorld.Get());
    TestNotNull(TEXT("Test door spawns"), TestDoor);
    if (TestDoor == nullptr)
    {
        return false;
    }

    TestFalse(TEXT("Test door starts closed"), TestDoor->IsOpen());
    TestDoor->SetActorRotation(FRotator(0.0, 37.0, 0.0));
    TestTrue(TEXT("First interaction opens the door"), TestDoor->Interact_Implementation(nullptr));
    TestTrue(TEXT("Door records the open state"), TestDoor->IsOpen());
    TestEqual(TEXT("Door hinge rotates relative to its actor when opened"), TestDoor->GetDoorPanelYaw(), 85.0f);
    TestEqual(TEXT("Opening preserves the actor's world yaw"), TestDoor->GetActorRotation().Yaw, 37.0);
    TestTrue(TEXT("Second interaction closes the door"), TestDoor->Interact_Implementation(nullptr));
    TestFalse(TEXT("Door records the closed state"), TestDoor->IsOpen());
    TestEqual(TEXT("Door hinge returns when closed"), TestDoor->GetDoorPanelYaw(), 0.0f);
    TestEqual(TEXT("Closing preserves the actor's world yaw"), TestDoor->GetActorRotation().Yaw, 37.0);
    TestEqual(TEXT("Door counted both intentional activations"), TestDoor->GetInteractionCount(), 2);

    TestDoor->SetLocked(true);
    TestFalse(TEXT("Locked door rejects interaction"), TestDoor->Interact_Implementation(nullptr));
    TestEqual(TEXT("Rejected locked interaction is not counted"), TestDoor->GetInteractionCount(), 2);
    return true;
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(
    FTVInteractionControllerFallbackBindingTest,
    "TheVeil.Interaction.ControllerFallbackBindings",
    EAutomationTestFlags::EditorContext | EAutomationTestFlags::EngineFilter)

bool FTVInteractionControllerFallbackBindingTest::RunTest(const FString& Parameters)
{
    FTVScopedInteractionTestWorld TestWorld;
    ATheVeilPlayerController* Controller = SpawnInteractionTestActor<ATheVeilPlayerController>(TestWorld.Get());
    TestNotNull(TEXT("Controller spawns for interaction binding test"), Controller);
    if (Controller == nullptr)
    {
        return false;
    }

    UInputAction* MoveAction = NewObject<UInputAction>(Controller);
    UInputAction* LookAction = NewObject<UInputAction>(Controller);
    UInputAction* JumpAction = NewObject<UInputAction>(Controller);
    UInputAction* SprintAction = NewObject<UInputAction>(Controller);
    UInputAction* CrouchAction = NewObject<UInputAction>(Controller);
    UEnhancedInputComponent* InputComponent = NewObject<UEnhancedInputComponent>(Controller);
    TestNotNull(TEXT("Enhanced Input component is available"), InputComponent);
    if (InputComponent == nullptr)
    {
        return false;
    }

    Controller->ConfigureInputActionsForAutomationTests(
        MoveAction,
        LookAction,
        JumpAction,
        SprintAction,
        CrouchAction);
    Controller->BindInputComponentForAutomationTests(InputComponent);

    TestEqual(TEXT("Movement shell keeps nine event bindings without IA_Interact"), InputComponent->GetActionEventBindings().Num(), 9);
    TestEqual(TEXT("Keyboard and controller fallback keys are both bound when IA_Interact is absent"), InputComponent->KeyBindings.Num(), 2);

    Controller->BindInputComponentForAutomationTests(InputComponent);
    TestEqual(
        TEXT("Rebinding the same component does not add duplicate movement/action bindings"),
        InputComponent->GetActionEventBindings().Num(),
        9);
    TestEqual(
        TEXT("Rebinding the same component does not add duplicate fallback keys"),
        InputComponent->KeyBindings.Num(),
        2);
    return true;
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(
    FTVInteractionControllerActionBindingTest,
    "TheVeil.Interaction.ControllerActionBindingSuppressesFallback",
    EAutomationTestFlags::EditorContext | EAutomationTestFlags::EngineFilter)

bool FTVInteractionControllerActionBindingTest::RunTest(const FString& Parameters)
{
    FTVScopedInteractionTestWorld TestWorld;
    ATheVeilPlayerController* Controller = SpawnInteractionTestActor<ATheVeilPlayerController>(TestWorld.Get());
    TestNotNull(TEXT("Controller spawns for configured interaction binding test"), Controller);
    if (Controller == nullptr)
    {
        return false;
    }

    UInputAction* MoveAction = NewObject<UInputAction>(Controller);
    UInputAction* LookAction = NewObject<UInputAction>(Controller);
    UInputAction* JumpAction = NewObject<UInputAction>(Controller);
    UInputAction* SprintAction = NewObject<UInputAction>(Controller);
    UInputAction* CrouchAction = NewObject<UInputAction>(Controller);
    UInputAction* InteractAction = NewObject<UInputAction>(Controller);
    UEnhancedInputComponent* InputComponent = NewObject<UEnhancedInputComponent>(Controller);
    TestNotNull(TEXT("Enhanced Input component is available"), InputComponent);
    if (InputComponent == nullptr)
    {
        return false;
    }

    Controller->ConfigureInputActionsForAutomationTests(
        MoveAction,
        LookAction,
        JumpAction,
        SprintAction,
        CrouchAction,
        InteractAction);
    Controller->BindInputComponentForAutomationTests(InputComponent);

    int32 InteractStartedBindings = 0;
    for (const TUniquePtr<FEnhancedInputActionEventBinding>& Binding : InputComponent->GetActionEventBindings())
    {
        if (Binding->GetAction() == InteractAction && Binding->GetTriggerEvent() == ETriggerEvent::Started)
        {
            ++InteractStartedBindings;
        }
    }

    TestEqual(TEXT("Configured IA_Interact binds Started once"), InteractStartedBindings, 1);
    TestEqual(TEXT("Configured IA_Interact creates the tenth event binding"), InputComponent->GetActionEventBindings().Num(), 10);
    TestEqual(TEXT("Configured IA_Interact suppresses fallback key bindings"), InputComponent->KeyBindings.Num(), 0);
    return true;
}

#endif
