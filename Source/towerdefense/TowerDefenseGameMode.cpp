// Copyright Epic Games, Inc. All Rights Reserved.

#include "TowerDefenseGameMode.h"
#include "Kismet/GameplayStatics.h"
#include "Level/CameraPawn.h"
#include "Level/LevelAggregator.h"
#include "UserInterface/TowerDefenseHUD.h"
#include "TowerDefenseGameState.h"
#include "TowerDefensePlayerController.h"
#include "GameEvents.h"

ATowerDefenseGameMode::ATowerDefenseGameMode()
{
	DefaultPawnClass = ACameraPawn::StaticClass();
	GameStateClass = ATowerDefenseGameState::StaticClass();
	HUDClass = ATowerDefenseHUD::StaticClass();
	PlayerControllerClass = ATowerDefensePlayerController::StaticClass();
}

void ATowerDefenseGameMode::BeginPlay()
{
	ATowerDefenseGameState* LocalGameState = GetGameState<ATowerDefenseGameState>();
	ATowerDefenseHUD* HUD = Cast<ATowerDefenseHUD>(UGameplayStatics::GetPlayerController(this, 0)->GetHUD());

	FGameEvents::FPlayRequested PlayRequested;
	FGameEvents::FQuitRequested QuitRequested;
	FGameEvents::FLevelChanged LevelChanged;
	FGameEvents::FEnemyCountChanged EnemyCountChanged;
	FGameEvents::FLastWaypointReached LastWaypointReached;
	FGameEvents::FLevelWon LevelWon;
	FGameEvents::FLevelLost LevelLost;
	FGameEvents::FGameWon GameWon;

	LocalGameState->BindDelegates(PlayRequested, EnemyCountChanged, LastWaypointReached);
	HUD->BindDelegates(LevelChanged, EnemyCountChanged, PlayRequested, LevelWon, LevelLost, GameWon);
	QuitRequested.BindUObject(this, &ATowerDefenseGameMode::OnQuitRequested);

	LocalGameState->SetDelegates(EnemyCountChanged, LastWaypointReached, LevelChanged, LevelWon, LevelLost, GameWon);
	HUD->SetDelegates(PlayRequested, QuitRequested);

	FString LevelAggregatorReference = TEXT("LevelAggregator'/Game/Level/LevelAggregator.LevelAggregator'");
	ULevelAggregator* Aggregator = LoadObject<ULevelAggregator>(NULL, *LevelAggregatorReference, NULL, LOAD_None, NULL);
	HUD->SetMaxLevelNumber(Aggregator->Levels.Num());
}

void ATowerDefenseGameMode::OnQuitRequested()
{
	UGameplayStatics::GetPlayerController(this, 0)->ConsoleCommand("quit");
}
