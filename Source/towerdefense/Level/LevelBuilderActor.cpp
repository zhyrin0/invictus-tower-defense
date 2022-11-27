// Fill out your copyright notice in the Description page of Project Settings.

#include "LevelBuilderActor.h"

#include "Engine/StaticMesh.h"

#include "Tile/Tile.h"
#include "Tile/TileData.h"
#include "LevelData.h"

ALevelBuilderActor::ALevelBuilderActor()
{
	PrimaryActorTick.bCanEverTick = false;
	static auto TileAsset = ConstructorHelpers::FObjectFinder<UTileData>(
			TEXT("TileData'/Game/Level/Tile/TileData.TileData'"));
	BuildTileMap(TileAsset.Object);
}

FSpawnTowerRequestList ALevelBuilderActor::BuildLevel(
		int32 Width, int32 Height, TArray<FTilePlacement> Tiles) const
{
	FSpawnTowerRequestList Result;

	FString EmptyTileName = TEXT("Empty");
	UWorld* World = GetWorld();
	for (FTilePlacement& Placement : Tiles) {
		FVector TileLocation(Placement.Position, 0.0f);
		TileLocation *= TILE_SIZE;
		uint8 RotationCount = static_cast<uint8>(Placement.Rotation);
		FRotator TileRotation(0.0f, 90.0f * RotationCount, 0.0f);

		TArray<UStaticMesh*> Meshes;
		for (TSoftObjectPtr<UStaticMesh> MeshPtr : TileMap[Placement.TileName]) {
			UStaticMesh* Mesh = MeshPtr.LoadSynchronous();
			Meshes.Add(Mesh);
		}

		ATile* Tile = World->SpawnActor<ATile>(TileLocation, TileRotation);
		Tile->Initialize(Meshes);
		if (Placement.TileName == EmptyTileName) {
			Result.Add(&(Tile->SpawnTowerRequest));
		}
	}

	return Result;
}

void ALevelBuilderActor::BuildTileMap(UTileData* TileDB)
{
	if (TileDB == nullptr) {
		return;
	}
	for (FTileDescriptor& Descriptor : TileDB->Tiles) {
		TileMap.Add(Descriptor.TileName, Descriptor.Meshes);
	}
}
