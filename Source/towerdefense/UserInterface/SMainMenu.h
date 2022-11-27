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
	SLATE_END_ARGS()

	void Construct(const FArguments& InArgs);
	virtual bool SupportsKeyboardFocus() const override;

	FText GetPlayerName() const;
	void SetDelegates(FGameEvents::FPlayRequested& InPlayRequested,
			FGameEvents::FQuitRequested& InQuitRequested);
	void SetMaxLevelNumber(int32 MaxLevelNumber);

protected:
	FReply OnPlayClicked() const;
	FReply OnQuitClicked() const;

	TSharedPtr<class SEditableText> PlayerName;
	TSharedPtr<class SSpinBox<int32>> LevelNumber;
	FGameEvents::FPlayRequested PlayRequested;
	FGameEvents::FQuitRequested QuitRequested;
};
