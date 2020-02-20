#include "QuickBar.h"
#include "XRGameInstance.h"
#include "NetworkManager.h"
#include "PlayerSkillManager.h"
#include "GameFramework/InputSettings.h"

UQuickBar* UQuickBar::QuickBarInstance = nullptr;

UQuickBar::UQuickBar(const FObjectInitializer& ObjectInitializer) : UUserWidget(ObjectInitializer)
{
}

FName UQuickBar::GetKeyName(FName OriginName)
{
	if (OriginName == FName(L"One")) return FName(L"1");
	if (OriginName == FName(L"Two")) return FName(L"2");
	if (OriginName == FName(L"Three")) return FName(L"3");
	if (OriginName == FName(L"Four")) return FName(L"4");
	if (OriginName == FName(L"Five")) return FName(L"5");
	if (OriginName == FName(L"Six")) return FName(L"6");
	if (OriginName == FName(L"Seven")) return FName(L"7");
	if (OriginName == FName(L"Eight")) return FName(L"8");
	if (OriginName == FName(L"Nine")) return FName(L"9");
	if (OriginName == FName(L"Zero")) return FName(L"0");
	return OriginName;
}

void UQuickBar::RefreshKeyName(int Index)
{
	if (Index >= kQuickBarSize) return;
	if (Index < 0) return;
	FString TempString = L"QuickSlotNum" + FString::FromInt(Index + 1);
	FName TempName(*TempString);
	TArray<FInputActionKeyMapping> ResultArray;
	GetDefault<UInputSettings>()->GetActionMappingByName(TempName, ResultArray);
	if (ResultArray.Num() == 0) return;
	FName RealName = GetKeyName(ResultArray[0].Key.GetFName());
	list[Index]->KeyName->SetText(FText::FromName(RealName));

}

void UQuickBar::SetQuickSlot()
{
	for (int i = 0; i < kQuickBarSize; i++)
	{
		UXRGameInstance* XRGI = Cast<UXRGameInstance>(GetWorld()->GetGameInstance());
		RefreshKeyName(i);
		list[i]->SlotIndex = i;
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
