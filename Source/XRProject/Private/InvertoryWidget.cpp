// Fill out your copyright notice in the Description page of Project Settings.


#include "InvertoryWidget.h"

UInvertoryWidget::UInvertoryWidget(const FObjectInitializer& ObjectInitializer) : UUserWidget(ObjectInitializer)
{
	UE_LOG(LogTemp, Warning, TEXT("INCREDIBUILD"));
	bIsVisible = false;
}

int UInvertoryWidget::GetWidthSize()
{
	return Inventory::GetInstance().GetInventoryWidth();
}

int UInvertoryWidget::GetHeightSize()
{
	return Inventory::GetInstance().GetInventoryHeight();
}

void UInvertoryWidget::SetUp()
{
	int Size = Inventory::GetInstance().GetInventorySize();
	list.resize(Size);
	for (int i = 0; i < Size; i++)
	{
		list[i]->SlotObject = Inventory::GetInstance().GetItem(i);
	}
}
