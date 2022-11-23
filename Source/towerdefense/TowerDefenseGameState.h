// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameStateBase.h"
#include "Level/LevelBuilderActor.h"
#include "Tower/TowerSpawner.h"
#include "TowerDefenseGameState.generated.h"

UCLASS()
class TOWERDEFENSE_API ATowerDefenseGameState : public AGameStateBase
{
	GENERATED_BODY()

public:
	virtual void OnConstruction(const FTransform& Transform) override;

protected:
	UFUNCTION()
	void BeginLevel() const;

	UPROPERTY(VisibleAnywhere)
	ALevelBuilderActor* LevelBuilder;
	UPROPERTY(VisibleAnywhere)
	ATowerSpawner* TowerSpawner;
};
