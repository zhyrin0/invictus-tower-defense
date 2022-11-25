// Copyright Epic Games, Inc. All Rights Reserved.

#include "TowerDefenseGameMode.h"
#include "Kismet/GameplayStatics.h"
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

void ATowerDefenseGameMode::BeginPlay()
{
	ATowerDefenseHUD* HUD = Cast<ATowerDefenseHUD>(UGameplayStatics::GetPlayerController(this, 0)->GetHUD());
	HUD->GetLevelRequestedDelegate().AddUObject(this, &ATowerDefenseGameMode::OnLevelRequested);
	HUD->GetQuitRequestedDelegate().BindUObject(this, &ATowerDefenseGameMode::OnQuitRequested);
}

void ATowerDefenseGameMode::OnLevelRequested(FText PlayerName, int32 LevelNumber)
{
	GetGameState<ATowerDefenseGameState>()->BeginLevel(TEXT("TestLevel"));
}

void ATowerDefenseGameMode::OnQuitRequested()
{
	UGameplayStatics::GetPlayerController(this, 0)->ConsoleCommand("quit");
}
