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

UCharacterInfoWidget* UCharacterInfoWidget::GetInstance()
{
	return CharacterInfoInstance;
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
	CharacterInfoInstance = this;
}
