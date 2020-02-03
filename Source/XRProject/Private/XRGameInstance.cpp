// Fill out your copyright notice in the Description page of Project Settings.

#include "XRGameInstance.h"
#include "Inventory.h"
#include "XRAIController.h"
#include "XRPlayerController.h"
#include "Engine/Engine.h"

void UXRGameInstance::Init()
{
    NetworkManager = NewObject<UNetworkManager>();
    NetworkManager->StartThread();
    UNetworkManager::Instance = NetworkManager;
	XRAssetManager = NewObject<UXRAssetMgr>();
	ItemManager = NewObject<UItemManager>();
	MapManager = NewObject<UMapManager>();
	MapManager->Init();

	NetworkManager->GetPacketReceiveDelegate(ENetworkSCOpcode::kUserEnterTheMap)->BindUObject(
		this, &UXRGameInstance::HandleEnterZone);
	NetworkManager->GetPacketReceiveDelegate(ENetworkSCOpcode::kSpawnCharacter)->BindUObject(
		this, &UXRGameInstance::SpawnCharacterFromServer);
	NetworkManager->GetPacketReceiveDelegate(ENetworkSCOpcode::kUpdateCharacterPosition)->BindUObject(
		this, &UXRGameInstance::UpdateCharacterPosition);
	NetworkManager->GetPacketReceiveDelegate(ENetworkSCOpcode::kSetMonsterController)->BindUObject(
		this, &UXRGameInstance::SetMonsterController);
	NetworkManager->GetPacketReceiveDelegate(ENetworkSCOpcode::kUpdateMonsterAction)->BindUObject(
		this, &UXRGameInstance::UpdateMonsterAction);

	
}

void UXRGameInstance::Shutdown()
{
    NetworkManager->StopThread();
    UNetworkManager::Instance = nullptr;
}

void UXRGameInstance::HandleEnterZone(InputStream & input)
{
	int32_t ReadLevelID;
	input >> ReadLevelID;

	//UGameplayStatics::OpenLevel(GetWorld(), TEXT("LEVEL_Zone_1"));
	
	MapManager->Clear();
	MapManager->Init();

	ReadMapData(input);
	ReadBaseCharacterInfo(input);
	ReadInventoryInfo(input);
	ReadQuickSlot(input);

	MapManager->InitComplete = true;
}

void UXRGameInstance::ReadBaseCharacterInfo(InputStream & input)
{
	MapManager->ReadPossesPlayerFromServer(input);
}

void UXRGameInstance::ReadInventoryInfo(InputStream & input)
{
	auto GameInstance = Cast<UXRGameInstance>(UGameplayStatics::GetGameInstance(GetWorld()));
	if (GameInstance == nullptr) return;
	UItemManager;
	int Glod = input.ReadInt32();
	Inventory::GetInstance().SetGold(Glod);
	for (int i = 0; i < Inventory::GetInstance().GetInventorySize(); i++)
	{
		UItem* newItem = GameInstance->ItemManager->CreateItem(input).GetValue();
		if (newItem)
		{
			Inventory::GetInstance().AddItem(newItem, i);
		}
	}
}

void UXRGameInstance::ReadQuickSlot(InputStream & input)
{
	for (int i = 0; i < 10; i++)
	{
		int Type = input.ReadInt8();
		if (Type)
		{
			int ID = input.ReadInt32();
		}
	}
}

void UXRGameInstance::ReadMapData(InputStream & input)
{
	MapManager->ReadMapDataFromServer(input);
}


void UXRGameInstance::SpawnCharacterFromServer(class InputStream& input)
{
	MapManager->ReadPlayerSpawnFromServer(input);
	MapManager->PlayerSpawnReady = true;
}
void UXRGameInstance::UpdateCharacterPosition(class InputStream& input)
{
	int64 Id = input.ReadInt64();
	int32 state = input.ReadInt32();

	FVector Location = input.ReadFVector();
	FRotator Rotation = input.ReadFRotator();

	APlayerCharacter* TargetPlayer = MapManager->FindPlayer(Id);
	if (TargetPlayer == nullptr)
	{
		XRLOG(Warning, TEXT("Player not found"));
		return;
	}

	AAIController*  aicon = Cast<AAIController>(TargetPlayer->GetController());
	if (aicon == nullptr)
	{
		XRLOG(Warning, TEXT("AICon not found"));
		return;
	}
	else aicon->MoveToLocation(Location, 2, false, false);

}

void UXRGameInstance::SetMonsterController(InputStream& input)
{
	bool IsMonsterController = input.ReadBool();
	IsSuper = IsMonsterController;

}

void UXRGameInstance::UpdateMonsterAction(InputStream& input)
{
	auto firstPlayer = Cast<AXRPlayerController>(GetWorld()->GetFirstPlayerController());
	if (firstPlayer)
	{
		if (!firstPlayer->IsSpuer())
		{
			int64 ObjID = input.ReadInt64();
			int32 ActionID = input.ReadInt32();
			FVector Location = input.ReadFVector();
			FRotator Rotator = input.ReadFRotator();


			GEngine->AddOnScreenDebugMessage(112, 5.f, FColor::Blue, FString::Printf(TEXT("Recv MonsterUpdate  ObjectID: %s, ActionID: %s, Location : %s, Rotator: %s"),
				*FString::FromInt(ObjID), *FString::FromInt(ActionID),
				*Location.ToString(), *Rotator.ToString()));
		}
	}

}
