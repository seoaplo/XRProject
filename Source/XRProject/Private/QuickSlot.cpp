// Fill out your copyright notice in the Description page of Project Settings.


#include "QuickSlot.h"


UQuickSlot::UQuickSlot(const FObjectInitializer& ObjectInitializer) : UUserWidget(ObjectInitializer)
{
	bIsEmpty = true;
	bIsSkill = false;
	ID = 0;
}

void UQuickSlot::PushButton()
{
	// 퀵슬롯이 눌릴때 반응
}

UTexture2D * UQuickSlot::GetIcon()
{
	if (!bIsEmpty)
	{

	}
	return nullptr;
}

int UQuickSlot::GetCount()
{
	if (bIsEmpty || bIsSkill)
	{
		return 0;
	}
	else
	{
		return Inventory::GetInstance().GetItemCount(ID);
	}
}

void UQuickSlot::SetItem(UItem* NewItem)
{
	if (NewItem->GetItemType() == EItemType::CONSUMPTION)
	{
		bIsEmpty = false;
		bIsSkill = false;
		ID = NewItem->GetID();
	}
}

void UQuickSlot::SetSkill(UPlayerSkill* NewSkill)
{
	if (false) // 스킬 구현하고 액티브인 경우에만 들어올수 있도록 한다.
	{
		bIsEmpty = false;
		bIsSkill = true;
		//ID = NewSkill->GetID();
	}
}

void UQuickSlot::ClearSlot()
{
	bIsEmpty = true;
	bIsSkill = false;
	ID = 0;
	Update();
}
