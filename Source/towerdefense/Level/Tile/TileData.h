// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "UObject/SoftObjectPtr.h"

#include "TileData.generated.h"

USTRUCT()
struct FTileDescriptor
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere)
	FString TileName;

	UPROPERTY(EditAnywhere)
	TArray<TSoftObjectPtr<UStaticMesh>> Meshes;
};

UCLASS()
class TOWERDEFENSE_API UTileData : public UPrimaryDataAsset
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere)
	TArray<FTileDescriptor> Tiles;
};
