#include "Core/TheVeilGameMode.h"

#include "Characters/TheVeilCharacter.h"
#include "Core/TheVeilGameState.h"
#include "Core/TheVeilPlayerController.h"

ATheVeilGameMode::ATheVeilGameMode()
{
    DefaultPawnClass = ATheVeilCharacter::StaticClass();
    PlayerControllerClass = ATheVeilPlayerController::StaticClass();
    GameStateClass = ATheVeilGameState::StaticClass();
}
