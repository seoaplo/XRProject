#pragma once

const int kMaxInventorySlotSize = 30;

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
public:
	bool AddItem(UItem* item, int index = -1); // -1의 경우 빈 슬롯 찾아서 넣기, 그 외의 경우 해당 인덱스에
	UItem* ExchangeItem(UItem* item, int index); // 해당 인덱스의 슬롯에 아이템을 넣고 받아감 없으면 놀포인터
};