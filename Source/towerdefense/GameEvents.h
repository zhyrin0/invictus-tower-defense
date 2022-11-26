// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

struct FGameEvents
{
	// note: Parameter: int32 LevelNumber
	DECLARE_MULTICAST_DELEGATE_OneParam(FPlayRequested, int32)
	DECLARE_DELEGATE(FQuitRequested)
	// note: Parameter: int32 LevelNumber
	DECLARE_DELEGATE_OneParam(FLevelChanged, int32)
	// note: Parameters: int32 Remaining, int32 Destroyed
	DECLARE_MULTICAST_DELEGATE_TwoParams(FEnemyCountChanged, int32, int32)
	DECLARE_DELEGATE(FLastWaypointReached)
	DECLARE_DELEGATE(FLevelWon)
	DECLARE_DELEGATE(FLevelLost)
	DECLARE_DELEGATE(FGameWon)
};
