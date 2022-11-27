// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"

#include "../../Tower/SpawnTowerRequestMixin.h"

#include "Tile.generated.h"

class UStaticMesh;

UCLASS()
class TOWERDEFENSE_API ATile : public AActor, public ISpawnTowerRequestMixin
{
	GENERATED_BODY()

public:
	ATile();

	void Initialize(TArray<UStaticMesh*> Meshes);
	virtual void NotifyActorOnClicked(FKey ButtonPressed) override;

protected:
	bool CanPlaceTower() const;
};
