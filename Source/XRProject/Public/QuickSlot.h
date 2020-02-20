#pragma once

#include "CoreMinimal.h"
#include "PlayerSkill.h"
#include "Item.h"
#include "Inventory.h"
#include "Image.h"
#include "TextBlock.h"
#include "Blueprint/UserWidget.h"
#include "QuickSlot.generated.h"

UCLASS()
class XRPROJECT_API UQuickSlot : public UUserWidget
{
	GENERATED_BODY()
public:
	UQuickSlot(const FObjectInitializer& ObjectInitializer);
public:
	int SlotIndex;
	UPROPERTY(EditAnywhere, BlueprintReadwrite, Category = "C_Widget")
		UImage* Icon;
	UPROPERTY(EditAnywhere, BlueprintReadwrite, Category = "C_Widget")
		UObject* TargetObject;
	UPROPERTY(EditAnywhere, BlueprintReadwrite, Category = "C_Widget")
		UTextBlock* KeyName;
	bool bIsSkill;
	int ID;
public:
	UFUNCTION(BlueprintCallable, Category = "C_Function")
		virtual void PushButton();
	UFUNCTION(BlueprintCallable, Category = "C_Function")
		virtual int GetCount();
	UFUNCTION(BlueprintCallable, Category = "C_Function")
		virtual bool IsEmpty() { return !TargetObject; };
	UFUNCTION(BlueprintCallable, Category = "C_Function")
		virtual float GetMaxCoolDown();
	UFUNCTION(BlueprintCallable, Category = "C_Function")
		virtual float GetCurCoolDown();
	UFUNCTION(BlueprintCallable, Category = "C_Function")
		virtual void ClearSlot();
	UFUNCTION(BlueprintCallable, Category = "C_Function")
		virtual void SetIcon();
	UFUNCTION(BlueprintCallable, Category = "C_Function")
		void DropIn(UUserWidget* Target);
	UFUNCTION(BlueprintCallable, Category = "C_Function")
		void Clear();
	UFUNCTION(BlueprintImplementableEvent)
		void Update();
};