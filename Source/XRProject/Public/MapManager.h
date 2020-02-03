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
UCLASS()
class XRPROJECT_API UMapManager : public UObject
{
	GENERATED_BODY()
public:
	int64_t GetPlayerID() { return PlayerID; }
public:
	bool Init(UWorld* world, UNetworkManager& networkmanager);
	bool Clear();

	// 맵에 입장
	void ReadMapDataFromServer(InputStream& input);
	bool SpawnPlayer(int64_t objectid, FVector position, FRotator rotator);
	bool PossessPlayer(int64_t objectid, FVector position, FRotator rotator);
	APlayerCharacter* FindPlayer(int64_t objectid);

	// 몬스터 스폰 함수
	bool SpawnMonster(int32 MonsterID, int64_t objectid, FVector position, FRotator rotator);
private:
	int64_t PlayerID;
	UWorld* World;
	TMap<int64_t, APlayerCharacter*> CharacterList;
	TMap<int64_t, ANonePlayerCharacter*> MonsterList;
};
