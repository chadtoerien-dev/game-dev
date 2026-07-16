#include "Social/TVRelationshipSubsystem.h"
#include "Social/TVSocialInterpretationProfile.h"

FTVRelationshipRecord UTVRelationshipSubsystem::GetRelationship(const FName Observer, const FName Subject) const
{
    if (const FTVRelationshipRecord* Found = Relationships.FindByPredicate([Observer, Subject](const FTVRelationshipRecord& Existing)
    { return Existing.ObserverContestantId == Observer && Existing.SubjectContestantId == Subject; })) return *Found;
    FTVRelationshipRecord Default;
    Default.ObserverContestantId = Observer;
    Default.SubjectContestantId = Subject;
    return Default;
}
void UTVRelationshipSubsystem::SetRelationship(const FTVRelationshipRecord& Relationship, const FName Reason)
{
    if (Relationship.ObserverContestantId.IsNone() || Relationship.SubjectContestantId.IsNone() || Relationship.ObserverContestantId == Relationship.SubjectContestantId)
    {
        UE_LOG(LogTemp, Warning, TEXT("SetRelationship rejected invalid observer or subject IDs."));
        return;
    }
    FTVRelationshipRecord Sanitised = Relationship;
    Sanitised.LastChangeReason = Reason;
    ClampRelationship(Sanitised);
    FTVRelationshipDelta Delta;
    if (FTVRelationshipRecord* Existing = FindMutableRelationship(Sanitised.ObserverContestantId, Sanitised.SubjectContestantId))
    {
        Delta.Trust = Sanitised.Trust - Existing->Trust;
        Delta.Suspicion = Sanitised.Suspicion - Existing->Suspicion;
        Delta.Affinity = Sanitised.Affinity - Existing->Affinity;
        Delta.Respect = Sanitised.Respect - Existing->Respect;
        Delta.Fear = Sanitised.Fear - Existing->Fear;
        Delta.Credibility = Sanitised.Credibility - Existing->Credibility;
        *Existing = Sanitised;
        OnRelationshipChanged.Broadcast(*Existing, Delta);
        return;
    }
    Relationships.Add(Sanitised);
    OnRelationshipChanged.Broadcast(Relationships.Last(), Delta);
}
bool UTVRelationshipSubsystem::ApplyRelationshipDelta(const FName Observer, const FName Subject, const FTVRelationshipDelta& Delta, const FGuid MemoryId, const FName Reason)
{
    if (Observer.IsNone() || Subject.IsNone() || Observer == Subject) return false;
    FTVRelationshipRecord* Relationship = FindMutableRelationship(Observer, Subject);
    if (Relationship == nullptr)
    {
        FTVRelationshipRecord NewRelationship;
        NewRelationship.ObserverContestantId = Observer;
        NewRelationship.SubjectContestantId = Subject;
        Relationships.Add(NewRelationship);
        Relationship = &Relationships.Last();
    }
    Relationship->Trust += Delta.Trust;
    Relationship->Suspicion += Delta.Suspicion;
    Relationship->Affinity += Delta.Affinity;
    Relationship->Respect += Delta.Respect;
    Relationship->Fear += Delta.Fear;
    Relationship->Credibility += Delta.Credibility;
    Relationship->LastInfluencingMemoryId = MemoryId;
    Relationship->LastChangeReason = Reason;
    ClampRelationship(*Relationship);
    OnRelationshipChanged.Broadcast(*Relationship, Delta);
    UE_LOG(LogTemp, Log, TEXT("Relationship %s -> %s changed: suspicion %.1f, trust %.1f."), *Observer.ToString(), *Subject.ToString(), Relationship->Suspicion, Relationship->Trust);
    return true;
}
bool UTVRelationshipSubsystem::ApplyMemoryInterpretation(const FTVMemoryRecord& Memory, const UTVSocialInterpretationProfile* Profile)
{
    if (!IsValid(Profile) || Memory.OwnerContestantId.IsNone() || Memory.SubjectContestantId.IsNone() || Memory.OwnerContestantId == Memory.SubjectContestantId) return false;
    const FTVInterpretationRule* Rule = Profile->FindBestRule(Memory.EventTag, Memory.ContextTags);
    if (Rule == nullptr || Memory.Confidence < Rule->MinimumConfidence) return false;
    float Scale = FMath::Clamp(Memory.Confidence, 0.0f, 1.0f) * FMath::Clamp(Memory.PerceivedReliability, 0.0f, 1.0f) * Profile->GetAcquisitionMultiplier(Memory.Provenance.AcquisitionMethod);
    if (Rule->bScaleBySalience) Scale *= FMath::Clamp(Memory.Salience, 0.0f, 1.0f);
    if (Memory.TruthAssessment == ETVTruthAssessment::Questioned) Scale *= 0.5f;
    else if (Memory.TruthAssessment == ETVTruthAssessment::BelievedFalse) Scale *= -0.25f;
    Scale = FMath::Clamp(Scale, -2.0f, 2.0f);
    FTVRelationshipDelta Applied;
    Applied.Trust = Rule->BaseDelta.Trust * Scale;
    Applied.Suspicion = Rule->BaseDelta.Suspicion * Scale;
    Applied.Affinity = Rule->BaseDelta.Affinity * Scale;
    Applied.Respect = Rule->BaseDelta.Respect * Scale;
    Applied.Fear = Rule->BaseDelta.Fear * Scale;
    Applied.Credibility = Rule->BaseDelta.Credibility * Scale;
    return ApplyRelationshipDelta(Memory.OwnerContestantId, Memory.SubjectContestantId, Applied, Memory.MemoryId, TEXT("MemoryInterpretation"));
}
void UTVRelationshipSubsystem::RestoreRelationships(const TArray<FTVRelationshipRecord>& Records, const bool bBroadcast)
{
    Relationships = Records;
    for (FTVRelationshipRecord& Relationship : Relationships) ClampRelationship(Relationship);
    if (bBroadcast)
    {
        const FTVRelationshipDelta Empty;
        for (const FTVRelationshipRecord& Relationship : Relationships) OnRelationshipChanged.Broadcast(Relationship, Empty);
    }
}
void UTVRelationshipSubsystem::ResetRelationships() { Relationships.Reset(); }
FTVRelationshipRecord* UTVRelationshipSubsystem::FindMutableRelationship(const FName Observer, const FName Subject)
{
    return Relationships.FindByPredicate([Observer, Subject](const FTVRelationshipRecord& Existing)
    { return Existing.ObserverContestantId == Observer && Existing.SubjectContestantId == Subject; });
}
void UTVRelationshipSubsystem::ClampRelationship(FTVRelationshipRecord& R)
{
    R.Trust = FMath::Clamp(R.Trust, 0.0f, 100.0f);
    R.Suspicion = FMath::Clamp(R.Suspicion, 0.0f, 100.0f);
    R.Affinity = FMath::Clamp(R.Affinity, 0.0f, 100.0f);
    R.Respect = FMath::Clamp(R.Respect, 0.0f, 100.0f);
    R.Fear = FMath::Clamp(R.Fear, 0.0f, 100.0f);
    R.Credibility = FMath::Clamp(R.Credibility, 0.0f, 100.0f);
}
