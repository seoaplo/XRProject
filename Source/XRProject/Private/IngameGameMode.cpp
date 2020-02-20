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
	GetMapMgr().PossessPlayer(GetWorld());
	GetWorld()->GetFirstPlayerController()->SetInputMode(FInputModeUIOnly());
	GetWorld()->GetFirstPlayerController()->bShowMouseCursor = false;

	CurrentWidget->CharacterInfo->SetSlotInfo();

	GetMapMgr().Spawn_Character.BindUObject(this, &AIngameGameMode::SpawnRemotePlayer);

	GetNetMgr().GetPacketReceiveDelegate(ENetworkSCOpcode::kNotifyMatchResult)->BindUObject(
		this, &AIngameGameMode::NotifyMatchResult);
	GetNetMgr().GetPacketReceiveDelegate(ENetworkSCOpcode::kNotifyMatchCanceled)->BindUObject(
		this, &AIngameGameMode::NotifyMatchCanceled);
	GetNetMgr().GetPacketReceiveDelegate(ENetworkSCOpcode::kNotifyDungeonReward)->BindUObject(
		this, &AIngameGameMode::NotifyDungeonReward);
	GetNetMgr().GetPacketReceiveDelegate(ENetworkSCOpcode::kNotifyStartLevel)->BindUObject(
		this, &AIngameGameMode::NotifyStartLevel);

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

	if (CurrentWidget->SkillWindow)
	{
		CurrentWidget->SkillWindow->CreateSkillList();
	}
	if (CurrentWidget->QuickBar)
	{
		CurrentWidget->QuickBar->SetQuickSlot();
	}

	int32 SizeX = 0;
	int32 SizeY = 0;
	GetWorld()->GetFirstPlayerController()->GetViewportSize(SizeX, SizeY);

	GetWorld()->GetFirstPlayerController()->SetInputMode(FInputModeGameOnly());
	GetWorld()->GetFirstPlayerController()->bShowMouseCursor = false;
	if (GetMapMgr().IsDungeon())
	{
		NotifyLoadComplete();
	}
	else
	{
		GetWorld()->GetFirstPlayerController()->SetInputMode(FInputModeGameOnly());
	}
}


void AIngameGameMode::Tick(float deltatime)
{
	Super::Tick(deltatime);
	GetNetMgr().Update();
	GetMapMgr().Tick(deltatime);
}

void AIngameGameMode::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	Super::EndPlay(EndPlayReason);
}

void AIngameGameMode::SpawnRemotePlayer()
{
	GetMapMgr().RemotePlayerSpawn(GetWorld());
}
void AIngameGameMode::DeleteRemotePlayer(int64_t ObjectID)
{
	GetMapMgr().DeleteRemotePlayer(ObjectID);
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

void AIngameGameMode::NotifyDungeonReward(class InputStream& input)
{
	if (CurrentWidget == nullptr) return;
	CurrentWidget->CreateDungeonResultWidget(input);
}

void AIngameGameMode::NotifyStartLevel(class InputStream& input)
{
	XRLOG(Warning, TEXT("NotifyStartLevel"));
	GetMapMgr().MonsterListSpawn(GetWorld());
	GetWorld()->GetFirstPlayerController()->SetInputMode(FInputModeGameOnly());
}
void AIngameGameMode::NotifyLoadComplete()
{
	OutputStream out;
	out.WriteOpcode(ENetworkCSOpcode::kNotifyLoadComplete);
	out.CompletePacketBuild();
	GetNetMgr().SendPacket(out);

	XRLOG(Warning, TEXT("NotifyLoadComplete"));
}