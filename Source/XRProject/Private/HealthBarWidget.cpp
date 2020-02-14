// Fill out your copyright notice in the Description page of Project Settings.


#include "HealthBarWidget.h"

UHealthBarWidget* UHealthBarWidget::HealthBarInstance = nullptr;

UHealthBarWidget::UHealthBarWidget(const FObjectInitializer& ObjectInitializer) : UUserWidget(ObjectInitializer)
{
}

UHealthBarWidget::~UHealthBarWidget()
{
	HealthBarInstance = nullptr;
}

void UHealthBarWidget::SetInatance()
{
	UHealthBarWidget::HealthBarInstance = this;
}
