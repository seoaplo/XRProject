// Fill out your copyright notice in the Description page of Project Settings.


#include "ItemETC.h"
#include "XRGameInstance.h"
#include "InventoryWidget.h"

int UItemETC::GetCount()
{
	return AddInfo.Count;
}

int UItemETC::GetID()
{
	return DefaultInfo.ID;
}

void UItemETC::SetCount(int Count)
{
	AddInfo.Count = Count;
}

UTexture2D* UItemETC::GetIcon()
{
	UTexture2D* result = nullptr;
	auto GI = Cast<UXRGameInstance>(UInventoryWidget::GetInstance()->GetWorld()->GetGameInstance());
	GI->ItemManager->GetIcon(result, DefaultInfo.IconResourceID);
	return result;
}