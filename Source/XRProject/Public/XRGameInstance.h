// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "NetworkManager.h"
#include "XRAssetMgr.h"
#include "ItemManager.h"
#include "MapManager.h"
#include "XRGameInstance.generated.h"

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
	void ReqEnterZone();
    FORCEINLINE UNetworkManager& GetNetworkManager() {
        return *NetworkManager;

    }
	FORCEINLINE UXRAssetMgr* GetXRAssetMgr() { return XRAssetManager; }

	UMapManager& GetMapMgr() { return *MapManager; }

	UPROPERTY(VisibleAnywhere, Meta = (AllowPrivateAccess = true))
		UXRAssetMgr* XRAssetManager;
	
	UPROPERTY(VisibleAnywhere, Meta = (AllowPrivateAccess = true))
		UItemManager* ItemManager;

private:
	UPROPERTY(VisibleAnywhere, Meta = (AllowPrivateAccess = true))
		UNetworkManager* NetworkManager;
	UPROPERTY()
		UMapManager* MapManager;
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
	void SpawnCharacterFromServer(class InputStream& input);
	void UpdateCharacterPosition(class InputStream& input);
	void SetMonsterController(class InputStream& input);
	void UpdateMonsterAction(class InputStream& input);
	void UpdateCharacterMotion(class InputStream& input);
	void ActorDamaged(class InputStream& input);

	void NotifyChat(class InputStream& input);

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


#define GetNetMgr UNetworkManager::GetInstance
#define GetAssetMgr Cast<UXRGameInstance>(GetGameInstance())->GetXRAssetMgr
#define GetMapMgr Cast<UXRGameInstance>(GetGameInstance())->GetMapMgr
