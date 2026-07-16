#include "Evidence/TVEvidenceSubsystem.h"

#include "Core/TheVeilGameState.h"

FGuid UTVEvidenceSubsystem::AddEvidence(FTVEvidenceRecord Evidence)
{
    if (!Evidence.EvidenceTag.IsValid())
    {
        UE_LOG(LogTemp, Warning, TEXT("AddEvidence rejected a record with an invalid evidence tag."));
        return FGuid();
    }

    if (!Evidence.EvidenceId.IsValid())
    {
        Evidence.EvidenceId = FGuid::NewGuid();
    }
    else if (EvidenceRecords.ContainsByPredicate(
        [&Evidence](const FTVEvidenceRecord& Existing)
        {
            return Existing.EvidenceId == Evidence.EvidenceId;
        }))
    {
        UE_LOG(LogTemp, Warning, TEXT("AddEvidence rejected a duplicate evidence ID."));
        return Evidence.EvidenceId;
    }

    if (const UWorld* World = GetWorld())
    {
        if (const ATheVeilGameState* GameState = World->GetGameState<ATheVeilGameState>())
        {
            if (Evidence.CreatedRoundIndex == INDEX_NONE)
            {
                Evidence.CreatedRoundIndex = GameState->GetRoundIndex();
            }

            if (Evidence.CreatedPhase == ETheVeilPhase::None)
            {
                Evidence.CreatedPhase = GameState->GetCurrentPhase();
            }
        }
    }

    Evidence.Reliability = FMath::Clamp(Evidence.Reliability, 0.0f, 1.0f);
    EvidenceRecords.Add(Evidence);
    OnEvidenceAdded.Broadcast(Evidence);

    UE_LOG(LogTemp, Log, TEXT("Evidence added: %s at %s"),
        *Evidence.EvidenceTag.ToString(), *Evidence.LocationTag.ToString());
    return Evidence.EvidenceId;
}

bool UTVEvidenceSubsystem::SetEvidenceDiscovered(
    const FGuid EvidenceId,
    const bool bDiscovered)
{
    for (FTVEvidenceRecord& Record : EvidenceRecords)
    {
        if (Record.EvidenceId == EvidenceId)
        {
            Record.bDiscovered = bDiscovered;
            return true;
        }
    }

    return false;
}

TArray<FTVEvidenceRecord> UTVEvidenceSubsystem::SurfaceEvidenceForDawn(
    const int32 CurrentRoundIndex)
{
    LastDawnEvidence.Reset();

    for (FTVEvidenceRecord& Record : EvidenceRecords)
    {
        const bool bEligibleRound =
            Record.CreatedRoundIndex == INDEX_NONE ||
            Record.CreatedRoundIndex <= CurrentRoundIndex;

        if (!Record.bSurfacedAtDawn && bEligibleRound)
        {
            Record.bSurfacedAtDawn = true;
            Record.bDiscovered = true;
            LastDawnEvidence.Add(Record);
            OnEvidenceSurfacedAtDawn.Broadcast(Record);
        }
    }

    UE_LOG(
        LogTemp,
        Log,
        TEXT("Dawn surfaced %d evidence record(s) for round %d."),
        LastDawnEvidence.Num(),
        CurrentRoundIndex);

    return LastDawnEvidence;
}

void UTVEvidenceSubsystem::RestoreEvidenceRecords(
    const TArray<FTVEvidenceRecord>& Records,
    const bool bBroadcastAdded)
{
    EvidenceRecords = Records;
    LastDawnEvidence.Reset();

    if (bBroadcastAdded)
    {
        for (const FTVEvidenceRecord& Record : EvidenceRecords)
        {
            OnEvidenceAdded.Broadcast(Record);
        }
    }
}

void UTVEvidenceSubsystem::ResetEvidence()
{
    EvidenceRecords.Reset();
    LastDawnEvidence.Reset();
}
