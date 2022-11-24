// Fill out your copyright notice in the Description page of Project Settings.

#include "Projectile.h"
#include "Engine/StaticMesh.h"
#include "Materials/Material.h"
#include "Math/UnrealMathUtility.h"
#include "UObject/ConstructorHelpers.h"

AProjectile::AProjectile()
	: TravelTime(1.0f), TravelDelta(0.0f)
{
	PrimaryActorTick.bCanEverTick = true;
	RootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));
	Mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
	auto MeshAsset = ConstructorHelpers::FObjectFinder<UStaticMesh>(TEXT("StaticMesh'/Engine/BasicShapes/Sphere.Sphere'"));
	auto MaterialAsset = ConstructorHelpers::FObjectFinder<UMaterial>(TEXT("Material'/Engine/BasicShapes/BasicShapeMaterial.BasicShapeMaterial'"));
	MeshAsset.Object->SetMaterial(0, MaterialAsset.Object);
	Mesh->SetStaticMesh(MeshAsset.Object);
	Mesh->SetRelativeScale3D(FVector(0.25));
	Mesh->SetupAttachment(RootComponent);
}

void AProjectile::Initialize(TScriptInterface<ITargetableMixin> pTarget)
{
	SpawnLocation = GetActorLocation();
	Target = Target;
}

void AProjectile::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	TravelDelta += DeltaTime;
	if (!Target || TravelDelta > TravelTime) {
		Destroy();
		return;
	}
	float Alpha = TravelDelta / TravelTime;
	SetActorLocation(FMath::Lerp(SpawnLocation, Target->GetTargetLocation(), Alpha));
}
