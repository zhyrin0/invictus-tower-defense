// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Widgets/SCompoundWidget.h"

#include "../GameEvents.h"

class TOWERDEFENSE_API SIntermediateScreen : public SCompoundWidget
{
public:
	SLATE_BEGIN_ARGS(SIntermediateScreen)
	{}
	SLATE_ARGUMENT(FText, Text)
	SLATE_ARGUMENT(FText, ButtonText)
	SLATE_END_ARGS()

	void Construct(const FArguments& InArgs);
	virtual bool SupportsKeyboardFocus() const override;
	void SetDelegate(FGameEvents::FUIContinueRequested& InContinue);

protected:
	FReply OnContinueClicked() const;

	FGameEvents::FUIContinueRequested Continue;
};
