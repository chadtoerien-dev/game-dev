#include "Interaction/TVInteractionComponent.h"

#include "Camera/CameraComponent.h"
#include "DrawDebugHelpers.h"
#include "Engine/World.h"
#include "GameFramework/Actor.h"
#include "Interaction/TVInteractable.h"

UTVInteractionComponent::UTVInteractionComponent()
{
    PrimaryComponentTick.bCanEverTick = true;
}

void UTVInteractionComponent::TickComponent(
    const float DeltaTime,
    const ELevelTick TickType,
    FActorComponentTickFunction* ThisTickFunction)
{
    Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

    UpdateFocusedInteractable();

    UWorld* World = GetWorld();
    if (!bHasBufferedInteraction || World == nullptr)
    {
        return;
    }

    if (World->GetTimeSeconds() > BufferedInteractionExpiresAt)
    {
        bHasBufferedInteraction = false;
        return;
    }

    if (IsValid(FocusedActor) && TryInteractFocusedActor() == ETVInteractionAttemptResult::Succeeded)
    {
        bHasBufferedInteraction = false;
    }
}

void UTVInteractionComponent::UpdateFocusedInteractable()
{
#if WITH_DEV_AUTOMATION_TESTS
    if (bUseAutomationHitActorOverride)
    {
        AActor* HitActor = AutomationHitActor;
        FText Prompt;
        FText FailureReason;
        if (ResolveInteractable(HitActor, Prompt, FailureReason))
        {
            SetFocusedActor(HitActor, Prompt);
            return;
        }

        ClearFocusedActor();
        return;
    }
#endif

    UWorld* World = GetWorld();
    if (World == nullptr)
    {
        ClearFocusedActor();
        return;
    }

    FVector TraceStart;
    FVector TraceEnd;
    if (!BuildTrace(TraceStart, TraceEnd))
    {
        ClearFocusedActor();
        return;
    }

    FCollisionQueryParams QueryParams(SCENE_QUERY_STAT(TheVeilInteractionTrace), false);
    QueryParams.AddIgnoredActor(GetOwner());

    FHitResult Hit;
    const FCollisionShape TraceShape = FCollisionShape::MakeSphere(InteractionTraceRadius);
    const bool bHit = InteractionTraceRadius > 0.0f
        ? World->SweepSingleByChannel(
            Hit,
            TraceStart,
            TraceEnd,
            FQuat::Identity,
            TraceChannel,
            TraceShape,
            QueryParams)
        : World->LineTraceSingleByChannel(Hit, TraceStart, TraceEnd, TraceChannel, QueryParams);

    if (bDrawDebugInteractionTrace)
    {
        const FColor TraceColour = bHit ? FColor::Green : FColor::Silver;
        DrawDebugLine(World, TraceStart, TraceEnd, TraceColour, false, 0.0f, 0, 1.5f);
        DrawDebugSphere(World, bHit ? Hit.ImpactPoint : TraceEnd, InteractionTraceRadius, 12, TraceColour, false, 0.0f);
    }

    if (!bHit)
    {
        ClearFocusedActor();
        return;
    }

    AActor* HitActor = Hit.GetActor();
    FText Prompt;
    FText FailureReason;
    if (ResolveInteractable(HitActor, Prompt, FailureReason))
    {
        SetFocusedActor(HitActor, Prompt);
        return;
    }

    UE_LOG(
        LogTemp,
        VeryVerbose,
        TEXT("Interaction trace rejected actor=%s component=%s reason=%s"),
        *GetNameSafe(HitActor),
        *GetNameSafe(Hit.GetComponent()),
        *FailureReason.ToString());

    ClearFocusedActor();
}

ETVInteractionAttemptResult UTVInteractionComponent::RequestInteraction()
{
    const ETVInteractionAttemptResult ImmediateResult = TryInteract();
    if (ImmediateResult == ETVInteractionAttemptResult::Succeeded)
    {
        bHasBufferedInteraction = false;
        return ImmediateResult;
    }

    UWorld* World = GetWorld();
    if (World != nullptr && ImmediateResult == ETVInteractionAttemptResult::NoTarget)
    {
        bHasBufferedInteraction = true;
        BufferedInteractionExpiresAt = World->GetTimeSeconds() + InputBufferSeconds;
    }

    return ImmediateResult;
}

ETVInteractionAttemptResult UTVInteractionComponent::TryInteract()
{
    UpdateFocusedInteractable();
    return TryInteractFocusedActor();
}

bool UTVInteractionComponent::BuildTrace(FVector& OutStart, FVector& OutEnd) const
{
#if WITH_DEV_AUTOMATION_TESTS
    if (bUseAutomationTraceOverride)
    {
        OutStart = AutomationTraceStart;
        OutEnd = AutomationTraceStart + AutomationTraceDirection.GetSafeNormal() * InteractionRange;
        return true;
    }
#endif

    const AActor* Owner = GetOwner();
    if (Owner == nullptr)
    {
        return false;
    }

    if (const UCameraComponent* Camera = Owner->FindComponentByClass<UCameraComponent>())
    {
        OutStart = Camera->GetComponentLocation();
        // Interaction range is measured from the pawn, but the trace starts at the
        // third-person camera behind it. Include that separation so a valid target
        // in front of the pawn is not unreachable from the camera trace.
        const float CameraToOwnerDistance = FVector::Distance(OutStart, Owner->GetActorLocation());
        OutEnd = OutStart + Camera->GetForwardVector() * (InteractionRange + CameraToOwnerDistance);
        return true;
    }

    FVector EyeLocation = FVector::ZeroVector;
    FRotator EyeRotation = FRotator::ZeroRotator;
    Owner->GetActorEyesViewPoint(EyeLocation, EyeRotation);
    OutStart = EyeLocation;
    OutEnd = EyeLocation + EyeRotation.Vector() * InteractionRange;
    return true;
}

bool UTVInteractionComponent::IsActorInRange(const AActor* Candidate) const
{
    const AActor* Owner = GetOwner();
    if (Owner == nullptr || Candidate == nullptr)
    {
        return false;
    }

    return FVector::DistSquared(Owner->GetActorLocation(), Candidate->GetActorLocation()) <=
        FMath::Square(InteractionRange);
}

bool UTVInteractionComponent::ResolveInteractable(
    AActor* Candidate,
    FText& OutPrompt,
    FText& OutFailureReason) const
{
    if (!CanUseInteractable(Candidate))
    {
        return false;
    }

    if (!IsActorInRange(Candidate))
    {
        OutFailureReason = FText::FromString(TEXT("Out of range"));
        return false;
    }

    AActor* Owner = GetOwner();
    if (!CanInteractWithActor(Candidate, Owner, OutFailureReason))
    {
        return false;
    }

    OutPrompt = GetPromptFromInteractable(Candidate);
    return !OutPrompt.IsEmpty();
}

bool UTVInteractionComponent::CanUseInteractable(AActor* Candidate) const
{
    return IsValid(Candidate) &&
        (Candidate->GetClass()->ImplementsInterface(UTVInteractable::StaticClass()) ||
            Cast<ITVInteractable>(Candidate) != nullptr);
}

FText UTVInteractionComponent::GetPromptFromInteractable(AActor* Candidate) const
{
    AActor* Owner = GetOwner();
    if (!IsValid(Candidate))
    {
        return FText::GetEmpty();
    }

    const ITVInteractable* NativeInteractable = Cast<ITVInteractable>(Candidate);
    if (NativeInteractable != nullptr && !Candidate->GetClass()->HasAnyClassFlags(CLASS_CompiledFromBlueprint))
    {
        return NativeInteractable->GetInteractionPrompt_Implementation(Owner);
    }

    if (Candidate->GetClass()->ImplementsInterface(UTVInteractable::StaticClass()))
    {
        return ITVInteractable::Execute_GetInteractionPrompt(Candidate, Owner);
    }

    return FText::GetEmpty();
}

bool UTVInteractionComponent::CanInteractWithActor(
    AActor* Candidate,
    AActor* Interactor,
    FText& OutFailureReason) const
{
    if (!IsValid(Candidate))
    {
        return false;
    }

    const ITVInteractable* NativeInteractable = Cast<ITVInteractable>(Candidate);
    if (NativeInteractable != nullptr && !Candidate->GetClass()->HasAnyClassFlags(CLASS_CompiledFromBlueprint))
    {
        return NativeInteractable->CanInteract_Implementation(Interactor, OutFailureReason);
    }

    if (Candidate->GetClass()->ImplementsInterface(UTVInteractable::StaticClass()))
    {
        return ITVInteractable::Execute_CanInteract(Candidate, Interactor, OutFailureReason);
    }

    return false;
}

bool UTVInteractionComponent::ExecuteInteraction(AActor* Candidate, AActor* Interactor) const
{
    if (!IsValid(Candidate))
    {
        return false;
    }

    ITVInteractable* NativeInteractable = Cast<ITVInteractable>(Candidate);
    if (NativeInteractable != nullptr && !Candidate->GetClass()->HasAnyClassFlags(CLASS_CompiledFromBlueprint))
    {
        return NativeInteractable->Interact_Implementation(Interactor);
    }

    if (Candidate->GetClass()->ImplementsInterface(UTVInteractable::StaticClass()))
    {
        return ITVInteractable::Execute_Interact(Candidate, Interactor);
    }

    return false;
}

FText UTVInteractionComponent::GetResultTextFromInteractable(AActor* Candidate) const
{
    AActor* Owner = GetOwner();
    if (!IsValid(Candidate))
    {
        return FText::GetEmpty();
    }

    const ITVInteractable* NativeInteractable = Cast<ITVInteractable>(Candidate);
    if (NativeInteractable != nullptr && !Candidate->GetClass()->HasAnyClassFlags(CLASS_CompiledFromBlueprint))
    {
        return NativeInteractable->GetInteractionResultText_Implementation(Owner);
    }
    if (Candidate->GetClass()->ImplementsInterface(UTVInteractable::StaticClass()))
    {
        return ITVInteractable::Execute_GetInteractionResultText(Candidate, Owner);
    }
    return FText::GetEmpty();
}

FText UTVInteractionComponent::GetLastInteractionFeedback() const
{
    return HasVisibleInteractionFeedback() ? LastInteractionFeedback : FText::GetEmpty();
}

bool UTVInteractionComponent::HasVisibleInteractionFeedback() const
{
    const UWorld* World = GetWorld();
    return World != nullptr && !LastInteractionFeedback.IsEmpty() && World->GetTimeSeconds() <= InteractionFeedbackExpiresAt;
}

void UTVInteractionComponent::SetFocusedActor(AActor* NewFocusedActor, const FText& NewPrompt)
{
    if (FocusedActor == NewFocusedActor && CurrentPrompt.EqualTo(NewPrompt))
    {
        return;
    }

    FocusedActor = NewFocusedActor;
    CurrentPrompt = NewPrompt;
    UE_LOG(
        LogTemp,
        Display,
        TEXT("Interaction focus acquired actor=%s prompt=%s"),
        *GetNameSafe(NewFocusedActor),
        *NewPrompt.ToString());
    OnFocusedInteractableChanged.Broadcast(FocusedActor, CurrentPrompt);
}

void UTVInteractionComponent::ClearFocusedActor()
{
    if (!IsValid(FocusedActor) && CurrentPrompt.IsEmpty())
    {
        return;
    }

    FocusedActor = nullptr;
    CurrentPrompt = FText::GetEmpty();
    OnFocusedInteractableChanged.Broadcast(nullptr, CurrentPrompt);
}

ETVInteractionAttemptResult UTVInteractionComponent::TryInteractFocusedActor()
{
    AActor* Target = FocusedActor.Get();
    if (!IsValid(Target))
    {
        ClearFocusedActor();
        return ETVInteractionAttemptResult::NoTarget;
    }

    if (!IsActorInRange(Target))
    {
        ClearFocusedActor();
        return ETVInteractionAttemptResult::OutOfRange;
    }

    FText Prompt;
    FText FailureReason;
    if (!ResolveInteractable(Target, Prompt, FailureReason))
    {
        ClearFocusedActor();
        return ETVInteractionAttemptResult::Blocked;
    }

    const bool bInteracted = ExecuteInteraction(Target, GetOwner());
    if (bInteracted)
    {
        LastInteractionFeedback = GetResultTextFromInteractable(Target);
        UWorld* World = GetWorld();
        if (World != nullptr && !LastInteractionFeedback.IsEmpty())
        {
            InteractionFeedbackExpiresAt = World->GetTimeSeconds() + 4.0;
            UE_LOG(LogTemp, Display, TEXT("Interaction feedback: %s"), *LastInteractionFeedback.ToString());
        }
        UpdateFocusedInteractable();
        return ETVInteractionAttemptResult::Succeeded;
    }

    return ETVInteractionAttemptResult::Blocked;
}

#if WITH_DEV_AUTOMATION_TESTS
void UTVInteractionComponent::SetTraceOverrideForAutomationTests(
    const FVector& TraceStart,
    const FVector& TraceDirection)
{
    bUseAutomationTraceOverride = true;
    AutomationTraceStart = TraceStart;
    AutomationTraceDirection = TraceDirection.GetSafeNormal();
}

void UTVInteractionComponent::ClearTraceOverrideForAutomationTests()
{
    bUseAutomationTraceOverride = false;
}

void UTVInteractionComponent::SetTraceHitActorForAutomationTests(AActor* HitActor)
{
    bUseAutomationHitActorOverride = true;
    AutomationHitActor = HitActor;
}

void UTVInteractionComponent::ClearTraceHitActorForAutomationTests()
{
    bUseAutomationHitActorOverride = false;
    AutomationHitActor = nullptr;
}

void UTVInteractionComponent::SetInteractionRangeForAutomationTests(const float InInteractionRange)
{
    InteractionRange = InInteractionRange;
}
#endif
