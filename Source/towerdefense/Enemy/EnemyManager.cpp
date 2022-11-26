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
	Waypoints.Empty();
	Waypoints.Reserve(pWaypoints.Num());
	for (FGridPosition& GridPosition : pWaypoints) {
		Waypoints.Emplace(GridPosition.XPosition * 100.0f, GridPosition.YPosition * 100.0f, ZOffset);
	}
	EnemiesToSpawn = EnemyCount;
	EnemiesRemaining = EnemiesToSpawn;
	EnemiesDestroyed = 0;
	SpawnTimer = FTimerHandle();
	TimerManager.SetTimer(SpawnTimer, SpawnTimerTimeout, EnemySpawnCooldown, true, EnemySpawnDelay);
}

void AEnemyManager::Spawn()
{
	FVector SpawnLocation(Waypoints[0]);
	AEnemy* Enemy = GetWorld()->SpawnActor<AEnemy>(SpawnLocation, FRotator());
	Enemy->RequestNextWaypoint.BindUObject(this, &AEnemyManager::OnEnemyRequestNextWaypoint);
	Enemy->LastWaypointReached.BindUObject(this, &AEnemyManager::OnEnemyLastWaypointReached);
	Enemy->TargetDestroyed.AddUObject(this, &AEnemyManager::OnEnemyDestroyed);
	--EnemiesToSpawn;
	if (EnemiesToSpawn < 1) {
		FTimerManager& TimerManager = GetWorldTimerManager();
		TimerManager.PauseTimer(SpawnTimer);
		TimerManager.ClearTimer(SpawnTimer);
	}
	Enemy->Initialize();
	TScriptInterface<ITargetableMixin> Target(Cast<UObject>(Enemy));
	EnemySpawned.ExecuteIfBound(Target);
}

bool AEnemyManager::OnEnemyRequestNextWaypoint(FVector CurrentWaypoint, FVector& OutNextWaypoint)
{
	bool IsNextWaypoint = false;
	for (FVector Waypoint : Waypoints) {
		if (IsNextWaypoint) {
			OutNextWaypoint = Waypoint;
			return false;
		}
		if (CurrentWaypoint.Equals(Waypoint, 0.5)) {
			IsNextWaypoint = true;
		}
	}
	return true;
}

void AEnemyManager::OnEnemyLastWaypointReached(TScriptInterface<ITargetableMixin> Enemy)
{
	GEngine->AddOnScreenDebugMessage(-1, 10.0f, FColor::White, TEXT("OnEnemyLastWaypointReached"));
	EnemyDestroyed.ExecuteIfBound(Enemy);
}

void AEnemyManager::OnEnemyDestroyed(TScriptInterface<ITargetableMixin> Enemy)
{
	--EnemiesRemaining;
	++EnemiesDestroyed;
	if (EnemiesRemaining < 1) {
		GEngine->AddOnScreenDebugMessage(-1, 10.0f, FColor::Yellow, TEXT("All enemies defeated!"));
	}
	GEngine->AddOnScreenDebugMessage(-1, 10.0f, FColor::White, TEXT("OnEnemyDestroyed"));
	EnemyDestroyed.ExecuteIfBound(Enemy);
}
