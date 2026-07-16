#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameStateBase.h"
#include "Core/TVGameplayTypes.h"
#include "TheVeilGameState.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_FourParams(
    FTVPhaseChanged,
    ETheVeilPhase, PreviousPhase,
    ETheVeilPhase, NewPhase,
    int32, RoundIndex,
    FName, Reason);

UCLASS(BlueprintType)
class THEVEIL_API ATheVeilGameState : public AGameStateBase
{
    GENERATED_BODY()

public:
    ATheVeilGameState();

    UPROPERTY(BlueprintAssignable, Category="The Veil|Phase")
    FTVPhaseChanged OnPhaseChanged;

    UFUNCTION(BlueprintCallable, BlueprintAuthorityOnly, Category="The Veil|Phase")
    bool ApplyPhaseState(ETheVeilPhase NewPhase, int32 NewRoundIndex, float NewDurationSeconds, FName Reason);

    UFUNCTION(BlueprintPure, Category="The Veil|Phase")
    ETheVeilPhase GetCurrentPhase() const { return CurrentPhase; }

    UFUNCTION(BlueprintPure, Category="The Veil|Phase")
    int32 GetRoundIndex() const { return RoundIndex; }

    UFUNCTION(BlueprintPure, Category="The Veil|Phase")
    int32 GetTransitionSerial() const { return TransitionSerial; }

    UFUNCTION(BlueprintPure, Category="The Veil|Phase")
    float GetPhaseDurationSeconds() const { return PhaseDurationSeconds; }

    UFUNCTION(BlueprintPure, Category="The Veil|Phase")
    float GetRemainingPhaseTime() const;

    UFUNCTION(BlueprintPure, Category="The Veil|Phase")
    FName GetLastTransitionReason() const { return LastTransitionReason; }

    UFUNCTION(BlueprintPure, Category="The Veil|Phase")
    FTVPhaseSnapshot CreatePhaseSnapshot() const;

    virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

protected:
    UPROPERTY(ReplicatedUsing=OnRep_CurrentPhase, BlueprintReadOnly, Category="Phase")
    ETheVeilPhase CurrentPhase = ETheVeilPhase::None;

    UPROPERTY(Replicated, BlueprintReadOnly, Category="Phase")
    int32 RoundIndex = 1;

    UPROPERTY(Replicated, BlueprintReadOnly, Category="Phase")
    int32 TransitionSerial = 0;

    UPROPERTY(Replicated, BlueprintReadOnly, Category="Phase")
    float PhaseStartServerTime = 0.0f;

    UPROPERTY(Replicated, BlueprintReadOnly, Category="Phase")
    float PhaseDurationSeconds = 0.0f;

    UPROPERTY(Replicated, BlueprintReadOnly, Category="Phase")
    FName LastTransitionReason = NAME_None;

    UFUNCTION()
    void OnRep_CurrentPhase();

private:
    ETheVeilPhase LastBroadcastPhase = ETheVeilPhase::None;
    void BroadcastPhaseChange(ETheVeilPhase PreviousPhase);
};
