// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Components/StaticMeshComponent.h"
#include "TowerActor.generated.h"

USTRUCT()
struct FTowerComponentConstructionData
{
	GENERATED_BODY()

public:
	FTowerComponentConstructionData() = default;
	FTowerComponentConstructionData(UStaticMeshComponent*, USceneComponent*, float, FString);

	UPROPERTY()
	UStaticMeshComponent* Component;
	UPROPERTY()
	USceneComponent* Parent;
	UPROPERTY()
	float ZOffset;
	UPROPERTY()
	FString AssetPath;
};

UCLASS()
class TOWERDEFENSE_API ATowerActor : public AActor
{
	GENERATED_BODY()

public:
	ATowerActor();

	virtual void Tick(float DeltaTime) override;

protected:
	virtual void BeginPlay() override;

	UPROPERTY(EditAnywhere)
	float RotationDegreesPerSecond;

	UPROPERTY(VisibleAnywhere)
	UStaticMeshComponent* Base;
	UPROPERTY(VisibleAnywhere)
	UStaticMeshComponent* Middle;
	UPROPERTY(VisibleAnywhere)
	UStaticMeshComponent* Top;
	UPROPERTY(VisibleAnywhere)
	UStaticMeshComponent* CannonBase;
	UPROPERTY(VisibleAnywhere)
	UStaticMeshComponent* Cannon;

private:
	void ConstructComponents();
};