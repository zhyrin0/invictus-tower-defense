// Fill out your copyright notice in the Description page of Project Settings.

#include "TowerDefenseHUD.h"
#include "Engine/Engine.h"
#include "Widgets/SWeakWidget.h"
#include "SMainMenu.h"

void ATowerDefenseHUD::BeginPlay()
{
	Super::BeginPlay();
	MainMenu = SNew(SMainMenu).OwningHUD(this);
	ShowMainMenu();
}

void ATowerDefenseHUD::ShowMainMenu()
{
	if (GEngine && GEngine->GameViewport) {
		GEngine->GameViewport->AddViewportWidgetContent(MainMenu.ToSharedRef());
		if (PlayerOwner) {
			PlayerOwner->SetInputMode(FInputModeUIOnly());
		}
	}
}

void ATowerDefenseHUD::HideMainMenu()
{
	if (GEngine && GEngine->GameViewport && MainMenu.IsValid()) {
		GEngine->GameViewport->RemoveViewportWidgetContent(MainMenu.ToSharedRef());
		if (PlayerOwner) {
			PlayerOwner->SetInputMode(FInputModeGameAndUI());
		}
	}
}
