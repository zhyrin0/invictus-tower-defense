// Fill out your copyright notice in the Description page of Project Settings.

#include "TowerDefenseGameState.h"

#include "Enemy/EnemyManager.h"
#include "Enemy/TargetableMixin.h"
#include "Level/CameraPawn.h"
#include "Level/LevelAggregator.h"
#include "Level/LevelBuilder.h"
#include "Level/LevelData.h"
#include "Tower/SpawnTowerRequestMixin.h"
#include "Tower/TowerManager.h"


ATowerDefenseGameState::ATowerDefenseGameState()
{
	static auto LevelAggregatorAsset = ConstructorHelpers::FObjectFinder<ULevelAggregator>(
		TEXT("LevelAggregator'/Game/Level/LevelAggregator.LevelAggregator'"));

	LevelAggregator = LevelAggregatorAsset.Object;
}

void ATowerDefenseGameState::OnConstruction(const FTransform& Transform)
{
	Super::OnConstruction(Transform);
	CurrentLevel = 0;
	UWorld* World = GetWorld();
	LevelBuilder = World->SpawnActor<ALevelBuilder>();
	EnemyManager = World->SpawnActor<AEnemyManager>();
	TowerManager = World->SpawnActor<ATowerManager>();
}

void ATowerDefenseGameState::Initialize(ACameraPawn* InCameraPawn)
{
	CameraPawn = InCameraPawn;
}

void ATowerDefenseGameState::BindDelegates(FGameEvents::FPlayRequested& InPlayRequested,
		FGameEvents::FEnemyCountChanged& InEnemyCountChanged,
		FGameEvents::FLastWaypointReached& InLastWaypointReached,
		FGameEvents::FUIContinueRequested& InContinueRequested)
{
	ITargetableMixin::FSpawned& EnemySpawned = EnemyManager->GetEnemySpawnedDelegate();
	ITargetableMixin::FDestroyed& EnemyDestroyed = EnemyManager->GetEnemyDestroyedDelegate();
	ITargetableMixin::FDestroyed& EnemyRemoved = EnemyManager->GetEnemyReachedLastWaypoint();
	TowerManager->BindDelegates(EnemySpawned, EnemyDestroyed, EnemyRemoved);
	InPlayRequested.AddUObject(this, &ATowerDefenseGameState::OnPlayRequested);
	InEnemyCountChanged.AddUObject(this, &ATowerDefenseGameState::OnEnemyCountChanged);
	InLastWaypointReached.BindUObject(this, &ATowerDefenseGameState::OnLastWaypointReached);
	InContinueRequested.AddUObject(this, &ATowerDefenseGameState::OnContinueToNextLevel);
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

void ATowerDefenseGameState::OnPlayRequested(int32 LevelNumber)
{
	CurrentLevel = LevelNumber;
	BeginLevel(LevelAggregator->Levels[CurrentLevel - 1].LoadSynchronous());
}

void ATowerDefenseGameState::BeginLevel(ULevelData* Level) const
{
	if (Level == nullptr) {
		GEngine->AddOnScreenDebugMessage(-1, 10.0f, FColor::Red, TEXT("Couldn't load level."));
		return;
	}
	FSpawnTowerRequestList Requests = LevelBuilder->BuildLevel(Level->Size, Level->Tiles);
	for (auto Request : Requests) {
		Request->BindUObject(TowerManager, &ATowerManager::Spawn);
	}
	EnemyManager->BeginLevel(Level->Waypoints, Level->EnemyCount,
							 Level->EnemySpawnDelay, Level->EnemySpawnCooldown);
	LevelChanged.ExecuteIfBound(CurrentLevel);
	CameraPawn->SetActorLocation(FVector(Level->Size / 2.0f * 100.0f, 0.0f));
	CameraPawn->SetActorRotation(FRotator(0.0f, -90.0f, 0.0f));
	CameraPawn->SetStartingCameraOffset(FMath::Max(Level->Size.X, Level->Size.Y) * 225.0f);
}

void ATowerDefenseGameState::OnEnemyCountChanged(int32 Remaining, int32 Destroyed)
{
	if (Remaining < 1) {
		LevelBuilder->ClearLevel();
		EnemyManager->ClearLevel();
		TowerManager->ClearLevel();
		if (CurrentLevel == LevelAggregator->Levels.Num()) {
			GameWon.ExecuteIfBound();
		} else {
			LevelWon.ExecuteIfBound();
		}
	}
}

void ATowerDefenseGameState::OnLastWaypointReached()
{
	LevelBuilder->ClearLevel();
	EnemyManager->ClearLevel();
	TowerManager->ClearLevel();
	LevelLost.ExecuteIfBound();
}

void ATowerDefenseGameState::OnContinueToNextLevel()
{
	++CurrentLevel;
	BeginLevel(LevelAggregator->Levels[CurrentLevel - 1].LoadSynchronous());
}
