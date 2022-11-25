// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Widgets/SCompoundWidget.h"
#include "../GameEvents.h"

template<typename T>
class SSpinBox;

class TOWERDEFENSE_API SMainMenu : public SCompoundWidget
{
public:
	SLATE_BEGIN_ARGS(SMainMenu)
	{}
	SLATE_ARGUMENT(TWeakObjectPtr<class ATowerDefenseHUD>, OwningHUD)
	SLATE_END_ARGS()

	void Construct(const FArguments& InArgs);
	virtual bool SupportsKeyboardFocus() const override;

	FGameEvents::FLevelRequested PlayClicked;
	FGameEvents::FQuitRequested QuitClicked;

protected:
	FReply OnPlayClicked() const;
	FReply OnQuitClicked() const;

	TWeakObjectPtr<class ATowerDefenseHUD> OwningHUD;
	TSharedPtr<class SEditableText> PlayerName;
	TSharedPtr<class SSpinBox<int32>> LevelNumber;
};
