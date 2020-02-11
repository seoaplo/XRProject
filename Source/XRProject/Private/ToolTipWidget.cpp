#include "ToolTipWidget.h"
#include "ConstructorHelpers.h"
#include "WidgetTree.h"

UToolTipWidget::UToolTipWidget(const FObjectInitializer& ObjectInitializer) : UUserWidget(ObjectInitializer)
{

}

FString UToolTipWidget::FromType(int Type)
{
	switch (Type)
	{
	case 0: return FString(L"옷");
	case 1:	return FString(L"장갑");
	case 2:	return FString(L"신발");
	case 3:	return FString(L"무기");
	default:
		break;
	}
	return FString();
}

void UToolTipWidget::AddInfo(FString Data)
{
	UTextBlock* AddText = WidgetTree->ConstructWidget<UTextBlock>(UTextBlock::StaticClass());

	FText AText = FText::FromString(Data);
	AddText->SetText(AText);
	AddText->Font.FontObject = FontL;
	AddText->Font.Size = 10;

	InfoTag->AddChild(AddText);

	UVerticalBoxSlot* VSlot = Cast<UVerticalBoxSlot>(AddText->Slot);
	if (VSlot)
	{
		VSlot->HorizontalAlignment = EHorizontalAlignment::HAlign_Left;
		VSlot->VerticalAlignment = EVerticalAlignment::VAlign_Center;
	}
}

void UToolTipWidget::Update()
{
	if (!TargetParent || !NameTag || !InfoTag) return;
	UItem* Target = Cast<UItem>(TargetParent->SlotObject);
	if (!Target) return;
	switch (Target->GetItemType())
	{
	case EItemType::ETC:
	{
		UItemETC* CastingTarget = Cast<UItemETC>(Target);
		if (!CastingTarget) return;
		FText Name = FText::FromString(CastingTarget->DefaultInfo.Name);
		NameTag->SetText(Name);


		AddInfo(CastingTarget->DefaultInfo.ToolTip);
		break;
	}
	case EItemType::CONSUMPTION:
	{
		UItemConsumption* CastingTarget = Cast<UItemConsumption>(Target);
		if (!CastingTarget) return;
		FText Name = FText::FromString(CastingTarget->DefaultInfo.Name);
		NameTag->SetText(Name);

		if (CastingTarget->DefaultInfo.RecoveryHP) AddInfo(L"HP 회복 : " + FString::FromInt(CastingTarget->DefaultInfo.RecoveryHP));
		if (CastingTarget->DefaultInfo.RecoveryStamina) AddInfo(L"스태미나 회복 : " + FString::FromInt(CastingTarget->DefaultInfo.RecoveryStamina));

		AddInfo(CastingTarget->DefaultInfo.ToolTip);
		break;
	}
	case EItemType::EQUIPMENT:
	{
		UItemEquipment* CastingTarget = Cast<UItemEquipment>(Target);
		if (!CastingTarget) return;
		FText Name = FText::FromString(CastingTarget->DefaultInfo.Name);
		NameTag->SetText(Name);
		AddInfo(L"장비 분류 : " + FromType(CastingTarget->DefaultInfo.Type));
		AddInfo(L"레벨 제한 : " + FString::FromInt(CastingTarget->DefaultInfo.ReqLEV));
		if (CastingTarget->DefaultInfo.ReqSTR) AddInfo(L"힘 제한 : " + FString::FromInt(CastingTarget->DefaultInfo.ReqSTR));
		if (CastingTarget->DefaultInfo.ReqDEX) AddInfo(L"민첩 제한 : " + FString::FromInt(CastingTarget->DefaultInfo.ReqDEX));
		if (CastingTarget->DefaultInfo.ReqINT) AddInfo(L"지능 제한 : " + FString::FromInt(CastingTarget->DefaultInfo.ReqINT));
		if (CastingTarget->DefaultInfo.ATK) AddInfo(L"공격력 : +" + FString::FromInt(CastingTarget->DefaultInfo.ATK));
		if (CastingTarget->DefaultInfo.DEF) AddInfo(L"방어력 : +" + FString::FromInt(CastingTarget->DefaultInfo.DEF));
		if (CastingTarget->DefaultInfo.STR) AddInfo(L"힘 : +" + FString::FromInt(CastingTarget->DefaultInfo.STR));
		if (CastingTarget->DefaultInfo.DEX) AddInfo(L"민첩 : +" + FString::FromInt(CastingTarget->DefaultInfo.DEX));
		if (CastingTarget->DefaultInfo.INT) AddInfo(L"지능 : +" + FString::FromInt(CastingTarget->DefaultInfo.INT));
		AddInfo(CastingTarget->DefaultInfo.ToolTip);
		break;
	}
	default:
		break;
	}
}
