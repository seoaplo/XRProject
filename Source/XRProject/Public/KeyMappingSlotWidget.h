// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "TextBlock.h"
#include "KeyMappingSlotWidget.generated.h"

/**
 * 
 */
UCLASS()
class XRPROJECT_API UKeyMappingSlotWidget : public UUserWidget
{
	GENERATED_BODY()
public:
	float Scale;
	FName ActionName;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "C_Widget")
		FName DisplayName;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "C_Widget")
		UTextBlock* TActionName;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "C_Widget")
		UTextBlock* TKeyName;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "C_Widget")
		UUserWidget* ParentWidget;
};
