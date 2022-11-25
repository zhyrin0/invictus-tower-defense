// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Widgets/SCompoundWidget.h"

class TOWERDEFENSE_API SMainMenu : public SCompoundWidget
{
public:
	SLATE_BEGIN_ARGS(SMainMenu)
	{}
	SLATE_ARGUMENT(TWeakObjectPtr<class ATowerDefenseHUD>, OwningHUD)
	SLATE_END_ARGS()

	void Construct(const FArguments& InArgs);

	virtual bool SupportsKeyboardFocus() const override;

	FReply OnPlayClicked() const;
	FReply OnQuitClicked() const;

	TWeakObjectPtr<class ATowerDefenseHUD> OwningHUD;
};
