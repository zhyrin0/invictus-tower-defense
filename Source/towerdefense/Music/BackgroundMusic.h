// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"

#include "BackgroundMusic.generated.h"

class UAudioComponent;

UCLASS()
class TOWERDEFENSE_API ABackgroundMusic : public AActor
{
	GENERATED_BODY()

public:
	ABackgroundMusic();

protected:
	virtual void BeginPlay() override;

	UPROPERTY()
	UAudioComponent* Audio;
};
