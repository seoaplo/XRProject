// Fill out your copyright notice in the Description page of Project Settings.


#include "InventoryWidget.h"

UInventoryWidget::UInventoryWidget(const FObjectInitializer& ObjectInitializer) : UUserWidget(ObjectInitializer)
{
	bIsVisible = false;
}

void UInventoryWidget::SetVisible(bool IsVisible)
{
	bIsVisible = IsVisible;
	if (bIsVisible)
	{
		Visibility = ESlateVisibility::Visible;
	}
	else
	{
		Visibility = ESlateVisibility::Hidden;
	}
}

bool UInventoryWidget::GetVisible()
{
	return bIsVisible;
}

void UInventoryWidget::SwitchVisible()
{
	bIsVisible = !bIsVisible;
	if (bIsVisible)
	{
		Visibility = ESlateVisibility::Visible;
	}
	else
	{
		Visibility = ESlateVisibility::Hidden;
	}
}

int UInventoryWidget::GetWidthSize()
{
	return Inventory::GetInstance().GetInventoryWidth();
}

int UInventoryWidget::GetHeightSize()
{
	return Inventory::GetInstance().GetInventoryHeight();
}

void UInventoryWidget::SetUp()
{
	int Size = Inventory::GetInstance().GetInventorySize();
	for (int i = 0; i < Size; i++)
	{
		list[i]->SlotObject = Inventory::GetInstance().GetItem(i);
	}
}

void UInventoryWidget::AddList(USlotWidget * newslot)
{
	if (newslot) list.push_back(newslot);
}
