// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "UObject/SoftObjectPtr.h"
#include "Tile/TileDescriptors.h"
#include "LevelBuilderActor.generated.h"

UCLASS()
class TOWERDEFENSE_API ALevelBuilderActor : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ALevelBuilderActor();

	void BuildLevel(FString LevelName) const;

	static constexpr int32 TILE_SIZE = 100;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UPROPERTY(EditAnywhere)
	UTileDescriptors* TileDB;

private:
	void BuildTileMap();
	
	TMap<FString, TArray<TSoftObjectPtr<UStaticMesh>>> TileMap;

};
