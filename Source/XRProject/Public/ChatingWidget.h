// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Color.h"
#include "ChatingManager.h"
#include "Blueprint/UserWidget.h"
#include "ChatingWidget.generated.h"




UCLASS()
class XRPROJECT_API UChatingWidget : public UUserWidget
{
	GENERATED_BODY()
public:
	UChatingWidget(const FObjectInitializer& ObjectInitializer);
public:
	static UChatingWidget* ChatingWidgetInstance;
public:
	FLinearColor GetColorFormType(TextType Type);
	void ReciveChat(TextType Type, std::wstring Chat, bool IsEraseLastText = false);
public:
	UFUNCTION(BlueprintCallable, Category = "C_Chat")
		void Setup();
	UFUNCTION(BlueprintCallable, Category = "C_Chat")
		void SendChat(FText Chat);
	UFUNCTION(BlueprintImplementableEvent)
		void AddText(FLinearColor const& ColorPam, FText const& TextPam);
	UFUNCTION(BlueprintImplementableEvent)
		void EraseLastText();
	UFUNCTION(BlueprintImplementableEvent)
		void SendBoxFocus(APlayerController* Target);
};
