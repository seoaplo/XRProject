// Fill out your copyright notice in the Description page of Project Settings.


#include "DungeonResultWidget.h"
#include "XRGameInstance.h"

void UDungeonResultWidget::GetReward(InputStream& input)
{
	UXRGameInstance* XRGI = Cast<UXRGameInstance>(GetWorld()->GetGameInstance());
	if (!XRGI) return;
	Gold = input.ReadInt32();
	Exp = input.ReadInt32();
	int ListSize = input.ReadInt32();
	ItemList.SetNum(ListSize);
	for (int i = 0; i < ListSize; i++)
	{
		ItemList[i] = XRGI->ItemManager->CreateItem(input).GetValue();
	}
}