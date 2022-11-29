// Copyright Epic Games, Inc. All Rights Reserved.

#include "TowerDefenseGameMode.h"

#include "Kismet/GameplayStatics.h"

#include "Level/CameraPawn.h"
#include "Level/LevelAggregator.h"
#include "Music/BackgroundMusic.h"
#include "UserInterface/TowerDefenseHUD.h"
#include "GameEvents.h"
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
	GetWorld()->SpawnActor<ABackgroundMusic>();

	ATowerDefenseGameState* LocalGameState = GetGameState<ATowerDefenseGameState>();
	LocalGameState->Initialize(Cast<ACameraPawn>(UGameplayStatics::GetPlayerPawn(GetWorld(), 0)));
	ATowerDefenseHUD* HUD = Cast<ATowerDefenseHUD>(UGameplayStatics::GetPlayerController(this, 0)->GetHUD());

	FGameEvents::FPlayRequested PlayRequested;
	FGameEvents::FQuitRequested QuitRequested;
	FGameEvents::FLevelChanged LevelChanged;
	FGameEvents::FEnemyCountChanged EnemyCountChanged;
	FGameEvents::FLastWaypointReached LastWaypointReached;
	FGameEvents::FLevelWon LevelWon;
	FGameEvents::FLevelLost LevelLost;
	FGameEvents::FGameWon GameWon;
	FGameEvents::FUIContinueRequested ContinueToNextLevel;
	FGameEvents::FUIContinueRequested ContinueToMainMenu;

	LocalGameState->BindDelegates(PlayRequested, EnemyCountChanged, LastWaypointReached, ContinueToNextLevel);
	HUD->BindDelegates(LevelChanged, EnemyCountChanged, PlayRequested, LevelWon, LevelLost, GameWon, ContinueToNextLevel, ContinueToMainMenu);
	QuitRequested.BindUObject(this, &ATowerDefenseGameMode::OnQuitRequested);

	LocalGameState->SetDelegates(EnemyCountChanged, LastWaypointReached, LevelChanged, LevelWon, LevelLost, GameWon);
	HUD->SetDelegates(PlayRequested, QuitRequested, ContinueToNextLevel, ContinueToMainMenu, ContinueToMainMenu);

	FString LevelAggregatorReference = TEXT("LevelAggregator'/Game/Level/LevelAggregator.LevelAggregator'");
	ULevelAggregator* Aggregator = LoadObject<ULevelAggregator>(NULL, *LevelAggregatorReference, NULL, LOAD_None, NULL);
	HUD->SetMaxLevelNumber(Aggregator->Levels.Num());
}

void ATowerDefenseGameMode::OnQuitRequested()
{
	UGameplayStatics::GetPlayerController(this, 0)->ConsoleCommand("quit");
}
