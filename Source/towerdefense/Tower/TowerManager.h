// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "UObject/ScriptInterface.h"

#include "../Enemy/TargetableMixin.h"

#include "TowerManager.generated.h"

class ATower;

UCLASS()
class TOWERDEFENSE_API ATowerManager : public AActor
{
	GENERATED_BODY()

public:
	ATowerManager();

	virtual void Tick(float DeltaTime) override;
	void BindDelegates(ITargetableMixin::FSpawned& InTargetSpawned,
			ITargetableMixin::FDestroyed& InTargetDestroyed);
	void ClearLevel();
	void Spawn(FVector Location);

protected:
	using FTargetLocationMap = TMap<TScriptInterface<ITargetableMixin>, FVector>;

	void SelectTargets() const;
	FTargetLocationMap GetTargetMap() const;
	TScriptInterface<ITargetableMixin> GetNearestTarget(FVector TowerLocation, const FTargetLocationMap& TargetMap) const;

	UFUNCTION()
	void OnTargetSpawned(TScriptInterface<ITargetableMixin> Target);
	UFUNCTION()
	void OnTargetDestroyed(TScriptInterface<ITargetableMixin> Target);


	UPROPERTY(EditAnywhere)
	float TargetingFrequency;
	float TargetingTimeout;
	float TargetingDelta;
	int32 ZOffset;
	TArray<ATower*> Towers;
	TArray<TScriptInterface<ITargetableMixin>> Targets;
};
