#include "KeyMappingWidget.h"
#include "WidgetTree.h"

void UKeyMappingWidget::Init()
{
	TArray<FInputAxisKeyMapping> AxisList = GetDefault<UInputSettings>()->GetAxisMappings();
	for (int i = 0; i < AxisList.Num(); i++)
	{
		if (AxisList[i].AxisName == FName(L"Turn")) continue;
		if (AxisList[i].AxisName == FName(L"LookUp")) continue;
		UKeyMappingSlotWidget* NewSlot = CreateWidget<UKeyMappingSlotWidget>(GetWorld(), SlotClass);
		if (NewSlot)
		{
			ListBox->AddChild(NewSlot);
			NewSlot->ActionName = AxisList[i].AxisName;
			NewSlot->DisplayName = GetHangleName(AxisList[i].AxisName, AxisList[i].Scale);
			NewSlot->TActionName->SetText(FText::FromName(NewSlot->DisplayName));
			NewSlot->TKeyName->SetText(FText::FromName(AxisList[i].Key.GetFName()));
			NewSlot->Scale = AxisList[i].Scale;
			NewSlot->ParentWidget = this;
			WidgetList.insert(std::make_pair(AxisList[i].AxisName, NewSlot));
		}
	}
	TArray<FInputActionKeyMapping> KeyList = GetDefault<UInputSettings>()->GetActionMappings();
	for (int i = 0; i < KeyList.Num(); i++)
	{
		if (KeyList[i].ActionName == FName(L"Attack")) continue;
		UKeyMappingSlotWidget* NewSlot = CreateWidget<UKeyMappingSlotWidget>(GetWorld(), SlotClass);
		if (NewSlot)
		{
			ListBox->AddChild(NewSlot);
			NewSlot->ActionName = KeyList[i].ActionName;
			NewSlot->DisplayName = GetHangleName(KeyList[i].ActionName);
			NewSlot->TActionName->SetText(FText::FromName(NewSlot->DisplayName));
			NewSlot->TKeyName->SetText(FText::FromName(KeyList[i].Key.GetFName()));
			NewSlot->Scale = 0;
			NewSlot->ParentWidget = this;
			WidgetList.insert(std::make_pair(KeyList[i].ActionName, NewSlot));
		}
	}
}

FName UKeyMappingWidget::GetHangleName(FName EnglishName, float Scale)
{
	if (EnglishName == FName(L"Forward")) return (Scale == 1) ? FName(L"앞으로 이동") : FName(L"뒤로 이동");
	if (EnglishName == FName(L"Right")) return (Scale == 1) ? FName(L"오른쪽 이동") : FName(L"왼쪽 이동");
	if (EnglishName == FName(L"Sprint")) return FName(L"달리기");
	if (EnglishName == FName(L"Roll")) return FName(L"구르기");
	if (EnglishName == FName(L"ShowCursor")) return FName(L"마우스 보이기");
	if (EnglishName == FName(L"OpenInventory")) return FName(L"가방 열기");
	if (EnglishName == FName(L"OpenCharacterInfo")) return FName(L"캐릭터창 열기");
	if (EnglishName == FName(L"OpenPartyMatching")) return FName(L"던전 매칭창 열기");
	if (EnglishName == FName(L"QuickSlotNum1")) return FName(L"1번 퀵슬롯");
	if (EnglishName == FName(L"QuickSlotNum2")) return FName(L"2번 퀵슬롯");
	if (EnglishName == FName(L"QuickSlotNum3")) return FName(L"3번 퀵슬롯");
	if (EnglishName == FName(L"QuickSlotNum4")) return FName(L"4번 퀵슬롯");
	if (EnglishName == FName(L"QuickSlotNum5")) return FName(L"5번 퀵슬롯");
	if (EnglishName == FName(L"QuickSlotNum6")) return FName(L"6번 퀵슬롯");
	if (EnglishName == FName(L"QuickSlotNum7")) return FName(L"7번 퀵슬롯");
	if (EnglishName == FName(L"QuickSlotNum8")) return FName(L"8번 퀵슬롯");
	if (EnglishName == FName(L"QuickSlotNum9")) return FName(L"9번 퀵슬롯");
	if (EnglishName == FName(L"QuickSlotNum10")) return FName(L"10번 퀵슬롯");
	return EnglishName;
}

void UKeyMappingWidget::ChangeKeyAction(UKeyMappingSlotWidget* Target, FKey Key, bool IsShift, bool IsCtrl, bool IsAlt)
{
	if (Target->Scale == 0)
	{
		TArray<FInputActionKeyMapping> OutMappings;
		UInputSettings::GetInputSettings()->GetActionMappingByName(Target->ActionName, OutMappings);
		if (OutMappings.Num() == 0) return;
		UInputSettings::GetInputSettings()->RemoveActionMapping(OutMappings[0]);

		const FInputActionKeyMapping NewKeyMap(Target->ActionName, Key, IsShift, IsCtrl, IsAlt);
		UInputSettings::GetInputSettings()->AddActionMapping(NewKeyMap, true);
	}
	else
	{
		TArray<FInputAxisKeyMapping> OutMappings;
		UInputSettings::GetInputSettings()->GetAxisMappingByName(Target->ActionName, OutMappings);
		if (OutMappings.Num() == 0) return;
		for (int i = 0; i < OutMappings.Num(); i++)
		{
			if (OutMappings[i].Scale != Target->Scale) continue;
			UInputSettings::GetInputSettings()->RemoveAxisMapping(OutMappings[i]);
		}

		const FInputAxisKeyMapping NewKeyMap(Target->ActionName, Key, Target->Scale);
		UInputSettings::GetInputSettings()->AddAxisMapping(NewKeyMap, true);
	}
	Target->TKeyName->SetText(FText::FromName(Key.GetFName()));
}