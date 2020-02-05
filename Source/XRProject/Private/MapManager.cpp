// Fill out your copyright notice in the Description page of Project Settings.


#include "MapManager.h"
#include "XRGameInstance.h"
#include "XRPlayerController.h"

UMapManager::UMapManager()
{
	MapList.Add(100, TEXT("LEVEL_Village"));
	MapList.Add(111, TEXT("LEVEL_Zone_1"));
	MapList.Add(112, TEXT("LEVEL_Zone_2"));
	MapList.Add(113, TEXT("LEVLE_Boss"));
}
bool UMapManager::Init()
{
	PlayerID = -1;
	LevelID = -1;

	MapPotalData.MapID = -1;
	MapPotalData.Location = FVector(0.0f, 0.0f, 0.0f);
	MapPotalData.Distance = 0.0f;

	Spawn_Character.Unbind();
	Delete_Character.Unbind();

	return true;
}
bool UMapManager::Clear()
{

	for (auto& Character : CharacterList)
	{
		Character.Value->Destroy();
	}
	for (auto& Monster : MonsterList)
	{
		Monster.Value->Destroy();
	}

	if (MapPotal != nullptr)
	{
		MapPotal->Destroy();
		MapPotal = nullptr;
	}
	CharacterList.Reset();
	MonsterList.Reset();

	CharacterDataList.clear();
	MonsterDataList.clear();

	InPotalPlayerList.clear();

	return true;
}

// 맵에 입장


void UMapManager::ReadMapDataFromServer(InputStream& Input)
{
	int32_t characterlistsize = 0;
	int32_t monsterlistsize = 0;

	Input >> LevelID;

	Input >> characterlistsize;
	CharacterDataList.resize(characterlistsize + 1);
	ReadPlayerFromServer(Input);

	Input >> monsterlistsize;
	MonsterDataList.resize(monsterlistsize);
	ReadMosnterFromServer(Input);
}
void UMapManager::ReadPotalTriggerFromServer(InputStream& Input)
{
	MapPotalData.MapID = Input.ReadInt32();
	MapPotalData.Location = Input.ReadFVector();
	MapPotalData.Distance = Input.ReadFloat32();
}
void UMapManager::ReadMosnterFromServer(InputStream& Input)
{

	for (auto& CurrentData : MonsterDataList)
	{
		Input >> CurrentData.MonsterID;
		Input >> CurrentData.ObjectID;
		Input >> CurrentData.Location;
		Input >> CurrentData.Rotator;
		
		CurrentData.Rotator = FRotator(0, 0, 0);

		CurrentData.Current_HP = Input.ReadFloat32();
		CurrentData.Max_HP = Input.ReadFloat32();
		CurrentData.Attack_Min = Input.ReadFloat32();
		CurrentData.Attack_Max = Input.ReadFloat32();
		CurrentData.Attack_Range = Input.ReadFloat32();
		CurrentData.Attack_Speed = Input.ReadFloat32();
		CurrentData.Speed = Input.ReadFloat32();
		CurrentData.Defence = Input.ReadFloat32();
	}
}

void UMapManager::ReadPlayerFromServer(InputStream& Input)
{
	for (int iCount = 0; iCount < CharacterDataList.size() - 1; iCount++)
	{
		CharacterData& CurrentData = CharacterDataList[iCount];

		Input >> CurrentData.ObjectID;
		Input >> CurrentData.Location;
		Input >> CurrentData.Rotator;

		CurrentData.Current_HP = Input.ReadFloat32();
		CurrentData.Max_HP = Input.ReadFloat32();
		CurrentData.Attack_Min = Input.ReadFloat32();
		CurrentData.Attack_Max = Input.ReadFloat32();
		CurrentData.Attack_Range = Input.ReadFloat32();
		CurrentData.Attack_Speed = Input.ReadFloat32();
		CurrentData.Speed = Input.ReadFloat32();
		CurrentData.Defence = Input.ReadFloat32();

		CurrentData.Name = Input.ReadCString();
		CurrentData.Level = Input.ReadInt32();
		CurrentData.Gender = Input.ReadInt32();
		CurrentData.FaceID = Input.ReadInt32();
		CurrentData.HairID = Input.ReadInt32();
		CurrentData.STR = Input.ReadInt32();
		CurrentData.DEX = Input.ReadInt32();
		CurrentData.INT = Input.ReadInt32();
		CurrentData.CurrentStamina = Input.ReadFloat32();
		CurrentData.MaxStamina = Input.ReadFloat32();

		int EquipmentSize = 4;
		for (int iCount = 0; iCount < EquipmentSize; iCount++)
		{
			Equipment& CurrentEquip = CurrentData.EquipArray[iCount];
			CurrentEquip.Type = Input.ReadInt32();
			if (CurrentEquip.Type)
			{
				if (CurrentEquip.Type == 3)
				{
					CurrentEquip.ID = Input.ReadInt32();
					CurrentEquip.AddATK = Input.ReadInt32();
					CurrentEquip.AddDEF = Input.ReadInt32();
					CurrentEquip.AddSTR = Input.ReadInt32();
					CurrentEquip.AddDex = Input.ReadInt32();
					CurrentEquip.AddInt = Input.ReadInt32();
				}
				CurrentEquip.Count = Input.ReadInt32();
			}
		}
	}
}


void UMapManager::ReadPossesPlayerFromServer(InputStream& Input)
{

	CharacterData& CurrentData = CharacterDataList[CharacterDataList.size() - 1];

	Input >> CurrentData.ObjectID;
	Input >> CurrentData.Location;
	Input >> CurrentData.Rotator;

	CurrentData.Current_HP = Input.ReadFloat32();
	CurrentData.Max_HP = Input.ReadFloat32();
	CurrentData.Attack_Min = Input.ReadFloat32();
	CurrentData.Attack_Max = Input.ReadFloat32();
	CurrentData.Attack_Range = Input.ReadFloat32();
	CurrentData.Attack_Speed = Input.ReadFloat32();
	CurrentData.Speed = Input.ReadFloat32();
	CurrentData.Defence = Input.ReadFloat32();

	CurrentData.Name = Input.ReadCString();
	CurrentData.Level = Input.ReadInt32();
	CurrentData.Gender = Input.ReadInt32();
	CurrentData.FaceID = Input.ReadInt32();
	CurrentData.HairID = Input.ReadInt32();
	CurrentData.STR = Input.ReadInt32();
	CurrentData.DEX = Input.ReadInt32();
	CurrentData.INT = Input.ReadInt32();
	CurrentData.CurrentStamina = Input.ReadFloat32();
	CurrentData.MaxStamina = Input.ReadFloat32();

	int EquipmentSize = 4;
	for (int i = 0; i < EquipmentSize; i++)
	{
		Equipment& CurrentEquip = CurrentData.EquipArray[EquipmentSize];
		CurrentEquip.Type = Input.ReadInt32();
		if (CurrentEquip.Type)
		{
			if (CurrentEquip.Type == 3)
			{
				CurrentEquip.ID = Input.ReadInt32();
				CurrentEquip.AddATK = Input.ReadInt32();
				CurrentEquip.AddDEF = Input.ReadInt32();
				CurrentEquip.AddSTR = Input.ReadInt32();
				CurrentEquip.AddDex = Input.ReadInt32();
				CurrentEquip.AddInt = Input.ReadInt32();
			}
			CurrentEquip.Count = Input.ReadInt32();
		}
	}
	PlayerID = CurrentData.ObjectID;
}
bool UMapManager::ReadPlayerDeleteFromServer(InputStream& Input)
{
	Delete_Character.ExecuteIfBound();
	return true;
}

APlayerCharacter* UMapManager::FindPlayer(int64_t objectid)
{
	APlayerCharacter* FindedPlayer = CharacterList.FindRef(objectid);
	return FindedPlayer;
}
ANonePlayerCharacter* UMapManager::FindMonster(int64_t ObjectID)
{
	ANonePlayerCharacter* FindedMonster = MonsterList.FindRef(ObjectID);
	return FindedMonster;
}
bool UMapManager::ReadPlayerSpawnFromServer(InputStream& Input)
{
	CharacterData CurrentData;
	Input >> CurrentData.ObjectID;
	Input >> CurrentData.Location;
	Input >> CurrentData.Rotator;

	CurrentData.Current_HP = Input.ReadFloat32();
	CurrentData.Max_HP = Input.ReadFloat32();
	CurrentData.Attack_Min = Input.ReadFloat32();
	CurrentData.Attack_Max = Input.ReadFloat32();
	CurrentData.Attack_Range = Input.ReadFloat32();
	CurrentData.Attack_Speed = Input.ReadFloat32();
	CurrentData.Speed = Input.ReadFloat32();
	CurrentData.Defence = Input.ReadFloat32();

	CurrentData.Name = Input.ReadCString();
	CurrentData.Level = Input.ReadInt32();
	CurrentData.Gender = Input.ReadInt32();
	CurrentData.FaceID = Input.ReadInt32();
	CurrentData.HairID = Input.ReadInt32();
	CurrentData.STR = Input.ReadInt32();
	CurrentData.DEX = Input.ReadInt32();
	CurrentData.INT = Input.ReadInt32();
	CurrentData.CurrentStamina = Input.ReadFloat32();
	CurrentData.MaxStamina = Input.ReadFloat32();

	int EquipmentSize = 4;
	for (int i = 0; i < EquipmentSize; i++)
	{
		Equipment& CurrentEquip = CurrentData.EquipArray[EquipmentSize];
		CurrentEquip.Type = Input.ReadInt32();
		if (CurrentEquip.Type)
		{
			if (CurrentEquip.Type == 3)
			{
				CurrentEquip.ID = Input.ReadInt32();
				CurrentEquip.AddATK = Input.ReadInt32();
				CurrentEquip.AddDEF = Input.ReadInt32();
				CurrentEquip.AddSTR = Input.ReadInt32();
				CurrentEquip.AddDex = Input.ReadInt32();
				CurrentEquip.AddInt = Input.ReadInt32();
			}
			CurrentEquip.Count = Input.ReadInt32();
		}
	}
	CharacterDataList.push_back(CurrentData);
	Spawn_Character.ExecuteIfBound();
	return true;
}

bool UMapManager::OpenMap(UWorld* World)
{
	if (World == nullptr) return false;
	FName* LevelName = MapList.Find(LevelID);
	if (LevelName == nullptr) return false;

	UGameplayStatics::OpenLevel(World, *LevelName);
	return true;
}

bool UMapManager::PotalTriggerSpawn(UWorld* world)
{

	if (MapPotalData.MapID < 0) return false;

	FActorSpawnParameters Param;
	Param.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
	AActor* actor =
		world->SpawnActor
		(APotalTrigger::StaticClass(), &MapPotalData.Location, &FRotator(0,0,0), Param);

	if (actor == nullptr) return false;
	MapPotal = Cast<APotalTrigger>(actor);
	MapPotal->MapID = MapPotalData.MapID;

	MapPotal->BeginOverlapDelegate.BindUObject(this, UMapManager::PotalInPlayer);
	MapPotal->EndOverlapDelegate.BindUObject(this, UMapManager::PotalOutPlayer);

	return true;
}
bool UMapManager::PlayerListSpawn(UWorld* World)
{
	if (World == nullptr) return false;
	for (auto& CurrentData : CharacterDataList)
	{
		FActorSpawnParameters Param;
		Param.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn;
		AActor* actor =
			World->SpawnActor
			(APlayerCharacter::StaticClass(), &CurrentData.Location, &CurrentData.Rotator, Param);
		
		if (actor == nullptr) return false;
 		
		APlayerCharacter* Player = Cast<APlayerCharacter>(actor); 

		if(CurrentData.ObjectID != PlayerID)
			Player->InitializeCharacter(false, CurrentData);
		else
			Player->InitializeCharacter(true, CurrentData);

		auto GameInstance = Cast <UXRGameInstance>(Player->GetGameInstance());

		if (Player)
		{
			APlayerCharacter** CheckPlayer = CharacterList.Find(CurrentData.ObjectID);
			if (CheckPlayer != nullptr)
			{
				Player->Destroy();
				continue;
			}
			if (Player->PlayerStatComp == nullptr) continue;
	
			//Player->PlayerStatComp->GetStatDataFromServer(Input);
			CharacterList.Add(CurrentData.ObjectID, Player);
		}
	}
	CharacterDataList.clear();
	return true;
}
bool UMapManager::PossessPlayer(UWorld* World)
{
	APlayerCharacter* Player = CharacterList.FindRef(PlayerID);
	if (Player == nullptr) return false;
	AXRPlayerController* MyPlayerController = Cast<AXRPlayerController>(World->GetPlayerControllerIterator()->Get());
	if (MyPlayerController == nullptr) return false;
	MyPlayerController->Possess(Player);
	PlayerCharacter = Player;
	return true;
}
bool UMapManager::MonsterListSpawn(UWorld* World)
{
	if (World == nullptr) return false;

	for (auto& CurrentData : MonsterDataList)
	{
		FActorSpawnParameters param;
		param.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn;
		AActor* actor =
			World->SpawnActor
			(ANonePlayerCharacter::StaticClass(), &CurrentData.Location, &CurrentData.Rotator, param);

		ANonePlayerCharacter* Monster = Cast<ANonePlayerCharacter>(actor);
		if (Monster)
		{
			ANonePlayerCharacter** CheckMonster = MonsterList.Find(CurrentData.ObjectID);
			if (CheckMonster != nullptr)
			{
				Monster->Destroy();
				continue;
			}
			//if (Monster->PlayerStatComp == nullptr) return false;
			//Monster->PlayerStatComp->GetStatDataFromServer(Input);
			Monster->NpcLoadStart(CurrentData.MonsterID);
			Monster->ObjectID = CurrentData.ObjectID;
			MonsterList.Add(CurrentData.ObjectID, Monster);
		}
	}
	MonsterDataList.clear();
	return true;
}

bool UMapManager::RemotePlayerSpawn(UWorld* world)
{
	return PlayerListSpawn(world);
}
bool UMapManager::DeleteRemotePlayer(UWorld* World)
{
	return false;
}
void UMapManager::PotalInPlayer(AActor* Character)
{
	APlayerCharacter* Player = Cast<APlayerCharacter>(Character);
	if (Player == nullptr) return;

	const int64_t* Key = CharacterList.FindKey(Player);
	if (Key == nullptr) return;

	auto FindKey = std::find(InPotalPlayerList.front, InPotalPlayerList.end, *Key);
	if (FindKey == InPotalPlayerList.end())
	{
		InPotalPlayerList.push_back(*Key);
	}
	if (InPotalPlayerList.size() == CharacterList.GetAllocatedSize())
	{
		SendChangeZoneFromClient();
	}
}
void UMapManager::PotalOutPlayer(AActor* Character)
{
	APlayerCharacter* Player = Cast<APlayerCharacter>(Character);
	if (Player == nullptr) return;

	const int64_t* Key = CharacterList.FindKey(Player);
	if (Key == nullptr) return;

	auto FindKey = std::find(InPotalPlayerList.front, InPotalPlayerList.end, *Key);
	if (FindKey != InPotalPlayerList.end())
	{
		InPotalPlayerList.erase(FindKey);
	}
}
// 서버로 데이터 송신
void UMapManager::SendChangeZoneFromClient()
{
	XRLOG(Warning, TEXT("ZoneChange"));

	std::string ID = AccountManager::GetInstance().GetAccountID();
	OutputStream out;
	out.WriteOpcode(ENetworkCSOpcode::kZoneConrifmRequest);
	out.WriteInt32(MapPotalData.MapID);
	out.CompletePacketBuild();
	GetNetMgr().SendPacket(out);
}