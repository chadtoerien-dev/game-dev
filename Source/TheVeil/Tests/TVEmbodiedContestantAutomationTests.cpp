#if WITH_DEV_AUTOMATION_TESTS

#include "Misc/AutomationTest.h"

#include "Characters/TVDevelopmentContestant.h"
#include "Engine/World.h"
#include "Memory/TVMemoryComponent.h"
#include "Memory/TVWitnessComponent.h"
#include "Social/TVContestantStateComponent.h"

namespace
{
class FTVScopedEmbodiedTestWorld
{
public:
    FTVScopedEmbodiedTestWorld() : World(UWorld::CreateWorld(EWorldType::Game, false)) {}
    ~FTVScopedEmbodiedTestWorld()
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
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(
    FTVEmbodiedContestantIdentityTest,
    "TheVeil.EmbodiedContestants.IdentityComponentsAndInteraction",
    EAutomationTestFlags::EditorContext | EAutomationTestFlags::EngineFilter)

bool FTVEmbodiedContestantIdentityTest::RunTest(const FString& Parameters)
{
    FTVScopedEmbodiedTestWorld TestWorld;
    UWorld* World = TestWorld.Get();
    TestNotNull(TEXT("Test world exists"), World);
    if (World == nullptr)
    {
        return false;
    }

    const FName Ids[] = {TEXT("Eleanor"), TEXT("Marcus"), TEXT("Beatrice")};
    TSet<FName> UniqueIds;
    for (const FName Id : Ids)
    {
        ATVDevelopmentContestant* Contestant = World->SpawnActor<ATVDevelopmentContestant>();
        TestNotNull(TEXT("Development contestant spawns"), Contestant);
        if (Contestant == nullptr)
        {
            continue;
        }

        TestTrue(TEXT("First stable identity initialisation succeeds"), Contestant->InitialiseContestant(Id, FText::FromName(Id), FText::FromString(TEXT("Test greeting")), FVector(100.0f, 0.0f, 0.0f)));
        TestEqual(TEXT("Stable identity reaches the state component"), Contestant->GetContestantId(), Id);
        TestFalse(TEXT("A different identity cannot replace the stable ID"), Contestant->InitialiseContestant(TEXT("Impostor"), FText::FromString(TEXT("Wrong name")), FText::FromString(TEXT("Wrong greeting")), FVector::ZeroVector));
        TestEqual(TEXT("Rejected identity leaves the display name unchanged"), Contestant->GetDisplayName().ToString(), Id.ToString());
        TestNotNull(TEXT("Contestant state component exists"), Contestant->FindComponentByClass<UTVContestantStateComponent>());
        TestNotNull(TEXT("Memory component exists"), Contestant->FindComponentByClass<UTVMemoryComponent>());
        TestNotNull(TEXT("Witness component exists"), Contestant->FindComponentByClass<UTVWitnessComponent>());
        TestTrue(TEXT("Contestant is interactable"), Contestant->GetClass()->ImplementsInterface(UTVInteractable::StaticClass()));
        TestTrue(TEXT("Interaction succeeds"), Contestant->Interact_Implementation(nullptr));
        TestTrue(TEXT("Interaction records spoken state"), Contestant->HasSpokenToPlayer());
        TestTrue(TEXT("Contestant provides typed interaction-result copy"), !Contestant->GetInteractionResultText_Implementation(nullptr).IsEmpty());
        UniqueIds.Add(Contestant->GetContestantId());
    }

    TestEqual(TEXT("All three contestant identities are unique"), UniqueIds.Num(), 3);
    return true;
}

#endif
