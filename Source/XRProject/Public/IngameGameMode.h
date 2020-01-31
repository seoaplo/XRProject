// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AccountManager.h"
#include "XRGameInstance.h"
#include "NetworkOpcode.h"
#include "PlayerCharacter.h"
#include "MapManager.h"
#include "XRProjectGameModeBase.h"
#include "InGameMainWidget.h"
#include "IngameGameMode.generated.h"

/**
 * 
 */
UCLASS()
class XRPROJECT_API AIngameGameMode : public AXRProjectGameModeBase
{
	GENERATED_BODY()
	
public:
	AIngameGameMode();
	virtual ~AIngameGameMode();
public:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "C_GameMode")
		TSubclassOf<UInGameMainWidget> MainWidget;
	UPROPERTY()
		UInGameMainWidget* CurrentWidget;

	UMapManager& GetMapMgr() 
	{
		return *MapManager; 
	}

public:
	/*현재 미사용 Dummy 코드이며, 인게임에서 캐릭터가 생성될 때 패킷에 의해 장비, HP등을 초기화하고자할때 개선해서 사용할 예정. */
	void PlayerCharacterInitializeFromServer(InputStream& input);
	/*현재 미사용 Dummy 코드이며, 인게임에서 인벤토리 아이템을 클릭하여 장비를 교체할 때 사용할 예정(개선필요없음)*/
	void PlayerCharacterItemChange(InputStream& input);
	/*Hair나 Face같은 아이템이 아닌 파츠들을 교체할 때 사용하는 코드. 패킷에 의해 제어됨*/
	void LoadPartsComplete(FSoftObjectPath AssetPath, EPartsType Type);
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
public:
	virtual void BeginPlay() override;
	virtual void Tick(float deltatime) override;
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason);
private:
	UPROPERTY()
	UMapManager* MapManager;
};

#define MapMgr AIngameGameMode::GetMapMgr()