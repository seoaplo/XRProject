// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "GameFramework/InputSettings.h"
#include "VerticalBox.h"
#include <string>
#include <map>
#include "KeyMappingSlotWidget.h"
#include "KeyMappingWidget.generated.h"
/**
 * 
 */
UCLASS()
class XRPROJECT_API UKeyMappingWidget : public UUserWidget
{
	GENERATED_BODY()
public:
	std::map<FName, UKeyMappingSlotWidget*> WidgetList;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "C_WidgetClass")
		TSubclassOf<UKeyMappingSlotWidget> SlotClass;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "C_Widget")
		UVerticalBox* ListBox;
public:
	FName GetHangleName(FName EnglishName, float Scale = 0);
	UFUNCTION(BlueprintCallable)
		void Init();
	UFUNCTION(BlueprintCallable)
		void ChangeKeyAction(UKeyMappingSlotWidget* Target, FKey Key, bool IsShift, bool IsCtrl, bool IsAlt);
};
