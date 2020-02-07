#pragma once
#include "CustomSingleton.h"
#include "Item.h"

const int kInventoryHeight = 6;
const int kInventoryWidth = 5;
const int kMaxInventorySlotSize = kInventoryHeight * kInventoryWidth;

class Inventory : public CustomSingleton<Inventory>
{
public:
	friend class CustomSingleton<Inventory>;
private:
	Inventory();
public:
	~Inventory();
private:
	UItem* Slot[kMaxInventorySlotSize];
	int64 Gold;
public:
	bool UseSlot(int SlotID);
	int64 GetGold();
	void SetGold(int64 Money);
	void AddGold(int64 Money);
	int GetInventoryHeight();
	int GetInventoryWidth();
	int GetInventorySize();
	int GetItemCount(int64 ID);
	UItem* GetItem(int index); // 아이템 객체 리턴
	void SetItem(UItem* Item, int Index);
	bool AddItem(UItem* item, int index); // -1의 경우 빈 슬롯 찾아서 넣기, 그 외의 경우 해당 인덱스에
	void ExchangeItem(int index1, int index2);
};