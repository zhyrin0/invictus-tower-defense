// Fill out your copyright notice in the Description page of Project Settings.

#include "EnemyManager.h"

#include "Kismet/GameplayStatics.h"
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

void AEnemyManager::ClearLevel()
{
	TArray<AActor*> Enemies;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), AEnemy::StaticClass(), Enemies);
	for (AActor* Enemy : Enemies) {
		Enemy->Destroy();
	}
}

void AEnemyManager::BeginLevel(TArray<FVector2D> InWaypoints,
		int32 EnemyCount, float EnemySpawnDelay, float EnemySpawnCooldown)
{
	FTimerManager& TimerManager = GetWorldTimerManager();
	if (TimerManager.TimerExists(SpawnTimer)) {
		TimerManager.PauseTimer(SpawnTimer);
		TimerManager.ClearTimer(SpawnTimer);
	}
	Waypoints.Empty();
	Waypoints.Reserve(InWaypoints.Num());
	for (FVector2D& GridPosition : InWaypoints) {
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
	AEnemy* Enemy = GetWorld()->SpawnActor<AEnemy>(SpawnLocation, FRotator::ZeroRotator);
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
	EnemyDestroyed.Broadcast(Enemy);
	EnemyCountChanged.Broadcast(EnemiesRemaining, EnemiesDestroyed);
}
