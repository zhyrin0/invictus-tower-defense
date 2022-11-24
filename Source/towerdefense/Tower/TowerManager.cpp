// Fill out your copyright notice in the Description page of Project Settings.

#include "TowerManager.h"
#include "Kismet/GameplayStatics.h"
#include "Math/NumericLimits.h"
#include "UObject/ScriptInterface.h"
#include "../Enemy/TargetableMixin.h"
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

void ATowerManager::Spawn(FVector Location)
{
	Location.Z += ZOffset;
	GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Yellow,
			FString::Printf(TEXT("Tower placed at %.0fx%.0f."), Location.X / 100.0f, Location.Y / 100.0f));
	Towers.Add(GetWorld()->SpawnActor<ATowerActor>(Location, FRotator()));
}

void ATowerManager::SelectTargets() const
{
	if (Towers.IsEmpty()) {
		return;
	}
	FTargetLocationMap Targets = GetTargets();
	for (ATowerActor* Tower : Towers) {
		TScriptInterface<ITargetableMixin> Target(GetTarget(Tower->GetActorLocation(), Targets));
		Tower->SetTarget(Target);
	}
}

ATowerManager::FTargetLocationMap ATowerManager::GetTargets() const
{
	FTargetLocationMap Result;
	TArray<AActor*> GenericTargets;
	UGameplayStatics::GetAllActorsWithInterface(GetWorld(), UTargetableMixin::StaticClass(), GenericTargets);
	for (AActor* Generic : GenericTargets) {
		ITargetableMixin* Target = Cast<ITargetableMixin>(Generic);
		Result.Add(Target, Target->GetTargetLocation());
	}
	return Result;
}

UObject* ATowerManager::GetTarget(FVector TowerLocation, const FTargetLocationMap& Targets) const
{
	ITargetableMixin* Result = nullptr;
	float SmallestDistance = TNumericLimits<float>::Max();
	for (const auto& Pair : Targets) {
		float CurrentDistance = FVector::DistSquared(TowerLocation, Pair.Value);
		if (CurrentDistance < SmallestDistance) {
			Result = Pair.Key;
			SmallestDistance = CurrentDistance;
		}
	}
	return Cast<UObject>(Result);
}
