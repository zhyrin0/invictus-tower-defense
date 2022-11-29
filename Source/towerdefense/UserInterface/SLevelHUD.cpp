// Fill out your copyright notice in the Description page of Project Settings.

#include "SLevelHUD.h"

#include "Widgets/Layout/SBackgroundBlur.h"
#include "SlateOptMacros.h"

#define LOCTEXT_NAMESPACE "UI"

BEGIN_SLATE_FUNCTION_BUILD_OPTIMIZATION
void SLevelHUD::Construct(const FArguments& InArgs)
{
	FMargin ContainerPadding(15.0f, 15.0f);
	FMargin InlinePadding(25.0f, 0.0f);
	float Column1Width = 550.0f;
	FSlateFontInfo TextStyle = FCoreStyle::Get().GetFontStyle("EmbossedText");
	TextStyle.Size = 24.0f;
	FColor Color = FColor::White;
	FLinearColor Shadow = FLinearColor(0.0f, 0.0f, 0.0f, 0.75f);
	FVector2D ShadowOffset = FVector2D(2.0f, 2.0f);

	ChildSlot
	[
		SNew(SBox)
		.HAlign(HAlign_Left).VAlign(VAlign_Top)
		.Padding(ContainerPadding)
		[
			SNew(SVerticalBox)
// note: Player name
			+ SVerticalBox::Slot()
			[
				SNew(SHorizontalBox)
				+ SHorizontalBox::Slot()
				.Padding(InlinePadding)
				[
					SNew(SBox)
					.HAlign(HAlign_Fill)
					.WidthOverride(Column1Width)
					[
						SNew(STextBlock)
						.Text(LOCTEXT("PLAYER_NAME", "Name:")).Font(TextStyle).ColorAndOpacity(Color)
						.ShadowColorAndOpacity(Shadow).ShadowOffset(ShadowOffset)
						.Justification(ETextJustify::Right)
					]
				]
				+ SHorizontalBox::Slot()
				[
					SAssignNew(PlayerName, STextBlock)
					.Font(TextStyle).ColorAndOpacity(Color)
					.ShadowColorAndOpacity(Shadow).ShadowOffset(ShadowOffset)
				]
			]
// note: Level number
			+ SVerticalBox::Slot()
			[
				SNew(SHorizontalBox)
				+ SHorizontalBox::Slot()
				.Padding(InlinePadding)
				[
					SNew(STextBlock)
					.Text(LOCTEXT("LEVEL", "Level:")).Font(TextStyle).ColorAndOpacity(Color)
					.ShadowColorAndOpacity(Shadow).ShadowOffset(ShadowOffset)
					.Justification(ETextJustify::Right)
				]
				+ SHorizontalBox::Slot()
				[
					SAssignNew(LevelNumber, STextBlock)
					.Font(TextStyle).ColorAndOpacity(Color)
					.ShadowColorAndOpacity(Shadow).ShadowOffset(ShadowOffset)
				]
			]
// note: Enemies destroyed
			+ SVerticalBox::Slot()
			[
				SNew(SHorizontalBox)
				+ SHorizontalBox::Slot()
				.Padding(InlinePadding)
				[
					SNew(STextBlock)
					.Text(LOCTEXT("ENEMIES_DESTROYED", "Enemies destroyed:")).Font(TextStyle).ColorAndOpacity(Color)
					.ShadowColorAndOpacity(Shadow).ShadowOffset(ShadowOffset)
					.Justification(ETextJustify::Right)
				]
				+ SHorizontalBox::Slot()
				[
					SAssignNew(EnemiesDestroyed, STextBlock)
					.Font(TextStyle).ColorAndOpacity(Color)
					.ShadowColorAndOpacity(Shadow).ShadowOffset(ShadowOffset)
				]
			]
// note: Enemies remaining
			+ SVerticalBox::Slot()
			[
				SNew(SHorizontalBox)
				+ SHorizontalBox::Slot()
				.Padding(InlinePadding)
				[
					SNew(STextBlock)
					.Text(LOCTEXT("ENEMIES_REMAINING", "Enemies remaining:")).Font(TextStyle).ColorAndOpacity(Color)
					.ShadowColorAndOpacity(Shadow).ShadowOffset(ShadowOffset)
					.Justification(ETextJustify::Right)
				]
				+ SHorizontalBox::Slot()
				[
					SAssignNew(EnemiesRemaining, STextBlock)
					.Font(TextStyle).ColorAndOpacity(Color)
					.ShadowColorAndOpacity(Shadow).ShadowOffset(ShadowOffset)
				]
			]
// note: Zoom hint
			+ SVerticalBox::Slot()
			[
				SNew(STextBlock)
				.Text(LOCTEXT("USE_MOUSEWHEEL", "Use <mousewheel> to zoom.")).Font(TextStyle).ColorAndOpacity(Color)
				.ShadowColorAndOpacity(Shadow).ShadowOffset(ShadowOffset)
			]
// note: Camera movement hint
			+ SVerticalBox::Slot()
			[
				SNew(STextBlock)
				.Text(LOCTEXT("USE_WASD", "Use <W,A,S,D> to move camera.")).Font(TextStyle).ColorAndOpacity(Color)
				.ShadowColorAndOpacity(Shadow).ShadowOffset(ShadowOffset)
			]
// note: Camera rotation hint
			+ SVerticalBox::Slot()
			[
				SNew(STextBlock)
				.Text(LOCTEXT("USE_QE", "Use <Q,E> to rotate camera")).Font(TextStyle).ColorAndOpacity(Color)
				.ShadowColorAndOpacity(Shadow).ShadowOffset(ShadowOffset)
			]
		]
	];
}
END_SLATE_FUNCTION_BUILD_OPTIMIZATION

bool SLevelHUD::SupportsKeyboardFocus() const
{
	return false;
}

void SLevelHUD::BindDelegates(FGameEvents::FLevelChanged& InLevelChanged,
		FGameEvents::FEnemyCountChanged& InEnemyCountChanged)
{
	InLevelChanged.BindLambda([this](int32 InLevelNumber) {
		OnLevelChanged(InLevelNumber);
	});
	InEnemyCountChanged.AddLambda([this](int32 Remaining, int32 Destroyed) {
		OnEnemyCountChanged(Remaining, Destroyed);
	});
}

void SLevelHUD::OnLevelChanged(int32 InLevelNumber)
{
	SetLevelNumber(InLevelNumber);
}

void SLevelHUD::OnEnemyCountChanged(int32 Remaining, int32 Destroyed)
{
	SetEnemiesRemaining(Remaining);
	SetEnemiesDestroyed(Destroyed);
}

void SLevelHUD::SetPlayerName(FText InPlayerName)
{
	PlayerName->SetText(InPlayerName);
}

void SLevelHUD::SetLevelNumber(int32 InLevelNumber)
{
	LevelNumber->SetText(FText::AsNumber(InLevelNumber));
}

void SLevelHUD::SetEnemiesRemaining(int32 Remaining)
{
	EnemiesRemaining->SetText(FText::AsNumber(Remaining));
}

void SLevelHUD::SetEnemiesDestroyed(int32 Destroyed)
{
	EnemiesDestroyed->SetText(FText::AsNumber(Destroyed));
}

#undef LOCTEXT_NAMESPACE
