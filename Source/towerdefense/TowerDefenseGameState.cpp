// Fill out your copyright notice in the Description page of Project Settings.

#include "TowerDefenseGameState.h"
#include "Level/LevelData.h"
#include "Tower/SpawnTowerRequestMixin.h"
#include "Enemy/TargetableMixin.h"

void ATowerDefenseGameState::OnConstruction(const FTransform& Transform)
{
	Super::OnConstruction(Transform);
	CurrentLevel = 0;
	UWorld* World = GetWorld();
	LevelBuilder = World->SpawnActor<ALevelBuilderActor>();
	EnemyManager = World->SpawnActor<AEnemyManager>();
	TowerManager = World->SpawnActor<ATowerManager>();
}

void ATowerDefenseGameState::BindDelegates(FGameEvents::FPlayRequested& InPlayRequested,
		FGameEvents::FEnemyCountChanged& InEnemyCountChanged,
		FGameEvents::FLastWaypointReached& InLastWaypointReached)
{
	ITargetableMixin::FSpawned& EnemySpawned = EnemyManager->GetEnemySpawnedDelegate();
	ITargetableMixin::FDestroyed& EnemyDestroyed = EnemyManager->GetEnemyDestroyedDelegate();
	TowerManager->BindDelegates(EnemySpawned, EnemyDestroyed);
	InPlayRequested.AddUObject(this, &ATowerDefenseGameState::OnPlayRequested);
	InEnemyCountChanged.AddUObject(this, &ATowerDefenseGameState::OnEnemyCountChanged);
	InLastWaypointReached.BindUObject(this, &ATowerDefenseGameState::OnLastWaypointReached);
}

void ATowerDefenseGameState::SetDelegates(FGameEvents::FEnemyCountChanged& InEnemyManagerEnemyCountChanged,
		FGameEvents::FLastWaypointReached& InEnemyManagerLastWaypointReached,
		FGameEvents::FLevelChanged& InLevelChanged,
		FGameEvents::FLevelWon& InLevelWon,
		FGameEvents::FLevelLost& InLevelLost,
		FGameEvents::FGameWon& InGameWon)
{
	EnemyManager->SetDelegates(InEnemyManagerEnemyCountChanged, InEnemyManagerLastWaypointReached);
	LevelChanged = InLevelChanged;
	LevelWon = InLevelWon;
	LevelLost = InLevelLost;
	GameWon = InGameWon;
}

void ATowerDefenseGameState::BeginLevel(FString LevelName) const
{
	FString Reference = FString::Printf(TEXT("LevelData'/Game/Level/%s.%s'"), *LevelName, *LevelName);
	ULevelData* Data = LoadObject<ULevelData>(NULL, *Reference, NULL, LOAD_None, NULL);
	if (Data == nullptr) {
		GEngine->AddOnScreenDebugMessage(-1, 10.0f, FColor::Red, TEXT("Couldn't load level."));
		return;
	}
	FSpawnTowerRequestList Requests = LevelBuilder->BuildLevel(Data->Width, Data->Height, Data->Tiles);
	for (auto Request : Requests) {
		Request->BindUObject(TowerManager, &ATowerManager::Spawn);
	}
	EnemyManager->BeginLevel(Data->Waypoints, Data->EnemyCount,
			Data->EnemySpawnDelay, Data->EnemySpawnCooldown);
	LevelChanged.ExecuteIfBound(CurrentLevel);
}

void ATowerDefenseGameState::OnPlayRequested(int32 LevelNumber)
{
	CurrentLevel = LevelNumber;
	BeginLevel(FString::FormatAsNumber(LevelNumber));
}

void ATowerDefenseGameState::OnEnemyCountChanged(int32 Remaining, int32 Destroyed)
{
	if (Remaining < 1) {
		static constexpr int LAST_LEVEL = -1;
		if (CurrentLevel == LAST_LEVEL) {
			GameWon.ExecuteIfBound();
		} else {
			LevelWon.ExecuteIfBound();
			++CurrentLevel;
			// todo: Start next level
			LevelChanged.ExecuteIfBound(CurrentLevel);
		}
	}
}

void ATowerDefenseGameState::OnLastWaypointReached()
{
	LevelLost.ExecuteIfBound();
}
