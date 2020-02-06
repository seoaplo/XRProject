#include "SlotWidget.h"
#include "..\Public\SlotWidget.h"
#include "NetworkManager.h"
#include "Inventory.h"
#include "ItemETC.h"
#include "ItemConsumption.h"
#include "ItemEquipment.h"

USlotWidget::USlotWidget(const FObjectInitializer& ObjectInitializer) : UUserWidget(ObjectInitializer)
{
	SlotObject = nullptr;
}

UTexture2D * USlotWidget::GetIcon()
{
	if (!IsEmpty())
	{
		switch (SlotObject->ItemType)
		{
		case EItemType::ETC:
		{
			UItemETC* Temp = Cast<UItemETC>(SlotObject);
			if(Temp) return Temp->GetIcon();
			break;
		}
		case EItemType::CONSUMPTION:
		{
			UItemConsumption* Temp = Cast<UItemConsumption>(SlotObject);
			if (Temp) return Temp->GetIcon();
			break;
		}
		case EItemType::EQUIPMENT:
		{
			UItemEquipment* Temp = Cast<UItemEquipment>(SlotObject);
			if (Temp) return Temp->GetIcon();
			break;
		}
		default:
			break;
		}
	}
	return nullptr;
}

int USlotWidget::GetCount()
{
	if (!IsEmpty())
	{
		return SlotObject->GetCount();
	}
	return 0;
}

bool USlotWidget::IsEmpty()
{
	return !SlotObject;
}

void USlotWidget::SetSlotObject()
{
	if (Index < 0 || Inventory::GetInstance().GetInventorySize() < Index) return;
	if(IsEquipment)
	{
		//SlotObject = GetItem;
	}
	else
	{
		SlotObject = Inventory::GetInstance().GetItem(Index);
	}
	Update();
}

void USlotWidget::DropIn(UUserWidget * SlotWidget)
{
	USlotWidget* Target = Cast<USlotWidget>(SlotWidget);
	if (Target)
	{
		OutputStream Out;
		Out.WriteOpcode(ENetworkCSOpcode::kInventoryUpdateRequest);
		Out << (int8)IsEquipment;
		Out << (int32)Index;
		Out << (int8)Target->IsEquipment;
		Out << (int32)Target->Index;
		Out.CompletePacketBuild();
		UNetworkManager::GetInstance().SendPacket(Out);
	}
}
