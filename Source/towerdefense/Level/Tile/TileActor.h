// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Engine/StaticMesh.h"
#include "../../Tower/SpawnTowerRequestMixin.h"
#include "TileActor.generated.h"

UCLASS()
class TOWERDEFENSE_API ATileActor : public AActor, public ISpawnTowerRequestMixin
{
	GENERATED_BODY()

public:
	ATileActor();

	void Initialize(TArray<UStaticMesh*> Meshes);
	virtual void NotifyActorOnClicked(FKey ButtonPressed) override;
	bool CanPlaceTower() const;
};
