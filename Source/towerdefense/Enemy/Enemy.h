// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "UObject/ScriptInterface.h"

#include "TargetableMixin.h"

#include "Enemy.generated.h"

UCLASS()
class TOWERDEFENSE_API AEnemy : public AActor, public ITargetableMixin
{
	GENERATED_BODY()

public:
	DECLARE_DELEGATE_RetVal_ThreeParams(bool, FRequestNextWaypoint, TScriptInterface<ITargetableMixin>, FVector, FVector&)
	DECLARE_DELEGATE(FDamageTaken)

	AEnemy();

	virtual void Tick(float DeltaTime) override;
	virtual float TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent,
			AController* EventInstigator, AActor* DamageCauser) override;
	virtual FVector GetTargetLocation() const override;
	void SetDelegates(FRequestNextWaypoint& InRequestNextWaypoint, FDamageTaken& InDamageTaken);

protected:
	virtual void BeginPlay() override;
	void DoRequestNextWaypoint();

	float Health;
	float SpeedTilePerSecond;
	FVector CurrentWaypoint;
	FVector Direction;
	FRequestNextWaypoint RequestNextWaypoint;
	FDamageTaken DamageTaken;
};
