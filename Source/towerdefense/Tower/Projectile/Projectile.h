// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "UObject/ScriptInterface.h"

#include "Projectile.generated.h"

class ITargetableMixin;

UCLASS()
class TOWERDEFENSE_API AProjectile : public AActor
{
	GENERATED_BODY()

public:
	AProjectile();

	void Initialize(TScriptInterface<ITargetableMixin> InTarget);
	virtual void Tick(float DeltaTime) override;
	UFUNCTION()
	void OnBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
			UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

protected:
	UFUNCTION()
	void OnTargetDestroyed(TScriptInterface<ITargetableMixin> InTarget);

	float TravelTime;
	float TravelDelta;
	float Damage;
	FVector SpawnLocation;
	TScriptInterface<ITargetableMixin> Target;
};
