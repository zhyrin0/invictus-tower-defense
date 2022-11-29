// Fill out your copyright notice in the Description page of Project Settings.

#include "LevelBuilder.h"

#include "Engine/StaticMesh.h"
#include "Kismet/GameplayStatics.h"

#include "../Grid2D.h"
#include "Tile/Tile.h"
#include "Tile/TileData.h"
#include "LevelData.h"

ALevelBuilder::ALevelBuilder()
{
	static auto TileAsset = ConstructorHelpers::FObjectFinder<UTileData>(
			TEXT("TileData'/Game/Level/Tile/TileData.TileData'"));
	
	PrimaryActorTick.bCanEverTick = false;
	BuildTileMap(TileAsset.Object);
}

void ALevelBuilder::ClearLevel()
{
	TArray<AActor*> Tiles;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), ATile::StaticClass(), Tiles);
	for (AActor* Tile : Tiles) {
		Tile->Destroy();
	}
}

FSpawnTowerRequestList ALevelBuilder::BuildLevel(FVector2D Size, const TArray<FTilePlacement>& Tiles) const
{
	static FString EmptyTileName = TEXT("Empty");
	
	FSpawnTowerRequestList Result;

	TArray<FVector2D> UnbuiltTiles;
	for (float Y = 0.0f; Y < Size.Y; ++Y) {
		for (float X = 0.0f; X < Size.X; ++X) {
			UnbuiltTiles.Emplace(X, Y);
		}
	}

	for (const FTilePlacement& Placement : Tiles) {
		if (!TileMap.Contains(Placement.TileName)) {
			continue;
		}
		uint8 RotationCount = static_cast<uint8>(Placement.Rotation);
		ATile* Tile = BuildTile(Placement.TileName, Placement.Position, RotationCount);
		if (Placement.TileName == EmptyTileName) {
			Result.Add(&(Tile->SpawnTowerRequest));
		}
		UnbuiltTiles.Remove(Placement.Position);
	}

	for (FVector2D UnbuiltPosition : UnbuiltTiles) {
		ATile* Tile = BuildTile(EmptyTileName, UnbuiltPosition, 0);
		Result.Add(&(Tile->SpawnTowerRequest));
	}

	return Result;
}

ATile* ALevelBuilder::BuildTile(FString Name, FVector2D Position, int32 RotationCount) const
{
	ATile* Result = nullptr;
	FVector Location = FGrid2D::GridToGlobal(Position);
	FRotator Rotation(0.0f, 90.0f * RotationCount, 0.0f);
	TArray<UStaticMesh*> Meshes;
	for (TSoftObjectPtr<UStaticMesh> MeshPtr : TileMap[Name]) {
		UStaticMesh* Mesh = MeshPtr.LoadSynchronous();
		Meshes.Add(Mesh);
	}
	Result = GetWorld()->SpawnActor<ATile>(Location, Rotation);
	Result->Initialize(Meshes);
	return Result;
}

void ALevelBuilder::BuildTileMap(UTileData* TileDB)
{
	if (TileDB == nullptr) {
		return;
	}
	for (FTileDescriptor& Descriptor : TileDB->Tiles) {
		TileMap.Add(Descriptor.TileName, Descriptor.Meshes);
	}
}
