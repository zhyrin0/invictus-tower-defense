// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "TowerDefenseGameMode.generated.h"

UCLASS()
class TOWERDEFENSE_API ATowerDefenseGameMode : public AGameModeBase
{
	GENERATED_BODY()

public:
	ATowerDefenseGameMode();

protected:
	virtual void BeginPlay() override;

	UFUNCTION()
	void OnLevelRequested(FText PlayerName, int32 LevelNumber);
	UFUNCTION()
	void OnQuitRequested();
};
