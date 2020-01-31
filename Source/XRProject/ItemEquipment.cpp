// Fill out your copyright notice in the Description page of Project Settings.


#include "ItemEquipment.h"
#include "XRGameInstance.h"
UItemEquipment::UItemEquipment()
{

}

int UItemEquipment::GetID()
{
	return DefaultInfo.ID;
}

UTexture2D* UItemEquipment::GetIcon()
{
	UTexture2D* result = nullptr;
	auto GI = Cast<UXRGameInstance>(GetWorld()->GetGameInstance());
	GI->ItemManager->GetIcon(result, DefaultInfo.IconResourceID);
	return result;
}