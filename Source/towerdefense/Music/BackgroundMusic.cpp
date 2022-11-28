// Fill out your copyright notice in the Description page of Project Settings.

#include "BackgroundMusic.h"

#include "Components/AudioComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Sound/SoundWave.h"

ABackgroundMusic::ABackgroundMusic()
{
	PrimaryActorTick.bCanEverTick = false;
}

void ABackgroundMusic::BeginPlay()
{
	Super::BeginPlay();
	FString MusicReference = TEXT("SoundWave'/Game/Music/TheDivineElf.TheDivineElf'");
	USoundWave* Music = LoadObject<USoundWave>(NULL, *MusicReference, NULL, LOAD_None, NULL);
	Audio = UGameplayStatics::SpawnSound2D(GetWorld(), Music, 1.0f, 1.0f, 0.0f, nullptr, true, false);
}
