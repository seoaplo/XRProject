#include "CharacterSelectSceneGameMode.h"
#include "PlayerCharacter.h"
#include "AccountManager.h"
#include "XRGameInstance.h"
#include "InputStream.h"



ACharacterSelectSceneGameMode::ACharacterSelectSceneGameMode()
{
	PrimaryActorTick.bCanEverTick = true;
}

void ACharacterSelectSceneGameMode::BeginPlay()
{
	CurrentWidget = CreateWidget<UCharacterSelectWidget>(GetWorld(), LoginWidget);
	if (CurrentWidget != nullptr)
	{
		CurrentWidget->AddToViewport();
	}

	// 인스턴스에서 로비서버의 IP, port를 중계하는 방식으로 바꾸기 & 옵코드 유효한 옵코드로 변경
	GetNetMgr().GetPacketReceiveDelegate(ENetworkSCOpcode::kCreateAccountResult)->BindUObject(
		this, &ACharacterSelectSceneGameMode::HandleCharacterCreateResult);

	GetNetMgr().GetPacketReceiveDelegate(ENetworkSCOpcode::kLoginResult)->BindUObject(
		this, &ACharacterSelectSceneGameMode::HandleCharacterSelectionResult);

	string ip = AccountManager::GetInstance().GetLobbyIP();
	int16 Port = AccountManager::GetInstance().GetLobbyPort();
	GetNetMgr().Connect(ip.c_str(), Port, nullptr);
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

void ACharacterSelectSceneGameMode::HandleCharacterCreateResult(InputStream& input)
{
}

void ACharacterSelectSceneGameMode::HandleCharacterSelectionResult(InputStream& input)
{
}

void ACharacterSelectSceneGameMode::HandleCharacterList(InputStream& input)
{
}
