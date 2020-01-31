// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "InventoryWidget.h"
#include "QuickBar.h"
#include "Blueprint/UserWidget.h"
#include "InGameMainWidget.generated.h"

/**
 * 
 */
UCLASS()
class XRPROJECT_API UInGameMainWidget : public UUserWidget
{
	GENERATED_BODY()
public:

	UInGameMainWidget(const FObjectInitializer& ObjectInitializer);
public:
	bool bIsVisible;
public:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "C_GameMode")
		UInventoryWidget* Inventory;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "C_GameMode")
		UQuickBar* QuickBar;
};
