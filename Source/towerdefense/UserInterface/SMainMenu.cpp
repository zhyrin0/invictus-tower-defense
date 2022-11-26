// Fill out your copyright notice in the Description page of Project Settings.

#include "SMainMenu.h"
#include "Math/Color.h"
#include "SlateOptMacros.h"
#include "Widgets/Input/SEditableText.h"
#include "Widgets/Input/SSpinBox.h"
#include "Widgets/Layout/SBackgroundBlur.h"

#define LOCTEXT_NAMESPACE "UI"

BEGIN_SLATE_FUNCTION_BUILD_OPTIMIZATION
void SMainMenu::Construct(const FArguments& InArgs)
{
	FMargin DefaultPadding(0.0f, 15.0f);
	FSlateFontInfo TextStyle = FCoreStyle::Get().GetFontStyle("EmbossedText");
	TextStyle.Size = 36.0f;
	FColor Color = FColor::White;
	FLinearColor Shadow = FLinearColor(0.0f, 0.0f, 0.0f, 0.75f);
	FVector2D ShadowOffset = FVector2D(2.0f, 2.0f);

	ChildSlot
	.HAlign(HAlign_Fill).VAlign(VAlign_Fill)
	[
		SNew(SBackgroundBlur)
		.HAlign(HAlign_Fill).VAlign(VAlign_Fill)
		.BlurRadius(100).BlurStrength(5.0f)
		[
			SNew(SOverlay)
			+ SOverlay::Slot()
			.HAlign(HAlign_Center).VAlign(VAlign_Center)
			[
				SNew(SVerticalBox)
				+ SVerticalBox::Slot()
				.HAlign(HAlign_Fill).VAlign(VAlign_Top)
				.Padding(DefaultPadding)
				[
// note: Player name input
					SNew(SHorizontalBox)
					+ SHorizontalBox::Slot()
					.HAlign(HAlign_Center).VAlign(VAlign_Center)
					[
						SNew(STextBlock)
						.Text(LOCTEXT("PLAYER_NAME", "Name:")).Font(TextStyle).ColorAndOpacity(Color)
						.ShadowColorAndOpacity(Shadow).ShadowOffset(ShadowOffset)
						.Justification(ETextJustify::Right)
						.MinDesiredWidth(200.0f)
					]
					+ SHorizontalBox::Slot()
					.HAlign(HAlign_Left).VAlign(VAlign_Center)
					[
						SNew(SBorder)
						.BorderBackgroundColor(FColor::Red)
						[
							SAssignNew(PlayerName, SEditableText)
							.HintText(LOCTEXT("PLAYER_NAME_PLACEHOLDER", "Name")).Font(TextStyle).ColorAndOpacity(Color)
							.MinDesiredWidth(400.0f)
						]
					]
				]
				+ SVerticalBox::Slot()
				.HAlign(HAlign_Fill).VAlign(VAlign_Top)
				.Padding(DefaultPadding)
				[
// note: Level input
					SNew(SHorizontalBox)
					+ SHorizontalBox::Slot()
					.HAlign(HAlign_Center).VAlign(VAlign_Center)
					[
						SNew(STextBlock)
						.Text(LOCTEXT("LEVEL", "Level:")).Font(TextStyle).ColorAndOpacity(Color)
						.ShadowColorAndOpacity(Shadow).ShadowOffset(ShadowOffset)
						.Justification(ETextJustify::Right)
						.MinDesiredWidth(200.0f)
					]
					+ SHorizontalBox::Slot()
					.HAlign(HAlign_Left).VAlign(VAlign_Center)
					[
						SNew(SBorder)
						.BorderBackgroundColor(FColor::Red)
						[
							SAssignNew(LevelNumber, SSpinBox<int32>)
							.Value(1).MinValue(1).MaxValue(5)
							.Font(TextStyle)
							.MinDesiredWidth(100.0f)
						]
					]
				]
				+ SVerticalBox::Slot()
				.HAlign(HAlign_Center).VAlign(VAlign_Top)
				.Padding(DefaultPadding)
				[
// note: Play button
					SNew(SButton)
					.OnClicked(this, &SMainMenu::OnPlayClicked)
					[
						SNew(STextBlock)
						.Text(LOCTEXT("PLAY_GAME", "Play")).Font(TextStyle)
					]
				]
				+ SVerticalBox::Slot()
				.HAlign(HAlign_Center).VAlign(VAlign_Top)
				.Padding(DefaultPadding)
				[
// note: Quit button
					SNew(SButton)
					.OnClicked(this, &SMainMenu::OnQuitClicked)
					[
						SNew(STextBlock)
						.Text(LOCTEXT("QUIT_GAME", "Quit")).Font(TextStyle)
					]
				]
			]
		]
	];
	
}
END_SLATE_FUNCTION_BUILD_OPTIMIZATION

bool SMainMenu::SupportsKeyboardFocus() const
{
	return true;
}

FText SMainMenu::GetPlayerName() const
{
	return PlayerName->GetText();
}

void SMainMenu::SetDelegates(FGameEvents::FPlayRequested& InPlayRequested,
		FGameEvents::FQuitRequested& InQuitRequested)
{
	PlayRequested = InPlayRequested;
	QuitRequested = InQuitRequested;
}

FReply SMainMenu::OnPlayClicked() const
{
	PlayRequested.Broadcast(LevelNumber->GetValue());
	return FReply::Handled();
}

FReply SMainMenu::OnQuitClicked() const
{
	QuitRequested.ExecuteIfBound();
	return FReply::Handled();
}

#undef LOCTEXT_NAMESPACE
