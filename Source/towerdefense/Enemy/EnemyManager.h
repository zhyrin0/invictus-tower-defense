// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "TimerManager.h"
#include "Engine/EngineTypes.h"
#include "../Level/LevelDescriptor.h"
#include "EnemyManager.generated.h"

UCLASS()
class TOWERDEFENSE_API AEnemyManager : public AActor
{
	GENERATED_BODY()

public:
	AEnemyManager();

	void BeginLevel(TArray<FGridPosition> pWaypoints, int32 EnemyCount, float pEnemySpawnDelay, float pEnemySpawnCooldown);

protected:
	virtual void BeginPlay() override;
	void Spawn();
	UFUNCTION()
	FVector OnEnemyRequestNextWaypoint(FVector Location);

	static constexpr float ZOffset = 30.0f;
	UPROPERTY(VisibleAnywhere)
	TArray<FGridPosition> Waypoints;
	UPROPERTY(VisibleAnywhere)
	int32 EnemiesToSpawn;
	UPROPERTY(VisibleAnywhere)
	int32 EnemiesRemaining;
	UPROPERTY(VisibleAnywhere)
	FTimerHandle SpawnTimer;
	FTimerDelegate SpawnTimerTimeout;
};
