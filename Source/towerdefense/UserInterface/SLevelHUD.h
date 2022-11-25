// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Widgets/SCompoundWidget.h"

class TOWERDEFENSE_API SLevelHUD : public SCompoundWidget
{
public:
	SLATE_BEGIN_ARGS(SLevelHUD)
	{}
	SLATE_END_ARGS()

	void Construct(const FArguments& InArgs);
	virtual bool SupportsKeyboardFocus() const override;

	void SetPlayerName(FText NewPlayerName);
	void SetLevelNumber(int32 NewLevelNumber);
	void SetEnemiesRemaining(int32 NewEnemiesRemaining);
	void SetEnemiesDestroyed(int32 NewEnemiesDestroyed);

protected:
	TSharedPtr<class STextBlock> PlayerName;
	TSharedPtr<class STextBlock> LevelNumber;
	TSharedPtr<class STextBlock> EnemiesRemaining;
	TSharedPtr<class STextBlock> EnemiesDestroyed;
};
