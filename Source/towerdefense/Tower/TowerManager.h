// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "TowerManager.generated.h"

class ATowerActor;
class ITargetableMixin;

UCLASS()
class TOWERDEFENSE_API ATowerManager : public AActor
{
	GENERATED_BODY()

public:
	ATowerManager();

	virtual void Tick(float DeltaTime) override;
	void Spawn(FVector Location);

protected:
	using FTargetLocationMap = TMap<ITargetableMixin*, FVector>;

	void SelectTargets() const;
	FTargetLocationMap GetTargets() const;
	UObject* GetTarget(FVector TowerLocation, const FTargetLocationMap& Targets) const;


	UPROPERTY(EditAnywhere)
	float TargetingFequency;
	UPROPERTY(VisibleAnywhere)
	float TargetingDelta;
	UPROPERTY(EditAnywhere)
	int32 ZOffset;
	UPROPERTY(VisibleAnywhere)
	TArray<ATowerActor*> Towers;
};
