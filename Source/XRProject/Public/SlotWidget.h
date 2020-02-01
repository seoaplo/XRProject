// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "SlotObject.h"
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
		USlotObject* SlotObject;
public:
	UFUNCTION(BlueprintCallable, Category = "C_Function")
		virtual UTexture2D* GetIcon();
	UFUNCTION(BlueprintCallable, Category = "C_Function")
		virtual int GetCount();
	UFUNCTION(BlueprintCallable, Category = "C_Function")
		virtual bool IsEmpty();
	UFUNCTION(BlueprintImplementableEvent)
		void Update();
};
