#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "TVInteractionComponent.generated.h"

UENUM(BlueprintType)
enum class ETVInteractionAttemptResult : uint8
{
    Succeeded,
    NoTarget,
    Blocked,
    OutOfRange
};

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(
    FTVInteractionFocusChangedSignature,
    AActor*, FocusedActor,
    FText, Prompt);

UCLASS(ClassGroup=(TheVeil), BlueprintType, Blueprintable, meta=(BlueprintSpawnableComponent))
class THEVEIL_API UTVInteractionComponent : public UActorComponent
{
    GENERATED_BODY()

public:
    UTVInteractionComponent();

    virtual void TickComponent(
        float DeltaTime,
        ELevelTick TickType,
        FActorComponentTickFunction* ThisTickFunction) override;

    UFUNCTION(BlueprintCallable, Category="The Veil|Interaction")
    void UpdateFocusedInteractable();

    UFUNCTION(BlueprintCallable, Category="The Veil|Interaction")
    ETVInteractionAttemptResult RequestInteraction();

    UFUNCTION(BlueprintCallable, Category="The Veil|Interaction")
    ETVInteractionAttemptResult TryInteract();

    UFUNCTION(BlueprintPure, Category="The Veil|Interaction")
    AActor* GetFocusedActor() const { return IsValid(FocusedActor) ? FocusedActor.Get() : nullptr; }

    UFUNCTION(BlueprintPure, Category="The Veil|Interaction")
    FText GetCurrentPrompt() const { return CurrentPrompt; }

    UFUNCTION(BlueprintPure, Category="The Veil|Interaction")
    bool HasFocusedInteractable() const { return IsValid(FocusedActor); }

#if WITH_DEV_AUTOMATION_TESTS
    void SetTraceOverrideForAutomationTests(const FVector& TraceStart, const FVector& TraceDirection);
    void ClearTraceOverrideForAutomationTests();
    void SetTraceHitActorForAutomationTests(AActor* HitActor);
    void ClearTraceHitActorForAutomationTests();
    void SetInteractionRangeForAutomationTests(float InInteractionRange);
#endif

    UPROPERTY(BlueprintAssignable, Category="The Veil|Interaction")
    FTVInteractionFocusChangedSignature OnFocusedInteractableChanged;

protected:
    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="The Veil|Interaction|Tuning", meta=(ClampMin="0.0", Units="cm"))
    float InteractionRange = 320.0f;

    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="The Veil|Interaction|Tuning", meta=(ClampMin="0.0", Units="cm"))
    float InteractionTraceRadius = 14.0f;

    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="The Veil|Interaction|Tuning", meta=(ClampMin="0.0", Units="s"))
    float InputBufferSeconds = 0.16f;

    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="The Veil|Interaction|Debug")
    TEnumAsByte<ECollisionChannel> TraceChannel = ECC_Visibility;

    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="The Veil|Interaction|Debug")
    bool bDrawDebugInteractionTrace = false;

private:
    bool BuildTrace(FVector& OutStart, FVector& OutEnd) const;
    bool IsActorInRange(const AActor* Candidate) const;
    bool ResolveInteractable(AActor* Candidate, FText& OutPrompt, FText& OutFailureReason) const;
    bool CanUseInteractable(AActor* Candidate) const;
    FText GetPromptFromInteractable(AActor* Candidate) const;
    bool CanInteractWithActor(AActor* Candidate, AActor* Interactor, FText& OutFailureReason) const;
    bool ExecuteInteraction(AActor* Candidate, AActor* Interactor) const;
    void SetFocusedActor(AActor* NewFocusedActor, const FText& NewPrompt);
    void ClearFocusedActor();
    ETVInteractionAttemptResult TryInteractFocusedActor();

    UPROPERTY(Transient)
    TObjectPtr<AActor> FocusedActor;

    UPROPERTY(Transient)
    FText CurrentPrompt;

    bool bHasBufferedInteraction = false;
    double BufferedInteractionExpiresAt = 0.0;

#if WITH_DEV_AUTOMATION_TESTS
    bool bUseAutomationTraceOverride = false;
    FVector AutomationTraceStart = FVector::ZeroVector;
    FVector AutomationTraceDirection = FVector::ForwardVector;
    bool bUseAutomationHitActorOverride = false;
    AActor* AutomationHitActor = nullptr;
#endif
};
