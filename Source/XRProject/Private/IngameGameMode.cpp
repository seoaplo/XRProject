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

	GetMapMgr().Spawn_Character.BindUObject(this, &AIngameGameMode::SpawnRemotePlayer);

	GetNetMgr().GetPacketReceiveDelegate(ENetworkSCOpcode::kNotifyMatchResult)->BindUObject(
		this, &AIngameGameMode::NotifyMatchResult);
	GetNetMgr().GetPacketReceiveDelegate(ENetworkSCOpcode::kNotifyMatchCanceled)->BindUObject(
		this, &AIngameGameMode::NotifyMatchCanceled);
	GetNetMgr().GetPacketReceiveDelegate(ENetworkSCOpcode::kNotifyDungeonReward)->BindUObject(
		this, &AIngameGameMode::NotifyDungeonReward);
	GetNetMgr().GetPacketReceiveDelegate(ENetworkSCOpcode::kNotifyStartLevel)->BindUObject(
		this, &AIngameGameMode::NotifyStartLevel);

	GetMapMgr().RemoteCharacterDelete.BindUObject(this, &AIngameGameMode::DeleteRemotePlayer);

	GetMapMgr().CreateLoadingWidget(GetWorld(), 1.0f);

	if (GetMapMgr().IsDungeon())
	{
		NotifyLoadComplete();
		GetMapMgr().DeleteWidget();
	}
	else
	{
		GetMapMgr().SetCompleteLoad(true);
		CurrentWidget = CreateWidget<UInGameMainWidget>(GetWorld(), MainWidget);
		if (CurrentWidget != nullptr)
		{
			CurrentWidget->AddToViewport();
		}

		GetMapMgr().PlayerListSpawn(GetWorld());
		//GetMapMgr().PossessPlayer(GetWorld());

		GetWorld()->GetFirstPlayerController()->bShowMouseCursor = false;
		GetMapMgr().GetPlayer()->ToggleMouseCursor();
		CurrentWidget->CharacterInfo->SetSlotInfo();

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
		GetMapMgr().DeleteWidget();
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
void AIngameGameMode::AddRemotePlayerForMinimap(APlayerCharacter* RemotePlayer)
{
	if (RemotePlayer == nullptr) return;
	if (CurrentWidget->MiniMap == nullptr) return;
	
	UMiniMapWidget& CurrentMiniMap = *(CurrentWidget->MiniMap);
	CurrentMiniMap.AddActorList(Cast<AActor>(RemotePlayer), static_cast<int>(EMiniMapObjectType::EParty));
}
void AIngameGameMode::DeleteRemotePlayer(int64_t ObjectID)
{
	if (CurrentWidget->MiniMap != nullptr)
	{
		UMiniMapWidget& CurrentMiniMap = *(CurrentWidget->MiniMap);

		APlayerCharacter* FindRemotePlayer = GetMapMgr().FindPlayer(ObjectID);
		if (FindRemotePlayer)
		{
			CurrentMiniMap.DeleteActorList(Cast<AActor>(FindRemotePlayer));
		}
	}
}
void AIngameGameMode::DeathMonster(int64_t ObjectID)
{
	if (CurrentWidget->MiniMap != nullptr)
	{
		UMiniMapWidget& CurrentMiniMap = *(CurrentWidget->MiniMap);

		ANonePlayerCharacter* FindMonster = GetMapMgr().FindMonster(ObjectID);
		if (FindMonster)
		{
			CurrentMiniMap.DeleteActorList(Cast<AActor>(FindMonster));
		}
	}
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
	CurrentWidget = CreateWidget<UInGameMainWidget>(GetWorld(), MainWidget);
	if (CurrentWidget != nullptr)
	{
		CurrentWidget->AddToViewport();
	}

	PrimaryActorTick.bCanEverTick = true;
	GetMapMgr().SetCompleteLoad(true);

	XRLOG(Warning, TEXT("NotifyStartLevel"));
	GetMapMgr().PlayerListSpawn(GetWorld());
	GetMapMgr().MonsterListSpawn(GetWorld());
	//GetMapMgr().PossessPlayer(GetWorld());

	GetWorld()->GetFirstPlayerController()->bShowMouseCursor = false;

	GetMapMgr().GetPlayer()->AddControllerYawInput(105.0f);
	GetMapMgr().GetPlayer()->SetActorRotation(FRotator(0.0f, -90.0f, 0.0f));
	GetMapMgr().GetPlayer()->ToggleMouseCursor();

	CurrentWidget->CharacterInfo->SetSlotInfo();

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
}
void AIngameGameMode::NotifyLoadComplete()
{
	OutputStream out;
	out.WriteOpcode(ENetworkCSOpcode::kNotifyLoadComplete);
	out.CompletePacketBuild();
	GetNetMgr().SendPacket(out);

	XRLOG(Warning, TEXT("NotifyLoadComplete"));
}