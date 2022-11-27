// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "UObject/SoftObjectPtr.h"

#include "LevelData.h"

#include "LevelAggregator.generated.h"

UCLASS()
class TOWERDEFENSE_API ULevelAggregator : public UPrimaryDataAsset
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere)
	TArray<TSoftObjectPtr<ULevelData>> Levels;
};
