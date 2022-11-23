// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "TowerSpawner.generated.h"

UCLASS()
class TOWERDEFENSE_API ATowerSpawner : public AActor
{
	GENERATED_BODY()

public:
	ATowerSpawner();

	void Spawn(FVector Location) const;

protected:
	UPROPERTY(EditAnywhere)
	int32 ZOffset;
};
