// Fill out your copyright notice in the Description page of Project Settings.

#include "SIntermediateScreen.h"

#include "Widgets/Layout/SBackgroundBlur.h"
#include "SlateOptMacros.h"

BEGIN_SLATE_FUNCTION_BUILD_OPTIMIZATION
void SIntermediateScreen::Construct(const FArguments& InArgs)
{
	int32 BlurRadius = 100;
	float BlurStrength = 5.0f;
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
		.BlurRadius(BlurRadius).BlurStrength(BlurStrength)
		[
			SNew(SOverlay)
			+ SOverlay::Slot()
			.HAlign(HAlign_Center).VAlign(VAlign_Center)
			[
				SNew(SVerticalBox)
// note: Text
				+ SVerticalBox::Slot()
				.HAlign(HAlign_Center).VAlign(VAlign_Center)
				.Padding(DefaultPadding)
				[
					SNew(STextBlock)
					.Text(InArgs._Text).Font(TextStyle).ColorAndOpacity(Color)
					.ShadowColorAndOpacity(Shadow).ShadowOffset(ShadowOffset)
				]
// note: Continue button
				+ SVerticalBox::Slot()
				.HAlign(HAlign_Center).VAlign(VAlign_Center)
				.Padding(DefaultPadding)
				[
					SNew(SButton)
					.OnClicked(this, &SIntermediateScreen::OnContinueClicked)
					[
						SNew(STextBlock)
						.Text(InArgs._ButtonText).Font(TextStyle)
					]
				]
			]
		]
	];
}
END_SLATE_FUNCTION_BUILD_OPTIMIZATION

bool SIntermediateScreen::SupportsKeyboardFocus() const
{
	return true;
}

void SIntermediateScreen::SetDelegate(FGameEvents::FUIContinueRequested& InContinue)
{
	Continue = InContinue;
}

FReply SIntermediateScreen::OnContinueClicked() const
{
	Continue.Broadcast();
	return FReply::Handled();
}
