// Fill out your copyright notice in the Description page of Project Settings.

#include "TowerManager.h"

#include "Kismet/GameplayStatics.h"

#include "Tower.h"
#include "TowerData.h"

ATowerManager::ATowerManager()
	: TargetingDelta(0.0f), ZOffset(20)
{
	static auto DataAsset = ConstructorHelpers::FObjectFinder<UTowerData>(
			TEXT("TowerData'/Game/Tower/TowerData.TowerData'"));

	PrimaryActorTick.bCanEverTick = true;
	TargetingRange = DataAsset.Object->AttackRangeInTiles * 100.0f;
	TargetingTimeout = 1.0f / DataAsset.Object->TargetingFrequency;
}

void ATowerManager::Tick(float DeltaTime)
{
	TargetingDelta += DeltaTime;
	if (TargetingDelta >= TargetingTimeout) {
		SelectTargets();
		TargetingDelta -= TargetingTimeout;
	}
}

void ATowerManager::BindDelegates(ITargetableMixin::FSpawned& InTargetSpawned,
		ITargetableMixin::FDestroyed& InTargetDestroyed,
		ITargetableMixin::FDestroyed& InTargetRemoved)
{
	InTargetSpawned.BindUObject(this, &ATowerManager::OnTargetSpawned);
	InTargetDestroyed.AddUObject(this, &ATowerManager::OnTargetDestroyed);
	InTargetRemoved.AddUObject(this, &ATowerManager::OnTargetDestroyed);
}

void ATowerManager::ClearLevel()
{
	for (ATower* Tower : Towers) {
		Tower->Destroy();
	}
	Towers.Empty();
	Targets.Empty();
}

void ATowerManager::Spawn(FVector Location)
{
	Location.Z += ZOffset;
	Towers.Add(GetWorld()->SpawnActor<ATower>(Location, FRotator()));
}

void ATowerManager::OnTargetSpawned(TScriptInterface<ITargetableMixin> Target)
{
	Targets.Emplace(Target);
	SelectTargets();
	TargetingDelta = 0.0f;
}

void ATowerManager::OnTargetDestroyed(TScriptInterface<ITargetableMixin> Target)
{
	Targets.Remove(Target);
	SelectTargets();
	TargetingDelta = 0.0f;
}

void ATowerManager::SelectTargets() const
{
	if (Towers.IsEmpty()) {
		return;
	}
	if (Targets.IsEmpty()) {
		TScriptInterface<ITargetableMixin> InvalidTarget;
		for (ATower* Tower : Towers) {
			Tower->SetTarget(InvalidTarget);
		}
		return;
	}
	FTargetLocationMap TargetMap = GetTargetMap();
	for (ATower* Tower : Towers) {
		Tower->SetTarget(GetNearestTarget(Tower->GetActorLocation(), GetTargetMap()));
	}
}

ATowerManager::FTargetLocationMap ATowerManager::GetTargetMap() const
{
	FTargetLocationMap Result;
	for (TScriptInterface<ITargetableMixin> Target : Targets) {
		Result.Add(Target, Target->GetTargetLocation());
	}
	return Result;
}

TScriptInterface<ITargetableMixin> ATowerManager::GetNearestTarget(FVector TowerLocation, const FTargetLocationMap& TargetMap) const
{
	TScriptInterface<ITargetableMixin> Result;
	float SmallestDistance = TargetingRange * TargetingRange;
	for (const auto& Pair : TargetMap) {
		float CurrentDistance = FVector::DistSquared(TowerLocation, Pair.Value);
		if (CurrentDistance < SmallestDistance) {
			Result = Pair.Key;
			SmallestDistance = CurrentDistance;
		}
	}
	return Result;
}
