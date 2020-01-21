// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Initialization.h"
#include "CoreMinimal.h"
#include "SlotObject.h"
#include "Item.generated.h"

// ------------------------------------------------------------------
// 아이템
// 무기, 장비, 소비, 
// ------------------------------------------------------------------

UCLASS()
class XRPROJECT_API UItem : public USlotObject
{
	GENERATED_BODY()
public:
	UFUNCTION(BlueprintCallable, Category = "C_Function")
		virtual bool Use();
	UFUNCTION(BlueprintCallable, Category = "C_Function")
		virtual int GetCount();
};
