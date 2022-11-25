// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

struct FGameEvents
{
	DECLARE_MULTICAST_DELEGATE_TwoParams(FLevelRequested, FText, int32)
	DECLARE_DELEGATE(FQuitRequested)
};
