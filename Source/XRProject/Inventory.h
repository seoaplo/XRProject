#pragma once

const int kInventoryHeight = 6;
const int kInventoryWidth = 5;
const int kMaxInventorySlotSize = kInventoryHeight * kInventoryWidth;

#include "CustomSingleton.h"
#include "Item.h"

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
	int64 GetGold();
	void SetGold(int64 Money);
	void AddGold(int64 Money);
	int GetInventoryHeight();
	int GetInventoryWidth();
	int GetInventorySize();
	UItem* GetItem(int index); // 아이템 객체 리턴
	bool AddItem(UItem* item, int index = -1); // -1의 경우 빈 슬롯 찾아서 넣기, 그 외의 경우 해당 인덱스에
	UItem* ExchangeItem(UItem* item, int index); // 해당 인덱스의 슬롯에 아이템을 넣고 받아감 없으면 놀포인터
};