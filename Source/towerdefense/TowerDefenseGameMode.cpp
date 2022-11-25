// Copyright Epic Games, Inc. All Rights Reserved.

#include "TowerDefenseGameMode.h"
#include "Level/CameraPawn.h"
#include "UserInterface/TowerDefenseHUD.h"
#include "TowerDefenseGameState.h"
#include "TowerDefensePlayerController.h"

ATowerDefenseGameMode::ATowerDefenseGameMode()
{
	DefaultPawnClass = ACameraPawn::StaticClass();
	GameStateClass = ATowerDefenseGameState::StaticClass();
	HUDClass = ATowerDefenseHUD::StaticClass();
	PlayerControllerClass = ATowerDefensePlayerController::StaticClass();
}
