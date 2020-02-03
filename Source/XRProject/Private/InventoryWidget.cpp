#include "InventoryWidget.h"
#include <string>

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

FString UInventoryWidget::GetGlod()
{
	int Money = Inventory::GetInstance().GetGold();
	std::string str = std::to_string(Money);
	int Length = str.length();
	int Remain = Length % 3;

	int Flag = 0;
	int SicleFlag = 0;

	std::string Result;
	for (int i = 0; i < Remain; i++)
	{
		Result += str[Flag];
		Flag++;
		if (Flag == Remain)
		{
			Result += ',';
		}
	}
	while (true)
	{
		if (Flag == Length) break;
		if (SicleFlag == 3)
		{
			Result += ',';
			SicleFlag = 0;
		}
		SicleFlag++;
		Result += str[Flag++];
	}
	FString ReturnValue(Result.c_str());
	return ReturnValue;
}

void UInventoryWidget::AddList(USlotWidget * newslot)
{
	if (newslot)
	{
		newslot->Index = list.size();
		list.push_back(newslot);
		newslot->SetSlotObject();
	}
}