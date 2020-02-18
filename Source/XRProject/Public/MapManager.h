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
#include "PotalTrigger.h"
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
	UMapManager();
public:
	int64_t GetPlayerID() { return PlayerID; }
	int32_t GetMapID() { return LevelID; }
	APlayerCharacter* GetPlayer() { return PlayerCharacter; }
	const TMap<int64_t, APlayerCharacter*>& GetCharacterList() { return CharacterList; }
	const TMap<int64_t, ANonePlayerCharacter*>& GetMonsterList() { return MonsterList; }
public:
	bool Init();
	bool Clear();

	bool IsDungeon() { if ((LevelID % 100) > 0) return true; else return false;}
	// 맵에 입장
	void ReadMapDataFromServer(InputStream& Input);
	void ReadPlayerFromServer(InputStream& Input);
	void ReadPossesPlayerFromServer(InputStream& Input);
	void ReadMosnterFromServer(InputStream& Input);
	bool ReadPlayerSpawnFromServer(InputStream& Input);
	bool ReadPlayerDeleteFromServer(InputStream& Input);

	// 서버로 데이터 송신
	void SendChangeZoneFromClient();

	// 오브젝트 탐색
	APlayerCharacter* FindPlayer(int64_t ObjectID);
	ANonePlayerCharacter* FindMonster(int64_t ObjectID);
	
	// 오픈 레벨
	bool OpenMap(UWorld* World);
	bool PlayerListSpawn(UWorld* world);
	bool MonsterListSpawn(UWorld* world);
	//bool TriggerListSpawn(UWorld* world);
	bool RemotePlayerSpawn(UWorld* world);
	bool PossessPlayer(UWorld* World);
	// 포탈 관련 함수
	void PotalInPlayer(AActor* OtherCharacter);
	// 삭제
	bool DeleteRemotePlayer(UWorld* World);

	void InputExpData(class InputStream& input); //EXP 받을방법이 없어서 임의로 만든 클래스.
public:
	CharacterDataProcess Spawn_Character;
	CharacterDataProcess Delete_Character;
	TArray<int> CharacterSkillIDList; //캐릭터 스킬 리스트 받아놓는곳

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
