// Fill out your copyright notice in the Description page of Project Settings.


#include "QuickBar.h"

UQuickBar::UQuickBar(const FObjectInitializer& ObjectInitializer) : UUserWidget(ObjectInitializer)
{
}

void UQuickBar::SetQuickSlot(InputStream & input)
{
	for (int i = 0; i < QuickBarData::GetInstance().GetQuickBarSize(); i++)
	{
		int32 SlotNum;
		int8 Type;
		input >> SlotNum;
		input >> Type;
		switch (Type)
		{
		case 0:
			break;
		case 1:
		{
			int32 ID;
			input >> ID;
			list[SlotNum]->SetSkill(nullptr); // 스킬 생성한 객체를 주는 걸로
			break;
		}
		case 2:
		{
			int32 ID;
			input >> ID;
			list[SlotNum]->SetItem(nullptr); // 스킬 생성한 객체를 주는 걸로
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
