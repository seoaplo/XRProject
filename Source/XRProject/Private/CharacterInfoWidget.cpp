#include "CharacterInfoWidget.h"

UCharacterInfoWidget* UCharacterInfoWidget::CharacterInfoInstance = nullptr;

UCharacterInfoWidget::UCharacterInfoWidget(const FObjectInitializer& ObjectInitializer) : UUserWidget(ObjectInitializer)
{
	bIsVisible = false;
	BodySlot = nullptr;
	HandSlot = nullptr;
	FootSlot = nullptr;
	WeaponSlot = nullptr;
}

void UCharacterInfoWidget::SetVisible(bool IsVisible)
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

bool UCharacterInfoWidget::GetVisible()
{
	return bIsVisible;
}

void UCharacterInfoWidget::SwitchVisible()
{
	SetVisible(!bIsVisible);
}

void UCharacterInfoWidget::SetSlot(USlotWidget * Body, USlotWidget * Hand, USlotWidget * Foot, USlotWidget * Weapon)
{
	CharacterInfoInstance = this;
	Body->Index = 0;
	Hand->Index = 1;
	Foot->Index = 2;
	Weapon->Index = 3;
	Body->IsEquipment = true;
	Hand->IsEquipment = true;
	Foot->IsEquipment = true;
	Weapon->IsEquipment = true;
	BodySlot = Body;
	HandSlot = Hand;
	FootSlot = Foot;
	WeaponSlot = Weapon;
	Slot[0] = BodySlot;
	Slot[1] = HandSlot;
	Slot[2] = FootSlot;
	Slot[3] = WeaponSlot;
}

void UCharacterInfoWidget::SetSlotInfo()
{
	BodySlot->SetSlotObject();
	HandSlot->SetSlotObject();
	FootSlot->SetSlotObject();
	WeaponSlot->SetSlotObject();
}
