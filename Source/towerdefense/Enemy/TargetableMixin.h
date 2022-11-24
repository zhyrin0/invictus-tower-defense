// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "UObject/ScriptInterface.h"
#include "TargetableMixin.generated.h"

UINTERFACE(MinimalAPI)
class UTargetableMixin : public UInterface
{
	GENERATED_BODY()
};

class TOWERDEFENSE_API ITargetableMixin
{
	GENERATED_BODY()

public:
	// todo: The plan was to have Towers notified when their target is destroyed to retarget,
	// but the functionality has been moved to TowerManager. Can be removed.
	DECLARE_MULTICAST_DELEGATE_OneParam(FTargetDestroyed, TScriptInterface<ITargetableMixin>)

	virtual FVector GetTargetLocation() const = 0;

	FTargetDestroyed TargetDestroyed;
};
