// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "XRGameInstance.h"
#include "PlayerSkill.h"
#include "PlayerSkillManager.h"
#include "SkillSlotWidget.h"
#include "SkillPanelWidget.h"
#include "VerticalBox.h"
#include "VerticalBoxSlot.h"
#include "SkillWindowWidget.generated.h"

/**
 * 
 */
UCLASS()
class XRPROJECT_API USkillWindowWidget : public UUserWidget
{
	GENERATED_BODY()
public:
	bool bIsVisible;
	UFUNCTION(BlueprintCallable, Category = "MG_Function")
		void SetVisible(bool IsVisible);
	UFUNCTION(BlueprintCallable, Category = "MG_Function")
		bool GetVisible();
	UFUNCTION(BlueprintCallable, Category = "MG_Function")
		void SwitchVisible();
	UPROPERTY(EditAnywhere, BlueprintReadwrite, Category = "C_Widget")
		TSubclassOf<USkillSlotWidget> SkillSlot;
	UPROPERTY(EditAnywhere, BlueprintReadwrite, Category = "C_Widget")
		TSubclassOf<USkillPanelWidget> SkillPanel;
	UPROPERTY(EditAnywhere, BlueprintReadwrite, Category = "C_List")
		UVerticalBox* ListBox;
public:
	UFUNCTION(BlueprintCallable, Category = "C_Function")
		void CreateSkillList();
};
