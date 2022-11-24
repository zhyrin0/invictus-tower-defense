// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "UObject/ScriptInterface.h"
#include "../Enemy/TargetableMixin.h"
#include "TowerManager.generated.h"

class ATowerActor;

UCLASS()
class TOWERDEFENSE_API ATowerManager : public AActor
{
	GENERATED_BODY()

public:
	ATowerManager();

	virtual void Tick(float DeltaTime) override;
	void Spawn(FVector Location);
	UFUNCTION()
	void OnTargetSpawned(TScriptInterface<ITargetableMixin> Target);
	UFUNCTION()
	void OnTargetDestroyed(TScriptInterface<ITargetableMixin> Target);

protected:
	using FTargetLocationMap = TMap<TScriptInterface<ITargetableMixin>, FVector>;

	void SelectTargets() const;
	FTargetLocationMap GetTargetMap() const;
	TScriptInterface<ITargetableMixin> GetNearestTarget(FVector TowerLocation, const FTargetLocationMap& TargetMap) const;


	UPROPERTY(EditAnywhere)
	float TargetingFequency;
	UPROPERTY(VisibleAnywhere)
	float TargetingDelta;
	UPROPERTY(EditAnywhere)
	int32 ZOffset;
	UPROPERTY(VisibleAnywhere)
	TArray<ATowerActor*> Towers;
	UPROPERTY(VisibleAnywhere)
	TArray<TScriptInterface<ITargetableMixin>> Targets;
};
