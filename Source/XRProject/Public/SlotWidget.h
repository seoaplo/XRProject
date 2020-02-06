// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Item.h"
#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "SlotWidget.generated.h"

/**
 * 
 */
UCLASS()
class XRPROJECT_API USlotWidget : public UUserWidget
{
	GENERATED_BODY()
public:
	USlotWidget(const FObjectInitializer& ObjectInitializer);
public:
		UItem* SlotObject;
		bool IsEquipment;
		int Index;
public:
	UFUNCTION(BlueprintCallable, Category = "C_Function")
		void SetSlotObject();
	UFUNCTION(BlueprintCallable, Category = "C_Function")
		virtual UTexture2D* GetIcon();
	UFUNCTION(BlueprintCallable, Category = "C_Function")
		virtual int GetCount();
	UFUNCTION(BlueprintCallable, Category = "C_Function")
		virtual bool IsEmpty();
	UFUNCTION(BlueprintCallable, Category = "C_Function")
		void DropIn(UUserWidget * SlotWidget);
	UFUNCTION(BlueprintImplementableEvent)
		void Update();
};
