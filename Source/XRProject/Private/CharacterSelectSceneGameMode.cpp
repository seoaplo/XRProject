#include "CharacterSelectSceneGameMode.h"
#include "PlayerCharacter.h"
#include "AccountManager.h"
#include "XRGameInstance.h"
#include "ChatingManager.h"
#include "NickNameWidget.h"
#include "InputStream.h"
#include <functional>
#include <locale>


void ACharacterSelectSceneGameMode::SendConfirmRequest()
{
	std::string ID = AccountManager::GetInstance().GetAccountID();
	OutputStream out;
	out.WriteOpcode(ENetworkCSOpcode::kLobbyConfirmRequest);
	out.WriteCString(ID.c_str());
	out.CompletePacketBuild();
	GetNetMgr().SendPacket(out);
}

void ACharacterSelectSceneGameMode::CreatePlayerCharacter(APlayerCharacter* Character, FCharacterSelectInfo& Info)
{
	UPlayerCharacterStatComponent* MyComponent = Character->PlayerStatComp;

	MyComponent->SetLevel(Info.Level);
	MyComponent->SetSTR(Info.Str);
	MyComponent->SetDEX(Info.Dex);
	MyComponent->SetINT(Info.Int);

	MyComponent->SetCharacterName(FString(Info.Name.c_str()));

	auto GameInstance = Cast<UXRGameInstance>(UGameplayStatics::GetGameInstance(GetWorld()));

	const int32 NudeBodyID = 130;
	const int32 NudeHandID = 140;
	const int32 NudeLegID =	 150;

	//헤어파츠
	FSoftObjectPath HairAssetPath = nullptr;
	FPartsResource* HairResourceTable = PartsDataTable->FindRow<FPartsResource>(*(FString::FromInt(Info.Hair)), TEXT("t"));
	HairAssetPath = GameInstance->GetXRAssetMgr()->FindResourceFromDataTable(HairResourceTable->ResourceID);
	FStreamableDelegate HairAssetLoadDelegate;
	HairAssetLoadDelegate = FStreamableDelegate::CreateUObject(this, &ACharacterSelectSceneGameMode::LoadPartsComplete,
		HairAssetPath, EPartsType::HAIR, Character);
	GameInstance->GetXRAssetMgr()->ASyncLoadAssetFromPath(HairAssetPath, HairAssetLoadDelegate);


	//페이스 파츠
	FSoftObjectPath FaceAssetPath = nullptr;
	FPartsResource* FaceResourceTable = PartsDataTable->FindRow<FPartsResource>(*(FString::FromInt(Info.Face)), TEXT("t"));
	FaceAssetPath = GameInstance->GetXRAssetMgr()->FindResourceFromDataTable(FaceResourceTable->ResourceID);
	FStreamableDelegate FaceAssetLoadDelegate;
	FaceAssetLoadDelegate = FStreamableDelegate::CreateUObject(this, &ACharacterSelectSceneGameMode::LoadPartsComplete,
		FaceAssetPath, EPartsType::FACE, Character);
	GameInstance->GetXRAssetMgr()->ASyncLoadAssetFromPath(FaceAssetPath, FaceAssetLoadDelegate);


	if (Info.armor_itemid == -1)
	{
		FSoftObjectPath NudeAssetPath = nullptr;
		FPartsResource* FaceResourceTable = PartsDataTable->FindRow<FPartsResource>(*(FString::FromInt(NudeBodyID)), TEXT("t"));
		NudeAssetPath = GameInstance->GetXRAssetMgr()->FindResourceFromDataTable(FaceResourceTable->ResourceID);
		FStreamableDelegate NudeAssetLoadDelegate;
		NudeAssetLoadDelegate = FStreamableDelegate::CreateUObject(this, &ACharacterSelectSceneGameMode::LoadPartsComplete,
			NudeAssetPath, EPartsType::NUDEBODY, Character);
		GameInstance->GetXRAssetMgr()->ASyncLoadAssetFromPath(NudeAssetPath, NudeAssetLoadDelegate);
	}
	else
		GameInstance->ItemManager->BuildItem(EItemType::EQUIPMENT, Info.armor_itemid, GetWorld(), Character);
	if (Info.hand_itemid == -1)
	{
		FSoftObjectPath NudeAssetPath = nullptr;
		FPartsResource* FaceResourceTable = PartsDataTable->FindRow<FPartsResource>(*(FString::FromInt(NudeHandID)), TEXT("t"));
		NudeAssetPath = GameInstance->GetXRAssetMgr()->FindResourceFromDataTable(FaceResourceTable->ResourceID);
		FStreamableDelegate NudeAssetLoadDelegate;
		NudeAssetLoadDelegate = FStreamableDelegate::CreateUObject(this, &ACharacterSelectSceneGameMode::LoadPartsComplete,
			NudeAssetPath, EPartsType::NUDEHAND, Character);
		GameInstance->GetXRAssetMgr()->ASyncLoadAssetFromPath(NudeAssetPath, NudeAssetLoadDelegate);
	}
	else
		GameInstance->ItemManager->BuildItem(EItemType::EQUIPMENT, Info.hand_itemid, GetWorld(), Character);
	if (Info.shoes_itemid == -1)
	{
		FSoftObjectPath NudeAssetPath = nullptr;
		FPartsResource* FaceResourceTable = PartsDataTable->FindRow<FPartsResource>(*(FString::FromInt(NudeLegID)), TEXT("t"));
		NudeAssetPath = GameInstance->GetXRAssetMgr()->FindResourceFromDataTable(FaceResourceTable->ResourceID);
		FStreamableDelegate NudeAssetLoadDelegate;
		NudeAssetLoadDelegate = FStreamableDelegate::CreateUObject(this, &ACharacterSelectSceneGameMode::LoadPartsComplete,
			NudeAssetPath, EPartsType::NUDELEG, Character);
		GameInstance->GetXRAssetMgr()->ASyncLoadAssetFromPath(NudeAssetPath, NudeAssetLoadDelegate);
	}
	else
		GameInstance->ItemManager->BuildItem(EItemType::EQUIPMENT, Info.shoes_itemid, GetWorld(), Character);

	MyComponent->SetGender(Info.gender);
}

void ACharacterSelectSceneGameMode::LoadPartsComplete(FSoftObjectPath AssetPath, EPartsType Type, APlayerCharacter* Character)
{
	TSoftObjectPtr<USkeletalMesh> LoadedMesh(AssetPath);

	Character->ChangePartsComponentsMesh(Type, LoadedMesh.Get());

}

ACharacterSelectSceneGameMode::ACharacterSelectSceneGameMode()
{
	PrimaryActorTick.bCanEverTick = true;

	FString PartsDataPath = TEXT("DataTable'/Game/Resources/DataTable/PartsTable.PartsTable'");
	static ConstructorHelpers::FObjectFinder<UDataTable> DT_PARTS(*PartsDataPath);
	if (DT_PARTS.Succeeded())
		PartsDataTable = DT_PARTS.Object;

	BeforeSlotNumber = 0;

	
}

void ACharacterSelectSceneGameMode::BeginPlay()
{
	// 캐선창에 돌아오는 경우도 생각하여 채팅리스트 초기화
	ChatingManager::GetInstance().ChatList.clear();

	CurrentWidget = CreateWidget<UCharacterSelectWidget>(GetWorld(), LoginWidget);
	if (CurrentWidget != nullptr)
	{
		CurrentWidget->AddToViewport();
	}

	GetNetMgr().GetPacketReceiveDelegate(ENetworkSCOpcode::kConfirmFailNotify)->BindUObject(
		this, &ACharacterSelectSceneGameMode::HandleCharacterCreateFail);

	GetNetMgr().GetPacketReceiveDelegate(ENetworkSCOpcode::kCharacterListNotify)->BindUObject(
		this, &ACharacterSelectSceneGameMode::HandleCharacterList);

	GetNetMgr().GetPacketReceiveDelegate(ENetworkSCOpcode::kCharacterCreateNotify)->BindUObject(
		this, &ACharacterSelectSceneGameMode::HandleCharacterCreate);

	GetNetMgr().GetPacketReceiveDelegate(ENetworkSCOpcode::kCharacterDeleteNotify)->BindUObject(
		this, &ACharacterSelectSceneGameMode::HandleCharacterDelete);

	GetNetMgr().GetPacketReceiveDelegate(ENetworkSCOpcode::kMigrateZoneNotify)->BindUObject(
		this, &ACharacterSelectSceneGameMode::HandleMigrateZone);

	/*캐릭터 선택창 카메라 배치*/
	MainCameraLocation = FVector(-120.0f, 2740.0f, 200.0f);
	CharacterActorLocation = MainCameraLocation + FVector(210.0f, 0.0f, 0.0f);
	MainCamera = GetWorld()->SpawnActor<ACameraActor>(ACameraActor::StaticClass(),
		MainCameraLocation, FRotator::ZeroRotator);

	APlayerController* CurrentController = UGameplayStatics::GetPlayerController(this, 0);
	CurrentController->SetViewTarget(MainCamera);

	std::string Ip = AccountManager::GetInstance().GetLobbyIP();
	int16 Port = AccountManager::GetInstance().GetLobbyPort();
	GetNetMgr().Connect(Ip.c_str(), Port, std::bind(&ACharacterSelectSceneGameMode::SendConfirmRequest, this));




}

void ACharacterSelectSceneGameMode::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	Super::EndPlay(EndPlayReason);
	GetNetMgr().Close();
}

void ACharacterSelectSceneGameMode::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);
	GetNetMgr().Update();
	GetMapMgr().Tick(DeltaSeconds);
}

void ACharacterSelectSceneGameMode::ChangeSelectedCharacter(int64 SlotNumber)
{
	if (CharacterList[BeforeSlotNumber]) CharacterList[BeforeSlotNumber]->SetActorHiddenInGame(true);
	CharacterList[SlotNumber]->SetActorHiddenInGame(false);
	BeforeSlotNumber = SlotNumber;

}

void ACharacterSelectSceneGameMode::HandleCharacterCreateFail(InputStream& input)
{
	CurrentWidget->CharacterLoginFail();
}



void ACharacterSelectSceneGameMode::HandleCharacterList(InputStream& input)
{
	//Connect하고 캐릭터 리스트 띄우기 
	int32_t SlotSize;
	input >> SlotSize;

	CharacterList.resize(5);

	for (int i = 0; i < SlotSize; i++)
	{
		int SlotNumber = input.ReadInt32();
		FCharacterSelectInfo Info;
		ZeroMemory(&Info, sizeof(FCharacterSelectInfo));
		std::string c_name= input.ReadCString();
		Info.Name = mbs_to_wcs(c_name, std::locale("kor"));
		input >> Info.Level; input >> Info.Str; input >> Info.Dex;
		input >> Info.Int;   input >> Info.Job;  input >> Info.Face;
		input >> Info.Hair; input >> Info.Gold; input >> Info.Zone;
		input >> Info.x; input >> Info.y; input >> Info.z;
		input >> Info.armor_itemid; input >> Info.hand_itemid; input >> Info.shoes_itemid;
		input >> Info.weapon_itemid; input >> Info.gender;

		CharacterData InitData;
		InitData.EquipArray[0].ID = Info.armor_itemid;
		InitData.EquipArray[1].ID = Info.hand_itemid;
		InitData.EquipArray[2].ID = Info.shoes_itemid;
		InitData.EquipArray[3].ID = Info.weapon_itemid;
		InitData.Max_HP = 10.0f;
		InitData.Current_HP = 10.0f;
		InitData.Gender = Info.gender;
		InitData.Level = Info.Level;
		

		APlayerCharacter* Character = GetWorld()->SpawnActor<APlayerCharacter>(APlayerCharacter::StaticClass(),
			CharacterActorLocation, FRotator(0.0f, -200.0f, 0.0f));
				
		Character->InitializeCharacter(false, InitData);
		UNickNameWidget* Widget =  Character->GetNickNameWidget();
		Widget->SetVisibility(ESlateVisibility::Hidden);

		Character->SetActorEnableCollision(false);

		CreatePlayerCharacter(Character, Info);
		Character->SetActorHiddenInGame(true);
		CharacterList[SlotNumber] = Character;
		CurrentWidget->AddCharacter(SlotNumber, Character);
	}
}

void ACharacterSelectSceneGameMode::HandleMigrateZone(InputStream& input)
{
	string InGameIP;
	int16 InGamePort;
	InGameIP = input.ReadCString();
	input >> InGamePort;
	AccountManager::GetInstance().SetInGameIP(InGameIP);
	AccountManager::GetInstance().SetInGamePort(InGamePort);

	GetNetMgr().Close();
	
	Cast<UXRGameInstance>(GetGameInstance())->LobbytoGame();
}

void ACharacterSelectSceneGameMode::HandleCharacterCreate(InputStream & input)
{
	int SlotNumber = input.ReadInt32();
	if (SlotNumber == -1)
	{
		CurrentWidget->CharacterCreateResult(false);
		return;
	}

	FCharacterSelectInfo Info;
	std::string c_name = input.ReadCString();
	Info.Name = mbs_to_wcs(c_name, std::locale("kor"));
	input >> Info.Level; input >> Info.Str; input >> Info.Dex;
	input >> Info.Int;   input >> Info.Job;  input >> Info.Face;
	input >> Info.Hair; input >> Info.Gold; input >> Info.Zone;
	input >> Info.x; input >> Info.y; input >> Info.z;
	input >> Info.armor_itemid; input >> Info.hand_itemid; input >> Info.shoes_itemid;
	input >> Info.weapon_itemid; input >> Info.gender;
	APlayerCharacter* Character = GetWorld()->SpawnActor<APlayerCharacter>(APlayerCharacter::StaticClass(),
		CharacterActorLocation, FRotator(0.0f, 180.0f, 0.0f));

	check(Character);

	CharacterData InitData;
	InitData.EquipArray[0].ID = Info.armor_itemid;
	InitData.EquipArray[1].ID = Info.hand_itemid;
	InitData.EquipArray[2].ID = Info.shoes_itemid;
	InitData.EquipArray[3].ID = Info.weapon_itemid;
	InitData.Max_HP = 10.0f;
	InitData.Current_HP = 10.0f;
	InitData.Gender = 0;

	Character->InitializeCharacter(false, InitData);
	Character->SetActorEnableCollision(false);

	CreatePlayerCharacter(Character, Info);
	Character->SetActorHiddenInGame(true);
	CharacterList[SlotNumber] = Character;
	CurrentWidget->AddCharacter(SlotNumber, Character);
}

void ACharacterSelectSceneGameMode::HandleCharacterDelete(InputStream & input)
{
	int SlotNumber = input.ReadInt32();
	CurrentWidget->DeleteCharacter(SlotNumber);
	CharacterList[SlotNumber]->Destroy();
	CharacterList[SlotNumber] = nullptr;
	CurrentWidget->UpdateList();
}