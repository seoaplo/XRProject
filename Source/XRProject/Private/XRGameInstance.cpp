// Fill out your copyright notice in the Description page of Project Settings.

#include "XRGameInstance.h"
#include "ChatingManager.h"
#include "Inventory.h"
#include "XRAIController.h"
#include "XRPlayerController.h"
#include "IngameGameMode.h"
#include "InventoryWidget.h"
#include "CharacterInfoWidget.h"
#include "Engine/Engine.h"
#include "EngineMinimal.h"

void UXRGameInstance::Init()
{
    NetworkManager = NewObject<UNetworkManager>();
    NetworkManager->StartThread();
    UNetworkManager::Instance = NetworkManager;
	XRAssetManager = NewObject<UXRAssetMgr>();
	ItemManager = NewObject<UItemManager>();
	MapManager = NewObject<UMapManager>();
	MapManager->Init();
	PlayerSkillManager = NewObject<UPlayerSkillManager>();


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
	NetworkManager->GetPacketReceiveDelegate(ENetworkSCOpcode::kNotifyCharacterWait)->BindUObject(
		this, &UXRGameInstance::CharacterWait);
	NetworkManager->GetPacketReceiveDelegate(ENetworkSCOpcode::kNotifyCharacterSprint)->BindUObject(
		this, &UXRGameInstance::CharacterSprint);
	NetworkManager->GetPacketReceiveDelegate(ENetworkSCOpcode::kNotifyCharacterRolling)->BindUObject(
		this, &UXRGameInstance::CharacterRolling);

	NetworkManager->GetPacketReceiveDelegate(ENetworkSCOpcode::kNotifyChat)->BindUObject(
		this, &UXRGameInstance::NotifyChat);

	NetworkManager->GetPacketReceiveDelegate(ENetworkSCOpcode::kUpdateCharacterPosition)->BindUObject(
		this, &UXRGameInstance::UpdateCharacterPosition);
		
	NetworkManager->GetPacketReceiveDelegate(ENetworkSCOpcode::kNotifyCharacterAttack)->BindUObject(
		this, &UXRGameInstance::UpdateCharacterMotion);

	NetworkManager->GetPacketReceiveDelegate(ENetworkSCOpcode::kInventoryUpdate)->BindUObject(
		this, &UXRGameInstance::UpdateInventory);

	NetworkManager->GetPacketReceiveDelegate(ENetworkSCOpcode::kActorDamaged)->BindUObject(
		this, &UXRGameInstance::ActorDamaged);
	
	NetworkManager->GetPacketReceiveDelegate(ENetworkSCOpcode::kNotifyCharacterDead)->BindUObject(
		this, &UXRGameInstance::CharacterDead);





}

void UXRGameInstance::Shutdown()
{
	GetNetMgr().Close();
    NetworkManager->StopThread();
    UNetworkManager::Instance = nullptr;
}  

void UXRGameInstance::LobbytoGame()
{
	std::string Ip = AccountManager::GetInstance().GetInGameIP();
	int16 Port = AccountManager::GetInstance().GetInGamePort();
	GetNetMgr().Connect(Ip.c_str(), Port, std::bind(&UXRGameInstance::SendConfirmRequest, this));
}
void UXRGameInstance::SendConfirmRequest()
{
	std::string ID = AccountManager::GetInstance().GetAccountID();
	OutputStream out;
	out.WriteOpcode(ENetworkCSOpcode::kZoneConrifmRequest);
	out.WriteCString(ID.c_str());
	out.CompletePacketBuild();
	GetNetMgr().SendPacket(out);

}

void UXRGameInstance::HandleEnterZone(InputStream & input)
{

	MapManager->Clear();
	MapManager->Init();

	ReadMapData(input);
	ReadBaseCharacterInfo(input);
	ReadInventoryInfo(input);
	ReadQuickSlot(input);
	MapManager->OpenMap(GetWorld());
}



void UXRGameInstance::ReadBaseCharacterInfo(InputStream & input)
{
	MapManager->ReadPossesPlayerFromServer(input);
}

void UXRGameInstance::ReadInventoryInfo(InputStream & input)
{
	auto GameInstance = Cast<UXRGameInstance>(UGameplayStatics::GetGameInstance(GetWorld()));
	if (GameInstance == nullptr) return;
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

void UXRGameInstance::UpdateInventory(InputStream & input)
{
	auto GameInstance = Cast<UXRGameInstance>(UGameplayStatics::GetGameInstance(GetWorld()));
	if (GameInstance == nullptr) return;
	int8 Type = input.ReadInt8();
	switch (Type)
	{
	case 0 : // 생성
	{
		break;
	}
	case 1 : // 삭제
	{
		break;
	}
	case 2 : // 업데이트
	{
		for (int i = 0; i < 2; i++)
		{
			bool IsEquipment = input.ReadBool();
			int SlotNum = input.ReadInt32();
			if (IsEquipment)
			{
				UItem* newItem = GameInstance->ItemManager->CreateItem(input).GetValue();
				UItemEquipment* EquipmentItem = Cast<UItemEquipment>(newItem);
				MapManager->GetPlayer()->SetEquippedItem((EEquipmentsType)SlotNum, EquipmentItem);
				UCharacterInfoWidget::GetInstance()->Slot[SlotNum]->SetSlotObject();

			}
			else
			{
				UItem* newItem = GameInstance->ItemManager->CreateItem(input).GetValue();
				Inventory::GetInstance().SetItem(newItem, SlotNum);
				UInventoryWidget::GetInstance()->list[SlotNum]->SetSlotObject();
			}
		}
		break;
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
	else
	{
		if (TargetPlayer->GetbIsOverallRollAnimPlaying() == false)
		{
			aicon->MoveToLocation(Location, 2, false, false);
		}
	}
}


void UXRGameInstance::SetMonsterController(InputStream& input)
{
	bool IsMonsterController = input.ReadBool();
	if (IsMonsterController)
	{
		XRLOG(Warning, TEXT("You are set up Super User "));
	}
	else
	{
		XRLOG(Warning, TEXT("You are set up General User "));
	}

	IsSuper = IsMonsterController;
	
}

void UXRGameInstance::UpdateMonsterAction(InputStream& input)
{

	int64 ObjID = input.ReadInt64();
	ANonePlayerCharacter* npcCharacter = MapManager->FindMonster(ObjID);
	if (npcCharacter)
	{
		npcCharacter->ExcuteRecvNpcAction(input);
	}
}


void UXRGameInstance::UpdateCharacterMotion(InputStream & input)
{
	int64 ObjectID = input.ReadInt64();
	int32 MotionID = input.ReadInt32();
	FVector TargetLocation = input.ReadFVector();
	FRotator TargetRotation = input.ReadFRotator();

	APlayerCharacter* TargetPlayer = nullptr;
	TargetPlayer = MapManager->FindPlayer(ObjectID);

	if (TargetPlayer)
	{
		TargetPlayer->SetActorRotation(TargetRotation);
		TargetPlayer->MyAnimInstance->PlayAttackOnlyPlayMontage();
		TargetPlayer->MyAnimInstance->JumpToComboMontageSection(MotionID);

		UE_LOG(LogTemp,
			Warning,
			TEXT("CharacterMotionDebug ID : %d, MotionID : %d, TargetVec : %f %f %f, TargetRot : %f %f %f"),
			ObjectID, MotionID, TargetLocation.X, TargetLocation.Y, TargetLocation.Z,
			TargetRotation.Yaw, TargetRotation.Pitch, TargetRotation.Roll);
	}


}

void UXRGameInstance::ActorDamaged(InputStream& input)
{
	int32 AttackerType = input.ReadInt32();
	int64 AttackerID = input.ReadInt64();
	int32 AttackedType = input.ReadInt32();
	int64 AttackedID = input.ReadInt64();
	int32 AttackActionID = input.ReadInt32();
	float AttackSetHp = input.ReadFloat32();
	//bool AttackIntensity = input.ReadBool();

	if (AttackerType == 1)
	{
		ANonePlayerCharacter* AttackerMonster = MapManager->FindMonster(AttackerID);
		APlayerCharacter* AttackedCharacter = MapManager->FindPlayer(AttackedID);
		
		//데미지 강격/약격 나누기 위한 잔재
		//XRDamageEvent MonsterDamageEvent;
		//MonsterDamageEvent.ID = AttackActionID;
		//MonsterDamageEvent.Intensity = AttackIntensity;

		if (AttackerMonster)
		{
			if (AttackedCharacter == MapManager->GetPlayer())
				AttackedCharacter->TakeDamage(AttackSetHp, FDamageEvent(), AttackerMonster->GetController(), AttackerMonster);
			else
			{
				//if(MonsterDamageEvent)
				AttackedCharacter->MyAnimInstance->PlayHitMontage();
			}
		}



	}
	else if (AttackerType == 0)
	{
		APlayerCharacter* AttackerCharacter = MapManager->FindPlayer(AttackerID);
		ANonePlayerCharacter* AttackedMonster = MapManager->FindMonster(AttackedID);

		if (AttackerCharacter)
		{
			AttackedMonster->NpcTakeDamaged(AttackSetHp, AttackerCharacter->GetController(), AttackerCharacter->ObjectID);
			UE_LOG(LogTemp, Warning, TEXT("Monster Damaged. ginstance258"));
		}
	}
}


void UXRGameInstance::NotifyChat(class InputStream& input)
{
	int32_t Type;
	std::string ChatString;
	input >> Type;
	input >> ChatString;
	ChatingManager::GetInstance().ReceiveChat(Type, ChatString);
}

void UXRGameInstance::CharacterWait(InputStream& input)
{
	int64 TargetID = input.ReadInt64();
	FVector TargetPos = input.ReadFVector();

	APlayerCharacter* TargetPlayer = MapManager->FindPlayer(TargetID);
	
	if (TargetPlayer != MapManager->GetPlayer())
	{
		TargetPlayer->bIsSprint = false;
		TargetPlayer->GetCharacterMovement()->MaxWalkSpeed = kNormalMovementSpeed;
	}
}

void UXRGameInstance::CharacterSprint(InputStream& input)
{
	int64 TargetID = input.ReadInt64();

	APlayerCharacter* TargetPlayer = MapManager->FindPlayer(TargetID);

	if (TargetPlayer != MapManager->GetPlayer())
	{
		TargetPlayer->bIsSprint = true;
		TargetPlayer->GetCharacterMovement()->MaxWalkSpeed = kSprintMovementSpeed;
		UE_LOG(LogTemp, Warning, TEXT("CharacterSprint Received"));
	}
}

void UXRGameInstance::CharacterDead(InputStream& input)
{
	int64 TargetID = input.ReadInt64();
	APlayerCharacter* TargetPlayer = MapManager->FindPlayer(TargetID);
	
	if (TargetPlayer != MapManager->GetPlayer())
	{
		TargetPlayer->bIsCharacterDead = true;
		TargetPlayer->SetCharacterLifeState(ECharacterLifeState::DEAD);
	}
}

void UXRGameInstance::CharacterRolling(InputStream& input)
{
	int64 RollerID = input.ReadInt64();
	FRotator RollerRot = input.ReadFRotator();

	APlayerCharacter* TargetPlayer = MapManager->FindPlayer(RollerID);

	if (TargetPlayer != MapManager->GetPlayer())
	{

		TargetPlayer->bIsRolling = true;
		TargetPlayer->bIsOverallRollAnimPlaying = true;
		AAIController*  aicon = Cast<AAIController>(TargetPlayer->GetController());
		if (!aicon)
			check(false);
		
		aicon->StopMovement();
		TargetPlayer->SetActorRotation(RollerRot);
		TargetPlayer->MyAnimInstance->PlayMoveOnlyPlayMontage();
		TargetPlayer->MyAnimInstance->JumpToMoveMontageSection(FString("RollSection"));
	}

}
