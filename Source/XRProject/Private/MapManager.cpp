// Fill out your copyright notice in the Description page of Project Settings.


#include "MapManager.h"
#include "XRPlayerController.h"

bool UMapManager::Init()
{
	PlayerID = -1;
	LevelID = -1;
	Spawn_Character.Unbind();
	Delete_Character.Unbind();

	MapList.Add(100, TEXT("LEVEL_Village"));
	MapList.Add(111, TEXT("LEVEL_Zone_1"));
	MapList.Add(112, TEXT("LEVEL_Zone_2"));
	MapList.Add(113, TEXT("LEVLE_Boss"));

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

	CharacterList.Reset();
	MonsterList.Reset();

	CharacterDataList.clear();
	MonsterDataList.clear();

	return true;
}

// ¸Ê¿¡ ÀÔÀå


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
bool UMapManager::PlayerListSpawn(UWorld* World)
{
	if (World == nullptr) return false;
	for (auto& CurrentData : CharacterDataList)
	{
		AActor* actor =
			World->SpawnActor
			(APlayerCharacter::StaticClass(), &CurrentData.Location, &CurrentData.Rotator);
	
		APlayerCharacter* Player = Cast<APlayerCharacter>(actor);
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
		AActor* actor =
			World->SpawnActor
			(ANonePlayerCharacter::StaticClass(), &CurrentData.Location, &CurrentData.Rotator);

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