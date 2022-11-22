// Fill out your copyright notice in the Description page of Project Settings.

#include "TileActor.h"
#include "Components/StaticMeshComponent.h"

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
	FVector Location = GetActorLocation() / 100.0f;
	GEngine->AddOnScreenDebugMessage(-1, 10.0f, FColor::Yellow,
			FString::Printf(TEXT("Clicked %.0fx%.0f"), Location.X, Location.Y));
}
