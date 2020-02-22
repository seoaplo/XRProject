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

void AUIController::BeginPlay()
{
	Super::BeginPlay();
	int32 SizeX = 0;
	int32 SizeY = 0;
	GetViewportSize(SizeX, SizeY);
	SetMouseLocation(SizeX / 2, SizeY / 2);
	SetInputMode(FInputModeGameAndUI());
	bShowMouseCursor = true;
}

void AUIController::SendBoxFocusOrSend()
{
	UChatingWidget::ChatingWidgetInstance->SendBoxFocus(this);
}

