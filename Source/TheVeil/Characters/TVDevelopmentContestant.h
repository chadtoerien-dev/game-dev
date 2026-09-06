#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Interaction/TVInteractable.h"
#include "TVDevelopmentContestant.generated.h"

class UStaticMeshComponent;
class UTVContestantStateComponent;
class UTVMemoryComponent;
class UTVWitnessComponent;

UCLASS()
class THEVEIL_API ATVDevelopmentContestant : public ACharacter, public ITVInteractable
{
    GENERATED_BODY()

public:
    ATVDevelopmentContestant();

    virtual void BeginPlay() override;
    virtual void Tick(float DeltaSeconds) override;
    virtual FText GetInteractionPrompt_Implementation(const AActor* Interactor) const override;
    virtual bool CanInteract_Implementation(const AActor* Interactor, FText& OutFailureReason) const override;
    virtual bool Interact_Implementation(AActor* Interactor) override;

    bool InitialiseContestant(FName InContestantId, const FText& InDisplayName, const FText& InGreeting, const FVector& InPatrolOffset);

    UFUNCTION(BlueprintPure, Category="The Veil|Contestant")
    FName GetContestantId() const;

    UFUNCTION(BlueprintPure, Category="The Veil|Contestant")
    FText GetDisplayName() const { return DisplayName; }

    UFUNCTION(BlueprintPure, Category="The Veil|Contestant")
    bool HasSpokenToPlayer() const { return bHasSpokenToPlayer; }

protected:
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="The Veil|Contestant")
    TObjectPtr<UStaticMeshComponent> PlaceholderMesh;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="The Veil|Contestant") TObjectPtr<UStaticMeshComponent> HeadMesh;
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="The Veil|Contestant") TObjectPtr<UStaticMeshComponent> LeftArmMesh;
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="The Veil|Contestant") TObjectPtr<UStaticMeshComponent> RightArmMesh;
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="The Veil|Contestant") TObjectPtr<UStaticMeshComponent> LeftLegMesh;
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="The Veil|Contestant") TObjectPtr<UStaticMeshComponent> RightLegMesh;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="The Veil|Contestant")
    TObjectPtr<UTVContestantStateComponent> ContestantState;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="The Veil|Contestant")
    TObjectPtr<UTVMemoryComponent> Memory;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="The Veil|Contestant")
    TObjectPtr<UTVWitnessComponent> Witness;

private:
    FText DisplayName;
    FText Greeting;
    FVector PatrolOffset = FVector(0.0f, 260.0f, 0.0f);
    FVector PatrolOrigin = FVector::ZeroVector;
    bool bHeadingOut = true;
    bool bHasSpokenToPlayer = false;
};
