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

	void BindDelegates(FGameEvents::FLevelChanged& InLevelHUDLevelChanged,
			FGameEvents::FEnemyCountChanged& InLevelHUDEnemyCountChanged,
			FGameEvents::FPlayRequested& InPlayRequested,
			FGameEvents::FLevelWon& InLevelWon,
			FGameEvents::FLevelLost& InLevelLost,
			FGameEvents::FGameWon& InGameWon,
			FGameEvents::FUIContinueRequested& InContinueToNextLevel,
			FGameEvents::FUIContinueRequested& InContinueToMainMenu);
	void SetDelegates(FGameEvents::FPlayRequested& InMainMenuPlayRequested,
			FGameEvents::FQuitRequested& InMainMenuQuitRequested,
			FGameEvents::FUIContinueRequested& InLevelWonContinueToNextLevel,
			FGameEvents::FUIContinueRequested& InLevelLostContinueToMainMenu,
			FGameEvents::FUIContinueRequested& InGameWonContinueToMainMenu);
	void SetMaxLevelNumber(int32 MaxLevelNumber);

protected:
	virtual void BeginPlay() override;
	UFUNCTION()
	void OnPlayRequested(int32 _LevelNumber);
	UFUNCTION()
	void OnLevelWon();
	UFUNCTION()
	void OnLevelLost();
	UFUNCTION()
	void OnGameWon();
	UFUNCTION()
	void OnContinueToNextLevel();
	UFUNCTION()
	void OnContinueToMainMenu();

	void ShowMainMenu();
	void HideMainMenu();
	void ShowLevelHUD();
	void HideLevelHUD();
	void ShowLevelWon();
	void HideLevelWon();
	void ShowLevelLost();
	void HideLevelLost();
	void ShowGameWon();
	void HideGameWon();
	void ShowWidget(TSharedPtr<class SWidget> Widget, TSharedPtr<class SWeakWidget> Container,
			EVisibility Visibility, bool UpdateInputMode, const FInputModeDataBase& InputMode);
	void HideWidget(TSharedPtr<class SWidget> Widget, TSharedPtr<class SWeakWidget> Container,
			bool UpdateInputMode, const FInputModeDataBase& InputMode);

	TSharedPtr<class SMainMenu> MainMenu;
	TSharedPtr<class SWeakWidget> MainMenuContainer;
	TSharedPtr<class SLevelHUD> LevelHUD;
	TSharedPtr<class SWeakWidget> LevelHUDContainer;
	TSharedPtr<class SIntermediateScreen> LevelWonScreen;
	TSharedPtr<class SWeakWidget> LevelWonScreenContainer;
	TSharedPtr<class SIntermediateScreen> LevelLostScreen;
	TSharedPtr<class SWeakWidget> LevelLostScreenContainer;
	TSharedPtr<class SIntermediateScreen> GameWonScreen;
	TSharedPtr<class SWeakWidget> GameWonScreenContainer;
};
