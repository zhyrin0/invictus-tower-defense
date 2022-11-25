// Fill out your copyright notice in the Description page of Project Settings.

#include "TowerDefenseHUD.h"
#include "Engine/Engine.h"
#include "Widgets/SWeakWidget.h"
#include "SMainMenu.h"

ATowerDefenseHUD::ATowerDefenseHUD()
{
	MainMenu = SNew(SMainMenu);
	MainMenuContainer = SNew(SWeakWidget).PossiblyNullContent(MainMenu.ToSharedRef());
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

void ATowerDefenseHUD::OnMainMenuPlayClicked(FText _PlayerName, int32 _LevelNumber)
{
	HideMainMenu();
}

void ATowerDefenseHUD::ShowMainMenu()
{
	if (GEngine && GEngine->GameViewport) {
		GEngine->GameViewport->AddViewportWidgetContent(MainMenuContainer.ToSharedRef());
		MainMenu->SetVisibility(EVisibility::Visible);
		if (PlayerOwner) {
			PlayerOwner->SetInputMode(FInputModeUIOnly());
		}
	}
}

void ATowerDefenseHUD::HideMainMenu()
{
	if (GEngine && GEngine->GameViewport && MainMenuContainer.IsValid()) {
		MainMenu->SetVisibility(EVisibility::Hidden);
		GEngine->GameViewport->RemoveViewportWidgetContent(MainMenuContainer.ToSharedRef());
		if (PlayerOwner) {
			PlayerOwner->SetInputMode(FInputModeGameAndUI());
		}
	}
}
