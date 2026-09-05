#include "Core/TheVeilGameMode.h"

#include "Characters/TheVeilCharacter.h"
#include "Characters/TVDevelopmentContestant.h"
#include "Core/TheVeilGameState.h"
#include "Core/TheVeilPlayerController.h"
#include "Engine/World.h"
#include "GameFramework/PlayerStart.h"
#include "Interaction/TVInteractionTestActors.h"
#include "Kismet/GameplayStatics.h"
#include "UI/TVDevelopmentPromptHUD.h"

ATheVeilGameMode::ATheVeilGameMode()
{
    DefaultPawnClass = ATheVeilCharacter::StaticClass();
    PlayerControllerClass = ATheVeilPlayerController::StaticClass();
    GameStateClass = ATheVeilGameState::StaticClass();
    HUDClass = ATVDevelopmentPromptHUD::StaticClass();
}

void ATheVeilGameMode::StartPlay()
{
    Super::StartPlay();
    SpawnDevelopmentInteractionActors();
    SpawnDevelopmentContestants();
}

void ATheVeilGameMode::SpawnDevelopmentInteractionActors()
{
    UWorld* World = GetWorld();
    if (World == nullptr)
    {
        return;
    }

    FString MapName = World->GetMapName();
    MapName.RemoveFromStart(World->StreamingLevelsPrefix);
    if (MapName != TEXT("L_Dev_Sandbox"))
    {
        return;
    }

    const bool bNeedsTestObject =
        UGameplayStatics::GetActorOfClass(World, ATheVeilInspectableTestObject::StaticClass()) == nullptr;
    const bool bNeedsTestDoor = UGameplayStatics::GetActorOfClass(World, ATheVeilTestDoor::StaticClass()) == nullptr;
    if (!bNeedsTestObject && !bNeedsTestDoor)
    {
        return;
    }

    const AActor* PlayerStart = UGameplayStatics::GetActorOfClass(World, APlayerStart::StaticClass());
    if (PlayerStart == nullptr)
    {
        UE_LOG(LogTemp, Warning, TEXT("The Veil development interaction actors were not spawned: no PlayerStart was found."));
        return;
    }

    const FVector StartLocation = PlayerStart->GetActorLocation();
    const FRotator StartRotation = PlayerStart->GetActorRotation();
    const FVector Forward = StartRotation.Vector();
    const FVector Right = FRotationMatrix(StartRotation).GetUnitAxis(EAxis::Y);

    FActorSpawnParameters SpawnParameters;
    SpawnParameters.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn;

    const FVector ObjectLocation = StartLocation + Forward * 240.0f + FVector(0.0f, 0.0f, 35.0f);
    ATheVeilInspectableTestObject* TestObject = nullptr;
    if (bNeedsTestObject)
    {
        TestObject = World->SpawnActor<ATheVeilInspectableTestObject>(
            ATheVeilInspectableTestObject::StaticClass(),
            ObjectLocation,
            StartRotation,
            SpawnParameters);
    }

    const FVector DoorLocation = StartLocation + Forward * 650.0f + Right * 180.0f + FVector(0.0f, 0.0f, 110.0f);
    ATheVeilTestDoor* TestDoor = nullptr;
    if (bNeedsTestDoor)
    {
        TestDoor = World->SpawnActor<ATheVeilTestDoor>(
            ATheVeilTestDoor::StaticClass(),
            DoorLocation,
            StartRotation,
            SpawnParameters);
    }

    UE_LOG(
        LogTemp,
        Display,
        TEXT("The Veil development interaction actors spawned near PlayerStart (object=%s, door=%s)."),
        bNeedsTestObject ? (TestObject != nullptr ? TEXT("yes") : TEXT("failed")) : TEXT("already present"),
        bNeedsTestDoor ? (TestDoor != nullptr ? TEXT("yes") : TEXT("failed")) : TEXT("already present"));
}

void ATheVeilGameMode::SpawnDevelopmentContestants()
{
    UWorld* World = GetWorld();
    if (World == nullptr)
    {
        return;
    }

    FString MapName = World->GetMapName();
    MapName.RemoveFromStart(World->StreamingLevelsPrefix);
    if (MapName != TEXT("L_Dev_Sandbox"))
    {
        return;
    }

    const AActor* PlayerStart = UGameplayStatics::GetActorOfClass(World, APlayerStart::StaticClass());
    if (PlayerStart == nullptr)
    {
        return;
    }

    TArray<AActor*> ExistingActors;
    UGameplayStatics::GetAllActorsOfClass(World, ATVDevelopmentContestant::StaticClass(), ExistingActors);
    TSet<FName> ExistingIds;
    for (const AActor* ExistingActor : ExistingActors)
    {
        if (const ATVDevelopmentContestant* Contestant = Cast<ATVDevelopmentContestant>(ExistingActor))
        {
            ExistingIds.Add(Contestant->GetContestantId());
        }
    }

    struct FDevelopmentContestantDefinition
    {
        FName Id;
        const TCHAR* DisplayName;
        const TCHAR* Greeting;
        FVector SpawnOffset;
        FVector PatrolOffset;
    };

    const FDevelopmentContestantDefinition Definitions[] = {
        {TEXT("Eleanor"), TEXT("Eleanor"), TEXT("I was beginning to wonder who else was awake."), FVector(420.0f, -420.0f, 90.0f), FVector(0.0f, 280.0f, 0.0f)},
        {TEXT("Marcus"), TEXT("Marcus"), TEXT("Keep your eyes open. This place rewards attention."), FVector(720.0f, -120.0f, 90.0f), FVector(240.0f, 0.0f, 0.0f)},
        {TEXT("Beatrice"), TEXT("Beatrice"), TEXT("We should speak again when there are fewer listeners."), FVector(520.0f, 460.0f, 90.0f), FVector(-180.0f, -220.0f, 0.0f)}};

    const FVector StartLocation = PlayerStart->GetActorLocation();
    const FRotator StartRotation = PlayerStart->GetActorRotation();
    const FVector Forward = StartRotation.Vector();
    const FVector Right = FRotationMatrix(StartRotation).GetUnitAxis(EAxis::Y);
    FActorSpawnParameters SpawnParameters;
    SpawnParameters.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn;

    for (const FDevelopmentContestantDefinition& Definition : Definitions)
    {
        if (ExistingIds.Contains(Definition.Id))
        {
            continue;
        }

        const FVector Location = StartLocation + Forward * Definition.SpawnOffset.X + Right * Definition.SpawnOffset.Y + FVector(0.0f, 0.0f, Definition.SpawnOffset.Z);
        ATVDevelopmentContestant* Contestant = World->SpawnActor<ATVDevelopmentContestant>(
            ATVDevelopmentContestant::StaticClass(), Location, StartRotation, SpawnParameters);
        if (Contestant != nullptr)
        {
            const FVector WorldPatrolOffset = Forward * Definition.PatrolOffset.X + Right * Definition.PatrolOffset.Y;
            Contestant->InitialiseContestant(
                Definition.Id,
                FText::FromString(Definition.DisplayName),
                FText::FromString(Definition.Greeting),
                WorldPatrolOffset);
        }
    }
}
