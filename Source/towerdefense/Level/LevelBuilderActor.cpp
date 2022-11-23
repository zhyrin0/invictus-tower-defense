// Fill out your copyright notice in the Description page of Project Settings.

#include "LevelBuilderActor.h"

#include "Engine/StaticMesh.h"
#include "Tile/TileActor.h"

ALevelBuilderActor::ALevelBuilderActor()
{
	PrimaryActorTick.bCanEverTick = false;
	auto TilesAsset = ConstructorHelpers::FObjectFinder<UTileDescriptors>(
		TEXT("TileDescriptors'/Game/Level/Tile/TileDescriptors.TileDescriptors'"));
	TileDB = TilesAsset.Object;
}

FSpawnTowerRequestList ALevelBuilderActor::BuildLevel(
		int32 Width, int32 Height, TArray<FTilePlacement> Tiles) const
{
	FSpawnTowerRequestList Result;

	FString EmptyTileName = TEXT("Empty");
	UWorld* World = GetWorld();
	for (FTilePlacement& Placement : Tiles) {
		FVector TileLocation(Placement.Position.XPosition, Placement.Position.YPosition, 0.0);
		TileLocation *= TILE_SIZE;
		uint8 RotationCount = static_cast<uint8>(Placement.Rotation);
		FRotator TileRotation(0.0f, 90.0f * RotationCount, 0.0f);

		TArray<UStaticMesh*> Meshes;
		for (TSoftObjectPtr<UStaticMesh> MeshPtr : TileMap[Placement.TileName]) {
			UStaticMesh* Mesh = MeshPtr.LoadSynchronous();
			Meshes.Add(Mesh);
		}

		ATileActor* Tile = World->SpawnActor<ATileActor>(TileLocation, TileRotation);
		Tile->Initialize(Meshes);
		if (Placement.TileName == EmptyTileName) {
			Result.Add(&(Tile->SpawnTowerRequest));
		}
	}

	return Result;
}

void ALevelBuilderActor::BeginPlay()
{
	Super::BeginPlay();
	BuildTileMap();
	BeganPlay.ExecuteIfBound(TEXT("TestLevel"));
}

void ALevelBuilderActor::BuildTileMap()
{
	if (TileDB == nullptr) {
		return;
	}
	for (FTileDescriptor& Descriptor : TileDB->Tiles) {
		TileMap.Add(Descriptor.TileName, Descriptor.Meshes);
	}
}
