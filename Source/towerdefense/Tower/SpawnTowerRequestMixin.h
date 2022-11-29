// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"

#include "SpawnTowerRequestMixin.generated.h"

UINTERFACE(MinimalAPI)
class USpawnTowerRequestMixin : public UInterface
{
	GENERATED_BODY()
};

class TOWERDEFENSE_API ISpawnTowerRequestMixin
{
	GENERATED_BODY()

public:
	DECLARE_DELEGATE_OneParam(FSpawnTowerRequest, FVector)

	FSpawnTowerRequest SpawnTowerRequest;
};

using FSpawnTowerRequestList = TArray<ISpawnTowerRequestMixin::FSpawnTowerRequest*>;
