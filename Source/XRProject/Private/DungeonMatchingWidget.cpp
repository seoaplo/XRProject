// Fill out your copyright notice in the Description page of Project Settings.


#include "DungeonMatchingWidget.h"
#include "NetworkManager.h"


UDungeonMatchingWidget::UDungeonMatchingWidget(const FObjectInitializer& ObjectInitializer) : UUserWidget(ObjectInitializer)
{
	SelectedDungeonID = 111;
	bIsVisible = false;
}

void UDungeonMatchingWidget::SetVisible(bool IsVisible)
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

bool UDungeonMatchingWidget::GetVisible()
{
	return bIsVisible;
}

void UDungeonMatchingWidget::SwitchVisible()
{
	SetVisible(!bIsVisible);
}

void UDungeonMatchingWidget::DungeonMatchingRequest(bool bTrueFalse)
{
	OutputStream out;
	out.WriteOpcode(ENetworkCSOpcode::kMatchingRequest);

	out << (int8_t)bTrueFalse;
	if (bTrueFalse) out << (int32_t)SelectedDungeonID;
	out.CompletePacketBuild();

	UNetworkManager::GetInstance().SendPacket(out);
}

void UDungeonMatchingWidget::DungeonEnterRequest(bool bTrueFalse)
{
	OutputStream out;
	out.WriteOpcode(ENetworkCSOpcode::kMatchingConfirmRequest);

	out << (int8_t)bTrueFalse;
	out.CompletePacketBuild();

	UNetworkManager::GetInstance().SendPacket(out);
}