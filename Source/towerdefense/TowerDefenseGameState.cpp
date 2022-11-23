// Fill out your copyright notice in the Description page of Project Settings.

#include "TowerDefenseGameState.h"
#include "Tower/SpawnTowerRequestMixin.h"

void ATowerDefenseGameState::OnConstruction(const FTransform& Transform)
{
	Super::OnConstruction(Transform);
	UWorld* World = GetWorld();
	LevelBuilder = World->SpawnActor<ALevelBuilderActor>();
	LevelBuilder->BeganPlay.BindUObject(this, &ATowerDefenseGameState::BeginLevel);
	TowerSpawner = World->SpawnActor<ATowerSpawner>();
}

void ATowerDefenseGameState::BeginLevel() const
{
	FSpawnTowerRequestList Requests = LevelBuilder->BuildLevel(TEXT("TestLevel"));
	for (auto Request : Requests) {
		Request->BindUObject(TowerSpawner, &ATowerSpawner::Spawn);
	}
}
