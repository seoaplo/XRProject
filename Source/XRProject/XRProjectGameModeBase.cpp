// Fill out your copyright notice in the Description page of Project Settings.


#include "XRProjectGameModeBase.h"
#include "PlayerCharacter.h"
#include "XRGameInstance.h"
#include "BaseCharacter.h"
#include "PlayerCharacter.h"
#include "NonePlayerCharacter.h"

AXRProjectGameModeBase::AXRProjectGameModeBase()
{
    PrimaryActorTick.bCanEverTick = true;

}

void AXRProjectGameModeBase::BeginPlay()
{
    Super::BeginPlay();
}

void AXRProjectGameModeBase::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
    Super::EndPlay(EndPlayReason);
    //GetNetMgr().Close();
}

void AXRProjectGameModeBase::Tick(float DeltaSeconds)
{
    Super::Tick(DeltaSeconds);
    //GetNetMgr().Update();
}

