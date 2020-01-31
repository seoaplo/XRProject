// Fill out your copyright notice in the Description page of Project Settings.


#include "ItemManager.h"
#include "XRGameInstance.h"

UItemManager::UItemManager()
{

	FString EquipmentDataPath = TEXT("DataTable'/Game/Resources/DataTable/EquipTable.EquipTable'");
	FString ConsumptionDataPath = TEXT("DataTable'/Game/Resources/DataTable/ConsumptionItemTable.ConsumptionItemTable'");
	FString ETCDataPath = TEXT("DataTable'/Game/Resources/DataTable/ETCItemTable.ETCItemTable'");
	FString PartsDataPath = TEXT("DataTable'/Game/Resources/DataTable/PartsTable.PartsTable'");

	static ConstructorHelpers::FObjectFinder<UDataTable> DT_EQUIPITEM(*EquipmentDataPath);
	static ConstructorHelpers::FObjectFinder<UDataTable> DT_CONSUMPTIONITEM(*ConsumptionDataPath);
	static ConstructorHelpers::FObjectFinder<UDataTable> DT_ETCITEM(*ETCDataPath);
	static ConstructorHelpers::FObjectFinder<UDataTable> DT_PARTS(*PartsDataPath);



	if (DT_EQUIPITEM.Succeeded()) EquipmentItemDataTable = DT_EQUIPITEM.Object;
	if (DT_CONSUMPTIONITEM.Succeeded()) ConsumptionItemDataTable = DT_CONSUMPTIONITEM.Object;
	if (DT_ETCITEM.Succeeded()) ETCItemDataTable = DT_ETCITEM.Object;
	if (DT_PARTS.Succeeded()) PartsDataTable = DT_PARTS.Object;

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
		FETCTableResource* Table = ETCItemDataTable->FindRow<FETCTableResource>(FName(*(FString::FromInt(ID))), TEXT("z"));
		if (Table == nullptr)
			check(false);
		UItemETC* Item = NewObject<UItemETC>();
		Item->SetCount(input.ReadInt32());
		return Item;
		break;
	}
	case EItemType::CONSUMPTION:
	{
		int ID = input.ReadInt32();
		int Count = input.ReadInt32();
		FConsumptionTableResource* Table = ConsumptionItemDataTable->FindRow<FConsumptionTableResource>(FName(*(FString::FromInt(ID))), TEXT("z"));
		if (Table == nullptr)
			check(false);
		UItemConsumption* Item = NewObject<UItemConsumption>();
		Item->SetCount(input.ReadInt32());
		return Item;
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
		Item->DefaultInfo.IconResourceID = Table->IconID;
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

void UItemManager::GetIcon(UTexture2D* OutTexture, int ID)
{
	FSoftObjectPath AssetPath;
	auto GI = Cast<UXRGameInstance>(GetWorld()->GetGameInstance());
	GI->GetXRAssetMgr()->FindResourceFromDataTable(ID);
	FStreamableDelegate ResultCallback;
	ResultCallback.BindLambda([AssetPath, &OutTexture, this]()
	{
		TSoftObjectPtr<UTexture2D> Loaded(AssetPath);
		OutTexture = Loaded.Get();
		XRLOG(Warning, TEXT("IconLoadComplete"));
	});
	GI->GetXRAssetMgr()->ASyncLoadAssetFromPath(AssetPath, ResultCallback);
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
		Item->DefaultInfo.IconResourceID = Table->IconID;
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
}


