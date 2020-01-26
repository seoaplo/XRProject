// Fill out your copyright notice in the Description page of Project Settings.

#pragma once


#include "EngineMinimal.h"
#include "CustomSingleton.h"
#include "ItemEquipment.h"
#include "ItemConsumption.h"
#include "ItemETC.h"
#include "Engine/DataTable.h"
#include "PlayerCharacter.h"
#include "XRGameInstance.h"
#include "XRAssetMgr.h"
/**
 * 
 */
class XRPROJECT_API ItemManager : public CustomSingleton<ItemManager>
{
public:
	friend class CustomSingleton<ItemManager>;
private:
	ItemManager();

public:
	~ItemManager();

public:
	class UDataTable* EquipmentItemDataTable;
	FEquipmentDefaultInfo* EquipmentItemData;

	//소비템

	//기타템
private:
	APlayerCharacter* CurrentPlayerCharacter;
	UItem* RetItem;
	int32 CurrentItemId;
	bool bItemLoaded;
public:
	/*주의 : Type는 원래 장비인지, 소비인지 등이어야 하나 Xrproject.h를 건드려야 하므로
	합의하에 진행할 것
	일반적으로 근데 타입을 전해줄 방법이 없을텐데.. 서버가 타입도 정해주지 않으면 장비,소비,기타 테이블이 따로있으므로
	범용적으로 사용할 수 없음
	*/
	TOptional<UItem*> GetItemFromId(int32 Type, int32 ID);
	void BuildItem(int32 ID, UWorld* World);
	bool SetPlayerCharacter(APlayerCharacter* Character);
	void LoadItemSkMeshAssetComplete(FSoftObjectPath AssetPath);
};

#define ITEM_MGR ItemManager::GetInstance()