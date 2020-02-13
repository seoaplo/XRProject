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
#include "CharacterInfoWidget.h"
#include "ExpBar.h"
#include "DungeonResultWidget.h"
#include "LoadingBarWidget.h"
#include "MainMenuWidget.h"
#include "InGameMainWidget.generated.h"

UCLASS()
class XRPROJECT_API UInGameMainWidget : public UUserWidget
{
	GENERATED_BODY()
public:
	UInGameMainWidget(const FObjectInitializer& ObjectInitializer);
public:
	bool bIsVisible;
public:
	UPROPERTY(EditAnywhere, BlueprintReadwrite, Category = "C_Widget")
		UInventoryWidget* Inventory;
	UPROPERTY(EditAnywhere, BlueprintReadwrite, Category = "C_Widget")
		UQuickBar* QuickBar;
	UPROPERTY(EditAnywhere, BlueprintReadwrite, Category = "C_Widget")
		UChatingWidget* ChatingBar;
	UPROPERTY(EditAnywhere, BlueprintReadwrite, Category = "C_Widget")
		UHealthBarWidget* HealthBar;
	UPROPERTY(EditAnywhere, BlueprintReadwrite, Category = "C_Widget")
		UDungeonMatchingWidget* MatchingWindow;
	UPROPERTY(EditAnywhere, BlueprintReadwrite, Category = "C_Widget")
		UCharacterInfoWidget* CharacterInfo;
	UPROPERTY(EditAnywhere, BlueprintReadwrite, Category = "C_Widget")
		UMiniMapWidget* MiniMap;
	UPROPERTY(EditAnywhere, BlueprintReadwrite, Category = "C_Widget")
		UExpBar* ExpBar;
	UPROPERTY(EditAnywhere, BlueprintReadwrite, Category = "C_Widget")
		UMainMenuWidget* MainMenu;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "C_WidgetClass")
		TSubclassOf<UDungeonResultWidget> DungeonResultWidget;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "C_WidgetClass")
		TSubclassOf<ULoadingBarWidget> LoadingBarWidget;
public:
	void CreateDungeonResultWidget(class InputStream& input);
	ULoadingBarWidget* CreateLoadingBarWidget();
};
