// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Widgets/SCompoundWidget.h"
#include "../GameEvents.h"

class TOWERDEFENSE_API SLevelHUD : public SCompoundWidget
{
public:
	SLATE_BEGIN_ARGS(SLevelHUD)
	{}
	SLATE_END_ARGS()

	void Construct(const FArguments& InArgs);
	virtual bool SupportsKeyboardFocus() const override;

	void BindDelegates(FGameEvents::FLevelChanged& InLevelChanged,
			FGameEvents::FEnemyCountChanged& InEnemyCountChanged);
	void SetPlayerName(FText InPlayerName);

protected:
	UFUNCTION()
	void OnLevelChanged(int32 InLevelNumber);
	UFUNCTION()
	void OnEnemyCountChanged(int32 Remaining, int32 Destroyed);
	void SetLevelNumber(int32 InLevelNumber);
	void SetEnemiesRemaining(int32 Remaining);
	void SetEnemiesDestroyed(int32 Destroyed);

	TSharedPtr<class STextBlock> PlayerName;
	TSharedPtr<class STextBlock> LevelNumber;
	TSharedPtr<class STextBlock> EnemiesRemaining;
	TSharedPtr<class STextBlock> EnemiesDestroyed;
};
