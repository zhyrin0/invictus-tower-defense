// Fill out your copyright notice in the Description page of Project Settings.

#include "TileActor.h"
#include "Components/StaticMeshComponent.h"
#include "../../Tower/Tower.h"

ATileActor::ATileActor()
{
	PrimaryActorTick.bCanEverTick = true;
	RootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));
}

void ATileActor::Initialize(TArray<UStaticMesh*> Meshes)
{
	for (UStaticMesh* Mesh : Meshes) {
		UStaticMeshComponent* Component = NewObject<UStaticMeshComponent>(this);
		Component->SetupAttachment(RootComponent);
		Component->SetStaticMesh(Mesh);
		Component->RegisterComponent();
	}
}

void ATileActor::NotifyActorOnClicked(FKey ButtonPressed)
{
	Super::NotifyActorOnClicked(ButtonPressed);
	if (CanPlaceTower()) {
		SpawnTowerRequest.Execute(GetActorLocation());
		SpawnTowerRequest.Unbind();
	} else {
		GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Yellow, TEXT("Nothing happens."));
	}
}

bool ATileActor::CanPlaceTower() const
{
	return SpawnTowerRequest.IsBound();
}
