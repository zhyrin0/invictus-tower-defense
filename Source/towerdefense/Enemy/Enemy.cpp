// Fill out your copyright notice in the Description page of Project Settings.

#include "Enemy.h"
#include "Engine/StaticMesh.h"

AEnemy::AEnemy()
	: Health(2.0f), Speed(50.0f), Direction(FVector::ZeroVector)
{
	PrimaryActorTick.bCanEverTick = true;
	Mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
	auto MeshAsset = ConstructorHelpers::FObjectFinder<UStaticMesh>(
			TEXT("StaticMesh'/Game/Enemy/Meshes/enemy_ufoPurple.enemy_ufoPurple'"));
	Mesh->SetStaticMesh(MeshAsset.Object);
	RootComponent = Mesh;

	Mesh->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
	Mesh->SetCollisionProfileName(FName(TEXT("OverlapAllDynamic")));
}

void AEnemy::Initialize()
{
	CurrentWaypoint = GetActorLocation();
	DoRequestNextWaypoint();
}

void AEnemy::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	AddActorLocalOffset(Direction * Speed * DeltaTime);
	if (GetActorLocation().Equals(CurrentWaypoint, 0.5)) {
		DoRequestNextWaypoint();
	}
}

float AEnemy::TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent,
						 AController* EventInstigator, AActor* DamageCauser)
{
	Health -= DamageAmount;
	if (Health < 1.0f) {
		TargetDestroyed.Broadcast(Cast<UObject>(this));
		Destroy();
	}
	return DamageAmount;
}

FVector AEnemy::GetTargetLocation() const
{
	return GetActorLocation();
}

void AEnemy::DoRequestNextWaypoint()
{
	if (RequestNextWaypoint.IsBound()) {
		FVector NextWaypoint;
		bool IsLastWaypointReached = RequestNextWaypoint.Execute(CurrentWaypoint, NextWaypoint);
		if (IsLastWaypointReached) {
			LastWaypointReached.ExecuteIfBound(Cast<UObject>(this));
			Destroy();
			return;
		}
		CurrentWaypoint = NextWaypoint;
		Direction = (CurrentWaypoint - GetActorLocation()).GetUnsafeNormal();
	}
}
