// Fill out your copyright notice in the Description page of Project Settings.

#include "TowerManager.h"
#include "TowerActor.h"

ATowerManager::ATowerManager()
	: ZOffset(20)
{
	PrimaryActorTick.bCanEverTick = false;
}

void ATowerManager::Spawn(FVector Location) const
{
	Location.Z += ZOffset;
	GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Yellow,
			FString::Printf(TEXT("Tower placed at %.0fx%.0f."), Location.X / 100.0f, Location.Y / 100.0f));
	GetWorld()->SpawnActor<ATowerActor>(Location, FRotator());
}
