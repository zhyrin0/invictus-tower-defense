// Fill out your copyright notice in the Description page of Project Settings.

#include "Enemy.h"
#include "Engine/StaticMesh.h"

AEnemy::AEnemy()
	: Speed(50.0f), Direction(FVector::ZeroVector)
{
	PrimaryActorTick.bCanEverTick = true;
	RootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));
	Mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("MeshComponent"));
	auto MeshAsset = ConstructorHelpers::FObjectFinder<UStaticMesh>(
			TEXT("StaticMesh'/Game/Enemy/Meshes/enemy_ufoPurple.enemy_ufoPurple'"));
	Mesh->SetStaticMesh(MeshAsset.Object);
	Mesh->SetupAttachment(RootComponent);
}

void AEnemy::Initialize(FVector Spawnpoint)
{
	if (RequestNextWaypoint.IsBound()) {
		SetCurrentWaypoint(RequestNextWaypoint.Execute(Spawnpoint));
	}
}

void AEnemy::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	AddActorLocalOffset(Direction * Speed * DeltaTime);
	if (GetActorLocation().Equals(CurrentWaypoint, 0.5)) {
		if (RequestNextWaypoint.IsBound()) {
			SetCurrentWaypoint(RequestNextWaypoint.Execute(CurrentWaypoint));
		}
	}
}

void AEnemy::SetCurrentWaypoint(FVector NewWaypoint)
{
	CurrentWaypoint = NewWaypoint;
	Direction = (CurrentWaypoint - GetActorLocation()).GetUnsafeNormal();
}
