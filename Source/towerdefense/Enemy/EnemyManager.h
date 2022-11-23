// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "EnemyManager.generated.h"

UCLASS()
class TOWERDEFENSE_API AEnemyManager : public AActor
{
	GENERATED_BODY()

public:
	AEnemyManager();

	void Spawn();

protected:
	virtual void BeginPlay() override;
};
