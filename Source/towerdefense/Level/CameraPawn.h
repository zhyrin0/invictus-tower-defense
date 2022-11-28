// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "CameraPawn.generated.h"

UCLASS()
class TOWERDEFENSE_API ACameraPawn : public APawn
{
	GENERATED_BODY()

public:
	ACameraPawn();

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

protected:
	UFUNCTION()
	void OnZoomIn();
	UFUNCTION()
	void OnZoomOut();

	static constexpr float DEFAULT_DISTANCE = 500.0f;
	static constexpr float DISTANCE_STEP = 50.0f;
	UPROPERTY()
	UCameraComponent* Camera;
};
