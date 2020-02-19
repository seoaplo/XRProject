#include "QuickBar.h"
#include "XRGameInstance.h"
#include "NetworkManager.h"
#include "PlayerSkillManager.h"

UQuickBar* UQuickBar::QuickBarInstance = nullptr;

UQuickBar::UQuickBar(const FObjectInitializer& ObjectInitializer) : UUserWidget(ObjectInitializer)
{
}

void UQuickBar::SetQuickSlot()
{
	for (int i = 0; i < kQuickBarSize; i++)
	{
		UXRGameInstance* XRGI = Cast<UXRGameInstance>(GetWorld()->GetGameInstance());
		switch (QuickBar::GetInstance().Data->Type)
		{
		case 0:
			break;
		case 1:
		{
			UPlayerSkill* Skill = XRGI->GetPlayerSkillManager()->FindSkillFromList(XRGI->GetPlayerSkillManager()->SkillListForPlalyer, QuickBar::GetInstance().Data->ID);
			list[i]->TargetObject = Skill;
			list[i]->bIsSkill = true;
			break;
		}
		case 2:
		{
			UItem* Item = XRGI->ItemManager->GetItemFromId(EItemType::CONSUMPTION, QuickBar::GetInstance().Data->ID).GetValue();
			list[i]->TargetObject = Item; // 스킬 생성한 객체를 주는 걸로
			list[i]->bIsSkill = false;
			break;
		}
		default:
			break;
		}
	}
}

void UQuickBar::AddQuickSlot(UQuickSlot * QuickSlot)
{
	list.push_back(QuickSlot);
}
