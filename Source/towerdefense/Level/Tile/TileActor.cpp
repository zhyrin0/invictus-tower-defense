// Fill out your copyright notice in the Description page of Project Settings.

#include "TileActor.h"
#include "Components/StaticMeshComponent.h"
#include "../../Tower/TowerActor.h"

ATileActor::ATileActor()
	: CanPlaceTower(false)
{
	PrimaryActorTick.bCanEverTick = true;
	RootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));
}

void ATileActor::Initialize(TArray<UStaticMesh*> Meshes, bool IsEmpty)
{
	CanPlaceTower = IsEmpty;
	for (UStaticMesh* Mesh : Meshes) {
		UStaticMeshComponent* Component = NewObject<UStaticMeshComponent>(this);
		Component->SetupAttachment(RootComponent);
		Component->SetStaticMesh(Mesh);
		Component->RegisterComponent();
	}
}

void ATileActor::BeginPlay()
{
	Super::BeginPlay();
}

void ATileActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void ATileActor::NotifyActorOnClicked(FKey ButtonPressed)
{
	Super::NotifyActorOnClicked(ButtonPressed);
	if (CanPlaceTower) {
		FVector Location = GetActorLocation() / 100.0f;
		GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Yellow,
				FString::Printf(TEXT("Tower placed at %.0fx%.0f"), Location.X, Location.Y));
		FVector SpawnLocation = GetActorLocation();
		SpawnLocation.Z += 20.0f;
		ATowerActor* Tower = GetWorld()->SpawnActor<ATowerActor>(SpawnLocation, FRotator());
		Tower->RotationDegreesPerSecond = 30.0f;
		CanPlaceTower = false;
	} else {
		GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Yellow, TEXT("Nothing happens."));
	}
}
