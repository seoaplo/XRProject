#include "..\Public\QuickBarData.h"
#include "NetworkManager.h"

QuickBarData::QuickBarData()
{
	for (int i = 0; i < kQuickBarSize; i++)
	{
		Slot[i].Type = 0;
		Slot[i].ObjectIndex = 0;
	}

}

QuickBarData::~QuickBarData()
{
}

void QuickBarData::SetSkill(int Index, USkill * Item)
{
	if (Index < 0 || kQuickBarSize <= Index) return;

}

void QuickBarData::SetItem(int Index, UItem * Item)
{
	if (Index < 0 || kQuickBarSize <= Index) return;
	if (Item->GetItemType() != EItemType::CONSUMPTION) return;
	Slot[Index].Type = 2;
	Slot[Index].ObjectIndex = Item->GetID();
}

void QuickBarData::UseQuickSlot(int Index)
{
	if (Index < 0 || kQuickBarSize <= Index) return;
	OutputStream Out;
	Out.WriteOpcode(ENetworkCSOpcode::kCharacterSelectionRequest);
	Out << (int32_t)Index;
	Out.CompletePacketBuild();
	UNetworkManager::GetInstance().SendPacket(Out);
}

int QuickBarData::GetQuickBarSize()
{
	return kQuickBarSize;
}
