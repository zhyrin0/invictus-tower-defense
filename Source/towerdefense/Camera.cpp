// Fill out your copyright notice in the Description page of Project Settings.

#include "Camera.h"

#include "Camera/CameraComponent.h"
#include "Components/SceneComponent.h"

#include "Grid2D.h"

ACamera::ACamera()
	: MovementDirection(FVector2D::ZeroVector),
	MovementSpeed(FGrid2D::GetTileSize() * 5.0f),
	GimballRotation(PITCH_ROTATION_MAX),
	YawRotation(0.0f)
{
	PrimaryActorTick.bCanEverTick = true;

	RootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));
	Gimball = CreateDefaultSubobject<USceneComponent>(TEXT("Gimball"));
	Camera = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));
	Gimball->SetupAttachment(RootComponent);
	Camera->SetupAttachment(Gimball);
	Gimball->SetRelativeRotation(FRotator(-1.0f * GimballRotation, 0.0f, 0.0f));
}

void ACamera::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	AddActorLocalOffset(FVector(MovementDirection.Y, MovementDirection.X, 0.0f) * MovementSpeed * DeltaTime);
	AddActorWorldRotation(FRotator(0.0f, YawRotation * YAW_ROTATION_DEGREES_PER_SECOND * DeltaTime, 0.0f));
}

// Called to bind functionality to input
void ACamera::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
	if (PlayerInputComponent) {
		PlayerInputComponent->BindAction(FName(TEXT("ZoomIn")), EInputEvent::IE_Pressed, this, &ACamera::OnZoomIn);
		PlayerInputComponent->BindAction(FName(TEXT("ZoomOut")), EInputEvent::IE_Pressed, this, &ACamera::OnZoomOut);
		PlayerInputComponent->BindAxis(FName(TEXT("Vertical")), this, &ACamera::OnVerticalAxis);
		PlayerInputComponent->BindAxis(FName(TEXT("Horizontal")), this, &ACamera::OnHorizontalAxis);
		PlayerInputComponent->BindAxis(FName(TEXT("Rotation")), this, &ACamera::OnRotation);
	}
}

void ACamera::SetStartingCameraOffset(float Offset)
{
	Camera->SetRelativeLocation(FVector::ZeroVector);
	Camera->AddLocalOffset(FVector(-1.0f * Offset, 0.0f, 0.0f));
}

void ACamera::OnZoomIn()
{
	Camera->AddLocalOffset(FVector(DISTANCE_STEP, 0.0f, 0.0f));
	GimballRotation = FMath::Clamp(GimballRotation - PITCH_ROTATION_STEP, PITCH_ROTATION_MIN, PITCH_ROTATION_MAX);
	Gimball->SetRelativeRotation(FRotator(-1.0f * GimballRotation, 0.0f, 0.0f));
}

void ACamera::OnZoomOut()
{
	Camera->AddLocalOffset(FVector(-1.0f * DISTANCE_STEP, 0.0f, 0.0f));
	GimballRotation = FMath::Clamp(GimballRotation + PITCH_ROTATION_STEP, PITCH_ROTATION_MIN, PITCH_ROTATION_MAX);
	Gimball->SetRelativeRotation(FRotator(-1.0f * GimballRotation, 0.0f, 0.0f));
}

void ACamera::OnVerticalAxis(float AxisValue)
{
	MovementDirection.Y = AxisValue;
	MovementDirection.Normalize();
}

void ACamera::OnHorizontalAxis(float AxisValue)
{
	MovementDirection.X = AxisValue;
	MovementDirection.Normalize();
}

void ACamera::OnRotation(float AxisValue)
{
	YawRotation = AxisValue;
}
