// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/HUD.h"
#include "TowerDefenseHUD.generated.h"

UCLASS()
class TOWERDEFENSE_API ATowerDefenseHUD : public AHUD
{
	GENERATED_BODY()

public:
	void ShowMainMenu();
	void HideMainMenu();

protected:
	virtual void BeginPlay() override;

	TSharedPtr<class SMainMenu> MainMenu;
};
