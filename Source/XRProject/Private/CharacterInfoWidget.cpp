#include "CharacterInfoWidget.h"


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
	Body->Index = 100;
	Hand->Index = 101;
	Foot->Index = 102;
	Weapon->Index = 103;
	BodySlot = Body;
	HandSlot = Hand;
	FootSlot = Foot;
	WeaponSlot = Weapon;
}
