// Fill out your copyright notice in the Description page of Project Settings.


#include "NickNameWidget.h"

#include "CanvasPanel.h"
#include "Button.h"
#include "Border.h"
#include "Widgets/DeclarativeSyntaxSupport.h"
#include "Widgets/Text/STextBlock.h"
#include "Widgets/SInvalidationPanel.h"
#include "Materials/MaterialInstanceDynamic.h"
#include "Widgets/SOverlay.h"
#include "WidgetTree.h"
#include "Widgets/Images/SImage.h"
#include "ConstructorHelpers.h"
#include <string>

UNickNameWidget::UNickNameWidget(const FObjectInitializer& ObjectInitializer) : UUserWidget(ObjectInitializer)
{
	WidgetTree = CreateDefaultSubobject<UWidgetTree>(TEXT("WidgetTree"));
	WidgetTree->SetFlags(RF_Transactional);
	SBox = WidgetTree->ConstructWidget<USizeBox>(USizeBox::StaticClass());


	WidgetTree->RootWidget = SBox;
	SBox->SetHeightOverride(32);

	VBOX = WidgetTree->ConstructWidget<UVerticalBox>(UVerticalBox::StaticClass());
	SBox->AddChild(VBOX);

	USizeBoxSlot* TempSlot = Cast<USizeBoxSlot>(VBOX->Slot);
	if (TempSlot)
	{
		TempSlot->HorizontalAlignment = EHorizontalAlignment::HAlign_Center;
		TempSlot->VerticalAlignment = EVerticalAlignment::VAlign_Center;
	}

	LevelBlock = WidgetTree->ConstructWidget<UTextBlock>(UTextBlock::StaticClass());
	NameBlock = WidgetTree->ConstructWidget<UTextBlock>(UTextBlock::StaticClass());
	TitleBlock = WidgetTree->ConstructWidget<UTextBlock>(UTextBlock::StaticClass());

	VBOX->AddChild(LevelBlock);
	VBOX->AddChild(NameBlock);
	VBOX->AddChild(TitleBlock);

	UVerticalBoxSlot* LevelSlot = Cast<UVerticalBoxSlot>(LevelBlock->Slot);
	if (LevelSlot)
	{
		LevelSlot->HorizontalAlignment = EHorizontalAlignment::HAlign_Center;
		LevelSlot->VerticalAlignment = EVerticalAlignment::VAlign_Center;
	}
	UVerticalBoxSlot* NameSlot = Cast<UVerticalBoxSlot>(NameBlock->Slot);
	if (NameSlot)
	{
		NameSlot->HorizontalAlignment = EHorizontalAlignment::HAlign_Center;
		NameSlot->VerticalAlignment = EVerticalAlignment::VAlign_Center;
	}
	UVerticalBoxSlot* TitleSlot = Cast<UVerticalBoxSlot>(TitleBlock->Slot);
	if (TitleSlot)
	{
		TitleSlot->HorizontalAlignment = EHorizontalAlignment::HAlign_Center;
		TitleSlot->VerticalAlignment = EVerticalAlignment::VAlign_Center;
	}

	static ConstructorHelpers::FObjectFinder<UFont> FontObjM(TEXT("Font'/Game/Resources/UI/Font/Font_NG_Medium_Font.Font_NG_Medium_Font'"));
	static ConstructorHelpers::FObjectFinder<UFont> FontObjL(TEXT("Font'/Game/Resources/UI/Font/Font_NG_Light_Font.Font_NG_Light_Font'"));

	LevelBlock->Font.FontObject = FontObjL.Object;
	NameBlock->Font.FontObject = FontObjM.Object;
	TitleBlock->Font.FontObject = FontObjL.Object;

	LevelBlock->ShadowColorAndOpacity.A = .5;
	NameBlock->ShadowColorAndOpacity.A = .5;
	TitleBlock->ShadowColorAndOpacity.A = .5;

	LevelBlock->Font.Size = 10;
	NameBlock->Font.Size = 12;
	TitleBlock->Font.Size = 10;

	std::wstring WSN(L"NickName");
	std::wstring WST(L"Beginner");
	FString Name(WSN.c_str());
	FString Title(WST.c_str());

	SettingLevel(99);
	SettingName(Name);
	SettingTitle(Title);
}

void UNickNameWidget::SettingLevel(int Level)
{
	FString StrLevel;
	StrLevel.AppendInt(Level);
	FText TextLevel = FText::FromString(StrLevel);
	LevelBlock->SetText(TextLevel);
}

void UNickNameWidget::SettingName(FString Name)
{
	FText TextName = FText::FromString(Name);
	NameBlock->SetText(TextName);
}

void UNickNameWidget::SettingTitle(FString Title)
{
	FString StrTitle = L"<" + Title + L">";
	FText TextTitle = FText::FromString(StrTitle);
	TitleBlock->SetText(TextTitle);
}
