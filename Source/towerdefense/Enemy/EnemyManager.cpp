// Fill out your copyright notice in the Description page of Project Settings.

#include "EnemyManager.h"
#include "Enemy.h"

AEnemyManager::AEnemyManager()
{
	PrimaryActorTick.bCanEverTick = false;
}

void AEnemyManager::BeginPlay()
{
	Super::BeginPlay();
}

void AEnemyManager::Spawn()
{
	GetWorld()->SpawnActor<AEnemy>(FVector(0.0f, 0.0f, 100.0f), FRotator());
}
