#include "CharacterSelectSceneGameMode.h"
#include "PlayerCharacter.h"
#include "AccountManager.h"
#include "XRGameInstance.h"
#include "InputStream.h"
#include <functional>

void ACharacterSelectSceneGameMode::SendConfirmRequest()
{
	std::string ID = AccountManager::GetInstance().GetAccountID();
	OutputStream out;
	out.WriteOpcode(ENetworkCSOpcode::kLobbyConfirmRequest);
	out.WriteCString(ID.c_str());
	out.CompletePacketBuild();
	GetNetMgr().SendPacket(out);
}

ACharacterSelectSceneGameMode::ACharacterSelectSceneGameMode()
{
	PrimaryActorTick.bCanEverTick = true;
}

void ACharacterSelectSceneGameMode::BeginPlay()
{
	// 인스턴스에서 로비서버의 IP, port를 중계하는 방식으로 바꾸기 & 옵코드 유효한 옵코드로 변경
	CurrentWidget = CreateWidget<UCharacterSelectWidget>(GetWorld(), LoginWidget);
	if (CurrentWidget != nullptr)
	{
		CurrentWidget->AddToViewport();
	}

	GetNetMgr().GetPacketReceiveDelegate(ENetworkSCOpcode::kConfirmFailNotify)->BindUObject(
		this, &ACharacterSelectSceneGameMode::HandleCharacterCreateFail);

	GetNetMgr().GetPacketReceiveDelegate(ENetworkSCOpcode::kCharacterListNotify)->BindUObject(
		this, &ACharacterSelectSceneGameMode::HandleCharacterList);

	GetNetMgr().GetPacketReceiveDelegate(ENetworkSCOpcode::kCharacterSlotNotify)->BindUObject(
		this, &ACharacterSelectSceneGameMode::HandleCharacterSlot);

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
}

void ACharacterSelectSceneGameMode::ChangeSelectedCharacter(int64 SlotNumber)
{

}

void ACharacterSelectSceneGameMode::HandleCharacterCreateFail(InputStream& input)
{
	CurrentWidget->CharacterLoginFail();
}

void ACharacterSelectSceneGameMode::HandleCharacterList(InputStream& input)
{
	int32_t SlotSize;
	input >> SlotSize;
	for (int i = 0; i < SlotSize; i++)
	{
		int32_t SlotNum;
		string Name;
		int32_t Level;
		int32_t Str;
		int32_t Dex;
		int32_t Int;
		int32_t Job;
		int32_t Face;
		int32_t Hair;
		int32_t Gold;
		int32_t Zone;
		float x, y, z;
		int32_t armor_itemid;
		int32_t hand_itemid;
		int32_t shoes_itemid;;
		int32_t weapon_itemid;
		int32_t sub_weapon_itemid;
		int32_t gender;

		input >> SlotNum;
		Name = input.ReadCString();
		input >> Level;
		input >> Str;
		input >> Dex;
		input >> Int;
		input >> Job;
		input >> Face;
		input >> Hair;
		input >> Gold;
		input >> Zone;
		input >> x;
		input >> y;
		input >> z;
		input >> armor_itemid;
		input >> hand_itemid;
		input >> shoes_itemid;;
		input >> weapon_itemid;
		input >> sub_weapon_itemid;
		input >> gender;
	}
}

void ACharacterSelectSceneGameMode::HandleCharacterSlot(InputStream& input)
{
	int32_t SlotNum;
	string Name;
	int32_t Level;
	int32_t Str;
	int32_t Dex;
	int32_t Int;
	int32_t Job;
	int32_t Face;
	int32_t Hair;
	int32_t Gold;
	int32_t Zone;
	float x, y, z;
	int32_t armor_itemid;
	int32_t hand_itemid;
	int32_t shoes_itemid;;
	int32_t weapon_itemid;
	int32_t sub_weapon_itemid;
	int32_t gender;

	input >> SlotNum;
	if (SlotNum == -1)
	{
		CurrentWidget->CharacterCreateResult(false);
		return;
	}
	else
	{
		Name = input.ReadCString();
		input >> Level;
		input >> Str;
		input >> Dex;
		input >> Int;
		input >> Job;
		input >> Face;
		input >> Hair;
		input >> Gold;
		input >> Zone;
		input >> x;
		input >> y;
		input >> z;
		input >> armor_itemid;
		input >> hand_itemid;
		input >> shoes_itemid;;
		input >> weapon_itemid;
		input >> sub_weapon_itemid;
		input >> gender;
	}
}
