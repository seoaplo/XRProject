// Fill out your copyright notice in the Description page of Project Settings.


#include "MapManager.h"

bool UMapManager::Init(UWorld* world, UNetworkManager& networkmanager)
{

	if (world == nullptr) return false;

	World = world;
	//SpawnPlayer(1, FVector(-6370.0, 850.0, 480.0), FRotator(0, 0, 0));
	//PossessPlayer(1, FVector(-6370.0, 850.0, 480.0), FRotator(0, 0, 0));
	return true;
}
bool UMapManager::Clear()
{
	CharacterList.Reset();
	MonsterList.Reset();

	return true;
}

// ¸Ê¿¡ ÀÔÀå
void UMapManager::ReadMapDataFromServer(InputStream& input)
{
	int32_t characterlistsize = 0;
	int32_t monsterlistsize = 0;

	input >> characterlistsize;
	for (int iCount = 0; iCount < characterlistsize; iCount++)
	{
		int64_t ObjectID;
		FVector PlayerLocation;
		FRotator PlayerRotator;
		input >> ObjectID;
		input >> PlayerLocation;
		input >> PlayerRotator;

		float HP = input.ReadFloat32();
		float MAXHP = input.ReadFloat32();
		float AttackMin = input.ReadFloat32();
		float AttackMax = input.ReadFloat32();
		float AttackRange = input.ReadFloat32();
		float AttackSpeed = input.ReadFloat32();
		float Defense = input.ReadFloat32();
		float Speed = input.ReadFloat32();
		std::string Name = input.ReadCString();
		int Level = input.ReadInt32();
		int Gender = input.ReadInt32();
		int FaceID = input.ReadInt32();
		int HairID = input.ReadInt32();
		int Str = input.ReadInt32();
		int Dex = input.ReadInt32();
		int Intel = input.ReadInt32();
		float Stamina = input.ReadFloat32();
		float MaxStamina = input.ReadFloat32();

		int EquipmentSize = 4;
		for (int i = 0; i < EquipmentSize; i++)
		{
			int Type = input.ReadInt32();
			if (Type)
			{
				if (Type == 3)
				{
					int ID = input.ReadInt32();
					int AddATK = input.ReadInt32();
					int AddDEF = input.ReadInt32();
					int AddSTR = input.ReadInt32();
					int AddDex = input.ReadInt32();
					int AddInt = input.ReadInt32();
				}
				int Count = input.ReadInt32();
			}
		}

		SpawnPlayer(ObjectID, PlayerLocation, PlayerRotator);
	}

	input >> monsterlistsize;
	for (int iCount = 0; iCount < monsterlistsize; iCount++)
	{
		int64_t ObjectID;
		int32   MonsterID;
		FVector MonsterLocation;
		FRotator MonsterRotator;

		input >> MonsterID;
		input >> ObjectID;
		input >> MonsterLocation;
		input >> MonsterRotator;

		float HP = input.ReadFloat32();
		float MAXHP = input.ReadFloat32();
		float AttackMin = input.ReadFloat32();
		float AttackMax = input.ReadFloat32();
		float AttackRange = input.ReadFloat32();
		float AttackSpeed = input.ReadFloat32();
		float Defense = input.ReadFloat32();
		float Speed = input.ReadFloat32();

		SpawnMonster(MonsterID,ObjectID, MonsterLocation, MonsterRotator);
	}
}

bool UMapManager::SpawnMonster(int32 MonsterID, int64_t objectid, FVector position, FRotator rotator)
{
	AActor* actor =
		World->SpawnActor
		(ANonePlayerCharacter::StaticClass(), &position, &rotator);

	ANonePlayerCharacter* Monster = Cast<ANonePlayerCharacter>(actor);
	if (Monster)
	{
		Monster->NpcLoadStart(MonsterID);
		ANonePlayerCharacter* CheckMonster = MonsterList.FindOrAdd(objectid);
		if (CheckMonster == nullptr)
		{
			CheckMonster = Monster;
		}
		else
		{
			Monster->Destroy();
		}
		return true;
	}
	else return false;
}
bool UMapManager::PossessPlayer(int64_t objectid, FVector position, FRotator rotator)
{
	APlayerCharacter* CheckPlayer = CharacterList.FindRef(objectid);
	if (CheckPlayer == nullptr) return false;
	World->GetPlayerControllerIterator()->Get()->Possess(CheckPlayer);
	return true;
}

bool UMapManager::SpawnPlayer(int64_t objectid, FVector position, FRotator rotator)
{
	AActor* actor = 
		World->SpawnActor
		(APlayerCharacter::StaticClass(), &position, &rotator);

	APlayerCharacter* Player = Cast<APlayerCharacter>(actor);
	if (Player)
	{
		APlayerCharacter** CheckPlayer = &CharacterList.FindOrAdd(objectid);
		if (*CheckPlayer == nullptr)
		{
			*CheckPlayer = Player;
		}
		else
		{
			Player->Destroy();
		}
		return true;
	}
	else return false;
}

APlayerCharacter* UMapManager::FindPlayer(int64_t objectid)
{
	APlayerCharacter* FindedPlayer = CharacterList.FindRef(objectid);
	return FindedPlayer;
}