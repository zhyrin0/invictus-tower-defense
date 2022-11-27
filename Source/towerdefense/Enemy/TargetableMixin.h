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
	DECLARE_DELEGATE_OneParam(FSpawned, TScriptInterface<ITargetableMixin>)
	DECLARE_MULTICAST_DELEGATE_OneParam(FDestroyed, TScriptInterface<ITargetableMixin>)

	virtual FVector GetTargetLocation() const = 0;

	FDestroyed TargetDestroyed;
};
