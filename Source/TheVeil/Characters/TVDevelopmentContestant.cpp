#include "Characters/TVDevelopmentContestant.h"

#include "Components/CapsuleComponent.h"
#include "Components/StaticMeshComponent.h"
#include "Core/TVGameplayTypes.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Memory/TVMemoryComponent.h"
#include "Memory/TVWitnessComponent.h"
#include "Social/TVContestantStateComponent.h"
#include "UObject/ConstructorHelpers.h"

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
    if (SphereMesh.Succeeded())
    {
        PlaceholderMesh->SetStaticMesh(SphereMesh.Object);
    }
    PlaceholderMesh->SetRelativeScale3D(FVector(0.55f, 0.55f, 1.65f));

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
