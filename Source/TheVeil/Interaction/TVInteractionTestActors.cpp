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
    ConfigureInteractionBounds(InteractionBounds, FVector(35.0f, 35.0f, 35.0f));

    ObjectMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("ObjectMesh"));
    ObjectMesh->SetupAttachment(InteractionBounds);
    ConfigureDevelopmentMesh(ObjectMesh);
    ObjectMesh->SetRelativeScale3D(FVector(0.35f, 0.35f, 0.35f));
}

FText ATheVeilInspectableTestObject::GetInteractionPrompt_Implementation(const AActor* Interactor) const
{
    return bCollectOnInteract
        ? FText::FromString(TEXT("Collect test object"))
        : FText::FromString(TEXT("Inspect test object"));
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
