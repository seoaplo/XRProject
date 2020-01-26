// Fill out your copyright notice in the Description page of Project Settings.


#include "XRProjectGameModeBase.h"
#include "PlayerCharacter.h"
#include "XRGameInstance.h"


AXRProjectGameModeBase::AXRProjectGameModeBase()
{
    PrimaryActorTick.bCanEverTick = true;

	DefaultPawnClass = APlayerCharacter::StaticClass();

}

void AXRProjectGameModeBase::BeginPlay()
{
    // LoginServer
    GetNetMgr().GetPacketReceiveDelegate(ENetworkSCOpcode::kCreateAccountResult)->BindUObject(
        this, &AXRProjectGameModeBase::HandleCreateAccountResult);

    GetNetMgr().GetPacketReceiveDelegate(ENetworkSCOpcode::kLoginResult)->BindUObject(
        this, &AXRProjectGameModeBase::HandleCreateAccountResult);

    // LobbyServer

    GetNetMgr().Connect("127.0.0.1", 11123, nullptr);
}

void AXRProjectGameModeBase::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
    Super::EndPlay(EndPlayReason);
    GetNetMgr().Close();
}

void AXRProjectGameModeBase::Tick(float DeltaSeconds)
{
    Super::Tick(DeltaSeconds);
    GetNetMgr().Update();
}

void AXRProjectGameModeBase::HandleCreateAccountResult(InputStream& input)
{
}

void AXRProjectGameModeBase::HandleLoginResult(InputStream& input)
{
}
