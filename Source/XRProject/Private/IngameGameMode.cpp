// Fill out your copyright notice in the Description page of Project Settings.


#include "IngameGameMode.h"

AIngameGameMode::AIngameGameMode()
{
	PrimaryActorTick.bCanEverTick = true;
	DefaultPawnClass = APlayerCharacter::StaticClass();
}

AIngameGameMode::~AIngameGameMode()
{
}



void AIngameGameMode::BeginPlay()
{
	Super::BeginPlay();
	MapMgr.Init(GetWorld(), GetNetMgr());
	/*GetNetMgr().GetPacketReceiveDelegate(ENetworkSCOpcode::kCharacterSlotNotify)->
		BindUObject(this, &AIngameGameMode::PlayerCharacterInitializeFromServer);*/

	//GetNetMgr().Connect("192.168.0.118", 8181, nullptr);
}

void AIngameGameMode::Tick(float deltatime)
{
	Super::Tick(deltatime);
	//GetNetMgr().Update();
}

void AIngameGameMode::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	Super::EndPlay(EndPlayReason);
	MapMgr.Clear();
	//GetNetMgr().Close();
}


void AIngameGameMode::PlayerCharacterInitializeFromServer(InputStream & input)
{
	UPlayerCharacterStatComponent* MyComponent = AccountManager::GetInstance().GetCurrentPlayerCharacter()->PlayerStatComp;
	APlayerCharacter* MyCharacter = AccountManager::GetInstance().GetCurrentPlayerCharacter();
	int32 UnusedData = 0;
	int32 TempData = 0; //ID등 필요한 정보 임시 기입
	input >> UnusedData; //slot
	MyComponent->SetCharacterName(input.ReadCString().c_str());
	input >> MyComponent->Level; input >> MyComponent->STR; input >> MyComponent->DEX; input >> MyComponent->INT;
	input >> UnusedData; //Job
	input >> TempData;

	auto GameInstance = Cast<UXRGameInstance>(UGameplayStatics::GetGameInstance(GetWorld()));
	
	//헤어파츠
	FSoftObjectPath HairAssetPath = nullptr;
	HairAssetPath = GameInstance->GetXRAssetMgr()->FindResourceFromDataTable(TempData);
	FStreamableDelegate HairAssetLoadDelegate;
	HairAssetLoadDelegate = FStreamableDelegate::CreateUObject(this, &AIngameGameMode::LoadPartsComplete,
		HairAssetPath, EPartsType::HAIR);
	GameInstance->GetXRAssetMgr()->ASyncLoadAssetFromPath(HairAssetPath, HairAssetLoadDelegate);
	

	input >> TempData;

	//페이스 파츠
	FSoftObjectPath FaceAssetPath = nullptr;
	FaceAssetPath = GameInstance->GetXRAssetMgr()->FindResourceFromDataTable(TempData);
	FStreamableDelegate FaceAssetLoadDelegate;
	FaceAssetLoadDelegate = FStreamableDelegate::CreateUObject(this, &AIngameGameMode::LoadPartsComplete,
		FaceAssetPath, EPartsType::FACE);
	GameInstance->GetXRAssetMgr()->ASyncLoadAssetFromPath(FaceAssetPath, FaceAssetLoadDelegate);

	input >> UnusedData;  input >> UnusedData; input >> UnusedData; input >> UnusedData; input >> UnusedData;

	input >> TempData;
	GameInstance->ItemManager->BuildItem(EItemType::EQUIPMENT, TempData, GetWorld());
	input >> TempData;
	GameInstance->ItemManager->BuildItem(EItemType::EQUIPMENT, TempData, GetWorld());
	input >> TempData;
	GameInstance->ItemManager->BuildItem(EItemType::EQUIPMENT, TempData, GetWorld());
	input >> TempData;
	GameInstance->ItemManager->BuildItem(EItemType::EQUIPMENT, TempData, GetWorld());
	input >> TempData;
	GameInstance->ItemManager->BuildItem(EItemType::EQUIPMENT, TempData, GetWorld());
	input >> TempData;
	AccountManager::GetInstance().GetCurrentPlayerCharacter()->PlayerStatComp->Gender = TempData;
}

void AIngameGameMode::PlayerCharacterItemChange(InputStream& input)
{
	int Type = -888; int ID = 0;
	auto GameInstance = Cast<UXRGameInstance>(UGameplayStatics::GetGameInstance(GetWorld()));
	input >> Type; input >> ID;
	
	if (Type < 0 || ID == 0)
		check(false);

	GameInstance->ItemManager->BuildItem(EItemType::EQUIPMENT, ID, GetWorld());
}

void AIngameGameMode::LoadPartsComplete(FSoftObjectPath AssetPath, EPartsType Type)
{
	TSoftObjectPtr<USkeletalMesh> LoadedMesh(AssetPath);

	AccountManager::GetInstance().GetCurrentPlayerCharacter()->ChangePartsComponentsMesh(Type, LoadedMesh.Get());


}
