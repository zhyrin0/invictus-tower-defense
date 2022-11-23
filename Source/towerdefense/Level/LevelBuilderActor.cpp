// Fill out your copyright notice in the Description page of Project Settings.

#include "LevelBuilderActor.h"

#include "Engine/StaticMesh.h"
#include "LevelDescriptor.h"
#include "Tile/TileActor.h"

ALevelBuilderActor::ALevelBuilderActor()
{
	PrimaryActorTick.bCanEverTick = false;
	auto TilesAsset = ConstructorHelpers::FObjectFinder<UTileDescriptors>(
		TEXT("TileDescriptors'/Game/Level/Tile/TileDescriptors.TileDescriptors'"));
	TileDB = TilesAsset.Object;
}

FSpawnTowerRequestList ALevelBuilderActor::BuildLevel(FString LevelName) const
{
	FSpawnTowerRequestList Result;

	FString Reference = FString::Printf(TEXT("LevelDescriptor'/Game/Level/%s.%s'"), *LevelName, *LevelName);
	ULevelDescriptor* Descriptor = LoadObject<ULevelDescriptor>(NULL, *Reference, NULL, LOAD_None, NULL);

	if (Descriptor == nullptr && GEngine) {
		GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::White, TEXT("Couldn't load!"));
		return Result;
	}

	FString EmptyTileName = TEXT("Empty");
	UWorld* World = GetWorld();
	for (FTilePlacement& Placement : Descriptor->Tiles) {

		FVector TileLocation(Placement.Position.XPosition, Placement.Position.YPosition, 0.0);
		TileLocation *= TILE_SIZE;

		uint8 RotationCount = static_cast<uint8>(Placement.Rotation);
		FRotator TileRotation(0.0, 90.0 * RotationCount, 0.0);

		TArray<UStaticMesh*> Meshes;
		for (TSoftObjectPtr<UStaticMesh> MeshPtr : TileMap[Placement.TileName]) {
			UStaticMesh* Mesh = MeshPtr.LoadSynchronous();
			Meshes.Add(Mesh);
		}

		ATileActor* Tile = World->SpawnActor<ATileActor>(TileLocation, TileRotation);
		bool IsEmpty = Placement.TileName == EmptyTileName;
		Tile->Initialize(Meshes, IsEmpty);
		if (IsEmpty) {
			Result.Add(&(Tile->SpawnTowerRequest));
		}
	}

	return Result;
}

void ALevelBuilderActor::BeginPlay()
{
	Super::BeginPlay();
	BuildTileMap();
	BeganPlay.ExecuteIfBound();
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
