#include "InventoryWidget.h"
#include "XRProject.h"
#include <string>

UInventoryWidget* UInventoryWidget::InventoryInstance = nullptr;

UInventoryWidget* UInventoryWidget::GetInstance()
{
	return InventoryInstance;
}
UInventoryWidget::UInventoryWidget(const FObjectInitializer& ObjectInitializer) : UUserWidget(ObjectInitializer)
{
	bIsVisible = false;
}

void UInventoryWidget::SetVisible(bool IsVisible)
{
	bIsVisible = IsVisible;
	if (bIsVisible)
	{
		SetVisibility(ESlateVisibility::Visible);
	}
	else
	{
		SetVisibility(ESlateVisibility::Hidden);
	}
}

bool UInventoryWidget::GetVisible()
{
	return bIsVisible;
}

void UInventoryWidget::SwitchVisible()
{
	SetVisible(!bIsVisible);
}

int UInventoryWidget::GetWidthSize()
{
	return Inventory::GetInstance().GetInventoryWidth();
}

int UInventoryWidget::GetHeightSize()
{
	return Inventory::GetInstance().GetInventoryHeight();
}

void UInventoryWidget::SetUp()
{
	InventoryInstance = this;
	int Size = Inventory::GetInstance().GetInventorySize();
	for (int i = 0; i < Size; i++)
	{
		list[i]->Index = i;
		list[i]->IsEquipment = false;
		list[i]->SetSlotObject();
	}
}

FText UInventoryWidget::GetGlod()
{
	int Money = Inventory::GetInstance().GetGold();
	return int_to_comma_text(Money);
}

void UInventoryWidget::AddList(USlotWidget * newslot)
{
	if (newslot) list.push_back(newslot);
}

void UInventoryWidget::ClearInstance()
{
	InventoryInstance = nullptr;
}
