// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "SlotWidget.h"
#include "Text.h"
#include "TextBlock.h"
#include "SlateFontInfo.h"
#include "PanelWidget.h"
#include "SizeBox.h"
#include "SizeBoxSlot.h"
#include "VerticalBoxSlot.h"
#include "VerticalBox.h"
#include "HorizontalBoxSlot.h"
#include "HorizontalBox.h"
#include "TextBlock.h"
#include "Engine/Font.h"
#include "CanvasPanel.h"
#include "Button.h"
#include "Border.h"
#include "Item.h"
#include "ItemEquipment.h"
#include "ItemConsumption.h"
#include "ItemETC.h"
#include "Blueprint/UserWidget.h"
#include "ToolTipWidget.generated.h"

UCLASS()
class XRPROJECT_API UToolTipWidget : public UUserWidget
{
	GENERATED_BODY()
public:
	UToolTipWidget(const FObjectInitializer& ObjectInitializer);
public:
	UPROPERTY(EditAnywhere, BlueprintReadwrite, Category = "C_Widget")
		UObject* FontM;
	UPROPERTY(EditAnywhere, BlueprintReadwrite, Category = "C_Widget")
		UObject* FontL;
	UPROPERTY(EditAnywhere, BlueprintReadwrite, Category = "C_Widget")
		USlotWidget* TargetParent;
	UPROPERTY(EditAnywhere, BlueprintReadwrite, Category = "C_Widget")
		UTextBlock* NameTag;
	UPROPERTY(EditAnywhere, BlueprintReadwrite, Category = "C_Widget")
		UVerticalBox* InfoTag;
public:
	FString FromType(int Type);
	void AddInfo(FString Data);
	UFUNCTION(BlueprintCallable, Category = "C_Function")
		void Update();
};
