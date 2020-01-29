// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include <vector>
#include <string>
#include "PlayerCharacter.h"
#include "CharacterSelectWidget.h"
#include "CoreMinimal.h"
#include "XRGameInstance.h"
#include "Camera/CameraActor.h"
#include "XRProjectGameModeBase.h"
#include "CharacterSelectSceneGameMode.generated.h"


struct FCharacterSelectInfo
{
public:
	string Name;
	int32 Level;
	int32 Str;
	int32 Dex;
	int32 Int;
	int32 Job;
	int32 Face;
	int32 Hair;
	int32 Gold;
	int32 Zone;
	float x, y, z;
	int32 armor_itemid;
	int32 hand_itemid;
	int32 shoes_itemid;;
	int32 weapon_itemid;
	int32 sub_weapon_itemid;
	int32 gender;
};

/**
 * 
 */
UCLASS()
class XRPROJECT_API ACharacterSelectSceneGameMode : public AXRProjectGameModeBase
{
	GENERATED_BODY()
public:
	ACharacterSelectSceneGameMode();
public:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "C_GameMode")
		TSubclassOf<UCharacterSelectWidget> LoginWidget;
	UPROPERTY()
		UCharacterSelectWidget* CurrentWidget;

public:
	std::vector<APlayerCharacter*> CharacterList; // 현재 접속한 계정의 캐릭터의 리스트(패킷으로 받아와야 한다.)
private:
	ACameraActor* MainCamera;
	FVector MainCameraLocation;
	FVector CharacterActorLocation;
	class UDataTable* PartsDataTable; //Hair, Face 데이터테이블
	int64 BeforeSlotNumber;
public:
	virtual void BeginPlay() override;
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;
	virtual void Tick(float DeltaSeconds) override;
public:
	void ChangeSelectedCharacter(int64 SlotNumber);
	void HandleCharacterCreateFail(class InputStream& input);
	void HandleCharacterList(class InputStream& input);
	void HandleCharacterSlot(class InputStream& input);
	void HandleCharacterCreate(class InputStream& input);
	void HandleCharacterDelete(class InputStream& input);
	void SendConfirmRequest();
	void CreatePlayerCharacter(APlayerCharacter* Character, FCharacterSelectInfo& Info);
	void LoadPartsComplete(FSoftObjectPath AssetPath, EPartsType Type, APlayerCharacter* Character);
};