// Fill out your copyright notice in the Description page of Project Settings.


#include "IngameGameMode.h"
#include "Inventory.h"
#include "XRGameInstance.h"
#include "XRAIController.h"
#include "XRPlayerController.h"
#include "Engine/Engine.h"

class APlayerCharacter;
AIngameGameMode::AIngameGameMode()
{
	IsSuper = false;
	PrimaryActorTick.bCanEverTick = true;
	PlayerControllerClass = AXRPlayerController::StaticClass();
}

AIngameGameMode::~AIngameGameMode()
{
}



void AIngameGameMode::BeginPlay()
{
	Super::BeginPlay();

	CurrentWidget = CreateWidget<UInGameMainWidget>(GetWorld(), MainWidget);
	if (CurrentWidget != nullptr)
	{
		CurrentWidget->AddToViewport();
	}
	PrimaryActorTick.bCanEverTick = true;

	IsSuper = Cast<UXRGameInstance>(GetGameInstance())->GetIsSuper();
	Cast<UXRGameInstance>(GetGameInstance())->ReqEnterZone();
	GetMapMgr().PlayerListSpawn(GetWorld());
	GetMapMgr().MonsterListSpawn(GetWorld());
	GetMapMgr().PossessPlayer(GetWorld());

	GetMapMgr().Spawn_Character.BindUObject(this, &AIngameGameMode::SpawnRemotePlayer);
	GetMapMgr().Delete_Character.BindUObject(this, &AIngameGameMode::DeleteRemotePlayer);

	GetNetMgr().GetPacketReceiveDelegate(ENetworkSCOpcode::kNotifyMatchResult)->BindUObject(
		this, &AIngameGameMode::NotifyMatchResult);
	GetNetMgr().GetPacketReceiveDelegate(ENetworkSCOpcode::kNotifyMatchCanceled)->BindUObject(
		this, &AIngameGameMode::NotifyMatchCanceled);
}


void AIngameGameMode::Tick(float deltatime)
{
	Super::Tick(deltatime);
	if (Cast<UXRGameInstance>(GetGameInstance())->GetIsSuper())
	{
		IsSuper = true;
	}
	GetNetMgr().Update();
}

void AIngameGameMode::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	Super::EndPlay(EndPlayReason);
}

void AIngameGameMode::SpawnRemotePlayer()
{
	GetMapMgr().RemotePlayerSpawn(GetWorld());
}
void AIngameGameMode::DeleteRemotePlayer()
{
	GetMapMgr().DeleteRemotePlayer(GetWorld());
}

void AIngameGameMode::NotifyMatchResult(class InputStream& input)
{
	if (CurrentWidget == nullptr) return;
	CurrentWidget->MatchingWindow->DungeonMatchingResult();
}
void AIngameGameMode::NotifyMatchCanceled(class InputStream& input)
{
	if (CurrentWidget == nullptr) return;
	CurrentWidget->MatchingWindow->DungeonEnterFail();
}