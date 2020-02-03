#pragma once

const int kQuickBarSize = 10;

#include "CustomSingleton.h"
#include "Item.h"
#include "Skill.h"

struct QuickData
{
	int Type; // 0 = NULL, 1 = Skill. 2 = Item
	int ObjectIndex;
};

class QuickBarData : public CustomSingleton<QuickBarData>
{
public:
	friend class CustomSingleton<QuickBarData>;
private:
	QuickBarData();
public:
	~QuickBarData();
private:
	QuickData Slot[10];
public:
	void SetSkill(int Index, USkill* Item);
	void SetItem(int Index, UItem* Item);
	void UseQuickSlot(int Index);
	int GetQuickBarSize();
};