// Fill out your copyright notice in the Description page of Project Settings.


#include "ItemETC.h"
#include "XRGameInstance.h"
#include "InventoryWidget.h"
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

int UItemETC::GetIconID()
{
	return DefaultInfo.IconResourceID;
}