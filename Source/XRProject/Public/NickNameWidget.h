// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "TextBlock.h"
#include "SlateFontInfo.h"
#include "PanelWidget.h"
#include "SizeBox.h"
#include "SizeBoxSlot.h"
#include "VerticalBoxSlot.h"
#include "VerticalBox.h"
#include "HorizontalBox.h"
#include "TextBlock.h"
#include "Engine/Font.h"
#include "CanvasPanel.h"
#include "Button.h"
#include "Border.h"
#include "NickNameWidget.generated.h"

/**
 * 
 */
UCLASS()
class XRPROJECT_API UNickNameWidget : public UUserWidget
{
	GENERATED_BODY()
public:
	UNickNameWidget(const FObjectInitializer& ObjectInitializer);
public:
	UPROPERTY()
		USizeBox* SBox;
	UPROPERTY()
		UVerticalBox* VBOX;
	UPROPERTY()
		UTextBlock* LevelBlock;
	UPROPERTY()
		UTextBlock* NameBlock;
	UPROPERTY()
		UTextBlock* TitleBlock;
	UFUNCTION()
		void SettingLevel(int Level);
	UFUNCTION()
		void SettingName(FString Name);
	UFUNCTION()
		void SettingTitle(FString Title);
};
