// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "UObject/SoftObjectPtr.h"

#include "../Tower/SpawnTowerRequestMixin.h"

#include "LevelBuilderActor.generated.h"

struct FTilePlacement;
class UTileData;

UCLASS()
class TOWERDEFENSE_API ALevelBuilderActor : public AActor
{
	GENERATED_BODY()

public:
	ALevelBuilderActor();

	// note: Returns a list of spawn request delegates of the empty tiles.
	FSpawnTowerRequestList BuildLevel(int32 Width, int32 Height, TArray<FTilePlacement> Tiles) const;

	static constexpr int32 TILE_SIZE = 100;

protected:
	void BuildTileMap(UTileData* TileDB);

	TMap<FString, TArray<TSoftObjectPtr<UStaticMesh>>> TileMap;
};
