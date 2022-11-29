// Fill out your copyright notice in the Description page of Project Settings.

#include "Tile.h"

#include "Components/StaticMeshComponent.h"

ATile::ATile()
{
	PrimaryActorTick.bCanEverTick = false;
	RootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));
}

void ATile::Initialize(const TArray<UStaticMesh*>& Meshes)
{
	for (UStaticMesh* Mesh : Meshes) {
		UStaticMeshComponent* Component = NewObject<UStaticMeshComponent>(this);
		Component->SetupAttachment(RootComponent);
		Component->SetStaticMesh(Mesh);
		Component->RegisterComponent();
	}
}

void ATile::NotifyActorOnClicked(FKey ButtonPressed)
{
	Super::NotifyActorOnClicked(ButtonPressed);
	if (CanPlaceTower()) {
		SpawnTowerRequest.Execute(GetActorLocation());
		SpawnTowerRequest.Unbind();
	}
}

bool ATile::CanPlaceTower() const
{
	return SpawnTowerRequest.IsBound();
}
