// Fill out your copyright notice in the Description page of Project Settings.

#include "Enemy.h"

#include "Components/StaticMeshComponent.h"
#include "Engine/StaticMesh.h"
#include "Math/UnrealMathUtility.h"

#include "EnemyData.h"

AEnemy::AEnemy()
	: Direction(FVector::ZeroVector)
{
	static auto DataAsset = ConstructorHelpers::FObjectFinder<UEnemyData>(
			TEXT("EnemyData'/Game/Enemy/EnemyData.EnemyData'"));
	static auto MeshAsset = ConstructorHelpers::FObjectFinder<UStaticMesh>(
			TEXT("StaticMesh'/Game/Enemy/Meshes/enemy_ufoPurple.enemy_ufoPurple'"));

	PrimaryActorTick.bCanEverTick = true;
	UStaticMeshComponent* Mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
	Mesh->SetStaticMesh(MeshAsset.Object);
	Mesh->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
	Mesh->SetCollisionProfileName(FName(TEXT("OverlapAllDynamic")));
	RootComponent = Mesh;
	Health = DataAsset.Object->Health;
	SpeedTilePerSecond = DataAsset.Object->SpeedTilePerSecond * 100.0f;
}

void AEnemy::BeginPlay()
{
	Super::BeginPlay();
	CurrentWaypoint = GetActorLocation();
	DoRequestNextWaypoint();
}

void AEnemy::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	FVector Location = GetActorLocation();
	FVector NewLocation = Location + Direction * SpeedTilePerSecond * DeltaTime;
	NewLocation = FMath::ClosestPointOnSegment(NewLocation, Location, CurrentWaypoint);
	SetActorLocation(NewLocation);
	if (NewLocation.Equals(CurrentWaypoint)) {
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
	DamageTaken.ExecuteIfBound();
	return DamageAmount;
}

FVector AEnemy::GetTargetLocation() const
{
	return GetActorLocation();
}

void AEnemy::SetDelegates(FRequestNextWaypoint& InRequestNextWaypoint, FDamageTaken& InDamageTaken)
{
	RequestNextWaypoint = InRequestNextWaypoint;
	DamageTaken = InDamageTaken;
}

void AEnemy::DoRequestNextWaypoint()
{
	if (RequestNextWaypoint.IsBound()) {
		FVector NextWaypoint;
		bool IsLastWaypointReached = RequestNextWaypoint.Execute(Cast<UObject>(this), CurrentWaypoint, NextWaypoint);
		if (IsLastWaypointReached) {
			Destroy();
			return;
		}
		CurrentWaypoint = NextWaypoint;
		Direction = (CurrentWaypoint - GetActorLocation()).GetUnsafeNormal();
	}
}
