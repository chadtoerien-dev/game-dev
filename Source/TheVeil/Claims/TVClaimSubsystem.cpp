#include "Claims/TVClaimSubsystem.h"
#include "Claims/TVClaimRules.h"
#include "Core/TheVeilGameState.h"
#include "Events/TVGameplayEventSubsystem.h"
#include "Memory/TVMemoryComponent.h"
#include "Social/TVRelationshipSubsystem.h"
#include "Engine/World.h"
#include "GameFramework/Actor.h"

FGuid UTVClaimSubsystem::RegisterClaim(FTVClaimRecord Claim, const bool bEvaluateContradictions)
{
    if (Claim.SpeakerContestantId.IsNone() || (!Claim.EventTag.IsValid() && !Claim.TopicTag.IsValid()))
    {
        UE_LOG(LogTemp, Warning, TEXT("RegisterClaim rejected an incomplete claim."));
        return FGuid();
    }
    if (!Claim.ClaimId.IsValid()) Claim.ClaimId = FGuid::NewGuid();
    if (const UWorld* World = GetWorld())
    {
        Claim.CreatedWorldTimeSeconds = World->GetTimeSeconds();
        if (const ATheVeilGameState* GameState = World->GetGameState<ATheVeilGameState>())
        {
            Claim.CreatedRoundIndex = GameState->GetRoundIndex();
            Claim.CreatedPhase = GameState->GetCurrentPhase();
        }
    }
    Claim.Confidence = FMath::Clamp(Claim.Confidence, 0.0f, 1.0f);
    Claim.AudienceContestantIds.RemoveAll([](const FName Id){ return Id.IsNone(); });
    if (Claims.ContainsByPredicate([&Claim](const FTVClaimRecord& Existing){ return Existing.ClaimId == Claim.ClaimId; })) return Claim.ClaimId;
    Claims.Add(Claim);
    OnClaimRegistered.Broadcast(Claims.Last());
    if (bEvaluateContradictions) EvaluateClaimAgainstExisting(Claims.Last());
    return Claim.ClaimId;
}

FGuid UTVClaimSubsystem::CreateClaimFromMemory(AActor* SpeakerActor, const FGuid MemoryId, const ETVClaimStance Stance, const ETVClaimVisibility Visibility, const TArray<FName>& AudienceIds, const FGameplayTag TopicTag, const FText Summary, const bool bLie)
{
    if (!IsValid(SpeakerActor)) return FGuid();
    UTVMemoryComponent* MemoryComponent = SpeakerActor->FindComponentByClass<UTVMemoryComponent>();
    if (!IsValid(MemoryComponent)) return FGuid();
    FTVMemoryRecord Memory;
    if (!MemoryComponent->FindMemory(MemoryId, Memory)) return FGuid();
    FTVClaimRecord Claim;
    Claim.SpeakerContestantId = MemoryComponent->ResolveOwnerContestantId();
    Claim.SubjectContestantId = Memory.SubjectContestantId;
    Claim.TargetContestantId = Memory.TargetContestantId;
    Claim.EventTag = Memory.EventTag;
    Claim.TopicTag = TopicTag;
    Claim.ClaimedLocationTag = Memory.LocationTag;
    Claim.ClaimedPhase = Memory.CreatedPhase;
    Claim.Stance = Stance;
    Claim.Visibility = Visibility;
    Claim.AudienceContestantIds = AudienceIds;
    Claim.Confidence = Memory.Confidence;
    Claim.SourceMemoryIds.Add(Memory.MemoryId);
    Claim.SupportingEvidenceIds = Memory.SupportingEvidenceIds;
    Claim.bDeliberateLie = bLie;
    Claim.StatementSummary = Summary.IsEmpty() ? Memory.Notes : Summary;
    return RegisterClaim(Claim, true);
}

bool UTVClaimSubsystem::WithdrawClaim(const FGuid ClaimId, const FName Reason)
{
    FTVClaimRecord* Claim = Claims.FindByPredicate([ClaimId](const FTVClaimRecord& Existing){ return Existing.ClaimId == ClaimId; });
    if (Claim == nullptr || Claim->bWithdrawn) return false;
    Claim->bWithdrawn = true;
    Claim->StatementSummary = FText::Format(FText::FromString(TEXT("{0} [Withdrawn: {1}]")), Claim->StatementSummary, FText::FromName(Reason));
    OnClaimRegistered.Broadcast(*Claim);
    return true;
}
bool UTVClaimSubsystem::FindClaim(const FGuid ClaimId, FTVClaimRecord& OutClaim) const
{
    if (const FTVClaimRecord* Found = Claims.FindByPredicate([ClaimId](const FTVClaimRecord& Existing){ return Existing.ClaimId == ClaimId; })) { OutClaim = *Found; return true; }
    return false;
}
TArray<FTVClaimRecord> UTVClaimSubsystem::GetClaimsAbout(const FName Subject) const { return Claims.FilterByPredicate([Subject](const FTVClaimRecord& Claim){ return Claim.SubjectContestantId == Subject; }); }
TArray<FTVClaimRecord> UTVClaimSubsystem::GetClaimsKnownBy(const FName ContestantId) const
{
    return Claims.FilterByPredicate([ContestantId](const FTVClaimRecord& Claim){ return Claim.SpeakerContestantId == ContestantId || Claim.AudienceContestantIds.Contains(ContestantId); });
}
TArray<FTVContradictionRecord> UTVClaimSubsystem::GetContradictionsKnownBy(const FName ContestantId) const
{
    return Contradictions.FilterByPredicate([ContestantId](const FTVContradictionRecord& C){ return C.KnownByContestantIds.Contains(ContestantId); });
}
void UTVClaimSubsystem::RestoreState(const TArray<FTVClaimRecord>& ClaimRecords, const TArray<FTVContradictionRecord>& ContradictionRecords, const bool bBroadcast)
{
    Claims = ClaimRecords;
    Contradictions = ContradictionRecords;
    if (bBroadcast)
    {
        for (const FTVClaimRecord& Claim : Claims) OnClaimRegistered.Broadcast(Claim);
        for (const FTVContradictionRecord& C : Contradictions) OnContradictionDetected.Broadcast(C);
    }
}
void UTVClaimSubsystem::ResetClaims() { Claims.Reset(); Contradictions.Reset(); }
void UTVClaimSubsystem::EvaluateClaimAgainstExisting(const FTVClaimRecord& NewClaim)
{
    for (const FTVClaimRecord& Existing : Claims)
    {
        if (Existing.ClaimId == NewClaim.ClaimId) continue;
        FTVContradictionRecord C = UTVClaimRules::EvaluateContradiction(Existing, NewClaim);
        if (C.Severity == ETVContradictionSeverity::None) continue;
        const bool bDuplicate = Contradictions.ContainsByPredicate([&C](const FTVContradictionRecord& R)
        { return (R.ClaimAId == C.ClaimAId && R.ClaimBId == C.ClaimBId) || (R.ClaimAId == C.ClaimBId && R.ClaimBId == C.ClaimAId); });
        if (bDuplicate) continue;
        Contradictions.Add(C);
        ApplyContradictionConsequences(Contradictions.Last());
        EmitContradictionEvent(Contradictions.Last());
        OnContradictionDetected.Broadcast(Contradictions.Last());
    }
}
void UTVClaimSubsystem::ApplyContradictionConsequences(const FTVContradictionRecord& C)
{
    if (C.AtFaultSpeakerContestantId.IsNone()) return;
    UTVRelationshipSubsystem* Relationships = GetWorld() != nullptr ? GetWorld()->GetSubsystem<UTVRelationshipSubsystem>() : nullptr;
    if (!IsValid(Relationships)) return;
    const float Scale = UTVClaimRules::GetSeverityScalar(C.Severity) * FMath::Clamp(C.Confidence, 0.0f, 1.0f);
    FTVRelationshipDelta Delta;
    Delta.Credibility = -25.0f * Scale;
    Delta.Trust = -8.0f * Scale;
    Delta.Suspicion = 12.0f * Scale;
    for (const FName Observer : C.KnownByContestantIds)
        if (Observer != C.AtFaultSpeakerContestantId) Relationships->ApplyRelationshipDelta(Observer, C.AtFaultSpeakerContestantId, Delta, FGuid(), TEXT("StatementContradiction"));
}
void UTVClaimSubsystem::EmitContradictionEvent(const FTVContradictionRecord& C)
{
    if (GetWorld() == nullptr) return;
    UTVGameplayEventSubsystem* Events = GetWorld()->GetSubsystem<UTVGameplayEventSubsystem>();
    if (!IsValid(Events)) return;
    const FGameplayTag Tag = FGameplayTag::RequestGameplayTag(FName(TEXT("Event.Statement.Contradiction")), false);
    if (!Tag.IsValid()) return;
    FTVGameplayEventRecord Event;
    Event.EventTag = Tag;
    Event.AttributedContestantId = C.AtFaultSpeakerContestantId;
    Event.Audience = ETVEventAudience::DirectParticipants;
    Event.SensoryChannel = ETVSensoryChannel::Public;
    Event.BaseReliability = C.Confidence;
    Event.Salience = UTVClaimRules::GetSeverityScalar(C.Severity);
    Event.Summary = C.Explanation;
    Events->EmitGameplayEvent(Event);
}
