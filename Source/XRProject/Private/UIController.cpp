// Fill out your copyright notice in the Description page of Project Settings
#include "UIController.h"
#include "ChatingWidget.h"
#include "HealthBarWidget.h"

AUIController::AUIController()
{
	bShowMouseCursor = true;
	bEnableClickEvents = true;
	//bEnableTouchEvents = true;
	bEnableMouseOverEvents = true;
	//bEnableTouchOverEvents = true;
}

void AUIController::Test_SendChat(FText const& Chat)
{
	std::wstring asd(*Chat.ToString());
	std::string c_id(asd.begin(), asd.end());
	ChatingManager::GetInstance().ReceiveChat(0, c_id);

}

void AUIController::Test_HP(int hp)
{
	UHealthBarWidget::GetInatance()->ApplyHp(hp);
}

void AUIController::SendBoxFocusOrSend()
{
	UChatingWidget::ChatingWidgetInstance->SendBoxFocus(this);
}
