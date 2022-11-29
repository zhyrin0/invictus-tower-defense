// Fill out your copyright notice in the Description page of Project Settings.

#include "TowerManager.h"

#include "Components/AudioComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Math/UnrealMathUtility.h"
#include "Sound/SoundWave.h"

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

void ATowerManager::BeginPlay()
{
	Super::BeginPlay();
	FString PlaceAudioReference = TEXT("SoundWave'/Game/Tower/SFX/qubodup-DoorClose03.qubodup-DoorClose03'");
	FString FireAudioReference = TEXT("SoundWave'/Game/Tower/SFX/cannon_fire.cannon_fire'");
	USoundWave* PlaceAudioAsset = LoadObject<USoundWave>(nullptr, *PlaceAudioReference, nullptr, LOAD_None);
	USoundWave* FireAudioAsset = LoadObject<USoundWave>(nullptr, *FireAudioReference, nullptr, LOAD_None);
	PlaceAudio = UGameplayStatics::CreateSound2D(GetWorld(), PlaceAudioAsset, 0.5f, 0.475f, 0.0f, nullptr, false, false);
	FireAudio = UGameplayStatics::CreateSound2D(GetWorld(), FireAudioAsset, 0.5f, 1.0f, 0.0f, nullptr, false, false);
}

void ATowerManager::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
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
	ATower* Tower = GetWorld()->SpawnActor<ATower>(Location, FRotator::ZeroRotator);
	Tower->Attacked.BindUObject(this, &ATowerManager::OnTowerAttacked);
	Towers.Add(Tower);
	PlaceAudio->Play();
}

void ATowerManager::OnTowerAttacked()
{
	FireAudio->SetPitchMultiplier(FMath::FRandRange(0.35f, 0.65f));
	FireAudio->Play();
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
	for (ATower* Tower : Towers) {
		FVector TowerLocation = Tower->GetActorLocation();
		TowerLocation.Z = 0.0f;
		Tower->SetTarget(GetNearestTarget(TowerLocation, GetTargetMap()));
	}
}

ATowerManager::FTargetLocationMap ATowerManager::GetTargetMap() const
{
	FTargetLocationMap Result;
	for (TScriptInterface<ITargetableMixin> Target : Targets) {
		FVector TargetLocation = Target->GetTargetLocation();
		TargetLocation.Z = 0.0f;
		Result.Add(Target, TargetLocation);
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
