#include "Characters/TVDevelopmentContestant.h"

#include "Components/CapsuleComponent.h"
#include "Components/StaticMeshComponent.h"
#include "Core/TVGameplayTypes.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Memory/TVMemoryComponent.h"
#include "Memory/TVWitnessComponent.h"
#include "Social/TVContestantStateComponent.h"
#include "UObject/ConstructorHelpers.h"

namespace
{
void ConfigureBodyPart(UStaticMeshComponent* Component, USceneComponent* Parent, UStaticMesh* Mesh, const FVector& Location, const FVector& Scale)
{
    Component->SetupAttachment(Parent);
    Component->SetStaticMesh(Mesh);
    Component->SetCollisionEnabled(ECollisionEnabled::NoCollision);
    Component->SetRelativeLocation(Location);
    Component->SetRelativeScale3D(Scale);
}
}

ATVDevelopmentContestant::ATVDevelopmentContestant()
{
    PrimaryActorTick.bCanEverTick = true;
    GetCapsuleComponent()->InitCapsuleSize(36.0f, 88.0f);
    GetCharacterMovement()->MaxWalkSpeed = 115.0f;
    GetCharacterMovement()->bOrientRotationToMovement = true;
    GetCharacterMovement()->RotationRate = FRotator(0.0f, 240.0f, 0.0f);

    PlaceholderMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("PlaceholderMesh"));
    PlaceholderMesh->SetupAttachment(GetCapsuleComponent());
    PlaceholderMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);
    static ConstructorHelpers::FObjectFinder<UStaticMesh> SphereMesh(TEXT("/Engine/BasicShapes/Sphere.Sphere"));
    static ConstructorHelpers::FObjectFinder<UStaticMesh> CubeMesh(TEXT("/Engine/BasicShapes/Cube.Cube"));
    ConfigureBodyPart(PlaceholderMesh, GetCapsuleComponent(), CubeMesh.Object, FVector(0.0f, 0.0f, 18.0f), FVector(0.38f, 0.24f, 0.55f));
    HeadMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("HeadMesh"));
    ConfigureBodyPart(HeadMesh, GetCapsuleComponent(), SphereMesh.Object, FVector(0.0f, 0.0f, 63.0f), FVector(0.24f));
    LeftArmMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("LeftArmMesh"));
    ConfigureBodyPart(LeftArmMesh, GetCapsuleComponent(), CubeMesh.Object, FVector(0.0f, -29.0f, 16.0f), FVector(0.13f, 0.13f, 0.48f));
    RightArmMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("RightArmMesh"));
    ConfigureBodyPart(RightArmMesh, GetCapsuleComponent(), CubeMesh.Object, FVector(0.0f, 29.0f, 16.0f), FVector(0.13f, 0.13f, 0.48f));
    LeftLegMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("LeftLegMesh"));
    ConfigureBodyPart(LeftLegMesh, GetCapsuleComponent(), CubeMesh.Object, FVector(0.0f, -12.0f, -48.0f), FVector(0.15f, 0.16f, 0.55f));
    RightLegMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("RightLegMesh"));
    ConfigureBodyPart(RightLegMesh, GetCapsuleComponent(), CubeMesh.Object, FVector(0.0f, 12.0f, -48.0f), FVector(0.15f, 0.16f, 0.55f));

    ContestantState = CreateDefaultSubobject<UTVContestantStateComponent>(TEXT("ContestantState"));
    Memory = CreateDefaultSubobject<UTVMemoryComponent>(TEXT("Memory"));
    Witness = CreateDefaultSubobject<UTVWitnessComponent>(TEXT("Witness"));
}

bool ATVDevelopmentContestant::InitialiseContestant(FName InContestantId, const FText& InDisplayName, const FText& InGreeting, const FVector& InPatrolOffset)
{
    if (ContestantState == nullptr || !ContestantState->InitialiseContestantId(InContestantId))
    {
        return false;
    }
    DisplayName = InDisplayName;
    Greeting = InGreeting;
    PatrolOffset = InPatrolOffset;
    if (InContestantId == TEXT("Marcus"))
    {
        PlaceholderMesh->SetRelativeScale3D(FVector(0.46f, 0.28f, 0.60f));
        HeadMesh->SetRelativeScale3D(FVector(0.27f));
    }
    else if (InContestantId == TEXT("Beatrice"))
    {
        PlaceholderMesh->SetRelativeScale3D(FVector(0.34f, 0.25f, 0.58f));
        HeadMesh->SetRelativeScale3D(FVector(0.22f, 0.25f, 0.22f));
    }
    return true;
}

void ATVDevelopmentContestant::BeginPlay()
{
    Super::BeginPlay();
    PatrolOrigin = GetActorLocation();
}

void ATVDevelopmentContestant::Tick(float DeltaSeconds)
{
    Super::Tick(DeltaSeconds);
    const FVector Target = PatrolOrigin + (bHeadingOut ? PatrolOffset : FVector::ZeroVector);
    FVector Delta = Target - GetActorLocation();
    Delta.Z = 0.0f;
    if (Delta.SizeSquared() < FMath::Square(35.0f))
    {
        bHeadingOut = !bHeadingOut;
        return;
    }
    AddMovementInput(Delta.GetSafeNormal(), 0.45f, true);
}

FText ATVDevelopmentContestant::GetInteractionPrompt_Implementation(const AActor* Interactor) const
{
    return bHasSpokenToPlayer
        ? FText::Format(NSLOCTEXT("TheVeil", "SpeakAgain", "Speak again with {0}"), DisplayName)
        : FText::Format(NSLOCTEXT("TheVeil", "SpeakWith", "Speak with {0}"), DisplayName);
}

bool ATVDevelopmentContestant::CanInteract_Implementation(const AActor* Interactor, FText& OutFailureReason) const
{
    OutFailureReason = FText::GetEmpty();
    return true;
}

bool ATVDevelopmentContestant::Interact_Implementation(AActor* Interactor)
{
    bHasSpokenToPlayer = true;
    if (GEngine != nullptr)
    {
        GEngine->AddOnScreenDebugMessage(
            static_cast<uint64>(GetUniqueID()),
            4.0f,
            FColor::Cyan,
            FText::Format(NSLOCTEXT("TheVeil", "DevelopmentGreeting", "{0}: {1}"), DisplayName, Greeting).ToString());
    }
    return true;
}

FName ATVDevelopmentContestant::GetContestantId() const
{
    return ContestantState != nullptr ? ContestantState->GetContestantId() : NAME_None;
}
