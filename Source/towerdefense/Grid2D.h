// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

struct FGrid2D
{
	static double GetTileSize();
	static FVector GridToGlobal(FVector2D GridLocation);
	static FVector2D GlobalToGrid(FVector GlobalLocation);
	static FVector EnemyOffset();
	static FVector TowerOffset();
	static FVector TowerMiddleOffset();
	static FVector TowerTopOffset();
	static FVector TowerCannonBaseOffset();

	// note: Tiles are arranged with X,Y coordinates and placed as such.
	// This would cause the map to look rotated if looking at it in a forward direction in global space.
	static constexpr float CAMERA_YAW = -90.0f;

private:
	FGrid2D();

	static FGrid2D Instance;
	double TileSize;
};
