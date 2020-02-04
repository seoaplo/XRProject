// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include <vector>
#include <list>
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

DECLARE_DELEGATE(CharacterDataProcess)

UCLASS()
class XRPROJECT_API UMapManager : public UObject
{
	GENERATED_BODY()
public:
	int64_t GetPlayerID() { return PlayerID; }
	APlayerCharacter* GetPlayer() { return PlayerCharacter; }
public:
	bool Init();
	bool Clear();

	// 맵에 입장
	void ReadMapDataFromServer(InputStream& Input);
	void ReadPlayerFromServer(InputStream& Input);
	void ReadPossesPlayerFromServer(InputStream& Input);
	void ReadMosnterFromServer(InputStream& Input);
	bool ReadPlayerSpawnFromServer(InputStream& Input);
	bool ReadPlayerDeleteFromServer(InputStream& Input);



	// 오브젝트 탐색
	APlayerCharacter* FindPlayer(int64_t ObjectID);
	ANonePlayerCharacter* FindMonster(int64_t ObjectID);
	
	// 오픈 레벨
	bool OpenMap(UWorld* World);
	// 스폰 함수들
	bool PlayerListSpawn(UWorld* world);
	bool MonsterListSpawn(UWorld* world);
	bool RemotePlayerSpawn(UWorld* world);
	bool PossessPlayer(UWorld* World);

	bool DeleteRemotePlayer(UWorld* World);
public:
	CharacterDataProcess Spawn_Character;
	CharacterDataProcess Delete_Character;
private:
	UPROPERTY()
	APlayerCharacter* PlayerCharacter;

	int32_t LevelID;
	int64_t PlayerID;
	std::vector<CharacterData> CharacterDataList;
	std::vector<MonsterData> MonsterDataList;

	TMap<int64_t, APlayerCharacter*> CharacterList;
	TMap<int64_t, ANonePlayerCharacter*> MonsterList;
	TMap<int32_t, FName> MapList;
};
