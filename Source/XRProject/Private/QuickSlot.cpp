#include "QuickSlot.h"
#include "SkillSlotWidget.h"
#include "SlotWidget.h"
#include "ItemConsumption.h"
#include "XRGameInstance.h"
#include "PlayerSkillManager.h"
#include "SkillCoolDown.h"

UQuickSlot::UQuickSlot(const FObjectInitializer& ObjectInitializer) : UUserWidget(ObjectInitializer)
{
	bIsSkill = false;
	ID = 0;
}

void UQuickSlot::PushButton()
{
	// ƒ¸ΩΩ∑‘¿Ã ¥≠∏±∂ß π›¿¿
	if (bIsSkill)
	{
		if (GetMapMgr().GetPlayer())
		{
			UPlayerSkill* PlayerSkill = Cast<UPlayerSkill>(TargetObject);
			PlayerSkill->Play(GetMapMgr().GetPlayer());
		}
	}
	else
	{
		UItem* Item = Cast<UItem>(TargetObject);
	}
}

int UQuickSlot::GetCount()
{
	if (IsEmpty() || bIsSkill)
	{
		return -1;
	}
	else
	{
		return Inventory::GetInstance().GetItemCount(ID);
	}
}

float UQuickSlot::GetMaxCoolDown()
{
	if (!IsEmpty() && bIsSkill)
	{
		UPlayerSkill* PlayerSkill = Cast<UPlayerSkill>(TargetObject);
		return PlayerSkill->GetCoolTime();
	}
	else
	{
		return -1;
	}
}

void UQuickSlot::Clear()
{
	TargetObject = nullptr;
	bIsSkill = false;
	ID = 0;
}

void UQuickSlot::SetIcon()
{
	if (IsEmpty())
	{
		Icon->SetVisibility(ESlateVisibility::Hidden);
		return;
	}
	UXRGameInstance* XRGI = Cast<UXRGameInstance>(GetWorld()->GetGameInstance());
	UPlayerSkill* PlayerSkill = Cast<UPlayerSkill>(TargetObject);
	UItem* Item = Cast<UItem>(TargetObject);
	if (PlayerSkill)
	{
		XRGI->ItemManager->GetIcon(Icon, PlayerSkill->GetIconID());
		Icon->SetVisibility(ESlateVisibility::Visible);
		return;
	}
	else if (Item)
	{
		XRGI->ItemManager->GetIcon(Icon, Item->GetIconID());
		Icon->SetVisibility(ESlateVisibility::Visible);
		return;
	}
	Icon->SetVisibility(ESlateVisibility::Hidden);
	return;
}

float UQuickSlot::GetCurCoolDown()
{
	if (!IsEmpty() && bIsSkill)
	{
		UXRGameInstance* XRGI = Cast<UXRGameInstance>(GetWorld()->GetGameInstance());
		int CoolTimeID = XRGI->GetPlayerSkillManager()->FindSkillFromCooldownList(ID);
		return XRGI->GetPlayerSkillManager()->CoolDownList[CoolTimeID]->GetRemainCoolTime();
	}
	else
	{
		return -1;
	}
}

void UQuickSlot::ClearSlot()
{
	ID = 0;
	Update();
}

void UQuickSlot::DropIn(UUserWidget * Target)
{
	UXRGameInstance* XRGI = Cast<UXRGameInstance>(GetWorld()->GetGameInstance());
	if (!XRGI) return;
	USlotWidget* ItemWidget = Cast<USlotWidget>(Target);
	USkillSlotWidget* SkillWidget = Cast<USkillSlotWidget>(Target);
	if (ItemWidget)
	{
		UItemConsumption* Item = Cast<UItemConsumption>(ItemWidget->SlotObject);
		if (!Item) return;
		SetIcon();
		ID = Item->GetID();
		bIsSkill = false;
	}
	else if (SkillWidget)
	{
		TargetObject = SkillWidget->TargetSkill;
		SetIcon();
		ID = SkillWidget->TargetSkill->GetID();
		bIsSkill = true;
	}
}
