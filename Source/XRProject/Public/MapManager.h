// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include <vector>
#include "UObject/NoExportTypes.h"
#include "PlayerCharacter.h"
#include "NonePlayerCharacter.h"
#include "NetworkOpcode.h"
#include "MapManager.generated.h"

/**
 * 
 */
UCLASS()
class XRPROJECT_API UMapManager : public UObject
{
	GENERATED_BODY()
public:
	int64_t GetPlayerID() { return PlayerID; }
public:
	bool Init(UWorld* world);
	bool Clear();
	bool SpawnPlayer(int64_t objectid, FVector position, FRotator rotator);
private:
	int64_t PlayerID;
	UWorld* World;
	TMap<int64_t, APlayerCharacter*> CharacterList;
	TMap<int64_t, ANonePlayerCharacter*> MonsterList;
};


/*

CS
	// ZoneServer
	kZoneConrifmRequest = 1000,
	kZoneInitializeRequest,
	kNotifyCurrentChrPosition,

SC
	kUserEnterTheMap = 1000,
    kSpawnCharacter,
    kExitCharacter,
    kUpdateCharacterPosition,


    kSpawnMonster,


    kInventoryUpdate,
    kQuickSlotUpdate,
*/