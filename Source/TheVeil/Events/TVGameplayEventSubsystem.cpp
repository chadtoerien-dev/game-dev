#include "Events/TVGameplayEventSubsystem.h"

#include "Core/TheVeilGameState.h"
#include "Engine/World.h"

FGuid UTVGameplayEventSubsystem::EmitGameplayEvent(
    FTVGameplayEventRecord GameplayEvent)
{
    if (!GameplayEvent.EventTag.IsValid())
    {
        UE_LOG(
            LogTemp,
            Warning,
            TEXT("EmitGameplayEvent rejected an event with an invalid tag."));
        return FGuid();
    }

    if (!GameplayEvent.EventId.IsValid())
    {
        GameplayEvent.EventId = FGuid::NewGuid();
    }
    else if (GameplayEvents.ContainsByPredicate(
        [&GameplayEvent](const FTVGameplayEventRecord& Existing)
        {
            return Existing.EventId == GameplayEvent.EventId;
        }))
    {
        UE_LOG(
            LogTemp,
            Warning,
            TEXT("EmitGameplayEvent rejected duplicate event ID %s."),
            *GameplayEvent.EventId.ToString());
        return GameplayEvent.EventId;
    }

    GameplayEvent.BaseReliability = FMath::Clamp(
        GameplayEvent.BaseReliability,
        0.0f,
        1.0f);
    GameplayEvent.Salience = FMath::Clamp(GameplayEvent.Salience, 0.0f, 1.0f);
    GameplayEvent.Loudness = FMath::Clamp(GameplayEvent.Loudness, 0.0f, 2.0f);

    if (const UWorld* World = GetWorld())
    {
        GameplayEvent.WorldTimeSeconds = World->GetTimeSeconds();

        if (const ATheVeilGameState* GameState =
            World->GetGameState<ATheVeilGameState>())
        {
            if (GameplayEvent.CreatedRoundIndex == INDEX_NONE)
            {
                GameplayEvent.CreatedRoundIndex = GameState->GetRoundIndex();
            }

            if (GameplayEvent.CreatedPhase == ETheVeilPhase::None)
            {
                GameplayEvent.CreatedPhase = GameState->GetCurrentPhase();
            }
        }
    }

    GameplayEvents.Add(GameplayEvent);
    OnGameplayEventEmitted.Broadcast(GameplayEvent);

    UE_LOG(
        LogTemp,
        Log,
        TEXT("Gameplay event emitted: %s (%s)."),
        *GameplayEvent.EventTag.ToString(),
        *GameplayEvent.EventId.ToString());

    return GameplayEvent.EventId;
}

bool UTVGameplayEventSubsystem::FindGameplayEvent(
    const FGuid EventId,
    FTVGameplayEventRecord& OutEvent) const
{
    if (!EventId.IsValid())
    {
        return false;
    }

    if (const FTVGameplayEventRecord* Found = GameplayEvents.FindByPredicate(
        [&EventId](const FTVGameplayEventRecord& Existing)
        {
            return Existing.EventId == EventId;
        }))
    {
        OutEvent = *Found;
        return true;
    }

    return false;
}

void UTVGameplayEventSubsystem::RestoreGameplayEvents(
    const TArray<FTVGameplayEventRecord>& Records,
    const bool bBroadcastRestoredEvents)
{
    GameplayEvents = Records;

    if (bBroadcastRestoredEvents)
    {
        for (const FTVGameplayEventRecord& GameplayEvent : GameplayEvents)
        {
            OnGameplayEventEmitted.Broadcast(GameplayEvent);
        }
    }
}

void UTVGameplayEventSubsystem::ResetGameplayEvents()
{
    GameplayEvents.Reset();
}
