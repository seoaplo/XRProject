// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include <vector>
#include "Inventory.h"
#include "SlotWidget.h"
#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "InvertoryWidget.generated.h"



UCLASS()
class XRPROJECT_API UInvertoryWidget : public UUserWidget
{
	GENERATED_BODY()
public:
	UInvertoryWidget(const FObjectInitializer& ObjectInitializer);
public:
	std::vector<USlotWidget*> list;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "MG_Info")
		bool bIsVisible;
public:
	UFUNCTION(BlueprintCallable, Category = "MG_Function")
		int GetWidthSize();
	UFUNCTION(BlueprintCallable, Category = "MG_Function")
		int GetHeightSize();
public:
	UFUNCTION(BlueprintCallable, Category = "MG_Function")
		void SetUp();

};
