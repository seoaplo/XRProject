#include "LoginSceneGameMode.h"
#include "PlayerCharacter.h"
#include "XRGameInstance.h"
#include "InputStream.h"



ALoginSceneGameMode::ALoginSceneGameMode()
{
	PrimaryActorTick.bCanEverTick = true;
}

void ALoginSceneGameMode::BeginPlay()
{
	CurrentWidget = CreateWidget<ULoginWidget>(GetWorld(), LoginWidget);
	if (CurrentWidget != nullptr)
	{
		CurrentWidget->AddToViewport();
	}

	GetNetMgr().GetPacketReceiveDelegate(ENetworkSCOpcode::kCreateAccountResult)->BindUObject(
		this, &ALoginSceneGameMode::HandleCreateAccountResult);

	GetNetMgr().GetPacketReceiveDelegate(ENetworkSCOpcode::kLoginResult)->BindUObject(
		this, &ALoginSceneGameMode::HandleLoginResult);

	GetNetMgr().Connect("192.168.0.118", 8181, nullptr);
}

void ALoginSceneGameMode::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	Super::EndPlay(EndPlayReason);
	GetNetMgr().Close();
}

void ALoginSceneGameMode::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);
	GetNetMgr().Update();
}

void ALoginSceneGameMode::HandleCreateAccountResult(InputStream& input)
{
	bool bResult;
	input >> bResult;
	CurrentWidget->SignUpResult(bResult);
}

void ALoginSceneGameMode::HandleLoginResult(InputStream& input)
{
	bool bResult;
	input >> bResult;
	if (bResult)
	{
		GetNetMgr().Close();
		UGameplayStatics::OpenLevel(this, TEXT("LEVEL_CharacterSelect"));
	}
	else
	{
		CurrentWidget->LoginFail();
	}
}