// Fill out your copyright notice in the Description page of Project Settings.


#include "Tower.h"

#include "Components/SceneComponent.h"
#include "Components/StaticMeshComponent.h"
#include "Engine/EngineTypes.h"
#include "Engine/StaticMesh.h"

#include "../Grid2D.h"
#include "../Enemy/TargetableMixin.h"
#include "Projectile/Projectile.h"
#include "TowerData.h"

ATower::ATower()
{
	static auto DataAsset = ConstructorHelpers::FObjectFinder<UTowerData>(
			TEXT("TowerData'/Game/Tower/TowerData.TowerData'"));
	static auto BaseAsset = ConstructorHelpers::FObjectFinder<UStaticMesh>(
			TEXT("StaticMesh'/Game/Tower/Meshes/towerRound_bottomC.towerRound_bottomC'"));
	static auto MiddleAsset = ConstructorHelpers::FObjectFinder<UStaticMesh>(
			TEXT("StaticMesh'/Game/Tower/Meshes/towerRound_middleA.towerRound_middleA'"));
	static auto TopAsset = ConstructorHelpers::FObjectFinder<UStaticMesh>(
			TEXT("StaticMesh'/Game/Tower/Meshes/towerRound_topB.towerRound_topB'"));
	static auto CannonBaseAsset = ConstructorHelpers::FObjectFinder<UStaticMesh>(
			TEXT("StaticMesh'/Game/Tower/Meshes/weapon_cannon_weapon_cannon.weapon_cannon_weapon_cannon'"));
	static auto CannonAsset = ConstructorHelpers::FObjectFinder<UStaticMesh>(
			TEXT("StaticMesh'/Game/Tower/Meshes/weapon_cannon_cannon.weapon_cannon_cannon'"));

	UStaticMeshComponent* Base = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Base"));
	UStaticMeshComponent* Middle = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Middle"));
	UStaticMeshComponent* Top = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Top"));
	CannonBase = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("CannonBase"));
	UStaticMeshComponent* Cannon = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Cannon"));
	ProjectileSpawn = CreateDefaultSubobject<USceneComponent>(TEXT("ProjectileSpawn"));

	Base->SetStaticMesh(BaseAsset.Object);
	Middle->SetStaticMesh(MiddleAsset.Object);
	Top->SetStaticMesh(TopAsset.Object);
	CannonBase->SetStaticMesh(CannonBaseAsset.Object);
	Cannon->SetStaticMesh(CannonAsset.Object);

	RootComponent = Base;
	Middle->SetupAttachment(Base);
	Top->SetupAttachment(Base);
	CannonBase->SetupAttachment(Base);
	Cannon->SetupAttachment(CannonBase);
	ProjectileSpawn->SetupAttachment(Cannon);

	Middle->SetRelativeLocation(FGrid2D::TowerMiddleOffset());
	Top->SetRelativeLocation(FGrid2D::TowerTopOffset());
	CannonBase->SetRelativeLocation(FGrid2D::TowerCannonBaseOffset());
	ProjectileSpawn->AddLocalOffset(FVector(0.0f, 50.0f, 10.0f));

	PrimaryActorTick.bCanEverTick = true;
	RotationDegreesPerSecond = DataAsset.Object->RotationDegreesPerSecond;
	AttackRate = 1.0 / DataAsset.Object->AttacksPerSecond;
	AttackTimeout.BindUObject(this, &ATower::OnAttackTimeout);
}

void ATower::BeginPlay()
{
	Super::BeginPlay();
	GetWorldTimerManager().SetTimer(AttackTimer, AttackTimeout, AttackRate, true);
}

void ATower::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	if (Target) {
		FVector CannonLocation = CannonBase->GetComponentLocation();
		FVector TargetLocation = Target->GetTargetLocation();
		TargetLocation.Z = CannonLocation.Z;
		FRotator NewRotation((TargetLocation - CannonLocation).Rotation());
		NewRotation.Yaw += ROTATION_YAW_OFFSET;
		CannonBase->SetWorldRotation(NewRotation);
	} else {
		CannonBase->AddRelativeRotation(FRotator(0.0f, RotationDegreesPerSecond * DeltaTime, 0.0f));
	}
}

void ATower::SetTarget(TScriptInterface<ITargetableMixin> NewTarget)
{
	Target = NewTarget;
}

void ATower::SetDelegate(FAttacked& InAttacked)
{
	Attacked = InAttacked;
}

void ATower::OnAttackTimeout()
{
	if (Target) {
		AProjectile* Projectile = GetWorld()->SpawnActor<AProjectile>(
				ProjectileSpawn->GetComponentLocation(), FRotator::ZeroRotator);
		Projectile->Initialize(Target);
		Attacked.ExecuteIfBound();
	}
}
