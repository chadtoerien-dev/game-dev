#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Interaction/TVInteractable.h"
#include "TVInteractionTestActors.generated.h"

class UBoxComponent;
class USceneComponent;
class UStaticMeshComponent;

UCLASS(Blueprintable)
class THEVEIL_API ATheVeilTestDoor : public AActor, public ITVInteractable
{
    GENERATED_BODY()

public:
    ATheVeilTestDoor();

    virtual FText GetInteractionPrompt_Implementation(const AActor* Interactor) const override;
    virtual bool CanInteract_Implementation(const AActor* Interactor, FText& OutFailureReason) const override;
    virtual bool Interact_Implementation(AActor* Interactor) override;
    virtual FText GetInteractionResultText_Implementation(const AActor* Interactor) const override;

    UFUNCTION(BlueprintPure, Category="The Veil|Interaction")
    bool IsOpen() const { return bIsOpen; }

    UFUNCTION(BlueprintPure, Category="The Veil|Interaction")
    int32 GetInteractionCount() const { return InteractionCount; }

    UFUNCTION(BlueprintPure, Category="The Veil|Interaction")
    float GetDoorPanelYaw() const;

    UFUNCTION(BlueprintCallable, Category="The Veil|Interaction")
    void SetLocked(bool bNewLocked) { bLocked = bNewLocked; }

protected:
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="The Veil|Interaction")
    TObjectPtr<USceneComponent> SceneRoot;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="The Veil|Interaction")
    TObjectPtr<USceneComponent> HingeRoot;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="The Veil|Interaction")
    TObjectPtr<UBoxComponent> InteractionBounds;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="The Veil|Interaction")
    TObjectPtr<UStaticMeshComponent> DoorMesh;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="The Veil|Interaction") TObjectPtr<UStaticMeshComponent> LeftFrameMesh;
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="The Veil|Interaction") TObjectPtr<UStaticMeshComponent> RightFrameMesh;
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="The Veil|Interaction") TObjectPtr<UStaticMeshComponent> HeaderFrameMesh;
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="The Veil|Interaction") TObjectPtr<UStaticMeshComponent> HandleMesh;

    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="The Veil|Interaction|Tuning")
    float OpenYawDegrees = 85.0f;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="The Veil|Interaction")
    bool bLocked = false;

private:
    UPROPERTY(Transient)
    bool bIsOpen = false;

    UPROPERTY(Transient)
    int32 InteractionCount = 0;
};

UCLASS(Blueprintable)
class THEVEIL_API ATheVeilInspectableTestObject : public AActor, public ITVInteractable
{
    GENERATED_BODY()

public:
    ATheVeilInspectableTestObject();

    virtual FText GetInteractionPrompt_Implementation(const AActor* Interactor) const override;
    virtual bool CanInteract_Implementation(const AActor* Interactor, FText& OutFailureReason) const override;
    virtual bool Interact_Implementation(AActor* Interactor) override;
    virtual FText GetInteractionResultText_Implementation(const AActor* Interactor) const override;

    UFUNCTION(BlueprintPure, Category="The Veil|Interaction")
    int32 GetInteractionCount() const { return InteractionCount; }

    UFUNCTION(BlueprintPure, Category="The Veil|Interaction")
    bool IsConsumed() const { return bConsumed; }

protected:
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="The Veil|Interaction")
    TObjectPtr<UBoxComponent> InteractionBounds;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="The Veil|Interaction")
    TObjectPtr<UStaticMeshComponent> ObjectMesh;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="The Veil|Interaction") TObjectPtr<UStaticMeshComponent> PedestalMesh;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="The Veil|Interaction")
    bool bCollectOnInteract = false;

private:
    UPROPERTY(Transient)
    int32 InteractionCount = 0;

    UPROPERTY(Transient)
    bool bConsumed = false;
};
