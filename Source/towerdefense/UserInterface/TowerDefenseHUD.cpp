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
	ShowMainMenu();
}

void ATowerDefenseHUD::BindDelegates(FGameEvents::FLevelChanged& InLevelHUDLevelChanged,
		FGameEvents::FEnemyCountChanged& InLevelHUDEnemyCountChanged,
		FGameEvents::FPlayRequested& InPlayRequested,
		FGameEvents::FLevelWon& InLevelWon,
		FGameEvents::FLevelLost& InLevelLost,
		FGameEvents::FGameWon& InGameWon)
{
	LevelHUD->BindDelegates(InLevelHUDLevelChanged, InLevelHUDEnemyCountChanged);
	InPlayRequested.AddUObject(this, &ATowerDefenseHUD::OnPlayRequested);
	InLevelWon.BindUObject(this, &ATowerDefenseHUD::OnLevelWon);
	InLevelLost.BindUObject(this, &ATowerDefenseHUD::OnLevelLost);
	InGameWon.BindUObject(this, &ATowerDefenseHUD::OnGameWon);
}

void ATowerDefenseHUD::SetDelegates(FGameEvents::FPlayRequested& InMainMenuPlayRequested,
		FGameEvents::FQuitRequested& InMainMenuQuitRequested)
{
	MainMenu->SetDelegates(InMainMenuPlayRequested, InMainMenuQuitRequested);
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
	GEngine->AddOnScreenDebugMessage(-1, 10.0f, FColor::Yellow, TEXT("OnLevelWon"));
}

void ATowerDefenseHUD::OnLevelLost()
{
	GEngine->AddOnScreenDebugMessage(-1, 10.0f, FColor::Yellow, TEXT("OnLevelLost"));
}

void ATowerDefenseHUD::OnGameWon()
{
	GEngine->AddOnScreenDebugMessage(-1, 10.0f, FColor::Yellow, TEXT("OnGameWon"));
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
