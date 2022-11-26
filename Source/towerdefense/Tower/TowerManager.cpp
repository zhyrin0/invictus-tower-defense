// Fill out your copyright notice in the Description page of Project Settings.

#include "TowerManager.h"
#include "Kismet/GameplayStatics.h"
#include "Math/NumericLimits.h"
#include "TowerActor.h"

ATowerManager::ATowerManager()
	: TargetingFequency(10.0f), TargetingDelta(0.0f), ZOffset(20)
{
	PrimaryActorTick.bCanEverTick = true;
}

void ATowerManager::Tick(float DeltaTime)
{
	TargetingDelta += DeltaTime;
	float TargetingTimeout = 1.0f / TargetingFequency;
	if (TargetingDelta >= TargetingTimeout) {
		SelectTargets();
		TargetingDelta -= TargetingTimeout;
	}
}

void ATowerManager::BindDelegates(ITargetableMixin::FSpawned& InTargetSpawned,
		ITargetableMixin::FDestroyed& InTargetDestroyed)
{
	InTargetSpawned.BindUObject(this, &ATowerManager::OnTargetSpawned);
	InTargetDestroyed.AddUObject(this, &ATowerManager::OnTargetDestroyed);
}

void ATowerManager::Spawn(FVector Location)
{
	Location.Z += ZOffset;
	GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Yellow,
			FString::Printf(TEXT("Tower placed at %.0fx%.0f."), Location.X / 100.0f, Location.Y / 100.0f));
	Towers.Add(GetWorld()->SpawnActor<ATowerActor>(Location, FRotator()));
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
		for (ATowerActor* Tower : Towers) {
			Tower->SetTarget(InvalidTarget);
		}
		return;
	}
	FTargetLocationMap TargetMap = GetTargetMap();
	for (ATowerActor* Tower : Towers) {
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
	float SmallestDistance = TNumericLimits<float>::Max();
	for (const auto& Pair : TargetMap) {
		float CurrentDistance = FVector::DistSquared(TowerLocation, Pair.Value);
		if (CurrentDistance < SmallestDistance) {
			Result = Pair.Key;
			SmallestDistance = CurrentDistance;
		}
	}
	return Result;
}
