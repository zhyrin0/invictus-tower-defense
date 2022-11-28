// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "Math/Vector2D.h"

#include "LevelData.generated.h"

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
	FString TileName;
	UPROPERTY(EditAnywhere)
	FVector2D Position;
	UPROPERTY(EditAnywhere)
	ETileRotation Rotation;
};

UCLASS()
class TOWERDEFENSE_API ULevelData : public UPrimaryDataAsset
{
	GENERATED_BODY()

public:
	ULevelData();

	UPROPERTY(EditAnywhere)
	FVector2D Size;
	UPROPERTY(EditAnywhere)
	TArray<FTilePlacement> Tiles;
	UPROPERTY(EditAnywhere)
	TArray<FVector2D> Waypoints;
	UPROPERTY(EditAnywhere)
	int32 EnemyCount;
	UPROPERTY(EditAnywhere)
	float EnemySpawnDelay;
	UPROPERTY(EditAnywhere)
	float EnemySpawnCooldown;
};
