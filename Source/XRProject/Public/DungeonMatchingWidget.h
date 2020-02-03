// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "DungeonMatchingWidget.generated.h"

/**
 * 
 */
UCLASS()
class XRPROJECT_API UDungeonMatchingWidget : public UUserWidget
{
	GENERATED_BODY()
public:
	UDungeonMatchingWidget(const FObjectInitializer& ObjectInitializer);
public:
	UPROPERTY(EditAnywhere, BlueprintReadwrite, Category = "C_Info")
		int SelectedDungeonID;
	bool bIsVisible;
public:
	UFUNCTION(BlueprintCallable, Category = "MG_Function")
		void SetVisible(bool IsVisible);
	UFUNCTION(BlueprintCallable, Category = "MG_Function")
		bool GetVisible();
	UFUNCTION(BlueprintCallable, Category = "MG_Function")
		void SwitchVisible();
	UFUNCTION(BlueprintCallable)
		void DungeonMatchingRequest(bool bTrueFalse);
	UFUNCTION(BlueprintCallable)
		void DungeonEnterRequest(bool bTrueFalse);
	UFUNCTION(BlueprintImplementableEvent)
		void DungeonMatchingResult();
	UFUNCTION(BlueprintImplementableEvent)
		void DungeonEnterFail();
};
