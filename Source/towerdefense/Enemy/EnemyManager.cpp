// Fill out your copyright notice in the Description page of Project Settings.

#include "EnemyManager.h"
#include "Enemy.h"

AEnemyManager::AEnemyManager()
{
	PrimaryActorTick.bCanEverTick = false;
	SpawnTimerTimeout.BindUObject(this, &AEnemyManager::Spawn);
}

void AEnemyManager::BeginPlay()
{
	Super::BeginPlay();
}

void AEnemyManager::BeginLevel(TArray<FGridPosition> pWaypoints,
		int32 EnemyCount, float EnemySpawnDelay, float EnemySpawnCooldown)
{
	FTimerManager& TimerManager = GetWorldTimerManager();
	if (TimerManager.TimerExists(SpawnTimer)) {
		TimerManager.PauseTimer(SpawnTimer);
		TimerManager.ClearTimer(SpawnTimer);
	}
	Waypoints = pWaypoints;
	EnemiesToSpawn = EnemyCount;
	EnemiesRemaining = 0;
	SpawnTimer = FTimerHandle();
	TimerManager.SetTimer(SpawnTimer, SpawnTimerTimeout, EnemySpawnCooldown, true, EnemySpawnDelay);
}

void AEnemyManager::Spawn()
{
	FVector SpawnLocation(Waypoints[0].XPosition, Waypoints[0].YPosition, 0.0f);
	SpawnLocation *= 100.0f;
	SpawnLocation.Z = 25.0f;
	GetWorld()->SpawnActor<AEnemy>(SpawnLocation, FRotator());
	++EnemiesRemaining;
	--EnemiesToSpawn;
	if (EnemiesToSpawn < 1) {
		FTimerManager& TimerManager = GetWorldTimerManager();
		TimerManager.PauseTimer(SpawnTimer);
		TimerManager.ClearTimer(SpawnTimer);
	}
}
