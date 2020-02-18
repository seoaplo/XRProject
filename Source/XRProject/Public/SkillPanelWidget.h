// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "SkillSlotWidget.h"
#include "SkillPanelWidget.generated.h"

/**
 * 
 */
UCLASS()
class XRPROJECT_API USkillPanelWidget : public UUserWidget
{
	GENERATED_BODY()
public:
	UPROPERTY(EditAnywhere, BlueprintReadwrite, Category = "C_Widget")
		USkillSlotWidget* SkillSlot;
	UFUNCTION(BlueprintImplementableEvent)
		void Update();
};
