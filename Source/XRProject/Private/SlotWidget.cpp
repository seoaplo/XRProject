#include "SlotWidget.h"
#include "..\Public\SlotWidget.h"
#include "XRGameInstance.h"
#include "Inventory.h"

USlotWidget::USlotWidget(const FObjectInitializer& ObjectInitializer) : UUserWidget(ObjectInitializer)
{
	SlotObject = nullptr;
}

UTexture2D * USlotWidget::GetIcon()
{
	if (!IsEmpty())
	{
		return SlotObject->GetIcon();
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
	if (Index < 0 || Inventory::GetInstance().GetInventorySize() <= Index) return;
	if (IsEquipment)
	{
		GetMapMgr().GetPlayer()->GetEquippedItem((EEquipmentsType)Index);
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
		Inventory::GetInstance().ExchangeItem(Index, Target->Index);
	}
}
