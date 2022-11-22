// Fill out your copyright notice in the Description page of Project Settings.


#include "LevelBuilderActor.h"

#include "Engine/StaticMesh.h"
#include "LevelDescriptor.h"
#include "Tile/TileActor.h"

// Sets default values
ALevelBuilderActor::ALevelBuilderActor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;
}

void ALevelBuilderActor::BuildLevel(FString LevelName) const
{
	FString Reference = FString::Printf(TEXT("LevelDescriptor'/Game/Level/%s.%s'"), *LevelName, *LevelName);
	ULevelDescriptor* Descriptor = LoadObject<ULevelDescriptor>(NULL, *Reference, NULL, LOAD_None, NULL);

	if (Descriptor == nullptr && GEngine) {
		GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::White, TEXT("Couldn't load!"));
		return;
	}

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
		bool IsEmpty = Placement.TileName == TEXT("Empty");
		Tile->Initialize(Meshes, IsEmpty);
	}
}

// Called when the game starts or when spawned
void ALevelBuilderActor::BeginPlay()
{
	Super::BeginPlay();
	BuildTileMap();
	BuildLevel(TEXT("TestLevel"));
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
