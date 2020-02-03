// Fill out your copyright notice in the Description page of Project Settings.


#include "XRPlayerController.h"



void AXRPlayerController::BeginPlay()
{
	Super::BeginPlay();
	XRLOG(Warning, TEXT("PlayerController IS Ready"));
	SetGenericTeamId(FGenericTeamId(10));
	TeamId = FGenericTeamId(10);
}

FGenericTeamId AXRPlayerController::GetGenericTeamId() const
{
	return TeamId;
}

void AXRPlayerController::SetupInputComponent()
{
	Super::SetupInputComponent();
	InputComponent->BindAction(TEXT("TemporaryEnterZone"), IE_Pressed, this, &AXRPlayerController::TemporaryEnterZone);
}

void AXRPlayerController::TemporaryEnterZone()
{
	if (EnterZoneFunc != nullptr) EnterZoneFunc();
}
