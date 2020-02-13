// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "NetworkManager.h"
#include "XRAssetMgr.h"
#include "ItemManager.h"
#include "MapManager.h"
#include "PlayerSkillManager.h"
#include "XRGameInstance.generated.h"

const float kSprintMovementSpeed = 750.0f;
const float kNormalMovementSpeed = 450.0f;
const float kMaxMovementAcceleration = 8192.0f;
const float kNormalMovementAcceleration = 2048.0f;
const float kRollingMovementSpeed = 800.0f;
const float kAttackMovementSpeed = 250.0f;

enum class StatBit : int64_t {
	kHP = 0x0000000000000001,
	kMaxHp = 0x0000000000000002,
	kAttackMin = 0x0000000000000004,
	kAttackMax = 0x0000000000000008,
	kDefence = 0x0000000000000010,
	kSpeed = 0x0000000000000020,
	kLv = 0x0000000000000040,
	kJob = 0x0000000000000080,
	kStr = 0x0000000000000100,
	kDex = 0x0000000000000200,
	kIntel = 0x0000000000000400,
	kGold = 0x0000000000000800,
	kExp = 0x0000000000001000,
	kMaxExp = 0x0000000000002000,
	kStamina = 0x0000000000004000,
	kMaxStamina = 0x0000000000008000,
};

class UIngameGameMode;
/**
 * 
 */
UCLASS()
class XRPROJECT_API UXRGameInstance : public UGameInstance
{
	GENERATED_BODY()
public:
	bool GetIsSuper() { return IsSuper; }
public:
    virtual void Init() override;
    virtual void Shutdown() override;
	void LobbytoGame();
    FORCEINLINE UNetworkManager& GetNetworkManager() {
        return *NetworkManager;

    }
	FORCEINLINE UXRAssetMgr* GetXRAssetMgr() { return XRAssetManager; }

	UMapManager& GetMapManager() { return *MapManager; }

	UPROPERTY(VisibleAnywhere, Meta = (AllowPrivateAccess = true))
		UXRAssetMgr* XRAssetManager;
	
	UPROPERTY(VisibleAnywhere, Meta = (AllowPrivateAccess = true))
		UItemManager* ItemManager;

	UPlayerSkillManager* GetPlayerSkillManager() { return PlayerSkillManager; }
	

public:


private:
	UPROPERTY(VisibleAnywhere, Meta = (AllowPrivateAccess = true))
		UNetworkManager* NetworkManager;
	UPROPERTY()
		UMapManager* MapManager;
	UPROPERTY()
		UPlayerSkillManager* PlayerSkillManager;
private:
	UPROPERTY(BlueprintReadWrite,EditAnywhere,Meta =(AllowPrivateAccess=true))
	bool  IsSuper = false;
private:
	void SendConfirmRequest(); /*존 입장 확인용*/
	void HandleEnterZone(class InputStream& input); /*첫 입장시 초기화 패킷*/
	/* 아래 4개의 함수는 HandleEnterZone에서 사용 될 함수로 각자 구현 요망*/
	void ReadBaseCharacterInfo(class InputStream& input); /*캐릭터 정보 읽기*/
	void ReadInventoryInfo(class InputStream& input); /*인벤토리 정보 읽기*/
	void ReadQuickSlot(class InputStream& input); /*퀵 슬롯 정보 읽기*/
	void ReadMapData(class InputStream& input); /*맵 데이터 정보 읽기*/
	void ReadSkillData(class InputStream& input); /*스킬 데이터 정보 읽기*/
	void SpawnCharacterFromServer(class InputStream& input);/* 실시간 캐릭터 스폰 데이터 읽기*/
	void UpdateCharacterPosition(class InputStream& input);
	void SetMonsterController(class InputStream& input);
	void UpdateMonsterAction(class InputStream& input);
	void UpdateCharacterMotion(class InputStream& input);
	void ActorDamaged(class InputStream& input);
	void CharacterWait(class InputStream& input);
	void CharacterSprint(class InputStream& input);
	void CharacterDead(class InputStream& input);
	void CharacterStatChange(class InputStream& input);
	void CharacterRolling(class InputStream& input);
	void UpdateInventory(class InputStream& input);
	void NotifyChat(class InputStream& input);
	void NotifySpawnPotal(class InputStream& input);
};

//넣었는데 문제있으면 말씀하십쇼 -수찬-
USTRUCT(BlueprintType)
struct FPartsResource : public FTableRowBase
{
	GENERATED_BODY()
public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "C_Resource")
	FString Name;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "C_Resource")
	int32 ResourceID;
};



USTRUCT(BlueprintType)
struct FXRDamageEvent : public FDamageEvent
{
	GENERATED_BODY()
public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "C_DamageInfo")
		int32 ID;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "C_DamageInfo")
		bool Intensity; //강격이면 true, 약격이면 false
};

#define GetNetMgr UNetworkManager::GetInstance
#define GetAssetMgr Cast<UXRGameInstance>(GetGameInstance())->GetXRAssetMgr
#define GetMapMgr Cast<UXRGameInstance>(GetGameInstance())->GetMapManager
