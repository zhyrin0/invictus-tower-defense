// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"

#include "Camera.generated.h"

class UCameraComponent;
class USceneComponent;

UCLASS()
class TOWERDEFENSE_API ACamera : public APawn
{
	GENERATED_BODY()

public:
	ACamera();

	virtual void Tick(float DeltaTime) override;
	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	void SetStartingCameraOffset(float Offset);

protected:
	UFUNCTION()
	void OnZoomIn();
	UFUNCTION()
	void OnZoomOut();
	UFUNCTION()
	void OnVerticalAxis(float AxisValue);
	UFUNCTION()
	void OnHorizontalAxis(float AxisValue);
	UFUNCTION()
	void OnRotation(float AxisValue);

	static constexpr float PITCH_ROTATION_MAX = 75.0f;
	static constexpr float PITCH_ROTATION_MIN = 40.0f;
	static constexpr float PITCH_ROTATION_STEP = 2.5f;
	static constexpr float YAW_ROTATION_DEGREES_PER_SECOND = 45.0f;
	static constexpr float DISTANCE_STEP = 50.0f;
	FVector2D MovementDirection;
	float MovementSpeed;
	float GimballRotation;
	float YawRotation;
	UPROPERTY()
	USceneComponent* Gimball;
	UPROPERTY()
	UCameraComponent* Camera;
};
