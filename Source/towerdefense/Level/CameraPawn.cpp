// Fill out your copyright notice in the Description page of Project Settings.


#include "CameraPawn.h"

#include "Camera/CameraComponent.h"

ACameraPawn::ACameraPawn()
{
	PrimaryActorTick.bCanEverTick = false;

	RootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));
	Camera = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));
	Camera->SetupAttachment(RootComponent);
	Camera->AddLocalRotation(FRotator(-70.0f, 0.0f, 0.0f));
}

// Called to bind functionality to input
void ACameraPawn::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
	if (PlayerInputComponent) {
		PlayerInputComponent->BindAction(FName(TEXT("ZoomIn")), EInputEvent::IE_Pressed, this, &ACameraPawn::OnZoomIn);
		PlayerInputComponent->BindAction(FName(TEXT("ZoomOut")), EInputEvent::IE_Pressed, this, &ACameraPawn::OnZoomOut);
	}
}

void ACameraPawn::SetStartingCameraOffset(float Offset)
{
	Camera->SetRelativeLocation(FVector::ZeroVector);
	Camera->AddLocalOffset(FVector(-1.0f * Offset, 0.0f, 0.0f));
}

void ACameraPawn::OnZoomIn()
{
	Camera->AddLocalOffset(FVector(DISTANCE_STEP, 0.0f, 0.0f));
}

void ACameraPawn::OnZoomOut()
{
	Camera->AddLocalOffset(FVector(-1.0f * DISTANCE_STEP, 0.0f, 0.0f));
}
