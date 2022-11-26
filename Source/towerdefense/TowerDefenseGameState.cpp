// Fill out your copyright notice in the Description page of Project Settings.

#include "TowerDefenseGameState.h"
#include "Level/LevelDescriptor.h"
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
	FString Reference = FString::Printf(TEXT("LevelDescriptor'/Game/Level/%s.%s'"), *LevelName, *LevelName);
	ULevelDescriptor* Descriptor = LoadObject<ULevelDescriptor>(NULL, *Reference, NULL, LOAD_None, NULL);
	if (Descriptor == nullptr) {
		GEngine->AddOnScreenDebugMessage(-1, 10.0f, FColor::Red, TEXT("Couldn't load level."));
		return;
	}
	FSpawnTowerRequestList Requests = LevelBuilder->BuildLevel(Descriptor->Width, Descriptor->Height, Descriptor->Tiles);
	for (auto Request : Requests) {
		Request->BindUObject(TowerManager, &ATowerManager::Spawn);
	}
	EnemyManager->BeginLevel(Descriptor->Waypoints, Descriptor->EnemyCount,
			Descriptor->EnemySpawnDelay, Descriptor->EnemySpawnCooldown);
	LevelChanged.ExecuteIfBound(CurrentLevel);
}

void ATowerDefenseGameState::OnPlayRequested(int32 LevelNumber)
{
	GEngine->AddOnScreenDebugMessage(-1, 10.0, FColor::Red, TEXT("OnPlayRequested"));
	CurrentLevel = LevelNumber;
	BeginLevel(TEXT("TestLevel"));
}

void ATowerDefenseGameState::OnEnemyCountChanged(int32 Remaining, int32 Destroyed)
{
	GEngine->AddOnScreenDebugMessage(-1, 10.0, FColor::Red, TEXT("OnEnemyCountChanged"));
	if (Remaining < 1) {
		static constexpr int LAST_LEVEL = -1;
		if (CurrentLevel == LAST_LEVEL) {
			GameWon.ExecuteIfBound();
		} else {
			LevelWon.ExecuteIfBound();
			++CurrentLevel;
			// Start Next Level
			LevelChanged.ExecuteIfBound(CurrentLevel);
		}
	}
}

void ATowerDefenseGameState::OnLastWaypointReached()
{
	GEngine->AddOnScreenDebugMessage(-1, 10.0, FColor::Red, TEXT("OnLastWaypointReached"));
	LevelLost.ExecuteIfBound();
}
