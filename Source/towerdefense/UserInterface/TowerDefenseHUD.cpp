// Fill out your copyright notice in the Description page of Project Settings.

#include "TowerDefenseHUD.h"
#include "Engine/Engine.h"
#include "Widgets/SWeakWidget.h"
#include "SLevelHUD.h"
#include "SMainMenu.h"

ATowerDefenseHUD::ATowerDefenseHUD()
{
	MainMenu = SNew(SMainMenu);
	MainMenuContainer = SNew(SWeakWidget).PossiblyNullContent(MainMenu.ToSharedRef());
	LevelHUD = SNew(SLevelHUD);
	LevelHUDContainer = SNew(SWeakWidget).PossiblyNullContent(LevelHUD.ToSharedRef());
}

void ATowerDefenseHUD::BeginPlay()
{
	Super::BeginPlay();
	MainMenu->PlayClicked.AddUObject(this, &ATowerDefenseHUD::OnMainMenuPlayClicked);
	ShowMainMenu();
}

FGameEvents::FLevelRequested& ATowerDefenseHUD::GetLevelRequestedDelegate()
{
	return MainMenu->PlayClicked;
}

FGameEvents::FQuitRequested& ATowerDefenseHUD::GetQuitRequestedDelegate()
{
	return MainMenu->QuitClicked;
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
	ShowWidget(LevelHUD, LevelHUDContainer, EVisibility::HitTestInvisible, false, FInputModeGameAndUI());
}

void ATowerDefenseHUD::HideLevelHUD()
{
	HideWidget(LevelHUD, LevelHUDContainer, false, FInputModeGameAndUI());
}

void ATowerDefenseHUD::OnMainMenuPlayClicked(FText _PlayerName, int32 _LevelNumber)
{
	HideMainMenu();
	LevelHUD->SetPlayerName(_PlayerName);
	LevelHUD->SetLevelNumber(_LevelNumber);
	ShowLevelHUD();
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
