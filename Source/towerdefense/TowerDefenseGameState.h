// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameStateBase.h"

#include "GameEvents.h"

#include "TowerDefenseGameState.generated.h"

class ACameraPawn;
class AEnemyManager;
class ALevelBuilder;
class ATowerManager;
class ULevelAggregator;
class ULevelData;

UCLASS()
class TOWERDEFENSE_API ATowerDefenseGameState : public AGameStateBase
{
	GENERATED_BODY()

public:
	ATowerDefenseGameState();

	virtual void OnConstruction(const FTransform& Transform) override;
	void Initialize(ACameraPawn* InCameraPawn);
	void BindDelegates(FGameEvents::FPlayRequested& InPlayRequested,
			FGameEvents::FEnemyCountChanged& InEnemyCountChanged,
			FGameEvents::FLastWaypointReached& InLastWaypointReached,
			FGameEvents::FUIContinueRequested& InContinueRequested);
	void SetDelegates(FGameEvents::FEnemyCountChanged& InEnemyManagerEnemyCountChanged,
			FGameEvents::FLastWaypointReached& InEnemyManagerLastWaypointReached,
			FGameEvents::FLevelChanged& InLevelChanged,
			FGameEvents::FLevelWon& InLevelWon,
			FGameEvents::FLevelLost& InLevelLost,
			FGameEvents::FGameWon& InGameWon);

protected:
	void BeginLevel(ULevelData* Level) const;
	UFUNCTION()
	void OnPlayRequested(int32 LevelNumber);
	UFUNCTION()
	void OnEnemyCountChanged(int32 Remaining, int32 Destroyed);
	UFUNCTION()
	void OnLastWaypointReached();
	UFUNCTION()
	void OnContinueToNextLevel();

	FGameEvents::FLevelChanged LevelChanged;
	FGameEvents::FLevelWon LevelWon;
	FGameEvents::FLevelLost LevelLost;
	FGameEvents::FGameWon GameWon;

	UPROPERTY()
	int32 CurrentLevel;
	UPROPERTY()
	ULevelAggregator* LevelAggregator;

	ACameraPawn* CameraPawn;
	UPROPERTY(VisibleAnywhere)
	ALevelBuilder* LevelBuilder;
	UPROPERTY(VisibleAnywhere)
	AEnemyManager* EnemyManager;
	UPROPERTY(VisibleAnywhere)
	ATowerManager* TowerManager;
};
