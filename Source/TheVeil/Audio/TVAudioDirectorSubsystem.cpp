#include "Audio/TVAudioDirectorSubsystem.h"

FGuid UTVAudioDirectorSubsystem::SubmitMusicRequest(
    const FGameplayTag StateTag,
    const int32 Priority,
    const float Intensity,
    UObject* Source,
    const FName Reason)
{
    if (!StateTag.IsValid())
    {
        UE_LOG(LogTemp, Warning, TEXT("SubmitMusicRequest rejected an invalid music state tag."));
        return FGuid();
    }

    FTVMusicRequest Request;
    Request.RequestId = FGuid::NewGuid();
    Request.StateTag = StateTag;
    Request.Priority = Priority;
    Request.Intensity = FMath::Clamp(Intensity, 0.0f, 1.0f);
    Request.Source = Source;
    Request.Reason = Reason;
    ActiveRequests.Add(Request);
    ResolveMusicState();
    return Request.RequestId;
}

bool UTVAudioDirectorSubsystem::RemoveMusicRequest(const FGuid RequestId)
{
    const int32 RemovedCount = ActiveRequests.RemoveAll(
        [&RequestId](const FTVMusicRequest& Request)
        {
            return Request.RequestId == RequestId;
        });

    if (RemovedCount > 0)
    {
        ResolveMusicState();
        return true;
    }

    return false;
}

void UTVAudioDirectorSubsystem::ClearAllMusicRequests()
{
    ActiveRequests.Reset();
    ResolveMusicState();
}

void UTVAudioDirectorSubsystem::RemoveInvalidRequests()
{
    ActiveRequests.RemoveAll(
        [](const FTVMusicRequest& Request)
        {
            return Request.Source != nullptr && !IsValid(Request.Source);
        });
}

void UTVAudioDirectorSubsystem::ResolveMusicState()
{
    RemoveInvalidRequests();

    const FTVMusicRequest* Winner = nullptr;
    for (const FTVMusicRequest& Request : ActiveRequests)
    {
        if (Winner == nullptr ||
            Request.Priority > Winner->Priority ||
            (Request.Priority == Winner->Priority && Request.Intensity > Winner->Intensity))
        {
            Winner = &Request;
        }
    }

    const FGameplayTag NewTag = Winner != nullptr ? Winner->StateTag : FGameplayTag();
    const float NewIntensity = Winner != nullptr ? Winner->Intensity : 0.0f;
    const FName NewReason = Winner != nullptr ? Winner->Reason : TEXT("NoActiveRequest");

    if (NewTag != ResolvedStateTag || !FMath::IsNearlyEqual(NewIntensity, ResolvedIntensity))
    {
        ResolvedStateTag = NewTag;
        ResolvedIntensity = NewIntensity;
        ResolvedReason = NewReason;
        OnResolvedMusicStateChanged.Broadcast(ResolvedStateTag, ResolvedIntensity, ResolvedReason);

        UE_LOG(LogTemp, Log, TEXT("Resolved music state: %s, intensity %.2f, reason %s"),
            *ResolvedStateTag.ToString(), ResolvedIntensity, *ResolvedReason.ToString());
    }
}
