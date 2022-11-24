// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Components/StaticMeshComponent.h"
#include "Enemy.generated.h"

UCLASS()
class TOWERDEFENSE_API AEnemy : public AActor
{
	GENERATED_BODY()

public:
	DECLARE_DELEGATE_RetVal_TwoParams(bool, FRequestNextWaypoint, FVector, FVector&)

	AEnemy();

	void Initialize();
	virtual void Tick(float DeltaTime) override;

	FRequestNextWaypoint RequestNextWaypoint;

protected:
	void DoRequestNextWaypoint();

	UPROPERTY(VisibleAnywhere)
	float Speed;
	UPROPERTY(VisibleAnywhere)
	FVector CurrentWaypoint;
	UPROPERTY(VisibleAnywhere)
	FVector Direction;
	UPROPERTY(EditAnywhere)
	UStaticMeshComponent* Mesh;
};
