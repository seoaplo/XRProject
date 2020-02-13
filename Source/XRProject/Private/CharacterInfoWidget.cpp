#include "CharacterInfoWidget.h"
#include "XRProject.h"
#include "XRGameInstance.h"

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
	SetCharacterInfo();
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

FText UCharacterInfoWidget::GetTargetState(EStateType Type)
{
	UXRGameInstance* XRGI = Cast<UXRGameInstance>(GetWorld()->GetGameInstance());
	if (!GetMapMgr().GetPlayer()) return FText();
	int Data = 0;
	switch (Type)
	{
	case EStateType::Name:
		return FText::FromString(GetMapMgr().GetPlayer()->PlayerStatComp->GetCharacterName());
		break;
	case EStateType::LEVEL:
		Data = GetMapMgr().GetPlayer()->PlayerStatComp->GetLevel();
		break;
	case EStateType::Gender:
		Data = GetMapMgr().GetPlayer()->PlayerStatComp->GetGender();
		break;
	case EStateType::MaxHP:
		Data = GetMapMgr().GetPlayer()->PlayerStatComp->GetMaxHP();
		break;
	case EStateType::MaxStamina:
		Data = GetMapMgr().GetPlayer()->PlayerStatComp->GetMaxStamina();
		break;
	case EStateType::ATK_Min:
		Data = GetMapMgr().GetPlayer()->PlayerStatComp->GetAttack_Min();
		break;
	case EStateType::ATK_Max:
		Data = GetMapMgr().GetPlayer()->PlayerStatComp->GetAttack_Max();
		break;
	case EStateType::ATK_Range:
		Data = GetMapMgr().GetPlayer()->PlayerStatComp->GetAttack_Range();
		break;
	case EStateType::ATK_Speed:
		Data = GetMapMgr().GetPlayer()->PlayerStatComp->GetAttack_Speed();
		break;
	case EStateType::DEF:
		Data = GetMapMgr().GetPlayer()->PlayerStatComp->GetDefence();
		break;
	case EStateType::STR:
		Data = GetMapMgr().GetPlayer()->PlayerStatComp->GetSTR();
		break;
	case EStateType::DEX:
		Data = GetMapMgr().GetPlayer()->PlayerStatComp->GetDEX();
		break;
	case EStateType::INT:
		Data = GetMapMgr().GetPlayer()->PlayerStatComp->GetINT();
		break;
	default:
		break;
	}
	return int_to_comma_text(Data);
}

void UCharacterInfoWidget::SetSlotInfo()
{
	BodySlot->SetSlotObject();
	HandSlot->SetSlotObject();
	FootSlot->SetSlotObject();
	WeaponSlot->SetSlotObject();
}
