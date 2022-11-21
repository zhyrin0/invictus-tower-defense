// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "VectorTypes.h"
#include "LevelDescriptor.generated.h"

USTRUCT()
struct FGridPosition
{
	GENERATED_BODY()

public:
	FGridPosition();

	UPROPERTY(EditAnywhere)
	int32 XPosition;

	UPROPERTY(EditAnywhere)
	int32 YPosition;
};

UENUM()
enum class ETileRotation : uint8
{
	NONE		UMETA(DisplayName = "None"),
	ROTATE_90	UMETA(DisplayName = "Rotate 90"),
	ROTATE_180	UMETA(DisplayName = "Rotate 180"),
	ROTATE_270	UMETA(DisplayName = "Rotate 270"),
};

USTRUCT()
struct FTilePlacement
{
	GENERATED_BODY()

public:
	FTilePlacement();

	UPROPERTY(EditAnywhere)
	FGridPosition Position;

	UPROPERTY(EditAnywhere)
	FString TileName;

	UPROPERTY(EditAnywhere)
	ETileRotation Rotation;
};

UCLASS()
class TOWERDEFENSE_API ULevelDescriptor : public UPrimaryDataAsset
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere)
	int32 Width;

	UPROPERTY(EditAnywhere)
	int32 Height;

	UPROPERTY(EditAnywhere)
	TArray<FTilePlacement> Tiles;

	UPROPERTY(EditAnywhere)
	TArray<FGridPosition> Waypoints;

	UPROPERTY(EditAnywhere)
	int32 EnemyCount;

	UPROPERTY(EditAnywhere)
	float EnemySpawnDelay;

	UPROPERTY(EditAnywhere)
	float EnemySpawnCooldown;
};
