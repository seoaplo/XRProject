// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "PlayerSkill.h"
#include "Image.h"
#include "SkillSlotWidget.generated.h"

/**
 * 
 */
UCLASS()
class XRPROJECT_API USkillSlotWidget : public UUserWidget
{
	GENERATED_BODY()
public:
	UPROPERTY(EditAnywhere, BlueprintReadwrite, Category = "C_Widget")
		UPlayerSkill* TargetSkill;
	UPROPERTY(EditAnywhere, BlueprintReadwrite, Category = "C_Widget")
		UImage* Icon;
public:
	UFUNCTION(BlueprintCallable, Category = "C_Function")
		bool IsEmpty();
	UFUNCTION(BlueprintCallable, Category = "C_Function")
		FString GetName();
	UFUNCTION(BlueprintCallable, Category = "C_Function")
		float GetMaxCooldown();
	UFUNCTION(BlueprintCallable, Category = "C_Function")
		float GetCurCooldown();
	UFUNCTION(BlueprintCallable, Category = "C_Function")
		float GetDamege();
	UFUNCTION(BlueprintCallable, Category = "C_Function")
		void UpdateIcon();
	UFUNCTION(BlueprintCallable, Category = "C_Function")
		void Play();
};
