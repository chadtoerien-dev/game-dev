#include "Interaction/TVInteractionTestActors.h"

#include "Components/BoxComponent.h"
#include "Components/SceneComponent.h"
#include "Components/StaticMeshComponent.h"
#include "UObject/ConstructorHelpers.h"

namespace
{
UStaticMesh* LoadDevelopmentCubeMesh()
{
    static ConstructorHelpers::FObjectFinder<UStaticMesh> CubeMesh(TEXT("/Engine/BasicShapes/Cube.Cube"));
    return CubeMesh.Succeeded() ? CubeMesh.Object : nullptr;
}

UStaticMesh* LoadDevelopmentSphereMesh()
{
    static ConstructorHelpers::FObjectFinder<UStaticMesh> SphereMesh(TEXT("/Engine/BasicShapes/Sphere.Sphere"));
    return SphereMesh.Succeeded() ? SphereMesh.Object : nullptr;
}

void ConfigureDevelopmentMesh(UStaticMeshComponent* MeshComponent)
{
    if (MeshComponent == nullptr)
    {
        return;
    }

    MeshComponent->SetStaticMesh(LoadDevelopmentCubeMesh());
    MeshComponent->SetCollisionEnabled(ECollisionEnabled::NoCollision);
    MeshComponent->SetGenerateOverlapEvents(false);
    MeshComponent->SetCanEverAffectNavigation(false);
}

void ConfigureInteractionBounds(UBoxComponent* BoundsComponent, const FVector& BoxExtent)
{
    if (BoundsComponent == nullptr)
    {
        return;
    }

    BoundsComponent->InitBoxExtent(BoxExtent);
    BoundsComponent->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
    BoundsComponent->SetCollisionObjectType(ECC_WorldDynamic);
    BoundsComponent->SetCollisionResponseToAllChannels(ECR_Block);
    BoundsComponent->SetGenerateOverlapEvents(false);
    BoundsComponent->SetCanEverAffectNavigation(false);
}
}

ATheVeilTestDoor::ATheVeilTestDoor()
{
    PrimaryActorTick.bCanEverTick = false;

    SceneRoot = CreateDefaultSubobject<USceneComponent>(TEXT("SceneRoot"));
    SetRootComponent(SceneRoot);

    HingeRoot = CreateDefaultSubobject<USceneComponent>(TEXT("HingeRoot"));
    HingeRoot->SetupAttachment(SceneRoot);

    InteractionBounds = CreateDefaultSubobject<UBoxComponent>(TEXT("InteractionBounds"));
    InteractionBounds->SetupAttachment(HingeRoot);
    ConfigureInteractionBounds(InteractionBounds, FVector(18.0f, 120.0f, 110.0f));
    InteractionBounds->SetRelativeLocation(FVector(0.0f, 120.0f, 0.0f));

    DoorMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("DoorMesh"));
    DoorMesh->SetupAttachment(InteractionBounds);
    ConfigureDevelopmentMesh(DoorMesh);
    DoorMesh->SetRelativeScale3D(FVector(0.18f, 1.2f, 2.2f));

    LeftFrameMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("LeftFrameMesh"));
    LeftFrameMesh->SetupAttachment(SceneRoot);
    ConfigureDevelopmentMesh(LeftFrameMesh);
    LeftFrameMesh->SetRelativeLocation(FVector(0.0f, -12.0f, 0.0f));
    LeftFrameMesh->SetRelativeScale3D(FVector(0.28f, 0.12f, 2.45f));
    RightFrameMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("RightFrameMesh"));
    RightFrameMesh->SetupAttachment(SceneRoot);
    ConfigureDevelopmentMesh(RightFrameMesh);
    RightFrameMesh->SetRelativeLocation(FVector(0.0f, 252.0f, 0.0f));
    RightFrameMesh->SetRelativeScale3D(FVector(0.28f, 0.12f, 2.45f));
    HeaderFrameMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("HeaderFrameMesh"));
    HeaderFrameMesh->SetupAttachment(SceneRoot);
    ConfigureDevelopmentMesh(HeaderFrameMesh);
    HeaderFrameMesh->SetRelativeLocation(FVector(0.0f, 120.0f, 122.0f));
    HeaderFrameMesh->SetRelativeScale3D(FVector(0.28f, 1.44f, 0.14f));
    HandleMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("HandleMesh"));
    HandleMesh->SetupAttachment(InteractionBounds);
    ConfigureDevelopmentMesh(HandleMesh);
    HandleMesh->SetStaticMesh(LoadDevelopmentSphereMesh());
    HandleMesh->SetRelativeLocation(FVector(-24.0f, 86.0f, 0.0f));
    HandleMesh->SetRelativeScale3D(FVector(0.10f));
}

FText ATheVeilTestDoor::GetInteractionPrompt_Implementation(const AActor* Interactor) const
{
    return bIsOpen
        ? FText::FromString(TEXT("Close test door"))
        : FText::FromString(TEXT("Open test door"));
}

bool ATheVeilTestDoor::CanInteract_Implementation(const AActor* Interactor, FText& OutFailureReason) const
{
    if (bLocked)
    {
        OutFailureReason = FText::FromString(TEXT("The test door is locked"));
        return false;
    }

    OutFailureReason = FText::GetEmpty();
    return true;
}

bool ATheVeilTestDoor::Interact_Implementation(AActor* Interactor)
{
    FText FailureReason;
    if (!CanInteract_Implementation(Interactor, FailureReason))
    {
        return false;
    }

    bIsOpen = !bIsOpen;
    ++InteractionCount;

    const float TargetYaw = bIsOpen ? OpenYawDegrees : 0.0f;
    HingeRoot->SetRelativeRotation(FRotator(0.0f, TargetYaw, 0.0f));
    return true;
}

float ATheVeilTestDoor::GetDoorPanelYaw() const
{
    return HingeRoot != nullptr ? HingeRoot->GetRelativeRotation().Yaw : 0.0f;
}

ATheVeilInspectableTestObject::ATheVeilInspectableTestObject()
{
    PrimaryActorTick.bCanEverTick = false;

    InteractionBounds = CreateDefaultSubobject<UBoxComponent>(TEXT("InteractionBounds"));
    SetRootComponent(InteractionBounds);
    ConfigureInteractionBounds(InteractionBounds, FVector(48.0f, 48.0f, 70.0f));

    ObjectMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("ObjectMesh"));
    ObjectMesh->SetupAttachment(InteractionBounds);
    ConfigureDevelopmentMesh(ObjectMesh);
    ObjectMesh->SetStaticMesh(LoadDevelopmentSphereMesh());
    ObjectMesh->SetRelativeLocation(FVector(0.0f, 0.0f, 25.0f));
    ObjectMesh->SetRelativeScale3D(FVector(0.24f));

    PedestalMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("PedestalMesh"));
    PedestalMesh->SetupAttachment(InteractionBounds);
    ConfigureDevelopmentMesh(PedestalMesh);
    PedestalMesh->SetRelativeLocation(FVector(0.0f, 0.0f, -20.0f));
    PedestalMesh->SetRelativeScale3D(FVector(0.42f, 0.42f, 0.70f));
}

FText ATheVeilInspectableTestObject::GetInteractionPrompt_Implementation(const AActor* Interactor) const
{
    return bCollectOnInteract
        ? FText::FromString(TEXT("Collect estate seal"))
        : FText::FromString(TEXT("Inspect estate seal"));
}

bool ATheVeilInspectableTestObject::CanInteract_Implementation(
    const AActor* Interactor,
    FText& OutFailureReason) const
{
    if (bConsumed)
    {
        OutFailureReason = FText::FromString(TEXT("The test object has already been collected"));
        return false;
    }

    OutFailureReason = FText::GetEmpty();
    return true;
}

bool ATheVeilInspectableTestObject::Interact_Implementation(AActor* Interactor)
{
    FText FailureReason;
    if (!CanInteract_Implementation(Interactor, FailureReason))
    {
        return false;
    }

    ++InteractionCount;
    if (bCollectOnInteract)
    {
        bConsumed = true;
        SetActorHiddenInGame(true);
        SetActorEnableCollision(false);
    }
    return true;
}
