// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Components/SphereComponent.h"
#include "Components/StaticMeshComponent.h"
#include "UObject/ScriptInterface.h"
#include "../../Enemy/TargetableMixin.h"
#include "Projectile.generated.h"

UCLASS()
class TOWERDEFENSE_API AProjectile : public AActor
{
	GENERATED_BODY()

public:
	AProjectile();

	void Initialize(TScriptInterface<ITargetableMixin> pTarget);
	virtual void Tick(float DeltaTime) override;

protected:
	UPROPERTY(VisibleAnywhere)
	float TravelTime;
	UPROPERTY(VisibleAnywhere)
	float TravelDelta;
	UPROPERTY(VisibleAnywhere)
	FVector SpawnLocation;
	UPROPERTY(VisibleAnywhere)
	TScriptInterface<ITargetableMixin> Target;

	UPROPERTY(EditAnywhere)
	USphereComponent* Sphere;
	UPROPERTY()
	UStaticMeshComponent* Mesh;
};
