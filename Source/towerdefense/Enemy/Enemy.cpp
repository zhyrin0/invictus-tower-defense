// Fill out your copyright notice in the Description page of Project Settings.

#include "Enemy.h"
#include "Engine/StaticMesh.h"

AEnemy::AEnemy()
{
	PrimaryActorTick.bCanEverTick = true;
	RootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));
	Mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("MeshComponent"));
	auto MeshAsset = ConstructorHelpers::FObjectFinder<UStaticMesh>(
			TEXT("StaticMesh'/Game/Enemy/Meshes/enemy_ufoPurple.enemy_ufoPurple'"));
	Mesh->SetStaticMesh(MeshAsset.Object);
	Mesh->SetupAttachment(RootComponent);
}

void AEnemy::BeginPlay()
{
	Super::BeginPlay();
}

void AEnemy::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}
