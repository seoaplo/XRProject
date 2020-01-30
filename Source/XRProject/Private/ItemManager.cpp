// Fill out your copyright notice in the Description page of Project Settings.


#include "ItemManager.h"
#include "XRGameInstance.h"

UItemManager::UItemManager()
{
	FString EquipmentDataPath = TEXT("DataTable'/Game/Resources/DataTable/EquipTable.EquipTable'"); //아이템테이블 - 장비
	static ConstructorHelpers::FObjectFinder<UDataTable> DT_EQUIPITEM(*EquipmentDataPath);
	if (DT_EQUIPITEM.Succeeded())
		EquipmentItemDataTable = DT_EQUIPITEM.Object;

	//주의 : 로드 안됨
	//EquipmentItemDataTable = LoadObject<UDataTable>(NULL, *EquipmentDataPath, NULL, LOAD_None, NULL);

	bItemLoaded = false;

}

UItemManager::~UItemManager()
{
}

TOptional<UItem*> UItemManager::GetItemFromId(int32 Type, int32 ID)
{
	if (EquipmentItemDataTable == nullptr)
		check(false);

	if (Type == 0)
	{
		FEquipmentTableResource* Table = EquipmentItemDataTable->FindRow<FEquipmentTableResource>(FName(*(FString::FromInt(ID))), TEXT("z"));

		if (Table == nullptr)
			check(false);

		UItemEquipment* Item = NewObject<UItemEquipment>();

		Item->DefaultInfo.ID = ID;
		Item->DefaultInfo.MaleMeshResourceID = Table->MaleMeshId;
		Item->DefaultInfo.MaleMeshResourceID = Table->FemaleMeshId;
		Item->DefaultInfo.Name = Table->Name;
		Item->DefaultInfo.Icon = Table->IconID;
		Item->DefaultInfo.Type = Table->Type;
		Item->DefaultInfo.SubType = Table->SubType;
		Item->DefaultInfo.ReqLEV = Table->RequiredLevel;
		Item->DefaultInfo.DEF = Table->DEF;
		Item->DefaultInfo.STR = Table->STR;
		Item->DefaultInfo.DEX = Table->DEX;
		Item->DefaultInfo.INT = Table->INT;
		Item->DefaultInfo.ReqSTR = Table->RequiredSTR;
		Item->DefaultInfo.ReqDEX = Table->RequiredDEX;
		Item->DefaultInfo.ReqINT = Table->RequiredINT;
		Item->DefaultInfo.ToolTip = Table->ToolTip;
		Item->ItemType = EItemType::EQUIPMENT;

		return Item;
	}
	return nullptr;
}

void UItemManager::BuildItem(int32 ID, UWorld* World)
{
	TOptional<UItem*> ItemOptional = GetItemFromId(0, ID);
	//is valid

	if (ItemOptional.IsSet())
	{
		RetItem = ItemOptional.GetValue();
		CurrentItemId = ID;
		bItemLoaded = true;
	}

	FSoftObjectPath AssetPath = nullptr;
	auto GameInstance = Cast<UXRGameInstance>(UGameplayStatics::GetGameInstance(World));

	if (RetItem->GetItemType() == EItemType::EQUIPMENT)
	{
		UItemEquipment* EquipmentItem = Cast<UItemEquipment>(RetItem);
		AssetPath = GameInstance->GetXRAssetMgr()->FindResourceFromDataTable(EquipmentItem->DefaultInfo.ID);
	}

	FStreamableDelegate AssetLoadDelegate;
	//AssetLoadDelegate.BindUObject(this, &ItemManager::LoadItemSkMeshAssetComplete, AssetPath);
	AssetLoadDelegate.BindUObject(this, &UItemManager::LoadItemSkMeshAssetComplete, AssetPath);
	GameInstance->GetXRAssetMgr()->ASyncLoadAssetFromPath(AssetPath, AssetLoadDelegate);
}

bool UItemManager::SetPlayerCharacter(APlayerCharacter * Character)
{
	if (Character == nullptr)
		return false;

	CurrentPlayerCharacter = Character;

	return true;
}

void UItemManager::LoadItemSkMeshAssetComplete(FSoftObjectPath AssetPath)
{
	TSoftObjectPtr<USkeletalMesh> LoadedMesh(AssetPath);

	if (bItemLoaded == true)
	{
		CurrentPlayerCharacter->ChangeEquipment(CurrentItemId, RetItem, LoadedMesh.Get());
		bItemLoaded = false;
	}

}
