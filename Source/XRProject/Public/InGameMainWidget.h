// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "InventoryWidget.h"
#include "QuickBar.h"
#include "ChatingWidget.h"
#include "HealthBarWidget.h"
#include "DungeonMatchingWidget.h"
#include "Blueprint/UserWidget.h"
#include "MiniMapWidget.h"
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
	UPROPERTY(EditAnywhere, BlueprintReadwrite, Category = "C_GameMode")
		UInventoryWidget* Inventory;
	UPROPERTY(EditAnywhere, BlueprintReadwrite, Category = "C_GameMode")
		UQuickBar* QuickBar;
	UPROPERTY(EditAnywhere, BlueprintReadwrite, Category = "C_GameMode")
		UChatingWidget* ChatingBar;
	UPROPERTY(EditAnywhere, BlueprintReadwrite, Category = "C_GameMode")
		UHealthBarWidget* HealthBar;
	UPROPERTY(EditAnywhere, BlueprintReadwrite, Category = "C_GameMode")
		UDungeonMatchingWidget* MatchingWindow;
	UPROPERTY(EditAnywhere, BlueprintReadwrite, Category = "C_GameMode")
		UMiniMapWidget* MiniMap;
};
