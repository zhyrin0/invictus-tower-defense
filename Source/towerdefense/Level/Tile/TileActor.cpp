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
		SpawnTowerRequest.ExecuteIfBound(GetActorLocation());
		CanPlaceTower = false;
	} else {
		GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Yellow, TEXT("Nothing happens."));
	}
}
