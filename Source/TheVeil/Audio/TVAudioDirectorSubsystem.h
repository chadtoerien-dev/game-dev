#pragma once

#include "CoreMinimal.h"
#include "Subsystems/WorldSubsystem.h"
#include "GameplayTagContainer.h"
#include "TVAudioDirectorSubsystem.generated.h"

USTRUCT(BlueprintType)
struct THEVEIL_API FTVMusicRequest
{
    GENERATED_BODY()

    UPROPERTY(BlueprintReadOnly, Category="Music")
    FGuid RequestId;

    UPROPERTY(BlueprintReadOnly, Category="Music")
    FGameplayTag StateTag;

    UPROPERTY(BlueprintReadOnly, Category="Music")
    int32 Priority = 0;

    UPROPERTY(BlueprintReadOnly, Category="Music", meta=(ClampMin="0.0", ClampMax="1.0"))
    float Intensity = 0.0f;

    UPROPERTY(BlueprintReadOnly, Category="Music")
    TObjectPtr<UObject> Source = nullptr;

    UPROPERTY(BlueprintReadOnly, Category="Music")
    FName Reason = NAME_None;
};

DECLARE_DYNAMIC_MULTICAST_DELEGATE_ThreeParams(
    FTVResolvedMusicStateChanged,
    FGameplayTag, StateTag,
    float, Intensity,
    FName, Reason);

UCLASS(BlueprintType)
class THEVEIL_API UTVAudioDirectorSubsystem : public UWorldSubsystem
{
    GENERATED_BODY()

public:
    UPROPERTY(BlueprintAssignable, Category="The Veil|Audio")
    FTVResolvedMusicStateChanged OnResolvedMusicStateChanged;

    UFUNCTION(BlueprintCallable, Category="The Veil|Audio")
    FGuid SubmitMusicRequest(
        FGameplayTag StateTag,
        int32 Priority,
        float Intensity,
        UObject* Source,
        FName Reason);

    UFUNCTION(BlueprintCallable, Category="The Veil|Audio")
    bool RemoveMusicRequest(FGuid RequestId);

    UFUNCTION(BlueprintCallable, Category="The Veil|Audio")
    void ClearAllMusicRequests();

    UFUNCTION(BlueprintPure, Category="The Veil|Audio")
    FGameplayTag GetResolvedStateTag() const { return ResolvedStateTag; }

    UFUNCTION(BlueprintPure, Category="The Veil|Audio")
    float GetResolvedIntensity() const { return ResolvedIntensity; }

    UFUNCTION(BlueprintPure, Category="The Veil|Audio")
    TArray<FTVMusicRequest> GetActiveRequests() const { return ActiveRequests; }

private:
    UPROPERTY()
    TArray<FTVMusicRequest> ActiveRequests;

    UPROPERTY()
    FGameplayTag ResolvedStateTag;

    UPROPERTY()
    float ResolvedIntensity = 0.0f;

    UPROPERTY()
    FName ResolvedReason = NAME_None;

    void ResolveMusicState();
    void RemoveInvalidRequests();
};
