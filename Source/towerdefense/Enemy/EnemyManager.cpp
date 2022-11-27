// Fill out your copyright notice in the Description page of Project Settings.

#include "EnemyManager.h"

#include "Math/Vector2D.h"

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

void AEnemyManager::BeginLevel(TArray<FVector2D> pWaypoints,
		int32 EnemyCount, float EnemySpawnDelay, float EnemySpawnCooldown)
{
	FTimerManager& TimerManager = GetWorldTimerManager();
	if (TimerManager.TimerExists(SpawnTimer)) {
		TimerManager.PauseTimer(SpawnTimer);
		TimerManager.ClearTimer(SpawnTimer);
	}
	Waypoints.Empty();
	Waypoints.Reserve(pWaypoints.Num());
	for (FVector2D& GridPosition : pWaypoints) {
		Waypoints.Emplace(GridPosition.X * 100.0f, GridPosition.Y * 100.0f, ZOffset);
	}
	EnemiesToSpawn = EnemyCount;
	EnemiesRemaining = EnemiesToSpawn;
	EnemiesDestroyed = 0;
	SpawnTimer = FTimerHandle();
	TimerManager.SetTimer(SpawnTimer, SpawnTimerTimeout, EnemySpawnCooldown, true, EnemySpawnDelay);
	EnemyCountChanged.Broadcast(EnemiesRemaining, EnemiesDestroyed);
}

void AEnemyManager::SetDelegates(FGameEvents::FEnemyCountChanged& InEnemyCountChanged,
		FGameEvents::FLastWaypointReached& InLastWaypointReached)
{
	EnemyCountChanged = InEnemyCountChanged;
	LastWaypointReached = InLastWaypointReached;
}

ITargetableMixin::FSpawned& AEnemyManager::GetEnemySpawnedDelegate()
{
	return EnemySpawned;
}

ITargetableMixin::FDestroyed& AEnemyManager::GetEnemyDestroyedDelegate()
{
	return EnemyDestroyed;
}

void AEnemyManager::Spawn()
{
	--EnemiesToSpawn;
	FVector SpawnLocation(Waypoints[0]);
	AEnemy* Enemy = GetWorld()->SpawnActor<AEnemy>(SpawnLocation, FRotator());
	AEnemy::FRequestNextWaypoint RequestNextWaypoint;
	RequestNextWaypoint.BindUObject(this, &AEnemyManager::OnEnemyRequestNextWaypoint);
	Enemy->SetDelegate(RequestNextWaypoint);
	Enemy->TargetDestroyed.AddUObject(this, &AEnemyManager::OnEnemyDestroyed);

	EnemySpawned.ExecuteIfBound(Cast<UObject>(Enemy));
	if (EnemiesToSpawn < 1) {
		FTimerManager& TimerManager = GetWorldTimerManager();
		TimerManager.PauseTimer(SpawnTimer);
		TimerManager.ClearTimer(SpawnTimer);
	}
}

bool AEnemyManager::OnEnemyRequestNextWaypoint(FVector CurrentWaypoint, FVector& OutNextWaypoint)
{
	for (int32 i = 1; i < Waypoints.Num(); ++i) {
		if (CurrentWaypoint.Equals(Waypoints[i - 1])) {
			OutNextWaypoint = Waypoints[i];
			return false;
		}
	}
	LastWaypointReached.ExecuteIfBound();
	return true;
}

void AEnemyManager::OnEnemyDestroyed(TScriptInterface<ITargetableMixin> Enemy)
{
	--EnemiesRemaining;
	++EnemiesDestroyed;
	if (EnemiesRemaining < 1) {
		GEngine->AddOnScreenDebugMessage(-1, 10.0f, FColor::Yellow, TEXT("All enemies defeated!"));
	}
	GEngine->AddOnScreenDebugMessage(-1, 10.0f, FColor::White, TEXT("OnEnemyDestroyed"));
	EnemyDestroyed.Broadcast(Enemy);
	EnemyCountChanged.Broadcast(EnemiesRemaining, EnemiesDestroyed);
}
