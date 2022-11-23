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

	void Initialize(TArray<UStaticMesh*> Meshes, bool IsEmpty);
	virtual void Tick(float DeltaTime) override;
	virtual void NotifyActorOnClicked(FKey ButtonPressed) override;

protected:
	virtual void BeginPlay() override;

	UPROPERTY(VisibleAnywhere)
	bool CanPlaceTower;
};
