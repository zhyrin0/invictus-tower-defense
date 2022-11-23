// Fill out your copyright notice in the Description page of Project Settings.

#include "TowerDefenseGameState.h"
#include "Level/LevelDescriptor.h"
#include "Tower/SpawnTowerRequestMixin.h"

void ATowerDefenseGameState::OnConstruction(const FTransform& Transform)
{
	Super::OnConstruction(Transform);
	UWorld* World = GetWorld();
	LevelBuilder = World->SpawnActor<ALevelBuilderActor>();
	LevelBuilder->BeganPlay.BindUObject(this, &ATowerDefenseGameState::BeginLevel);
	EnemyManager = World->SpawnActor<AEnemyManager>();
	TowerSpawner = World->SpawnActor<ATowerSpawner>();
}

void ATowerDefenseGameState::BeginLevel(FString LevelName) const
{
	FString Reference = FString::Printf(TEXT("LevelDescriptor'/Game/Level/%s.%s'"), *LevelName, *LevelName);
	ULevelDescriptor* Descriptor = LoadObject<ULevelDescriptor>(NULL, *Reference, NULL, LOAD_None, NULL);
	if (Descriptor == nullptr) {
		GEngine->AddOnScreenDebugMessage(-1, 10.0f, FColor::Red, TEXT("Couldn't load level."));
		return;
	}
	FSpawnTowerRequestList Requests = LevelBuilder->BuildLevel(Descriptor->Width, Descriptor->Height, Descriptor->Tiles);
	for (auto Request : Requests) {
		Request->BindUObject(TowerSpawner, &ATowerSpawner::Spawn);
	}
	EnemyManager->BeginLevel(Descriptor->Waypoints, Descriptor->EnemyCount,
			Descriptor->EnemySpawnDelay, Descriptor->EnemySpawnCooldown);
}
