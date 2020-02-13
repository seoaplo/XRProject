// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "LoadingBarWidget.generated.h"

/**
 * 
 */
UCLASS()
class XRPROJECT_API ULoadingBarWidget : public UUserWidget
{
	GENERATED_BODY()
public:
	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable)
		void ApplyPercentage(float Percentage); // 0~1°ªÀ¸·Î
};
