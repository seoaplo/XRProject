// Fill out your copyright notice in the Description page of Project Settings.


#include "MapManager.h"
#include "XRGameInstance.h"
#include "XRPlayerController.h"
#include "BossCharacter.h"
#include "WidgetBlueprint.h"

UMapManager::UMapManager()
{
	MapList.Add(100, 
		LevelPathData(FName(TEXT("LEVEL_Village")),
			wstring(L"World'/Game/Resources/Map/Village/LEVEL/LEVEL_Village.LEVEL_Village'")));
	MapList.Add(111,
		LevelPathData(FName(TEXT("LEVEL_Zone_1")), 
			wstring(L"World'/Game/Resources/Map/Zone/Level/LEVEL_Zone_1.LEVEL_Zone_1'")));
	MapList.Add(112,
		LevelPathData(FName(TEXT("LEVEL_Zone_2")),
			wstring(L"World'/Game/Resources/Map/Zone/Level/LEVEL_Zone_2.LEVEL_Zone_2'")));
	MapList.Add(113,
		LevelPathData(FName(TEXT("LEVLE_Boss")), 
			wstring(L"World'/Game/Resources/Map/Zone_Boss/Level/LEVLE_Boss.LEVLE_Boss'")));
}
bool UMapManager::Init()
{
	PlayerID = -1;
	LevelID = -1;

	Spawn_Character.Unbind();
	Delete_Character.Unbind();
	PreWorld = nullptr;
	return true;
}
bool UMapManager::Clear()
{

	for (auto& Character : CharacterList)
	{
		Character.Value->Destroy();
	}
	for (auto& Monster : MonsterList)
	{
		Monster.Value->Destroy();
	}
	CharacterList.Reset();
	MonsterList.Reset();

	CharacterDataList.clear();
	MonsterDataList.clear();

	return true;
}

// 맵에 입장


void UMapManager::ReadMapDataFromServer(InputStream& Input)
{
	int32_t characterlistsize = 0;
	int32_t monsterlistsize = 0;

	Input >> LevelID;

	Input >> characterlistsize;
	CharacterDataList.resize(characterlistsize + 1);
	ReadPlayerFromServer(Input);

	Input >> monsterlistsize;
	MonsterDataList.resize(monsterlistsize);
	ReadMosnterFromServer(Input);
}
void UMapManager::ReadMosnterFromServer(InputStream& Input)
{

	for (auto& CurrentData : MonsterDataList)
	{
		Input >> CurrentData.MonsterID;
		Input >> CurrentData.ObjectID;
		Input >> CurrentData.Location;
		Input >> CurrentData.Rotator;
		
		CurrentData.Rotator = FRotator(0, 0, 0);

		CurrentData.Current_HP = Input.ReadFloat32();
		CurrentData.Max_HP = Input.ReadFloat32();
		CurrentData.Attack_Min = Input.ReadFloat32();
		CurrentData.Attack_Max = Input.ReadFloat32();
		CurrentData.Attack_Range = Input.ReadFloat32();
		CurrentData.Attack_Speed = Input.ReadFloat32();
		CurrentData.Speed = Input.ReadFloat32();
		CurrentData.Defence = Input.ReadFloat32();
	}
}

void UMapManager::ReadPlayerFromServer(InputStream& Input)
{
	for (int iCount = 0; iCount < CharacterDataList.size() - 1; iCount++)
	{
		CharacterData& CurrentData = CharacterDataList[iCount];

		Input >> CurrentData.ObjectID;
		Input >> CurrentData.Location;
		Input >> CurrentData.Rotator;

		CurrentData.Current_HP = Input.ReadFloat32();
		CurrentData.Max_HP = Input.ReadFloat32();
		CurrentData.Attack_Min = Input.ReadFloat32();
		CurrentData.Attack_Max = Input.ReadFloat32();
		CurrentData.Attack_Range = Input.ReadFloat32();
		CurrentData.Attack_Speed = Input.ReadFloat32();
		CurrentData.Speed = Input.ReadFloat32();
		CurrentData.Defence = Input.ReadFloat32();


		std::string c_name = Input.ReadCString();
		CurrentData.Name = mbs_to_wcs(c_name, std::locale("kor"));
		CurrentData.Level = Input.ReadInt32();
		CurrentData.Gender = Input.ReadInt32();
		CurrentData.FaceID = Input.ReadInt32();
		CurrentData.HairID = Input.ReadInt32();
		CurrentData.STR = Input.ReadInt32();
		CurrentData.DEX = Input.ReadInt32();
		CurrentData.INT = Input.ReadInt32();
		CurrentData.Current_Stamina = Input.ReadFloat32();
		CurrentData.Max_Stamina = Input.ReadFloat32();

		const int EquipmentSize = 4;
		for (int iCount = 0; iCount < EquipmentSize; iCount++)
		{
			Equipment& CurrentEquip = CurrentData.EquipArray[iCount];
			CurrentEquip.Type = Input.ReadInt32();
			if (CurrentEquip.Type)
			{
				if (CurrentEquip.Type == 3)
				{
					CurrentEquip.ID = Input.ReadInt32();
					CurrentEquip.AddATK = Input.ReadInt32();
					CurrentEquip.AddDEF = Input.ReadInt32();
					CurrentEquip.AddSTR = Input.ReadInt32();
					CurrentEquip.AddDex = Input.ReadInt32();
					CurrentEquip.AddInt = Input.ReadInt32();
				}
				CurrentEquip.Count = Input.ReadInt32();
			}
		}


	}
}


void UMapManager::ReadPossesPlayerFromServer(InputStream& Input)
{
	CharacterData& CurrentData = CharacterDataList[CharacterDataList.size() - 1];

	Input >> CurrentData.ObjectID;
	Input >> CurrentData.Location;
	Input >> CurrentData.Rotator;

	CurrentData.Current_HP = Input.ReadFloat32();
	CurrentData.Max_HP = Input.ReadFloat32();
	CurrentData.Attack_Min = Input.ReadFloat32();
	CurrentData.Attack_Max = Input.ReadFloat32();
	CurrentData.Attack_Range = Input.ReadFloat32();
	CurrentData.Attack_Speed = Input.ReadFloat32();
	CurrentData.Speed = Input.ReadFloat32();
	CurrentData.Defence = Input.ReadFloat32();

	std::string c_name = Input.ReadCString();
	CurrentData.Name = mbs_to_wcs(c_name, std::locale("kor"));
	CurrentData.Level = Input.ReadInt32();
	CurrentData.Gender = Input.ReadInt32();
	CurrentData.FaceID = Input.ReadInt32();
	CurrentData.HairID = Input.ReadInt32();
	CurrentData.STR = Input.ReadInt32();
	CurrentData.DEX = Input.ReadInt32();
	CurrentData.INT = Input.ReadInt32();
	CurrentData.Current_Stamina = Input.ReadFloat32();
	CurrentData.Max_Stamina = Input.ReadFloat32();

	int EquipmentSize = 4;
	for (int i = 0; i < EquipmentSize; i++)
	{
		Equipment& CurrentEquip = CurrentData.EquipArray[i];
		CurrentEquip.Type = Input.ReadInt32();
		if (CurrentEquip.Type)
		{
			if (CurrentEquip.Type == 3)
			{
				CurrentEquip.ID = Input.ReadInt32();
				CurrentEquip.AddATK = Input.ReadInt32();
				CurrentEquip.AddDEF = Input.ReadInt32();
				CurrentEquip.AddSTR = Input.ReadInt32();
				CurrentEquip.AddDex = Input.ReadInt32();
				CurrentEquip.AddInt = Input.ReadInt32();
			}
			CurrentEquip.Count = Input.ReadInt32();
		}
	}
	PlayerID = CurrentData.ObjectID;

}
bool UMapManager::ReadPlayerDeleteFromServer(InputStream& Input)
{
	Delete_Character.ExecuteIfBound();
	return true;
}

APlayerCharacter* UMapManager::FindPlayer(int64_t objectid)
{
	APlayerCharacter* FindedPlayer = CharacterList.FindRef(objectid);
	return FindedPlayer;
}
ANonePlayerCharacter* UMapManager::FindMonster(int64_t ObjectID)
{
	ANonePlayerCharacter* FindedMonster = MonsterList.FindRef(ObjectID);
	return FindedMonster;
}
bool UMapManager::ReadPlayerSpawnFromServer(InputStream& Input)
{
	CharacterData CurrentData;
	Input >> CurrentData.ObjectID;
	Input >> CurrentData.Location;
	Input >> CurrentData.Rotator;

	CurrentData.Current_HP = Input.ReadFloat32();
	CurrentData.Max_HP = Input.ReadFloat32();
	CurrentData.Attack_Min = Input.ReadFloat32();
	CurrentData.Attack_Max = Input.ReadFloat32();
	CurrentData.Attack_Range = Input.ReadFloat32();
	CurrentData.Attack_Speed = Input.ReadFloat32();
	CurrentData.Speed = Input.ReadFloat32();
	CurrentData.Defence = Input.ReadFloat32();

	std::string c_name = Input.ReadCString();
	CurrentData.Name = mbs_to_wcs(c_name, std::locale("kor"));
	CurrentData.Level = Input.ReadInt32();
	CurrentData.Gender = Input.ReadInt32();
	CurrentData.FaceID = Input.ReadInt32();
	CurrentData.HairID = Input.ReadInt32();
	CurrentData.STR = Input.ReadInt32();
	CurrentData.DEX = Input.ReadInt32();
	CurrentData.INT = Input.ReadInt32();
	CurrentData.Current_Stamina = Input.ReadFloat32();
	CurrentData.Max_Stamina = Input.ReadFloat32();

	int EquipmentSize = 4;
	for (int i = 0; i < EquipmentSize; i++)
	{
		Equipment& CurrentEquip = CurrentData.EquipArray[i];
		CurrentEquip.Type = Input.ReadInt32();
		if (CurrentEquip.Type)
		{
			if (CurrentEquip.Type == 3)
			{
				CurrentEquip.ID = Input.ReadInt32();
				CurrentEquip.AddATK = Input.ReadInt32();
				CurrentEquip.AddDEF = Input.ReadInt32();
				CurrentEquip.AddSTR = Input.ReadInt32();
				CurrentEquip.AddDex = Input.ReadInt32();
				CurrentEquip.AddInt = Input.ReadInt32();
			}
			CurrentEquip.Count = Input.ReadInt32();
		}
	}
	CharacterDataList.push_back(CurrentData);
	Spawn_Character.ExecuteIfBound();
	return true;
}

bool UMapManager::OpenMap(UWorld* World)
{
	if (World == nullptr) return false;
	PreWorld = World;

	LevelPathData* LevelPath = MapList.Find(LevelID);
	if (LevelPath == nullptr) return false;


	FStringClassReference MyWidgetClassRef(TEXT("/Game/Resources/UI/Blueprint/Widget/LoadingBar/BP_LoadingBarWidget.BP_LoadingBarWidget_C"));
	UClass* MyWidgetClass = MyWidgetClassRef.TryLoadClass<UUserWidget>();
	if (MyWidgetClass == nullptr) return false;
	
	ULoadingBarWidget * BP_LoadingWidget = CreateWidget<ULoadingBarWidget>(World, MyWidgetClass);
	if (BP_LoadingWidget == nullptr) return false;

	

	BP_LoadingWidget->AddToViewport();

	float Percent = 0.0f;
	auto gInst = Cast<UXRGameInstance>(World->GetGameInstance());
	if (gInst)
	{
		FSoftObjectPath path(LevelPath->LevelName, LevelPath->LevelPath.c_str());

		FStreamableDelegate resultcallback;
		resultcallback.BindLambda([path, this, World, LevelPath, BP_LoadingWidget]()
		{
			TSoftObjectPtr<ULevel> LoadedMap(path);
			XRLOG(Warning, TEXT("Map ASync Load Complete"));

			BP_LoadingWidget->ApplyPercentage(1.0f);
			UGameplayStatics::OpenLevel(World, LevelPath->LevelName);
		});
		gInst->GetXRAssetMgr()->ASyncLoadAssetFromPath(path, resultcallback);

		Percent = Percent < 0.9f ? (Percent + 0.01f) : 0.9f;
		BP_LoadingWidget->ApplyPercentage(Percent);
	}
	return true;
}
bool UMapManager::PlayerListSpawn(UWorld* World)
{
	if (World == nullptr) return false;
	for (auto& CurrentData : CharacterDataList)
	{
		FActorSpawnParameters Param;
		Param.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn;
		AActor* actor =
			World->SpawnActor
			(APlayerCharacter::StaticClass(), &CurrentData.Location, &CurrentData.Rotator, Param);
		
		if (actor == nullptr) return false;
 		
		APlayerCharacter* Player = Cast<APlayerCharacter>(actor); 

		UXRGameInstance* GI = Cast<UXRGameInstance>(Player->GetWorld()->GetGameInstance());

		if (CurrentData.ObjectID != PlayerID)
		{
			Player->InitializeCharacter(false, CurrentData);
		}
		else
		{
			for (int ii = 0; ii < CharacterSkillIDList.Num(); ii++)
			{
				UPlayerSkillManager* SkillManager = GI->GetPlayerSkillManager();
				SkillManager->AddSkill(SkillManager->CreateSkillFromID(CharacterSkillIDList[ii]), true);
			}

			Player->InitializeCharacter(true, CurrentData);
		}

		auto GameInstance = Cast <UXRGameInstance>(Player->GetGameInstance());

		if (Player)
		{
			APlayerCharacter** CheckPlayer = CharacterList.Find(CurrentData.ObjectID);
			if (CheckPlayer != nullptr)
			{
				Player->Destroy();
				continue;
			}
			if (Player->PlayerStatComp == nullptr) continue;
			CharacterList.Add(CurrentData.ObjectID, Player);
		}
	}
	CharacterDataList.clear();
	return true;
}
bool UMapManager::PossessPlayer(UWorld* World)
{
	APlayerCharacter* Player = CharacterList.FindRef(PlayerID);
	if (Player == nullptr) return false;
	AXRPlayerController* MyPlayerController = Cast<AXRPlayerController>(World->GetPlayerControllerIterator()->Get());
	if (MyPlayerController == nullptr) return false;
	MyPlayerController->Possess(Player);
	PlayerCharacter = Player;
	return true;
}
bool UMapManager::MonsterListSpawn(UWorld* World)
{
	if (World == nullptr) return false;

	for (auto& CurrentData : MonsterDataList)
	{

		FActorSpawnParameters param;
		param.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn;
		AActor* actor = nullptr;

		if (CurrentData.MonsterID == 20003)
		{
			auto gInst = Cast<UXRGameInstance>(World->GetGameInstance());
	
				FMonsterTableRow* ResourceTableRow =
					gInst->GetXRAssetMgr()->NPCDataTable->FindRow<FMonsterTableRow>
				(FName(*(FString::FromInt(CurrentData.MonsterID))), FString(""));
				if (ResourceTableRow)
				{
					FSoftObjectPath BPPath =  gInst->GetXRAssetMgr()->FindResourceFromDataTable(ResourceTableRow->MonsterBP);
					UAssetManager::GetStreamableManager().RequestSyncLoad(BPPath);
					UAssetManager::GetStreamableManager().LoadSynchronous(BPPath);

					UClass* GeneratedBP = Cast<UClass>(StaticLoadObject(UClass::StaticClass(), NULL, *BPPath.ToString()));

					actor = World->SpawnActor<ABossCharacter>(GeneratedBP, CurrentData.Location, CurrentData.Rotator,param);
				} 
		}
		else
		{
			actor = World->SpawnActor
				(ANonePlayerCharacter::StaticClass(), &CurrentData.Location, &CurrentData.Rotator, param);
		}
		
		ANonePlayerCharacter* Monster = Cast<ANonePlayerCharacter>(actor);
		if (Monster)
		{
			ANonePlayerCharacter** CheckMonster = MonsterList.Find(CurrentData.ObjectID);
			if (CheckMonster != nullptr)
			{
				Monster->Destroy();
				continue;
			}
			Monster->NpcLoadStart(CurrentData.MonsterID);
			Monster->ObjectID = CurrentData.ObjectID;
			MonsterList.Add(CurrentData.ObjectID, Monster);
		}
	}
	MonsterDataList.clear();
	return true;
}


bool UMapManager::RemotePlayerSpawn(UWorld* world)
{
	return PlayerListSpawn(world);
}
bool UMapManager::DeleteRemotePlayer(UWorld* World)
{
	return false;
}
void UMapManager::PotalInPlayer(AActor* OtherCharacter)
{
	APlayerCharacter* Character = Cast<APlayerCharacter>(OtherCharacter);
	if (PlayerCharacter != Character) return;

	SendChangeZoneFromClient();
}
// 서버로 데이터 송신
void UMapManager::SendChangeZoneFromClient()
{
	XRLOG(Warning, TEXT("ZoneChange"));

	OutputStream out;
	out.WriteOpcode(ENetworkCSOpcode::kRequestChangeZone);
	out.CompletePacketBuild();
	GetNetMgr().SendPacket(out);
}
void UMapManager::InputExpData(class InputStream& input)
{
	CharacterData& CurrentData = CharacterDataList[CharacterDataList.size() - 1];
	CurrentData.Current_Exp	 = input.ReadInt32();
	CurrentData.Max_Exp		 = input.ReadInt32();
}