#include "Inventory.h"

Inventory::Inventory()
{
	for (int i = 0; i < kMaxInventorySlotSize; i++)
	{
		Slot[i] = nullptr;
	}
	Gold = 0;
}

Inventory::~Inventory()
{
}

int64 Inventory::GetGold()
{
	return Gold;
}

void Inventory::SetGold(int64 Money)
{
	Gold = Money;
}

void Inventory::AddGold(int64 Money)
{
	Gold += Money;
}

int Inventory::GetInventoryHeight()
{
	return kInventoryHeight;
}

int Inventory::GetInventoryWidth()
{
	return kInventoryWidth;
}

int Inventory::GetInventorySize()
{
	return kMaxInventorySlotSize;
}

UItem * Inventory::GetItem(int index)
{
	if (index < 0 || index >= kMaxInventorySlotSize) return nullptr;
	UItem* result = Slot[index];
	return result;
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
