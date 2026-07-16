#include "Memory/TVInformationRules.h"
#include "Misc/Crc.h"

FTVInformationTransmissionResult UTVInformationRules::BuildTransmittedMemory(const FTVMemoryRecord& SourceMemory, const FName SourceId, const FName RecipientId, const ETVInformationTransmissionMode Mode, const FTVInformationMutation& Mutation, int32 Seed)
{
    FTVInformationTransmissionResult Result;
    if (!SourceMemory.MemoryId.IsValid() || !SourceMemory.EventTag.IsValid())
    {
        Result.FailureReason = FText::FromString(TEXT("The source memory is invalid."));
        return Result;
    }
    if (SourceId.IsNone() || RecipientId.IsNone())
    {
        Result.FailureReason = FText::FromString(TEXT("Source and recipient contestant IDs are required."));
        return Result;
    }
    if (SourceId == RecipientId)
    {
        Result.FailureReason = FText::FromString(TEXT("A contestant cannot transmit information to themselves."));
        return Result;
    }
    if (Seed == 0)
    {
        const FString Stable = FString::Printf(TEXT("%s|%s|%s|%d"), *SourceMemory.MemoryId.ToString(), *SourceId.ToString(), *RecipientId.ToString(), static_cast<int32>(Mode));
        Seed = static_cast<int32>(FCrc::StrCrc32(*Stable));
    }
    FRandomStream Random(Seed);
    const float Jitter = Random.FRandRange(0.92f, 1.04f);
    float ConfidenceMultiplier = 1.0f;
    float ReliabilityMultiplier = 1.0f;
    ETVInformationAcquisitionMethod Acquisition = ETVInformationAcquisitionMethod::Reported;
    bool bAddsDistortion = false;
    bool bLie = false;
    switch (Mode)
    {
    case ETVInformationTransmissionMode::Report: ConfidenceMultiplier = 0.85f; ReliabilityMultiplier = 0.9f; Acquisition = ETVInformationAcquisitionMethod::Reported; break;
    case ETVInformationTransmissionMode::Rumour: ConfidenceMultiplier = 0.65f; ReliabilityMultiplier = 0.72f; Acquisition = ETVInformationAcquisitionMethod::Rumour; bAddsDistortion = true; break;
    case ETVInformationTransmissionMode::DeliberateLie: ConfidenceMultiplier = 0.9f; ReliabilityMultiplier = 0.85f; Acquisition = ETVInformationAcquisitionMethod::Reported; bAddsDistortion = true; bLie = true; break;
    default: break;
    }
    FTVMemoryRecord Out = SourceMemory;
    Out.MemoryId = FGuid::NewGuid();
    Out.OwnerContestantId = RecipientId;
    Out.Provenance.ParentMemoryId = SourceMemory.MemoryId;
    Out.Provenance.OriginEventId = SourceMemory.Provenance.OriginEventId;
    Out.Provenance.ImmediateSourceContestantId = SourceId;
    Out.Provenance.AcquisitionMethod = Acquisition;
    Out.Provenance.TransmissionDepth = SourceMemory.Provenance.TransmissionDepth + 1;
    Out.Provenance.DistortionCount = SourceMemory.Provenance.DistortionCount + (bAddsDistortion ? 1 : 0);
    Out.Provenance.bDeliberateLie = SourceMemory.Provenance.bDeliberateLie || bLie;
    if (Out.Provenance.OriginalObserverContestantId.IsNone()) Out.Provenance.OriginalObserverContestantId = SourceMemory.OwnerContestantId.IsNone() ? SourceId : SourceMemory.OwnerContestantId;
    if (Mutation.bOverrideEventTag && Mutation.EventTag.IsValid()) { Out.EventTag = Mutation.EventTag; ++Out.Provenance.DistortionCount; }
    if (Mutation.bOverrideSubjectContestant) { Out.SubjectContestantId = Mutation.SubjectContestantId; ++Out.Provenance.DistortionCount; }
    if (Mutation.bOverrideTargetContestant) { Out.TargetContestantId = Mutation.TargetContestantId; ++Out.Provenance.DistortionCount; }
    Out.Confidence = FMath::Clamp(SourceMemory.Confidence * ConfidenceMultiplier * Mutation.ConfidenceMultiplier * Jitter, 0.0f, 1.0f);
    Out.PerceivedReliability = FMath::Clamp(SourceMemory.PerceivedReliability * ReliabilityMultiplier * Mutation.ReliabilityMultiplier * Jitter, 0.0f, 1.0f);
    Out.LearnedWorldTimeSeconds = 0.0f;
    const FString Added = Mutation.AddedNotes.ToString();
    if (!Added.IsEmpty())
    {
        const FString SourceNotes = SourceMemory.Notes.ToString();
        Out.Notes = FText::FromString(SourceNotes.IsEmpty() ? Added : SourceNotes + TEXT(" ") + Added);
    }
    Result.bSucceeded = true;
    Result.Memory = Out;
    return Result;
}
