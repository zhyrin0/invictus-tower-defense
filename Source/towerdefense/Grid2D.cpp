// Fill out your copyright notice in the Description page of Project Settings.

#include "Grid2D.h"

#include "Engine/StaticMesh.h"

FGrid2D::FGrid2D()
{
	FString TileReference = TEXT("StaticMesh'/Game/Level/Tile/Meshes/tile.tile'");
	UStaticMesh* TileMesh = LoadObject<UStaticMesh>(nullptr, *TileReference);
	FVector3d MeshSize = TileMesh->GetBoundingBox().GetSize();
	TileSize = FMath::Max(MeshSize.X, MeshSize.Y);
}

double FGrid2D::GetTileSize()
{
	return FGrid2D::Instance.TileSize;
}

FVector FGrid2D::GridToGlobal(FVector2D GridLocation)
{
	return FVector(GridLocation * FGrid2D::Instance.TileSize, 0.0f);
}

FVector2D FGrid2D::GlobalToGrid(FVector GlobalLocation)
{
	return FVector2D(FMath::GridSnap(GlobalLocation.X, FGrid2D::Instance.TileSize),
			FMath::GridSnap(GlobalLocation.Y, FGrid2D::Instance.TileSize));
}

FVector FGrid2D::EnemyOffset()
{
	return FVector(0.0f, 0.0f, 30.0f);
}

FVector FGrid2D::TowerOffset()
{
	return FVector(0.0f, 0.0f, 20.0f);
}

FVector FGrid2D::TowerMiddleOffset()
{
	return FVector(0.0f, 0.0f, 50.0f);
}

FVector FGrid2D::TowerTopOffset()
{
	return FVector(0.0f, 0.0f, 100.0f);
}

FVector FGrid2D::TowerCannonBaseOffset()
{
	return FVector(0.0f, 0.0f, 130.0f);
}

FGrid2D FGrid2D::Instance;
