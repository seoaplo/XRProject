#include "Inventory.h"
#include "NetworkManager.h"
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

int Inventory::GetItemCount(int64 ID)
{
	int Count = 0;
	for (int i = 0; i < kMaxInventorySlotSize; i++)
	{
		if (Slot[i])
		{
			if (Slot[i]->GetID() == ID)
			{
				Count += Slot[i]->GetCount();
			}
		}

	}
	return Count;
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

bool Inventory::ExchangeItem(int index1, int index2)
{
	if (index1 < 0 || index1 >= kMaxInventorySlotSize) return false;
	if (index2 < 0 || index2 >= kMaxInventorySlotSize) return false;
	UItem* Temp = Slot[index1];
	Slot[index2] = Temp;
	Slot[index1] = Slot[index2];

	OutputStream Out;

	Out.WriteOpcode(ENetworkCSOpcode::kInventoryUpdateRequest);
	Out << (int32_t)index1;
	Out << (int32_t)index2;
	Out.CompletePacketBuild();

	UNetworkManager::GetInstance().SendPacket(Out);
	return true;
}

bool Inventory::UseSlot(int SlotID)
{
	if (SlotID < 0 || SlotID >= kMaxInventorySlotSize) return false;
	if (Slot[SlotID])
	{
		switch (Slot[SlotID]->GetItemType())
		{
		case EItemType::EQUIPMENT:
		{

		}
		case EItemType::CONSUMPTION:
		{
			OutputStream Out;
			Out.WriteOpcode(ENetworkCSOpcode::kNotifyCurrentChrPosition);
			Out << (int32_t)SlotID;
			Out.CompletePacketBuild();
			return true;
		}
		default:
			break;
		}
	}
	return false;
}