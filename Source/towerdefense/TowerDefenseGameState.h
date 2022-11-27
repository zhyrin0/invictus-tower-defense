// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameStateBase.h"
#include "Enemy/EnemyManager.h"
#include "Level/LevelBuilder.h"
#include "Tower/TowerManager.h"
#include "GameEvents.h"
#include "TowerDefenseGameState.generated.h"

UCLASS()
class TOWERDEFENSE_API ATowerDefenseGameState : public AGameStateBase
{
	GENERATED_BODY()

public:
	virtual void OnConstruction(const FTransform& Transform) override;
	void BindDelegates(FGameEvents::FPlayRequested& InPlayRequested,
			FGameEvents::FEnemyCountChanged& InEnemyCountChanged,
			FGameEvents::FLastWaypointReached& InLastWaypointReached);
	void SetDelegates(FGameEvents::FEnemyCountChanged& InEnemyManagerEnemyCountChanged,
			FGameEvents::FLastWaypointReached& InEnemyManagerLastWaypointReached,
			FGameEvents::FLevelChanged& InLevelChanged,
			FGameEvents::FLevelWon& InLevelWon,
			FGameEvents::FLevelLost& InLevelLost,
			FGameEvents::FGameWon& InGameWon);

protected:
	void BeginLevel(FString LevelName) const;
	UFUNCTION()
	void OnPlayRequested(int32 LevelNumber);
	UFUNCTION()
	void OnEnemyCountChanged(int32 Remaining, int32 Destroyed);
	UFUNCTION()
	void OnLastWaypointReached();

	FGameEvents::FLevelChanged LevelChanged;
	FGameEvents::FLevelWon LevelWon;
	FGameEvents::FLevelLost LevelLost;
	FGameEvents::FGameWon GameWon;

	UPROPERTY()
	int32 CurrentLevel;

	UPROPERTY(VisibleAnywhere)
	ALevelBuilder* LevelBuilder;
	UPROPERTY(VisibleAnywhere)
	AEnemyManager* EnemyManager;
	UPROPERTY(VisibleAnywhere)
	ATowerManager* TowerManager;
};
