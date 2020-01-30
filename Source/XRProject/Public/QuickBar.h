// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

const int kQuickBarSize = 10;


#include <vector>
#include "InputStream.h"
#include "CoreMinimal.h"
#include "QuickSlot.h"
#include "Blueprint/UserWidget.h"
#include "QuickBar.generated.h"

/**
 * 
 */
UCLASS()
class XRPROJECT_API UQuickBar : public UUserWidget
{
	GENERATED_BODY()
public:
	UQuickBar(const FObjectInitializer& ObjectInitializer);
public:
	std::vector<UQuickSlot*> list;
public:
	void SetQuickSlot(InputStream& input);
	UFUNCTION(BlueprintCallable, Category = "MG_Function")
		void AddQuickSlot(UQuickSlot* QuickSlot);
};
