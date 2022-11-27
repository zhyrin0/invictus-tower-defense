// Fill out your copyright notice in the Description page of Project Settings.

#include "Projectile.h"

#include "Components/SphereComponent.h"
#include "Components/StaticMeshComponent.h"
#include "Engine/StaticMesh.h"
#include "Kismet/GameplayStatics.h"
#include "Materials/Material.h"
#include "Math/UnrealMathUtility.h"
#include "UObject/ConstructorHelpers.h"

#include "../../Enemy/TargetableMixin.h"
#include "../TowerData.h"

AProjectile::AProjectile()
	: TravelDelta(0.0f)
{
	static auto DataAsset = ConstructorHelpers::FObjectFinder<UTowerData>(
			TEXT("TowerData'/Game/Tower/TowerData.TowerData'"));
	static auto MeshAsset = ConstructorHelpers::FObjectFinder<UStaticMesh>(
			TEXT("StaticMesh'/Engine/BasicShapes/Sphere.Sphere'"));

	PrimaryActorTick.bCanEverTick = true;
	USphereComponent* Sphere = CreateDefaultSubobject<USphereComponent>(TEXT("Sphere"));
	UStaticMeshComponent* Mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));

	Sphere->SetSphereRadius(12.5f);
	Sphere->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
	Sphere->SetCollisionProfileName(FName(TEXT("OverlapAllDynamic")));
	Sphere->SetGenerateOverlapEvents(true);
	Sphere->OnComponentBeginOverlap.AddDynamic(this, &AProjectile::OnBeginOverlap);
	Mesh->SetStaticMesh(MeshAsset.Object);
	Mesh->SetRelativeScale3D(FVector(0.25f));

	RootComponent = Sphere;
	Mesh->SetupAttachment(RootComponent);

	TravelTime = DataAsset.Object->ProjectileData.TravelTime;
	Damage = DataAsset.Object->ProjectileData.Damage;

	//auto MaterialAsset = ConstructorHelpers::FObjectFinder<UMaterial>(TEXT("Material'/Engine/BasicShapes/BasicShapeMaterial.BasicShapeMaterial'"));
	//MeshAsset.Object->SetMaterial(0, MaterialAsset.Object);
}

void AProjectile::Initialize(TScriptInterface<ITargetableMixin> InTarget)
{
	SpawnLocation = GetActorLocation();
	Target = InTarget;
	InTarget->TargetDestroyed.AddUObject(this, &AProjectile::OnTargetDestroyed);
}

void AProjectile::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	TravelDelta += DeltaTime;
	if (TravelDelta > TravelTime) {
		Destroy();
		return;
	}
	float Alpha = TravelDelta / TravelTime;
	SetActorLocation(FMath::Lerp(SpawnLocation, Target->GetTargetLocation(), Alpha));
}

void AProjectile::OnBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
		UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (OtherActor == Target.GetObject()) {
		UGameplayStatics::ApplyDamage(OtherActor, Damage, nullptr, nullptr, UDamageType::StaticClass());
		Destroy();
	}
}

void AProjectile::OnTargetDestroyed(TScriptInterface<ITargetableMixin> InTarget)
{
	Destroy();
}
