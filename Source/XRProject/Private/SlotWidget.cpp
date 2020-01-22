// Fill out your copyright notice in the Description page of Project Settings.


#include "SlotWidget.h"
#include "..\Public\SlotWidget.h"

USlotWidget::USlotWidget(const FObjectInitializer& ObjectInitializer) : UUserWidget(ObjectInitializer)
{
	SlotObject = nullptr;
}

UTexture2D * USlotWidget::GetIcon()
{
	if (!IsEmpty())
	{
		return SlotObject->GetIcon();
	}
	return nullptr;
}

int USlotWidget::GetCount()
{
	if (!IsEmpty())
	{
		return SlotObject->GetCount();
	}
	return 0;
}

bool USlotWidget::IsEmpty()
{
	return !SlotObject;
}
