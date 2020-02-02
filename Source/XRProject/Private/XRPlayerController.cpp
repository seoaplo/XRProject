// Fill out your copyright notice in the Description page of Project Settings.


#include "XRPlayerController.h"



void AXRPlayerController::BeginPlay()
{
	Super::BeginPlay();
	XRLOG(Warning, TEXT("PlayerController IS Ready"));
	
	//TeamId = FGenericTeamId(10);
}

//FGenericTeamId AXRPlayerController::GetGenericTeamId() const
//{
//	return TeamId;
//}
