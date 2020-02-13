// Fill out your copyright notice in the Description page of Project Settings.

#include "ItemEquipment.h"
#include "InventoryWidget.h"
#include "XRGameInstance.h"

UItemEquipment::UItemEquipment()
{

}

int UItemEquipment::GetCount()
{
	return 0;
}

int UItemEquipment::GetID()
{
	return DefaultInfo.ID;
}

int UItemEquipment::GetIconID()
{
	return DefaultInfo.IconResourceID;
}