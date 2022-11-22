// Fill out your copyright notice in the Description page of Project Settings.


#include "CameraPawn.h"

ACameraPawn::ACameraPawn()
{
 	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	RootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));
	Camera = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));
	Camera->SetupAttachment(RootComponent);
	Camera->AddLocalRotation(FRotator(-45.0f, 0.0f, 0.0f));
	Camera->AddLocalOffset(FVector(CameraDistance * -1.0f, 0.0f, 0.0f));
}

void ACameraPawn::BeginPlay()
{
	Super::BeginPlay();
}

void ACameraPawn::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

// Called to bind functionality to input
void ACameraPawn::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
}
