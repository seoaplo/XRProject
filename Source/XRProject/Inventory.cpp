#include "Inventory.h"

Inventory::Inventory()
{
	for (int i = 0; i < kMaxInventorySlotSize; i++)
	{
		Slot[i] = nullptr;
	}
}

Inventory::~Inventory()
{
}

bool Inventory::AddItem(UItem* item, int index)
{
	if (item == nullptr) return false;
	if (index == -1)
	{
		for (int i = 0; i < kMaxInventorySlotSize; i++)
		{
			if (Slot[i] == nullptr)
			{
				Slot[i] = item;
				return true;
			}
		}
		return false;
	}
	else
	{
		if (index < 0 || index >= kMaxInventorySlotSize) return false;
		if (Slot[index] == nullptr)
		{
			Slot[index] = item;
			return true;
		}
	}
	return false;
}

UItem* Inventory::ExchangeItem(UItem * item, int index)
{
	if (item == nullptr) return false;
	if (index < 0 || index >= kMaxInventorySlotSize) return false;
	UItem* result = Slot[index];
	Slot[index] = item;
	return result;
}
