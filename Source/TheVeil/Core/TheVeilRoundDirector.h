#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Core/TVGameplayTypes.h"
#include "TheVeilRoundDirector.generated.h"

class ATheVeilGameState;
class UTVPhaseSubsystem;
class UTVVotingProfile;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FTVPhaseDirectorEvent, ETheVeilPhase, Phase, FName, Reason);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FTVDawnConsequencesReady, int32, EvidenceCount);

UCLASS(BlueprintType)
class THEVEIL_API ATheVeilRoundDirector : public AActor
{
    GENERATED_BODY()

public:
    ATheVeilRoundDirector();
    virtual void BeginPlay() override;

    UPROPERTY(BlueprintAssignable, Category="The Veil|Round")
    FTVPhaseDirectorEvent OnPhaseEntered;

    UPROPERTY(BlueprintAssignable, Category="The Veil|Round")
    FTVPhaseDirectorEvent OnPhaseExited;

    UPROPERTY(BlueprintAssignable, Category="The Veil|Round")
    FTVDawnConsequencesReady OnDawnConsequencesReady;

    UFUNCTION(BlueprintCallable, Category="The Veil|Round")
    bool StartRoundFlow();

    UFUNCTION(BlueprintCallable, Category="The Veil|Round")
    bool AdvancePhase(FName Reason);

    UFUNCTION(BlueprintCallable, Category="The Veil|Round")
    bool RequestPhaseTransition(ETheVeilPhase NewPhase, FName Reason);

    UFUNCTION(BlueprintCallable, Category="The Veil|Round")
    bool RestartNightFromSocialCheckpoint();

    UFUNCTION(BlueprintPure, Category="The Veil|Round")
    ATheVeilGameState* GetTheVeilGameState() const;

    UFUNCTION(BlueprintPure, Category="The Veil|Round")
    float GetConfiguredDuration(ETheVeilPhase Phase) const;

protected:
    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Round")
    bool bAutoStart = true;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Round")
    bool bUseShortPrototypeLoop = true;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Round")
    bool bUseAutomaticPhaseTimers = false;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Round")
    bool bWriteCheckpointsToDisk = true;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Round")
    bool bRestoreSocialCheckpointBeforeNight = true;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Round")
    ETheVeilPhase InitialPhase = ETheVeilPhase::SocialFreeRoam;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Round")
    TMap<ETheVeilPhase, float> PhaseDurations;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Voting")
    TArray<FName> VotingContestantIds;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Voting")
    TArray<FName> VotingCandidateIds;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Voting")
    TObjectPtr<UTVVotingProfile> VotingProfile = nullptr;

private:
    FTimerHandle PhaseTimerHandle;
    void ConfigureDefaultDurations();
    void ScheduleAutomaticAdvance(ETheVeilPhase Phase);
    void HandlePhaseTimerExpired();
    void HandlePhaseExit(ETheVeilPhase Phase, ETheVeilPhase DestinationPhase);
    void HandlePhaseEnter(ETheVeilPhase Phase);
    UTVPhaseSubsystem* GetPhaseSubsystem() const;
};
