// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "UObject/SoftObjectPtr.h"
#include "Tile/TileDescriptors.h"
#include "../Tower/SpawnTowerRequestMixin.h"
#include "LevelBuilderActor.generated.h"

UCLASS()
class TOWERDEFENSE_API ALevelBuilderActor : public AActor
{
	GENERATED_BODY()

public:
	DECLARE_DELEGATE(FBeganPlay)

	ALevelBuilderActor();

	// note: Returns a list of spawn request delegates of the empty tiles.
	FSpawnTowerRequestList BuildLevel(FString LevelName) const;

	static constexpr int32 TILE_SIZE = 100;
	FBeganPlay BeganPlay;

protected:
	virtual void BeginPlay() override;

	UPROPERTY(EditAnywhere)
	UTileDescriptors* TileDB;

private:
	void BuildTileMap();

	TMap<FString, TArray<TSoftObjectPtr<UStaticMesh>>> TileMap;
};
