// Fill out your copyright notice in the Description page of Project Settings.

#include "Projectile.h"
#include "Engine/StaticMesh.h"
#include "Kismet/GameplayStatics.h"
#include "Materials/Material.h"
#include "Math/UnrealMathUtility.h"
#include "UObject/ConstructorHelpers.h"

AProjectile::AProjectile()
	: TravelTime(1.0f), TravelDelta(0.0f)
{
	PrimaryActorTick.bCanEverTick = true;
	Sphere = CreateDefaultSubobject<USphereComponent>(TEXT("Sphere"));
	Sphere->SetSphereRadius(12.5f);
	RootComponent = Sphere;
	Mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
	auto MeshAsset = ConstructorHelpers::FObjectFinder<UStaticMesh>(TEXT("StaticMesh'/Engine/BasicShapes/Sphere.Sphere'"));
	auto MaterialAsset = ConstructorHelpers::FObjectFinder<UMaterial>(TEXT("Material'/Engine/BasicShapes/BasicShapeMaterial.BasicShapeMaterial'"));
	MeshAsset.Object->SetMaterial(0, MaterialAsset.Object);
	Mesh->SetStaticMesh(MeshAsset.Object);
	Mesh->SetRelativeScale3D(FVector(0.25f));
	Mesh->SetupAttachment(RootComponent);

	Sphere->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
	Sphere->SetCollisionProfileName(FName(TEXT("OverlapAllDynamic")));
	Sphere->SetGenerateOverlapEvents(true);
	Sphere->OnComponentBeginOverlap.AddDynamic(this, &AProjectile::OnBeginOverlap);
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
		UGameplayStatics::ApplyDamage(OtherActor, 1.0f, nullptr, nullptr, UDamageType::StaticClass());
		Destroy();
	}
}

void AProjectile::OnTargetDestroyed(TScriptInterface<ITargetableMixin> InTarget)
{
	Destroy();
}
