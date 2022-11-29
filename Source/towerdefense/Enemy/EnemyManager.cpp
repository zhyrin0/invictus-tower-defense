// Fill out your copyright notice in the Description page of Project Settings.

#include "EnemyManager.h"

#include "Components/AudioComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Math/Vector2D.h"
#include "Math/UnrealMathUtility.h"
#include "Sound/SoundWave.h"

#include "Enemy.h"

AEnemyManager::AEnemyManager()
{
	PrimaryActorTick.bCanEverTick = false;
	SpawnTimerTimeout.BindUObject(this, &AEnemyManager::Spawn);
}

void AEnemyManager::BeginPlay()
{
	Super::BeginPlay();
	FString DamageAudioReference = TEXT("SoundWave'/Game/Enemy/SFX/qubodupPunch02.qubodupPunch02'");
	FString DestroyAudioReference = TEXT("SoundWave'/Game/Enemy/SFX/qubodupPunch04.qubodupPunch04'");
	USoundWave* DamageAudioAsset = LoadObject<USoundWave>(nullptr, *DamageAudioReference, nullptr, LOAD_None);
	USoundWave* DestroyAudioAsset = LoadObject<USoundWave>(nullptr, *DestroyAudioReference, nullptr, LOAD_None);
	DamageAudio = UGameplayStatics::CreateSound2D(GetWorld(), DamageAudioAsset, 1.0f, 1.0f, 0.0f, nullptr, false, false);
	DestroyAudio = UGameplayStatics::CreateSound2D(GetWorld(), DestroyAudioAsset, 1.0f, 1.0f, 0.0f, nullptr, false, false);
}

void AEnemyManager::ClearLevel()
{
	TArray<AActor*> Enemies;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), AEnemy::StaticClass(), Enemies);
	for (AActor* Enemy : Enemies) {
		Enemy->Destroy();
	}
}

void AEnemyManager::BeginLevel(const TArray<FVector2D>& InWaypoints,
		int32 EnemyCount, float EnemySpawnDelay, float EnemySpawnCooldown)
{
	FTimerManager& TimerManager = GetWorldTimerManager();
	if (TimerManager.TimerExists(SpawnTimer)) {
		TimerManager.PauseTimer(SpawnTimer);
		TimerManager.ClearTimer(SpawnTimer);
	}
	Waypoints.Empty();
	Waypoints.Reserve(InWaypoints.Num());
	for (FVector2D GridPosition : InWaypoints) {
		Waypoints.Emplace(GridPosition * 100.0f, ZOffset);
	}
	EnemiesToSpawn = EnemyCount;
	EnemiesRemaining = EnemiesToSpawn;
	EnemiesDestroyed = 0;
	SpawnTimer = FTimerHandle();
	TimerManager.SetTimer(SpawnTimer, SpawnTimerTimeout, EnemySpawnCooldown, true, EnemySpawnDelay);
	EnemyCountChanged.Broadcast(EnemiesRemaining, EnemiesDestroyed);
}

void AEnemyManager::SetDelegates(FGameEvents::FEnemyCountChanged& InEnemyCountChanged,
		FGameEvents::FLastWaypointReached& InLastWaypointReached)
{
	EnemyCountChanged = InEnemyCountChanged;
	LastWaypointReached = InLastWaypointReached;
}

ITargetableMixin::FSpawned& AEnemyManager::GetEnemySpawnedDelegate()
{
	return EnemySpawned;
}

ITargetableMixin::FDestroyed& AEnemyManager::GetEnemyDestroyedDelegate()
{
	return EnemyDestroyed;
}

ITargetableMixin::FDestroyed& AEnemyManager::GetEnemyReachedLastWaypoint()
{
	return EnemyReachedLastWaypoint;
}

void AEnemyManager::Spawn()
{
	if (EnemiesToSpawn < 1) {
		return;
	}
	--EnemiesToSpawn;
	FVector SpawnLocation(Waypoints[0]);
	AEnemy* Enemy = GetWorld()->SpawnActor<AEnemy>(SpawnLocation, FRotator::ZeroRotator);
	AEnemy::FRequestNextWaypoint RequestNextWaypoint;
	AEnemy::FDamageTaken DamageTaken;
	RequestNextWaypoint.BindUObject(this, &AEnemyManager::OnEnemyRequestNextWaypoint);
	DamageTaken.BindUObject(this, &AEnemyManager::OnEnemyDamaged);
	Enemy->SetDelegates(RequestNextWaypoint, DamageTaken);
	Enemy->TargetDestroyed.AddUObject(this, &AEnemyManager::OnEnemyDestroyed);

	EnemySpawned.ExecuteIfBound(Cast<UObject>(Enemy));
	if (EnemiesToSpawn < 1) {
		FTimerManager& TimerManager = GetWorldTimerManager();
		TimerManager.PauseTimer(SpawnTimer);
		TimerManager.ClearTimer(SpawnTimer);
	}
}

bool AEnemyManager::OnEnemyRequestNextWaypoint(TScriptInterface<ITargetableMixin> Enemy,
		FVector CurrentWaypoint, FVector& OutNextWaypoint)
{
	for (int32 i = 1; i < Waypoints.Num(); ++i) {
		if (CurrentWaypoint.Equals(Waypoints[i - 1])) {
			OutNextWaypoint = Waypoints[i];
			return false;
		}
	}
	EnemyReachedLastWaypoint.Broadcast(Enemy);
	LastWaypointReached.ExecuteIfBound();
	return true;
}

void AEnemyManager::OnEnemyDamaged()
{
	DamageAudio->SetPitchMultiplier(FMath::RandRange(0.85f, 1.15f));
	DamageAudio->Play();
}

void AEnemyManager::OnEnemyDestroyed(TScriptInterface<ITargetableMixin> Enemy)
{
	--EnemiesRemaining;
	++EnemiesDestroyed;
	EnemyDestroyed.Broadcast(Enemy);
	EnemyCountChanged.Broadcast(EnemiesRemaining, EnemiesDestroyed);
	DestroyAudio->SetPitchMultiplier(FMath::RandRange(0.85f, 1.15f));
	DestroyAudio->Play();
}
