#include "ItemConsumption.h"

bool UItemConsumption::Use()
{
	return false;
}

int UItemConsumption::GetCount()
{
	return AddInfo.Count;
}
