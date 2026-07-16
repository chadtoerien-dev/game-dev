#include "Core/TVRunSaveGame.h"

UTVRunSaveGame::UTVRunSaveGame()
{
    SaveVersion = CurrentSaveVersion;
    BuildIdentifier = TEXT("ClaimVoting-0.4");
    SavedAtUtc = FDateTime::UtcNow();
}
