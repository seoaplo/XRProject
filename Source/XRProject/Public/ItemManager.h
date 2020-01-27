// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "EngineMinimal.h"
#include "CustomSingleton.h"
#include "ItemEquipment.h"
#include "ItemConsumption.h"
#include "ItemETC.h"
#include "Engine/DataTable.h"
#include "PlayerCharacter.h"
#include "XRAssetMgr.h"
#include "UObject/NoExportTypes.h"
#include "ItemManager.generated.h"

/**
 * 
 */
UCLASS()
class XRPROJECT_API UItemManager : public UObject
{
	GENERATED_BODY()

public:
	UItemManager();
	~UItemManager();

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
	TOptional<UItem*> GetItemFromId(EItemType Type, int32 ID);
	void BuildItem(EItemType Type, int32 ID, UWorld* World);
	bool SetPlayerCharacter(APlayerCharacter* Character);
	void LoadItemSkMeshAssetComplete(FSoftObjectPath AssetPath,UItem* testitem);
	
};

USTRUCT(BlueprintType)
struct FEquipmentTableResource : public FTableRowBase
{
	GENERATED_BODY()
public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Resource")
		FString Name;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Resource")
		int32 IconID;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Resource")
		int32 MaleMeshId;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Resource")
		int32 FemaleMeshId;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Resource")
		int32 Type;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Resource")
		int32 SubType;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Resource")
		int32 RequiredLevel;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Resource")
		int32 ATK;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Resource")
		int32 DEF;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Resource")
		int32 STR;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Resource")
		int32 DEX;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Resource")
		int32 INT;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Resource")
		int32 RequiredSTR;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Resource")
		int32 RequiredDEX;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Resource")
		int32 RequiredINT;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Resource")
		FString ToolTip;
};