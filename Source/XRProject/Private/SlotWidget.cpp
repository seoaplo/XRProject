#include "SlotWidget.h"
#include "..\Public\SlotWidget.h"
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
	if (Index >= 0 || Inventory::GetInstance().GetInventorySize() > Index)
	{
		SlotObject = Inventory::GetInstance().GetItem(Index);
	}
	else
	{
		switch (Index)
		{
		case 100:
		{
			break;
		}
		case 101:
		{
			break;
		}
		case 102:
		{
			break;
		}
		case 103:
		{
			break;
		}
		default:
			break;
		}
	}
	Update();
}

void USlotWidget::DropIn(UUserWidget * SlotWidget)
{
	USlotWidget* Target = Cast<USlotWidget>(SlotWidget);
	if (Target)
	{
		if (Inventory::GetInstance().ExchangeItem(Index, Target->Index))
		{
			SetSlotObject();
			Target->SetSlotObject();
		}
	}
}
