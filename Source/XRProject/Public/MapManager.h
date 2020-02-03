// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include <vector>
#include "NetworkOpcode.h"
#include "MapPacketHelper.h"
#include "NetworkManager.h"
#include "PlayerCharacter.h"
#include "NonePlayerCharacter.h"
#include "UObject/NoExportTypes.h"
#include "MapManager.generated.h"

/**
 * 작성자 : 서승석
 */

struct CharacterData
{
	// Actor Spawn Data
	int64_t ObjectID = -1;
	FVector Location = FVector(0, 0, 0);
	FRotator Rotator = FRotator(0, 0, 0);

	// Base Character Stat Data
	int Current_HP = -1;
	int Max_HP = -1;
	int Attack_Min = -1;
	int Attack_Max = -1;
	int Attack_Range = -1;
	int Attack_Speed = -1;
	int Speed = -1;
	int Defence = -1;

	std::string Name;
	int Level = -1;
	int Gender= -1;
	int FaceID= -1;
	int HairID= -1;
	int STR = -1;
	int DEX = -1;
	int INT = -1;
	int CurrentStamina = -1;
	int MaxStamina = -1;


	// Equipment
	int Type = -1;
	int ID = -1;
	int AddATK = -1;
	int AddDEF = -1;
	int AddSTR = -1;
	int AddDex = -1;
	int AddInt = -1;
	int Count = -1;
};

struct MonsterData
{
	// Actor Spawn Data
	int32_t MonsterID = -1;
	int64_t ObjectID = -1;
	FVector Location = FVector(0, 0, 0);
	FRotator Rotator = FRotator(0, 0, 0);

	// Base Character Stat Data
	int Current_HP = -1;
	int Max_HP = -1;
	int Attack_Min = -1;
	int Attack_Max = -1;
	int Attack_Range = -1;
	int Attack_Speed = -1;
	int Speed = -1;
	int Defence = -1;
};

UCLASS()
class XRPROJECT_API UMapManager : public UObject
{
	GENERATED_BODY()
public:
	int64_t GetPlayerID() { return PlayerID; }
	bool	GetCheckReady() { return CheckReady;  }
	static void	TemporaryEnterZone();
public:
	bool Init();
	bool Clear();

	// 맵에 입장
	void ReadMapDataFromServer(InputStream& Input);
	void ReadPlayerFromServer(InputStream& Input);
	void ReadPossesPlayerFromServer(InputStream& Input);
	void ReadMosnterFromServer(InputStream& Input);
	bool ReadPlayerSpawnFromServer(InputStream& Input);

	APlayerCharacter* FindPlayer(int64_t ObjectID);
	ANonePlayerCharacter* FindMonster(int64_t ObjectID);

	// 스폰 함수들
	bool PlayerListSpawn(UWorld* world);
	bool MonsterListSpawn(UWorld* world);
	bool RemotePlayerSpawn(UWorld* world);
public:
	bool InitComplete;
	bool PlayerSpawnReady;
private:
	int32_t LevelID;
	int64_t PlayerID;
	UWorld* World;
	std::vector<CharacterData> CharacterDataList;
	std::vector<MonsterData> MonsterDataList;

	TMap<int64_t, APlayerCharacter*> CharacterList;
	TMap<int64_t, ANonePlayerCharacter*> MonsterList;
	static bool CheckReady;
};
