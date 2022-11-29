// Fill out your copyright notice in the Description page of Project Settings.

#include "TowerDefenseHUD.h"

#include "Engine/Engine.h"
#include "Widgets/SWeakWidget.h"

#include "SIntermediateScreen.h"
#include "SLevelHUD.h"
#include "SMainMenu.h"

#define LOCTEXT_NAMESPACE "UI"

ATowerDefenseHUD::ATowerDefenseHUD()
{
	MainMenu = SNew(SMainMenu);
	MainMenuContainer = SNew(SWeakWidget).PossiblyNullContent(MainMenu.ToSharedRef());
	LevelHUD = SNew(SLevelHUD);
	LevelHUDContainer = SNew(SWeakWidget).PossiblyNullContent(LevelHUD.ToSharedRef());
	LevelWonScreen = SNew(SIntermediateScreen)
			.Text(LOCTEXT("LEVEL_CLEARED", "Level cleared!"))
			.ButtonText(LOCTEXT("NEXT_LEVEL", "Next level"));
	LevelWonScreenContainer = SNew(SWeakWidget).PossiblyNullContent(LevelWonScreen.ToSharedRef());
	LevelLostScreen = SNew(SIntermediateScreen)
			.Text(LOCTEXT("LEVEL_LOST", "Level lost!"))
			.ButtonText(LOCTEXT("RETURN_TO_MAIN_MENU", "Return to main menu"));
	LevelLostScreenContainer = SNew(SWeakWidget).PossiblyNullContent(LevelLostScreen.ToSharedRef());
	GameWonScreen = SNew(SIntermediateScreen)
			.Text(LOCTEXT("GAME_WON", "Game won!"))
			.ButtonText(LOCTEXT("RETURN_TO_MAIN_MENU", "Return to main menu"));
	GameWonScreenContainer = SNew(SWeakWidget).PossiblyNullContent(GameWonScreen.ToSharedRef());
}

void ATowerDefenseHUD::BeginPlay()
{
	Super::BeginPlay();
	ShowMainMenu();
}

void ATowerDefenseHUD::BindDelegates(FGameEvents::FLevelChanged& InLevelHUDLevelChanged,
		FGameEvents::FEnemyCountChanged& InLevelHUDEnemyCountChanged,
		FGameEvents::FPlayRequested& InPlayRequested,
		FGameEvents::FLevelWon& InLevelWon,
		FGameEvents::FLevelLost& InLevelLost,
		FGameEvents::FGameWon& InGameWon,
		FGameEvents::FUIContinueRequested& InContinueToNextLevel,
		FGameEvents::FUIContinueRequested& InContinueToMainMenu)
{
	LevelHUD->BindDelegates(InLevelHUDLevelChanged, InLevelHUDEnemyCountChanged);
	InPlayRequested.AddUObject(this, &ATowerDefenseHUD::OnPlayRequested);
	InLevelWon.BindUObject(this, &ATowerDefenseHUD::OnLevelWon);
	InLevelLost.BindUObject(this, &ATowerDefenseHUD::OnLevelLost);
	InGameWon.BindUObject(this, &ATowerDefenseHUD::OnGameWon);
	InContinueToNextLevel.AddUObject(this, &ATowerDefenseHUD::OnContinueToNextLevel);
	InContinueToMainMenu.AddUObject(this, &ATowerDefenseHUD::OnContinueToMainMenu);
}

void ATowerDefenseHUD::SetDelegates(FGameEvents::FPlayRequested& InMainMenuPlayRequested,
		FGameEvents::FQuitRequested& InMainMenuQuitRequested,
		FGameEvents::FUIContinueRequested& InLevelWonContinueToNextLevel,
		FGameEvents::FUIContinueRequested& InLevelLostContinueToMainMenu,
		FGameEvents::FUIContinueRequested& InGameWonContinueToMainMenu)
{
	MainMenu->SetDelegates(InMainMenuPlayRequested, InMainMenuQuitRequested);
	LevelWonScreen->SetDelegate(InLevelWonContinueToNextLevel);
	LevelLostScreen->SetDelegate(InLevelLostContinueToMainMenu);
	GameWonScreen->SetDelegate(InGameWonContinueToMainMenu);
}

void ATowerDefenseHUD::SetMaxLevelNumber(int32 MaxLevelNumber)
{
	MainMenu->SetMaxLevelNumber(MaxLevelNumber);
}

void ATowerDefenseHUD::OnPlayRequested(int32 _LevelNumber)
{
	HideMainMenu();
	LevelHUD->SetPlayerName(MainMenu->GetPlayerName());
	ShowLevelHUD();
}

void ATowerDefenseHUD::OnLevelWon()
{
	HideLevelHUD();
	ShowLevelWon();
}

void ATowerDefenseHUD::OnLevelLost()
{
	HideLevelHUD();
	ShowLevelLost();
}

void ATowerDefenseHUD::OnGameWon()
{
	HideLevelHUD();
	ShowGameWon();
}

void ATowerDefenseHUD::OnContinueToNextLevel()
{
	HideLevelWon();
	ShowLevelHUD();
}

void ATowerDefenseHUD::OnContinueToMainMenu()
{
	HideLevelLost();
	HideGameWon();
	ShowMainMenu();
}

void ATowerDefenseHUD::ShowMainMenu()
{
	ShowWidget(MainMenu, MainMenuContainer, EVisibility::Visible, true, FInputModeUIOnly());
}

void ATowerDefenseHUD::HideMainMenu()
{
	HideWidget(MainMenu, MainMenuContainer, true, FInputModeGameAndUI());
}

void ATowerDefenseHUD::ShowLevelHUD()
{
	ShowWidget(LevelHUD, LevelHUDContainer, EVisibility::HitTestInvisible, true, FInputModeGameAndUI());
}

void ATowerDefenseHUD::HideLevelHUD()
{
	HideWidget(LevelHUD, LevelHUDContainer, false, FInputModeGameAndUI());
}

void ATowerDefenseHUD::ShowLevelWon()
{
	ShowWidget(LevelWonScreen, LevelWonScreenContainer, EVisibility::Visible, true, FInputModeUIOnly());
}

void ATowerDefenseHUD::HideLevelWon()
{
	HideWidget(LevelWonScreen, LevelWonScreenContainer, false, FInputModeGameAndUI());
}

void ATowerDefenseHUD::ShowLevelLost()
{
	ShowWidget(LevelLostScreen, LevelLostScreenContainer, EVisibility::Visible, true, FInputModeUIOnly());
}

void ATowerDefenseHUD::HideLevelLost()
{
	HideWidget(LevelLostScreen, LevelLostScreenContainer, false, FInputModeGameAndUI());
}

void ATowerDefenseHUD::ShowGameWon()
{
	ShowWidget(GameWonScreen, GameWonScreenContainer, EVisibility::Visible, true, FInputModeUIOnly());
}

void ATowerDefenseHUD::HideGameWon()
{
	HideWidget(GameWonScreen, GameWonScreenContainer, false, FInputModeGameAndUI());
}

void ATowerDefenseHUD::ShowWidget(TSharedPtr<class SWidget> Widget, TSharedPtr<class SWeakWidget> Container,
		EVisibility Visibility, bool UpdateInputMode, const FInputModeDataBase& InputMode)
{
	if (GEngine && GEngine->GameViewport) {
		GEngine->GameViewport->AddViewportWidgetContent(Container.ToSharedRef());
		Widget->SetVisibility(Visibility);
		if (UpdateInputMode && PlayerOwner) {
			PlayerOwner->SetInputMode(InputMode);
		}
	}
}

void ATowerDefenseHUD::HideWidget(TSharedPtr<class SWidget> Widget, TSharedPtr<class SWeakWidget> Container,
		bool UpdateInputMode, const FInputModeDataBase& InputMode)
{
	if (GEngine && GEngine->GameViewport && Container.IsValid()) {
		Widget->SetVisibility(EVisibility::Hidden);
		GEngine->GameViewport->RemoveViewportWidgetContent(Container.ToSharedRef());
		if (UpdateInputMode && PlayerOwner) {
			PlayerOwner->SetInputMode(InputMode);
		}
	}
}

#undef LOCTEXT_NAMESPACE
