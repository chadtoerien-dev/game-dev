#pragma once

#include "CoreMinimal.h"
#include "GameFramework/SaveGame.h"
#include "Core/TVGameplayTypes.h"
#include "TVRunSaveGame.generated.h"

UCLASS(BlueprintType)
class THEVEIL_API UTVRunSaveGame : public USaveGame
{
    GENERATED_BODY()

public:
    UTVRunSaveGame();

    static constexpr int32 CurrentSaveVersion = 3;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, SaveGame, Category="Save")
    int32 SaveVersion = CurrentSaveVersion;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, SaveGame, Category="Save")
    FString BuildIdentifier;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, SaveGame, Category="Save")
    FDateTime SavedAtUtc;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, SaveGame, Category="Save")
    FName CheckpointLabel = NAME_None;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, SaveGame, Category="Run")
    int32 RunSeed = 0;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, SaveGame, Category="Run")
    FTVPhaseSnapshot Phase;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, SaveGame, Category="Run")
    TArray<FTVOpportunityRuntimeState> Opportunities;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, SaveGame, Category="Run")
    TArray<FTVEvidenceRecord> Evidence;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, SaveGame, Category="Run")
    TArray<FTVGameplayEventRecord> GameplayEvents;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, SaveGame, Category="Run")
    TArray<FTVContestantStateSnapshot> Contestants;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, SaveGame, Category="Run")
    TArray<FTVRoutineSnapshot> Routines;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, SaveGame, Category="Run")
    TArray<FTVMemoryComponentSnapshot> Memories;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, SaveGame, Category="Run")
    TArray<FTVRelationshipRecord> Relationships;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, SaveGame, Category="Run")
    TArray<FTVClaimRecord> Claims;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, SaveGame, Category="Run")
    TArray<FTVContradictionRecord> Contradictions;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, SaveGame, Category="Run")
    TArray<FTVVoteCommitment> VoteCommitments;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, SaveGame, Category="Run")
    TArray<FTVVoteIntention> VoteIntentions;
};
