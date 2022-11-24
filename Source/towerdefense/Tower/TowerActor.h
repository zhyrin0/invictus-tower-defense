// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Components/StaticMeshComponent.h"
#include "Engine/EngineTypes.h"
#include "TimerManager.h"
#include "UObject/ScriptInterface.h"
#include "TowerActor.generated.h"

class ITargetableMixin;

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
	UFUNCTION()
	void SetTarget(TScriptInterface<ITargetableMixin> NewTarget);

protected:
	virtual void BeginPlay() override;
	UFUNCTION()
	void OnAttackTimeout();

	UPROPERTY(EditAnywhere)
	float RotationDegreesPerSecond;
	UPROPERTY(VisibleAnywhere)
	TScriptInterface<ITargetableMixin> Target;
	UPROPERTY(VisibleAnywhere)
	FTimerHandle AttackTimer;
	FTimerDelegate AttackTimeout;

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
