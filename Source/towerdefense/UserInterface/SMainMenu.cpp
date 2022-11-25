// Fill out your copyright notice in the Description page of Project Settings.

#include "SMainMenu.h"
#include "GameFramework/PlayerController.h"
#include "SlateOptMacros.h"
#include "TowerDefenseHUD.h"

#define LOCTEXT_NAMESPACE "UI"

BEGIN_SLATE_FUNCTION_BUILD_OPTIMIZATION
void SMainMenu::Construct(const FArguments& InArgs)
{
	OwningHUD = InArgs._OwningHUD;

	ChildSlot
	[
		SNew(SVerticalBox)
		// Game title
		+SVerticalBox::Slot()
		.HAlign(EHorizontalAlignment::HAlign_Center)
		.VAlign(EVerticalAlignment::VAlign_Center)
		[
			SNew(STextBlock)
			.Text(LOCTEXT("GAME_TITLE", "Tower Defense"))
		]
		// Start button
		+SVerticalBox::Slot()
		.HAlign(EHorizontalAlignment::HAlign_Center)
		.VAlign(EVerticalAlignment::VAlign_Center)
		[
			SNew(SButton)
			.OnClicked(this, &SMainMenu::OnPlayClicked)
			[
				SNew(STextBlock)
				.Text(LOCTEXT("START_GAME", "Start"))
			]
		]
		// Quit button
		+SVerticalBox::Slot()
		.HAlign(EHorizontalAlignment::HAlign_Center)
		.VAlign(EVerticalAlignment::VAlign_Center)
		[
			SNew(SButton)
			.OnClicked(this, &SMainMenu::OnQuitClicked)
			[
				SNew(STextBlock)
				.Text(LOCTEXT("QUIT_GAME", "Quit"))
			]
		]
	];
	
}
END_SLATE_FUNCTION_BUILD_OPTIMIZATION

FReply SMainMenu::OnPlayClicked() const
{
	if (OwningHUD.IsValid()) {
		OwningHUD->HideMainMenu();
	}
	return FReply::Handled();
}

FReply SMainMenu::OnQuitClicked() const
{
	if (OwningHUD.IsValid()) {
		if (APlayerController* Controller = OwningHUD->PlayerOwner) {
			Controller->ConsoleCommand("quit");
		}
	}
	return FReply::Handled();
}

bool SMainMenu::SupportsKeyboardFocus() const
{
	return true;
}

#undef LOCTEXT_NAMESPACE
