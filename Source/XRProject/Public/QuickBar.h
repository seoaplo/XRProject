// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

const int kQuickBarSize = 10;


#include <vector>
#include "InputStream.h"
#include "CoreMinimal.h"
#include "QuickSlot.h"
#include "CustomSingleton.h"
#include "Blueprint/UserWidget.h"
#include "QuickBar.generated.h"

struct QuickSlotData
{
	int8 Type;
	int32 ID;
};

class QuickBar : public CustomSingleton<QuickBar>
{
public:
	friend class CustomSingleton<QuickBar>;
private:
	QuickBar() {};
public:
	~QuickBar() {};
public:
	QuickSlotData Data[10];
};

UCLASS()
class XRPROJECT_API UQuickBar : public UUserWidget
{
	GENERATED_BODY()
public:
	UQuickBar(const FObjectInitializer& ObjectInitializer);
public:
	static UQuickBar* QuickBarInstance;
	static UQuickBar* GetInstance() { return QuickBarInstance; };
	UFUNCTION(BlueprintCallable, Category = "MG_Function")
		void SetInstance() { QuickBarInstance = this; };
	UFUNCTION(BlueprintCallable, Category = "MG_Function")
		void ClearInstance() { QuickBarInstance = nullptr; };
	std::vector<UQuickSlot*> list;
public:
	FName GetKeyName(FName OriginName);
	void RefreshKeyName(int Index);
	void SetQuickSlot();
	UFUNCTION(BlueprintCallable, Category = "MG_Function")
		void AddQuickSlot(UQuickSlot* QuickSlot);
};
