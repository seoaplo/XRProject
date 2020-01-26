// Fill out your copyright notice in the Description page of Project Settings.


#include "ItemManager.h"


ItemManager::ItemManager()
{
	FString EquipmentDataPath = TEXT(""); //아이템테이블 - 장비
	//static ConstructorHelpers::FObjectFinder<UDataTable> DT_EQUIPITEM(*EquipmentDataPath);
	//if (DT_EQUIPITEM.Succeeded())
	//	EquipmentItemDataTable = DT_EQUIPITEM.Object;

	//주의 : 로드 안됨
	EquipmentItemDataTable = LoadObject<UDataTable>(NULL, *EquipmentDataPath, NULL, LOAD_None, NULL);

	bItemLoaded = false;

}

ItemManager::~ItemManager()
{
}

TOptional<UItem*> ItemManager::GetItemFromId(int32 Type, int32 ID)
{
	if (EquipmentItemDataTable == nullptr)
		check(false);

	if (Type == 0)
	{
		FEquipmentDefaultInfo* Table = EquipmentItemDataTable->FindRow<FEquipmentDefaultInfo>(*(FString::FromInt(ID)), TEXT(""));
		
		if (Table == nullptr)
			check(false);

		UItemEquipment* Item = NewObject<UItemEquipment>();

		Item->DefaultInfo.ID = ID;
		Item->DefaultInfo.MaleMeshResourceID = Table->MaleMeshResourceID;
		Item->DefaultInfo.MaleMeshResourceID = Table->MaleMeshResourceID;
		Item->DefaultInfo.Name = Table->Name;
		Item->DefaultInfo.Icon = Table->Icon;
		Item->DefaultInfo.Type = Table->Type;
		Item->DefaultInfo.SubType = Table->SubType;
		Item->DefaultInfo.ReqLEV = Table->ReqLEV;
		Item->DefaultInfo.DEF = Table->DEF;
		Item->DefaultInfo.STR = Table->STR;
		Item->DefaultInfo.DEX = Table->DEX;
		Item->DefaultInfo.INT = Table->INT;
		Item->DefaultInfo.ReqSTR = Table->ReqSTR;
		Item->DefaultInfo.ReqDEX = Table->ReqDEX;
		Item->DefaultInfo.ReqINT = Table->ReqINT;
		Item->DefaultInfo.ToolTip = Table->ToolTip;
		
		return Item;
	}
	return nullptr;
}

void ItemManager::BuildItem(int32 ID, UWorld* World)
{
	TOptional<UItem*> ItemOptional = GetItemFromId(0, ID);
	//is valid
	
	if (ItemOptional.IsSet())
	{
		RetItem = ItemOptional.GetValue();
		CurrentItemId = ID;
		bItemLoaded = true;
	}
	auto GameInstance = Cast<UXRGameInstance>(UGameplayStatics::GetGameInstance(World));
	FSoftObjectPath AssetPath = GameInstance->XRAssetManager->FindResourceFromDataTable(ID);


	FStreamableDelegate AssetLoadDelegate;
	//AssetLoadDelegate.BindUObject(this, &ItemManager::LoadItemSkMeshAssetComplete, AssetPath);
	AssetLoadDelegate.BindRaw(this, &ItemManager::LoadItemSkMeshAssetComplete, AssetPath);
	GameInstance->XRAssetManager->ASyncLoadAssetFromPath(AssetPath.ToString(), AssetPath, AssetLoadDelegate);
}

bool ItemManager::SetPlayerCharacter(APlayerCharacter * Character)
{
	if(Character == nullptr)
		return false;

	CurrentPlayerCharacter = Character;

	return true;
}

void ItemManager::LoadItemSkMeshAssetComplete(FSoftObjectPath AssetPath)
{
	TSoftObjectPtr<USkeletalMesh> LoadedMesh(AssetPath);

	if (bItemLoaded == true)
	{
		CurrentPlayerCharacter->ChangeEquipment(CurrentItemId, RetItem, LoadedMesh.Get());
		bItemLoaded = false;
	}

}
