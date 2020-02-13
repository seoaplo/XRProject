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
	PrimaryActorTick.bCanEverTick = true;
	PlayerControllerClass = AXRPlayerController::StaticClass();
}

AIngameGameMode::~AIngameGameMode()
{
}



void AIngameGameMode::BeginPlay()
{
	Super::BeginPlay();

	PrimaryActorTick.bCanEverTick = true;
	CurrentWidget = CreateWidget<UInGameMainWidget>(GetWorld(), MainWidget);
	if (CurrentWidget != nullptr)
	{
		CurrentWidget->AddToViewport();
	}

	GetMapMgr().PlayerListSpawn(GetWorld());
	GetMapMgr().MonsterListSpawn(GetWorld());
	GetMapMgr().PossessPlayer(GetWorld());

	CurrentWidget->CharacterInfo->SetSlotInfo();

	GetMapMgr().Spawn_Character.BindUObject(this, &AIngameGameMode::SpawnRemotePlayer);
	GetMapMgr().Delete_Character.BindUObject(this, &AIngameGameMode::DeleteRemotePlayer);

	GetNetMgr().GetPacketReceiveDelegate(ENetworkSCOpcode::kNotifyMatchResult)->BindUObject(
		this, &AIngameGameMode::NotifyMatchResult);
	GetNetMgr().GetPacketReceiveDelegate(ENetworkSCOpcode::kNotifyMatchCanceled)->BindUObject(
		this, &AIngameGameMode::NotifyMatchCanceled);

	if (CurrentWidget->MiniMap != nullptr)
	{
		UMiniMapWidget& CurrentMiniMap = *(CurrentWidget->MiniMap);
		CurrentMiniMap.ListClear();
		CurrentMiniMap.SetMapID(GetMapMgr().GetMapID());
		CurrentMiniMap.SetMyCharacter(GetMapMgr().GetPlayer());
		for (auto& Character : GetMapMgr().GetCharacterList())
		{
			CurrentMiniMap.AddActorList(Character.Value, static_cast<int>(EMiniMapObjectType::EParty));
		}
		for (auto& Monster : GetMapMgr().GetMonsterList())
		{
			CurrentMiniMap.AddActorList(Monster.Value, static_cast<int>(EMiniMapObjectType::EEnemy));
		}
	}

	int32 SizeX = 0;
	int32 SizeY = 0;
	GetWorld()->GetFirstPlayerController()->GetViewportSize(SizeX, SizeY);

	GetWorld()->GetFirstPlayerController()->SetInputMode(FInputModeGameOnly());
	GetWorld()->GetFirstPlayerController()->bShowMouseCursor = false;
	
}


void AIngameGameMode::Tick(float deltatime)
{
	Super::Tick(deltatime);
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