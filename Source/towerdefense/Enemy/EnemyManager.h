// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "TimerManager.h"
#include "Engine/EngineTypes.h"
#include "UObject/ScriptInterface.h"
#include "../Level/LevelDescriptor.h"
#include "../GameEvents.h"
#include "TargetableMixin.h"
#include "EnemyManager.generated.h"

UCLASS()
class TOWERDEFENSE_API AEnemyManager : public AActor
{
	GENERATED_BODY()

public:
	DECLARE_DELEGATE_OneParam(FEnemySpawned, TScriptInterface<ITargetableMixin>)
	DECLARE_DELEGATE_OneParam(FEnemyDestroyed, TScriptInterface<ITargetableMixin>)

	AEnemyManager();

	void BeginLevel(TArray<FGridPosition> pWaypoints, int32 EnemyCount, float pEnemySpawnDelay, float pEnemySpawnCooldown);
	void SetDelegates(FGameEvents::FEnemyCountChanged& InEnemyCountChanged,
			FGameEvents::FLastWaypointReached& InLastWaypointReached);
	ITargetableMixin::FSpawned& GetEnemySpawnedDelegate();
	ITargetableMixin::FDestroyed& GetEnemyDestroyedDelegate();

protected:
	virtual void BeginPlay() override;
	void Spawn();
	// note: Returns true if last waypoint is reached,
	// otherwise returns false and writes next waypoint into out parameter.
	UFUNCTION()
	bool OnEnemyRequestNextWaypoint(FVector CurrentWaypoint, FVector& OutNextWaypoint);
	UFUNCTION()
	void OnEnemyDestroyed(TScriptInterface<ITargetableMixin> Enemy);

	ITargetableMixin::FSpawned EnemySpawned;
	ITargetableMixin::FDestroyed EnemyDestroyed;
	FGameEvents::FEnemyCountChanged EnemyCountChanged;
	FGameEvents::FLastWaypointReached LastWaypointReached;

	static constexpr float ZOffset = 30.0f;
	UPROPERTY(VisibleAnywhere)
	TArray<FVector> Waypoints;
	UPROPERTY(VisibleAnywhere)
	int32 EnemiesToSpawn;
	UPROPERTY(VisibleAnywhere)
	int32 EnemiesRemaining;
	UPROPERTY(VisibleAnywhere)
	int32 EnemiesDestroyed;
	UPROPERTY(VisibleAnywhere)
	FTimerHandle SpawnTimer;
	FTimerDelegate SpawnTimerTimeout;
};
