// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AccountManager.h"
#include "XRGameInstance.h"
#include "NetworkOpcode.h"
#include "PlayerCharacter.h"
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

public:
	/*현재 미사용 Dummy 코드이며, 인게임에서 캐릭터가 생성될 때 패킷에 의해 장비, HP등을 초기화하고자할때 개선해서 사용할 예정. */
	//void PlayerCharacterInitializeFromServer(InputStream& input);
	/*현재 미사용 Dummy 코드이며, 인게임에서 인벤토리 아이템을 클릭하여 장비를 교체할 때 사용할 예정(개선필요없음)*/
	//void PlayerCharacterItemChange(InputStream& input);
	/*Hair나 Face같은 아이템이 아닌 파츠들을 교체할 때 사용하는 코드. 패킷에 의해 제어됨*/
	//void LoadPartsComplete(FSoftObjectPath AssetPath, EPartsType Type);

public:
	virtual void BeginPlay() override;
	virtual void Tick(float deltatime) override;
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason);

	void SpawnRemotePlayer();
	void DeleteRemotePlayer();

	// widget data from server
	void NotifyMatchResult(class InputStream& input);
	void NotifyMatchCanceled(class InputStream& input);
private:
	FTimerHandle PacketExcuteTimerHandle;
};

