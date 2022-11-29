// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/EngineTypes.h"
#include "GameFramework/Actor.h"
#include "TimerManager.h"
#include "UObject/ScriptInterface.h"

#include "../GameEvents.h"
#include "TargetableMixin.h"

#include "EnemyManager.generated.h"

class UAudioComponent;

UCLASS()
class TOWERDEFENSE_API AEnemyManager : public AActor
{
	GENERATED_BODY()

public:
	DECLARE_DELEGATE_OneParam(FEnemySpawned, TScriptInterface<ITargetableMixin>)
	DECLARE_DELEGATE_OneParam(FEnemyDestroyed, TScriptInterface<ITargetableMixin>)

	AEnemyManager();

	void ClearLevel();
	void BeginLevel(const TArray<FVector2D>& InWaypoints, int32 EnemyCount, float EnemySpawnDelay, float EnemySpawnCooldown);
	void SetDelegates(FGameEvents::FEnemyCountChanged& InEnemyCountChanged,
			FGameEvents::FLastWaypointReached& InLastWaypointReached);
	ITargetableMixin::FSpawned& GetEnemySpawnedDelegate();
	ITargetableMixin::FDestroyed& GetEnemyDestroyedDelegate();
	ITargetableMixin::FDestroyed& GetEnemyReachedLastWaypoint();

protected:
	virtual void BeginPlay() override;
	void Spawn();
	// note: Returns true if last waypoint is reached,
	// otherwise returns false and writes next waypoint into out parameter.
	UFUNCTION()
	bool OnEnemyRequestNextWaypoint(TScriptInterface<ITargetableMixin> Enemy,
			FVector CurrentWaypoint, FVector& OutNextWaypoint);
	UFUNCTION()
	void OnEnemyDamaged();
	UFUNCTION()
	void OnEnemyDestroyed(TScriptInterface<ITargetableMixin> Enemy);

	ITargetableMixin::FSpawned EnemySpawned;
	ITargetableMixin::FDestroyed EnemyDestroyed;
	ITargetableMixin::FDestroyed EnemyReachedLastWaypoint;
	FGameEvents::FEnemyCountChanged EnemyCountChanged;
	FGameEvents::FLastWaypointReached LastWaypointReached;

	TArray<FVector> Waypoints;
	int32 EnemiesToSpawn;
	int32 EnemiesRemaining;
	int32 EnemiesDestroyed;
	FTimerHandle SpawnTimer;
	FTimerDelegate SpawnTimerTimeout;
	UPROPERTY()
	UAudioComponent* DamageAudio;
	UPROPERTY()
	UAudioComponent* DestroyAudio;
};
