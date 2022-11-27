// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"

#include "TowerData.generated.h"

USTRUCT()
struct FProjectileData
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere)
	float TravelTime;
	UPROPERTY(EditAnywhere)
	float Damage;
};

UCLASS()
class TOWERDEFENSE_API UTowerData : public UPrimaryDataAsset
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere)
	float RotationDegreesPerSecond;
	UPROPERTY(EditAnywhere)
	float AttacksPerSecond;
	UPROPERTY(EditAnywhere)
	FProjectileData ProjectileData;
};
