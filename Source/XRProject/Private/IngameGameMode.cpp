// Fill out your copyright notice in the Description page of Project Settings.


#include "IngameGameMode.h"
#include "Inventory.h"
#include "XRGameInstance.h"
#include "XRAIController.h"
#include "XRPlayerController.h"
#include "Engine/Engine.h"

AIngameGameMode::AIngameGameMode()
{
	IsSuper = false;
	PrimaryActorTick.bCanEverTick = true;
	PlayerControllerClass = AXRPlayerController::StaticClass();
}

AIngameGameMode::~AIngameGameMode()
{
}



void AIngameGameMode::BeginPlay()
{
	Super::BeginPlay();

	CurrentWidget = CreateWidget<UInGameMainWidget>(GetWorld(), MainWidget);
	if (CurrentWidget != nullptr)
	{
		CurrentWidget->AddToViewport();
	}
	PrimaryActorTick.bCanEverTick = true;

	//GetNetMgr().GetPacketReceiveDelegate(ENetworkSCOpcode::kUserEnterTheMap)->BindUObject(
	//	this, &AIngameGameMode::HandleEnterZone);
	//GetNetMgr().GetPacketReceiveDelegate(ENetworkSCOpcode::kSpawnCharacter)->BindUObject(
	//	this, &AIngameGameMode::SpawnCharacterFromServer);
	//GetNetMgr().GetPacketReceiveDelegate(ENetworkSCOpcode::kUpdateCharacterPosition)->BindUObject(
	//	this, &AIngameGameMode::UpdateCharacterPosition);
//
//
	//GetNetMgr().GetPacketReceiveDelegate(ENetworkSCOpcode::kSetMonsterController)->BindUObject(
	//	this, &AIngameGameMode::SetMonsterController);
//
	//GetNetMgr().GetPacketReceiveDelegate(ENetworkSCOpcode::kUpdateMonsterAction)->BindUObject(
	//	this, &AIngameGameMode::UpdateMonsterAction);
//
	//GetNetMgr().GetPacketReceiveDelegate(ENetworkSCOpcode::kActorDamaged)->BindUObject(
	//	this, &AIngameGameMode::GiveDamageToCharacter);
	//	
	//GetNetMgr().GetPacketReceiveDelegate(ENetworkSCOpcode::kNotifyCharacterAttack)->BindUObject(
	//	this, &AIngameGameMode::UpdateCharacterMotion);

	std::string Ip = AccountManager::GetInstance().GetInGameIP();
	int16 Port = AccountManager::GetInstance().GetInGamePort();
	GetNetMgr().Connect(Ip.c_str(), Port, std::bind(&AIngameGameMode::SendConfirmRequest, this));
	IsSuper = Cast<UXRGameInstance>(GetGameInstance())->GetIsSuper();
}

void AIngameGameMode::SendConfirmRequest()
{
	std::string ID = AccountManager::GetInstance().GetAccountID();
	OutputStream out;
	out.WriteOpcode(ENetworkCSOpcode::kZoneConrifmRequest);
	out.WriteCString(ID.c_str());
	out.CompletePacketBuild();
	GetNetMgr().SendPacket(out);

}

void AIngameGameMode::Tick(float deltatime)
{
	Super::Tick(deltatime);
	if (GetMapMgr().InitComplete)
	{
		PlayerControllerClass = AXRPlayerController::StaticClass();
		GetMapMgr().PlayerListSpawn(GetWorld());
		GetMapMgr().MonsterListSpawn(GetWorld());
		GetMapMgr().InitComplete = false;
	}
	if (GetMapMgr().PlayerSpawnReady)
	{
		GetMapMgr().RemotePlayerSpawn(GetWorld());
		GetMapMgr().PlayerSpawnReady = false;
	}
	if (Cast<UXRGameInstance>(GetGameInstance())->GetIsSuper())
	{
		IsSuper = true;
	}
	GetNetMgr().Update();
}

void AIngameGameMode::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	Super::EndPlay(EndPlayReason);
	GetMapMgr().Clear();
	GetNetMgr().Close();
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
void AIngameGameMode::SpawnCharacterFromServer(class InputStream& input)
{
	int64 Id = input.ReadInt64();
	FVector Location = input.ReadFVector();
	FRotator Rotation = input.ReadFRotator();

	MapMgr.SpawnPlayer(Id, Location, Rotation);

	float HP = input.ReadFloat32();
	float MAXHP = input.ReadFloat32();
	float AttackMin = input.ReadFloat32();
	float AttackMax = input.ReadFloat32();
	float AttackRange = input.ReadFloat32();
	float AttackSpeed = input.ReadFloat32();
	float Defense = input.ReadFloat32();
	float Speed = input.ReadFloat32();
	std::string Name = input.ReadCString();
	int Level = input.ReadInt32();
	int Gender = input.ReadInt32();
	int FaceID = input.ReadInt32();
	int HairID = input.ReadInt32();
	int Str = input.ReadInt32();
	int Dex = input.ReadInt32();
	int Intel = input.ReadInt32();
	float Stamina = input.ReadFloat32();
	float MaxStamina = input.ReadFloat32();

	APlayerCharacter* Character = nullptr;
	Character = GetMapMgr().FindPlayer(Id);
	if (Character == nullptr)
		check(false);

	int EquipmentSize = 4;
	for (int i = 0; i < EquipmentSize; i++)
	{
		int Type = input.ReadInt32();
		if (Type)
		{
			if (Type == 3)
			{
				int ID = input.ReadInt32();
				int AddATK = input.ReadInt32();
				int AddDEF = input.ReadInt32();
				int AddSTR = input.ReadInt32();
				int AddDex = input.ReadInt32();
				int AddInt = input.ReadInt32();
			}
			int Count = input.ReadInt32();
		}
	}
}
void AIngameGameMode::UpdateCharacterPosition(class InputStream& input)
{
	int64 Id = input.ReadInt64();
	int32 state = input.ReadInt32();
	
	FVector Location = input.ReadFVector();
	FRotator Rotation = input.ReadFRotator();

	APlayerCharacter* TargetPlayer = MapMgr.FindPlayer(Id);
	if (TargetPlayer == nullptr) 
	{
		XRLOG(Warning, TEXT("Player not found"));
		return;
	}

	AAIController*  aicon = Cast<AAIController>(TargetPlayer->GetController());
	if (aicon == nullptr)
	{
		XRLOG(Warning, TEXT("AICon not found"));
		return;
	}
	else aicon->MoveToLocation(Location, 2, false, false);

}

void AIngameGameMode::SetMonsterController(InputStream& input)
{
	bool IsMonsterController = input.ReadBool();
	IsSuper = IsMonsterController;
}

void AIngameGameMode::UpdateMonsterAction(InputStream& input)
{
	auto firstPlayer = Cast<AXRPlayerController>(GetWorld()->GetFirstPlayerController());
	if(firstPlayer)
	{
		if (!firstPlayer->IsSpuer())
		{
			int64 ObjID		=	input.ReadInt64();
			int32 ActionID	=	input.ReadInt32();
			FVector Location =	input.ReadFVector();
			FRotator Rotator =	input.ReadFRotator();


			GEngine->AddOnScreenDebugMessage(112, 5.f, FColor::Blue, FString::Printf(TEXT("Recv MonsterUpdate  ObjectID: %s, ActionID: %s, Location : %s, Rotator: %s"),
				*FString::FromInt(ObjID), *FString::FromInt(ActionID),
				*Location.ToString(),*Rotator.ToString()));
		}
	}

}

void AIngameGameMode::GiveDamageToCharacter(InputStream & input)
{
	int64 AttackerID = input.ReadInt64();
	int64 VictimID = input.ReadInt64();
	float HitDamage = input.ReadFloat32();

	//ANonePlayerCharacter* Attacker = MapMgr.FindPlayer(AttackerID);
	ANonePlayerCharacter* Attacker = nullptr;
	APlayerCharacter* TargetPlayer = MapMgr.FindPlayer(VictimID);
	
	TSubclassOf<UDamageType> const ValidDamageTypeClass = TSubclassOf<UDamageType>(UDamageType::StaticClass());
	FDamageEvent DamageEvent(ValidDamageTypeClass);

	if (Attacker && TargetPlayer)
	{
		TargetPlayer->TakeDamage(HitDamage, DamageEvent, Attacker->GetController(), Attacker);
		TargetPlayer->TakeDamage(HitDamage, DamageEvent, nullptr, nullptr);
	}

}

void AIngameGameMode::UpdateCharacterMotion(InputStream & input)
{
	int64 ObjectID = input.ReadInt64();
	int32 MotionID = input.ReadInt32();
	FVector TargetLocation = input.ReadFVector();
	FRotator TargetRotation = input.ReadFRotator();



	APlayerCharacter* TargetPlayer = nullptr;
	TargetPlayer = MapMgr.FindPlayer(ObjectID);

	if (TargetPlayer)
	{
		TargetPlayer->MyAnimInstance->PlayAttackOnlyPlayMontage();
		TargetPlayer->MyAnimInstance->JumpToComboMontageSection(MotionID);
		
		UE_LOG(LogTemp,
			Warning,
			TEXT("CharacterMotionDebug ID : %d, MotionID : %d, TargetVec : %f %f %f, TargetRot : %f %f %f"),
			ObjectID, MotionID, TargetLocation.X, TargetLocation.Y, TargetLocation.Z,
			TargetRotation.Yaw, TargetRotation.Pitch, TargetRotation.Roll);
	}


}
