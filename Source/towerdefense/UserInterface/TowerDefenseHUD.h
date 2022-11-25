// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/HUD.h"
#include "../GameEvents.h"
#include "TowerDefenseHUD.generated.h"

UCLASS()
class TOWERDEFENSE_API ATowerDefenseHUD : public AHUD
{
	GENERATED_BODY()

public:
	ATowerDefenseHUD();

	FGameEvents::FLevelRequested& GetLevelRequestedDelegate();
	FGameEvents::FQuitRequested& GetQuitRequestedDelegate();

protected:
	virtual void BeginPlay() override;
	// note: Main menu doesn't use any of the delegate's arguments.
	UFUNCTION()
	void OnMainMenuPlayClicked(FText _PlayerName, int32 _LevelNumber);
	void ShowMainMenu();
	void HideMainMenu();

	TSharedPtr<class SMainMenu> MainMenu;
	TSharedPtr<class SWeakWidget> MainMenuContainer;
};
