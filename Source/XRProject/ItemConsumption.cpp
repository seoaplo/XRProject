#include "ItemConsumption.h"
#include "InventoryWidget.h"
#include "XRGameInstance.h"

bool UItemConsumption::Use()
{
	return false;
}

int UItemConsumption::GetCount()
{
	return AddInfo.Count;
}

int UItemConsumption::GetID()
{
	return DefaultInfo.ID;
}

void UItemConsumption::SetCount(int Count)
{
	AddInfo.Count = Count;
}

int UItemConsumption::GetIconID()
{
	return DefaultInfo.IconResourceID;
}
