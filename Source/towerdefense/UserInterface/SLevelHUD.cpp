// Fill out your copyright notice in the Description page of Project Settings.

#include "SLevelHUD.h"
#include "SlateOptMacros.h"
#include "Widgets/Layout/SBackgroundBlur.h"

#define LOCTEXT_NAMESPACE "UI"

BEGIN_SLATE_FUNCTION_BUILD_OPTIMIZATION
void SLevelHUD::Construct(const FArguments& InArgs)
{
	FMargin LinePadding(0.0f, 5.0f);
	FMargin InlinePadding(5.0f, 0.0f);
	FSlateFontInfo TextStyle = FCoreStyle::Get().GetFontStyle("EmbossedText");
	TextStyle.Size = 24.0f;
	FColor Color = FColor::White;
	FLinearColor Shadow = FLinearColor(0.0f, 0.0f, 0.0f, 0.75f);
	FVector2D ShadowOffset = FVector2D(2.0f, 2.0f);

	ChildSlot
	.HAlign(HAlign_Left).VAlign(VAlign_Top)
	[
		SNew(SBackgroundBlur)
		.HAlign(HAlign_Left).VAlign(VAlign_Top)
		.BlurRadius(100).BlurStrength(5.0f)
		[
			SNew(SVerticalBox)
			+ SVerticalBox::Slot()
			.HAlign(HAlign_Left).VAlign(VAlign_Top)
			.Padding(LinePadding)
			[
// note: Player name
				SNew(SHorizontalBox)
				+ SHorizontalBox::Slot()
				.HAlign(HAlign_Left).VAlign(VAlign_Top)
				.Padding(InlinePadding)
				[
					SNew(STextBlock)
					.Text(LOCTEXT("PLAYER_NAME", "Name:")).Font(TextStyle).ColorAndOpacity(Color)
					.ShadowColorAndOpacity(Shadow).ShadowOffset(ShadowOffset)
				]
				+ SHorizontalBox::Slot()
				.HAlign(HAlign_Left).VAlign(VAlign_Top)
				.Padding(InlinePadding)
				[
					SAssignNew(PlayerName, STextBlock)
					.Font(TextStyle).ColorAndOpacity(Color)
					.ShadowColorAndOpacity(Shadow).ShadowOffset(ShadowOffset)
				]
// note: Level number
				+ SHorizontalBox::Slot()
				.HAlign(HAlign_Left).VAlign(VAlign_Top)
				.Padding(InlinePadding)
				[
					SNew(STextBlock)
					.Text(LOCTEXT("LEVEL", "Level:")).Font(TextStyle).ColorAndOpacity(Color)
					.ShadowColorAndOpacity(Shadow).ShadowOffset(ShadowOffset)
				]
				+ SHorizontalBox::Slot()
				.HAlign(HAlign_Left).VAlign(VAlign_Top)
				.Padding(InlinePadding)
				[
					SAssignNew(LevelNumber, STextBlock)
					.Font(TextStyle).ColorAndOpacity(Color)
					.ShadowColorAndOpacity(Shadow).ShadowOffset(ShadowOffset)
				]
			]
			+ SVerticalBox::Slot()
			.HAlign(HAlign_Left).VAlign(VAlign_Top)
			.Padding(LinePadding)
			[
				SNew(SHorizontalBox)
// note: Enemies destroyed
				+ SHorizontalBox::Slot()
				.HAlign(HAlign_Left).VAlign(VAlign_Top)
				.Padding(InlinePadding)
				[
					SNew(STextBlock)
					.Text(LOCTEXT("ENEMIES_DESTROYED", "Enemies destroyed:")).Font(TextStyle).ColorAndOpacity(Color)
					.ShadowColorAndOpacity(Shadow).ShadowOffset(ShadowOffset)
				]
				+ SHorizontalBox::Slot()
				.HAlign(HAlign_Left).VAlign(VAlign_Top)
				.Padding(InlinePadding)
				[
					SAssignNew(EnemiesDestroyed, STextBlock)
					.Font(TextStyle).ColorAndOpacity(Color)
					.ShadowColorAndOpacity(Shadow).ShadowOffset(ShadowOffset)
				]
// note: Enemies remaining
				+ SHorizontalBox::Slot()
				.HAlign(HAlign_Left).VAlign(VAlign_Top)
				.Padding(InlinePadding)
				[
					SNew(STextBlock)
					.Text(LOCTEXT("ENEMIES_REMAINING", "remaining:")).Font(TextStyle).ColorAndOpacity(Color)
					.ShadowColorAndOpacity(Shadow).ShadowOffset(ShadowOffset)
				]
				+ SHorizontalBox::Slot()
				.HAlign(HAlign_Left).VAlign(VAlign_Top)
				.Padding(InlinePadding)
				[
					SAssignNew(EnemiesRemaining, STextBlock)
					.Font(TextStyle).ColorAndOpacity(Color)
					.ShadowColorAndOpacity(Shadow).ShadowOffset(ShadowOffset)
				]
			]
		]
	];
	
}
END_SLATE_FUNCTION_BUILD_OPTIMIZATION

bool SLevelHUD::SupportsKeyboardFocus() const
{
	return false;
}

void SLevelHUD::SetPlayerName(FText NewPlayerName)
{
	PlayerName->SetText(NewPlayerName);
}

void SLevelHUD::SetLevelNumber(int32 NewLevelNumber)
{
	LevelNumber->SetText(FText::AsNumber(NewLevelNumber));
}

void SLevelHUD::SetEnemiesRemaining(int32 NewEnemiesRemaining)
{
	EnemiesRemaining->SetText(FText::AsNumber(NewEnemiesRemaining));
}

void SLevelHUD::SetEnemiesDestroyed(int32 NewEnemiesDestroyed)
{
	EnemiesDestroyed->SetText(FText::AsNumber(NewEnemiesDestroyed));
}

#undef LOCTEXT_NAMESPACE
