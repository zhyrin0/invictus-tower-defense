// Fill out your copyright notice in the Description page of Project Settings.


#include "LevelDescriptor.h"

FGridPosition::FGridPosition()
	: XPosition(0), YPosition(0)
{
}

FTilePlacement::FTilePlacement()
	: Position(), TileName(TEXT("Empty")), Rotation(ETileRotation::NONE)
{
}
