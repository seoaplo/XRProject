// Fill out your copyright notice in the Description page of Project Settings.


#include "ItemManager.h"
#include "XRGameInstance.h"

UItemManager::UItemManager()
{

	FString EquipmentDataPath = TEXT("DataTable'/Game/Resources/DataTable/EquipTable.EquipTable'");

	static ConstructorHelpers::FObjectFinder<UDataTable> DT_EQUIPITEM(*EquipmentDataPath);
	if (DT_EQUIPITEM.Succeeded())
		EquipmentItemDataTable = DT_EQUIPITEM.Object;

}

UItemManager::~UItemManager()
{
}

TOptional<UItem*> UItemManager::CreateItem(InputStream & input)
{
	EItemType Type = (EItemType)input.ReadInt32();
	if (EquipmentItemDataTable == nullptr)
		check(false);
	switch (Type)
	{
	case EItemType::NONE:
	{
		break;
	}
	case EItemType::ETC:
	{
		int ID = input.ReadInt32();
		int Count = input.ReadInt32();
		return nullptr;
		break;
	}
	case EItemType::CONSUMPTION:
	{
		int ID = input.ReadInt32();
		int Count = input.ReadInt32();
		return nullptr;
		break;
	}
	case EItemType::EQUIPMENT:
	{
		int ID = input.ReadInt32();
		FEquipmentTableResource* Table = EquipmentItemDataTable->FindRow<FEquipmentTableResource>(FName(*(FString::FromInt(ID))), TEXT("z"));

		if (Table == nullptr)
			check(false);

		UItemEquipment* Item = NewObject<UItemEquipment>();

		Item->AddInfo.AddATK = input.ReadInt32();
		Item->AddInfo.AddDEF = input.ReadInt32();
		Item->AddInfo.AddSTR = input.ReadInt32();
		Item->AddInfo.AddDEX = input.ReadInt32();
		Item->AddInfo.AddINT = input.ReadInt32();

		Item->DefaultInfo.ID = ID;
		Item->DefaultInfo.MaleMeshResourceID = Table->MaleMeshId;
		Item->DefaultInfo.FemaleMeshResourceID = Table->FemaleMeshId;
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
		int Count = input.ReadInt32();
		return Item;
		break;
	}
	default:
		break;
	}
	return nullptr;
}

TOptional<UItem*> UItemManager::GetItemFromId(EItemType Type, int32 ID)
{
	if (EquipmentItemDataTable == nullptr)
		check(false);
	switch (Type)
	{
	case EItemType::EQUIPMENT:
	{
		FEquipmentTableResource* Table = EquipmentItemDataTable->FindRow<FEquipmentTableResource>(FName(*(FString::FromInt(ID))), TEXT("z"));

		if (Table == nullptr)
			check(false);

		UItemEquipment* Item = NewObject<UItemEquipment>();

		Item->DefaultInfo.ID = ID;
		Item->DefaultInfo.MaleMeshResourceID = Table->MaleMeshId;
		Item->DefaultInfo.FemaleMeshResourceID = Table->FemaleMeshId;
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
		break;
	case EItemType::CONSUMPTION:
		break;
	case EItemType::ETC:
		break;
	default:
		break;
	}
	return nullptr;
}

void UItemManager::BuildItem(EItemType Type, int32 ID, UWorld* World, APlayerCharacter* TargetCharacter)
{
	TOptional<UItem*> ItemOptional = GetItemFromId(Type, ID);
	//is valid

	UItem* RetItem = nullptr;
	APlayerCharacter* CurrentTargetCharacter = TargetCharacter;
	if (ItemOptional.IsSet())
	{
		RetItem = ItemOptional.GetValue();
		CurrentItemId = ID;
	}

	FSoftObjectPath AssetPath = nullptr;
	auto GameInstance = Cast<UXRGameInstance>(UGameplayStatics::GetGameInstance(World));

	if (RetItem->GetItemType() == EItemType::EQUIPMENT)
	{
		UItemEquipment* EquipmentItem = Cast<UItemEquipment>(RetItem);
		if(AccountManager::GetInstance().GetCurrentPlayerCharacter()->bIsMale)
			AssetPath = GameInstance->GetXRAssetMgr()->FindResourceFromDataTable(EquipmentItem->DefaultInfo.MaleMeshResourceID);
		else
			AssetPath = GameInstance->GetXRAssetMgr()->FindResourceFromDataTable(EquipmentItem->DefaultInfo.FemaleMeshResourceID);
	}
	FStreamableDelegate AssetLoadDelegate;
	AssetLoadDelegate = FStreamableDelegate::CreateUObject(this, &UItemManager::LoadItemSkMeshAssetComplete, 
		AssetPath, RetItem, CurrentTargetCharacter);

	GameInstance->GetXRAssetMgr()->ASyncLoadAssetFromPath(AssetPath, AssetLoadDelegate);
}


void UItemManager::LoadItemSkMeshAssetComplete(FSoftObjectPath AssetPath,UItem* Item, APlayerCharacter* Character)
{
	TSoftObjectPtr<USkeletalMesh> LoadedMesh(AssetPath);

	if (Character == nullptr)
	{
		AccountManager::GetInstance().GetCurrentPlayerCharacter()->ChangeEquipment(Item, LoadedMesh.Get());
	}
	else
	{
		Character->ChangeEquipment(Item, LoadedMesh.Get());
	}
	XRLOG(Warning, TEXT("%s, LoadComplete"), *Character->GetName());
}


