// Fill out your copyright notice in the Description page of Project Settings.


#include "LevelData.h"

FTilePlacement::FTilePlacement()
	: TileName(TEXT("Empty")), Position(FVector2D::ZeroVector), Rotation(ETileRotation::NONE)
{
}

ULevelData::ULevelData()
{
	Waypoints.Add(FVector2D(0.0f, 0.0f));
}
