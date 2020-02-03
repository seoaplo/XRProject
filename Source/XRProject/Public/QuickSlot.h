#pragma once

#include "CoreMinimal.h"
#include "Skill.h"
#include "Item.h"
#include "Inventory.h"
#include "Blueprint/UserWidget.h"
#include "QuickSlot.generated.h"

UCLASS()
class XRPROJECT_API UQuickSlot : public UUserWidget
{
	GENERATED_BODY()
public:
	UQuickSlot(const FObjectInitializer& ObjectInitializer);
public:
	bool bIsEmpty;
	bool bIsSkill;
	int ID;
public:
	UFUNCTION(BlueprintCallable, Category = "C_Function")
		virtual void PushButton();
	UFUNCTION(BlueprintCallable, Category = "C_Function")
		virtual UTexture2D* GetIcon();
	UFUNCTION(BlueprintCallable, Category = "C_Function")
		virtual int GetCount();
	UFUNCTION(BlueprintCallable, Category = "C_Function")
		virtual bool IsEmpty() { return bIsEmpty; };
	UFUNCTION(BlueprintCallable, Category = "C_Function")
		virtual void SetItem(UItem* NewItem);
	UFUNCTION(BlueprintCallable, Category = "C_Function")
		virtual void SetSkill(USkill* NewSkill);
	UFUNCTION(BlueprintCallable, Category = "C_Function")
		virtual void ClearSlot();
	UFUNCTION(BlueprintImplementableEvent)
		void Update();
};