// Fill out your copyright notice in the Description page of Project Settings.


#include "TowerActor.h"
#include "Engine/StaticMesh.h"
#include "Projectile/Projectile.h"
#include "../Enemy/TargetableMixin.h"

FTowerComponentConstructionData::FTowerComponentConstructionData(
	UStaticMeshComponent* pComponent, USceneComponent* pParent, float pZOffset, FString pAssetPath)
	: Component(pComponent), Parent(pParent), ZOffset(pZOffset), AssetPath(pAssetPath)
{
}

ATowerActor::ATowerActor()
	: RotationDegreesPerSecond(30.0f)
{
	PrimaryActorTick.bCanEverTick = true;
	RootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));
	Base = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Base"));
	Middle = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Middle"));
	Top = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Top"));
	CannonBase = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("CannonBase"));
	Cannon = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Cannon"));
	ProjectileSpawn = CreateDefaultSubobject<USceneComponent>(TEXT("ProjectileSpawn"));
	ConstructComponents();
}

void ATowerActor::BeginPlay()
{
	Super::BeginPlay();
	AttackTimeout.BindUObject(this, &ATowerActor::OnAttackTimeout);
	FTimerManager& TimerManager = GetWorldTimerManager();
	TimerManager.SetTimer(AttackTimer, AttackTimeout, 1.5f, true);
}

void ATowerActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	if (Target) {
		FVector CannonLocation = CannonBase->GetComponentLocation();
		FVector TargetLocation = Target->GetTargetLocation();
		TargetLocation.Z = CannonLocation.Z;
		FRotator NewRotation((TargetLocation - CannonLocation).Rotation());
		NewRotation.Yaw -= 90.0f;
		CannonBase->SetWorldRotation(NewRotation);
	} else {
		CannonBase->AddRelativeRotation(FRotator(0.0f, RotationDegreesPerSecond * DeltaTime, 0.0f));
	}
}

void ATowerActor::SetTarget(TScriptInterface<ITargetableMixin> NewTarget)
{
	Target = NewTarget;
}

void ATowerActor::OnAttackTimeout()
{
	if (Target) {
		AProjectile* Projectile = GetWorld()->SpawnActor<AProjectile>(
				ProjectileSpawn->GetComponentLocation(), FRotator::ZeroRotator);
		Projectile->Initialize(Target);
	}
}

void ATowerActor::ConstructComponents()
{
	TArray<FTowerComponentConstructionData> BuildData;
	BuildData.Emplace(Base, RootComponent, 0.0f,
			TEXT("StaticMesh'/Game/Tower/Meshes/towerRound_bottomC.towerRound_bottomC'"));
	BuildData.Emplace(Middle, RootComponent, 50.0f,
			TEXT("StaticMesh'/Game/Tower/Meshes/towerRound_middleA.towerRound_middleA'"));
	BuildData.Emplace(Top, RootComponent, 100.0f,
			TEXT("StaticMesh'/Game/Tower/Meshes/towerRound_topB.towerRound_topB'"));
	BuildData.Emplace(CannonBase, RootComponent, 130.0f,
			TEXT("StaticMesh'/Game/Tower/Meshes/weapon_cannon_weapon_cannon.weapon_cannon_weapon_cannon'"));
	BuildData.Emplace(Cannon, CannonBase, 0.0f,
			TEXT("StaticMesh'/Game/Tower/Meshes/weapon_cannon_cannon.weapon_cannon_cannon'"));

	using MeshAsset = ConstructorHelpers::FObjectFinder<UStaticMesh>;
	for (auto& Data : BuildData) {
		MeshAsset Asset(*(Data.AssetPath));
		Data.Component->SetStaticMesh(Asset.Object);
		Data.Component->SetupAttachment(Data.Parent);
		Data.Component->SetRelativeLocation(FVector(0.0f, 0.0f, Data.ZOffset));
	}
	ProjectileSpawn->SetupAttachment(Cannon);
	ProjectileSpawn->AddLocalOffset(FVector(0.0f, 50.0f, 10.0f));
}
