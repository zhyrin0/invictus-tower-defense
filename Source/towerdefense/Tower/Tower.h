// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "TimerManager.h"
#include "UObject/ScriptInterface.h"

#include "Tower.generated.h"

struct FTimerHandle;
class ITargetableMixin;
class USceneComponent;
class UStaticMeshComponent;

UCLASS()
class TOWERDEFENSE_API ATower : public AActor
{
	GENERATED_BODY()

public:
	DECLARE_DELEGATE(FAttacked);

	ATower();

	virtual void Tick(float DeltaTime) override;
	void SetTarget(TScriptInterface<ITargetableMixin> NewTarget);

	FAttacked Attacked;

protected:
	virtual void BeginPlay() override;
	UFUNCTION()
	void OnAttackTimeout();

	static constexpr float ROTATION_YAW_OFFSET = -90.0f;

	float RotationDegreesPerSecond;
	TScriptInterface<ITargetableMixin> Target;
	float AttackRate;
	FTimerHandle AttackTimer;
	FTimerDelegate AttackTimeout;

	UPROPERTY()
	UStaticMeshComponent* CannonBase;
	UPROPERTY()
	USceneComponent* ProjectileSpawn;
};
