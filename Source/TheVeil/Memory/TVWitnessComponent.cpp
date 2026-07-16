#include "Memory/TVWitnessComponent.h"
#include "Events/TVGameplayEventSubsystem.h"
#include "Memory/TVMemoryComponent.h"
#include "Engine/World.h"
#include "CollisionQueryParams.h"
#include "GameFramework/Actor.h"

UTVWitnessComponent::UTVWitnessComponent() { PrimaryComponentTick.bCanEverTick = false; }
void UTVWitnessComponent::BeginPlay()
{
    Super::BeginPlay();
    MemoryComponent = GetOwner() != nullptr ? GetOwner()->FindComponentByClass<UTVMemoryComponent>() : nullptr;
    EventSubsystem = GetWorld() != nullptr ? GetWorld()->GetSubsystem<UTVGameplayEventSubsystem>() : nullptr;
    if (bAutomaticallyObserveGameplayEvents && IsValid(EventSubsystem)) EventSubsystem->OnGameplayEventEmitted.AddDynamic(this, &UTVWitnessComponent::HandleGameplayEventEmitted);
}
void UTVWitnessComponent::EndPlay(const EEndPlayReason::Type Reason)
{
    if (IsValid(EventSubsystem)) EventSubsystem->OnGameplayEventEmitted.RemoveDynamic(this, &UTVWitnessComponent::HandleGameplayEventEmitted);
    Super::EndPlay(Reason);
}
bool UTVWitnessComponent::TryWitnessGameplayEvent(const FTVGameplayEventRecord& Event, FTVMemoryRecord& OutMemory)
{
    if (!IsValid(MemoryComponent) || !Event.EventTag.IsValid() || Event.Salience < MinimumSalience || IgnoredEventTags.HasTag(Event.EventTag)) return false;
    const FName OwnerId = MemoryComponent->ResolveOwnerContestantId();
    if (OwnerId.IsNone()) return false;
    ETVInformationAcquisitionMethod Acquisition = ETVInformationAcquisitionMethod::Witnessed;
    float Confidence = 0.0f;
    bool bPerceived = false;
    switch (Event.Audience)
    {
    case ETVEventAudience::PublicBroadcast:
        bPerceived = true; Confidence = 0.9f; Acquisition = ETVInformationAcquisitionMethod::PublicAnnouncement; break;
    case ETVEventAudience::DirectParticipants:
        bPerceived = CanReceiveDirectEvent(Event, OwnerId); Confidence = bPerceived ? 1.0f : 0.0f; Acquisition = ETVInformationAcquisitionMethod::Witnessed; break;
    case ETVEventAudience::LocalSenses:
    default:
        switch (Event.SensoryChannel)
        {
        case ETVSensoryChannel::Visual: bPerceived = EvaluateVisualWitness(Event, Confidence); Acquisition = ETVInformationAcquisitionMethod::Witnessed; break;
        case ETVSensoryChannel::Auditory: bPerceived = EvaluateAuditoryWitness(Event, Confidence); Acquisition = ETVInformationAcquisitionMethod::Heard; break;
        case ETVSensoryChannel::Mixed:
            bPerceived = EvaluateVisualWitness(Event, Confidence); Acquisition = ETVInformationAcquisitionMethod::Witnessed;
            if (!bPerceived) { bPerceived = EvaluateAuditoryWitness(Event, Confidence); Acquisition = ETVInformationAcquisitionMethod::Heard; }
            break;
        case ETVSensoryChannel::Public: bPerceived = true; Confidence = 0.9f; Acquisition = ETVInformationAcquisitionMethod::PublicAnnouncement; break;
        default: break;
        }
        break;
    }
    if (!bPerceived) return false;
    OutMemory = BuildMemory(Event, OwnerId, Acquisition, Confidence);
    const FGuid AddedId = MemoryComponent->AddMemory(OutMemory, true, true);
    if (!AddedId.IsValid()) return false;
    OutMemory.MemoryId = AddedId;
    OnGameplayEventWitnessed.Broadcast(Event, OutMemory);
    return true;
}
void UTVWitnessComponent::HandleGameplayEventEmitted(const FTVGameplayEventRecord& Event) { FTVMemoryRecord Memory; TryWitnessGameplayEvent(Event, Memory); }
bool UTVWitnessComponent::CanReceiveDirectEvent(const FTVGameplayEventRecord& Event, const FName OwnerId) const
{ return OwnerId == Event.InstigatorContestantId || OwnerId == Event.TargetContestantId || OwnerId == Event.AttributedContestantId; }
bool UTVWitnessComponent::EvaluateVisualWitness(const FTVGameplayEventRecord& Event, float& OutConfidence) const
{
    if (GetOwner() == nullptr || GetWorld() == nullptr) return false;
    FVector EyeLocation; FRotator EyeRotation; GetOwner()->GetActorEyesViewPoint(EyeLocation, EyeRotation);
    const float Range = Event.MaxWitnessDistance > 0.0f ? Event.MaxWitnessDistance : VisualRange;
    const float Distance = FVector::Distance(EyeLocation, Event.WorldLocation);
    if (Distance > Range) return false;
    if (bRequireLineOfSightForVisualEvents)
    {
        FHitResult Hit; FCollisionQueryParams Params(SCENE_QUERY_STAT(TheVeilWitnessVisual), true, GetOwner());
        const bool bHit = GetWorld()->LineTraceSingleByChannel(Hit, EyeLocation, Event.WorldLocation, ECC_Visibility, Params);
        if (bHit && FVector::Distance(EyeLocation, Hit.ImpactPoint) + 25.0f < Distance) return false;
    }
    const float Normalised = Range > 0.0f ? FMath::Clamp(Distance / Range, 0.0f, 1.0f) : 0.0f;
    OutConfidence = FMath::Clamp(1.0f - (0.45f * Normalised), 0.45f, 1.0f);
    return true;
}
bool UTVWitnessComponent::EvaluateAuditoryWitness(const FTVGameplayEventRecord& Event, float& OutConfidence) const
{
    if (GetOwner() == nullptr || GetWorld() == nullptr) return false;
    const FVector Listener = GetOwner()->GetActorLocation();
    const float BaseRange = Event.MaxWitnessDistance > 0.0f ? Event.MaxWitnessDistance : HearingRange;
    const float Range = BaseRange * FMath::Max(0.1f, Event.Loudness);
    const float Distance = FVector::Distance(Listener, Event.WorldLocation);
    if (Distance > Range) return false;
    const float Normalised = Range > 0.0f ? FMath::Clamp(Distance / Range, 0.0f, 1.0f) : 0.0f;
    OutConfidence = FMath::Clamp(0.85f - (0.5f * Normalised), 0.25f, 0.85f);
    FHitResult Hit; FCollisionQueryParams Params(SCENE_QUERY_STAT(TheVeilWitnessAudio), true, GetOwner());
    if (GetWorld()->LineTraceSingleByChannel(Hit, Listener, Event.WorldLocation, ECC_Visibility, Params)) OutConfidence *= OccludedHearingConfidenceMultiplier;
    return OutConfidence > 0.0f;
}
FTVMemoryRecord UTVWitnessComponent::BuildMemory(const FTVGameplayEventRecord& Event, const FName OwnerId, const ETVInformationAcquisitionMethod Acquisition, const float Confidence) const
{
    FTVMemoryRecord Memory;
    Memory.MemoryId = FGuid::NewGuid();
    Memory.OwnerContestantId = OwnerId;
    Memory.EventTag = Event.EventTag;
    Memory.TargetContestantId = Event.TargetContestantId;
    Memory.LocationTag = Event.LocationTag;
    Memory.Confidence = FMath::Clamp(Confidence, 0.0f, 1.0f);
    Memory.PerceivedReliability = Event.BaseReliability;
    Memory.Salience = Event.Salience;
    Memory.TruthAssessment = ETVTruthAssessment::BelievedTrue;
    Memory.ContextTags = Event.ContextTags;
    Memory.CreatedRoundIndex = Event.CreatedRoundIndex;
    Memory.CreatedPhase = Event.CreatedPhase;
    Memory.Notes = Event.Summary;
    const bool bVisualId = Acquisition == ETVInformationAcquisitionMethod::Witnessed && Event.bRevealInstigatorOnVisualWitness;
    Memory.SubjectContestantId = bVisualId ? Event.InstigatorContestantId : Event.AttributedContestantId;
    if (Event.RelatedEvidenceId.IsValid()) Memory.SupportingEvidenceIds.Add(Event.RelatedEvidenceId);
    Memory.Provenance.OriginEventId = Event.EventId;
    Memory.Provenance.OriginalObserverContestantId = OwnerId;
    Memory.Provenance.AcquisitionMethod = Acquisition;
    return Memory;
}
