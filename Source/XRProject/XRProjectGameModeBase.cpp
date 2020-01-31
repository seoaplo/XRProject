// Fill out your copyright notice in the Description page of Project Settings.


#include "XRProjectGameModeBase.h"
#include "PlayerCharacter.h"
#include "XRGameInstance.h"
#include "BaseCharacter.h"
#include "PlayerCharacter.h"
#include "NonePlayerCharacter.h"

AXRProjectGameModeBase::AXRProjectGameModeBase()
{
    PrimaryActorTick.bCanEverTick = true;
	//DefaultPawnClass = APlayerCharacter::StaticClass();

}

void AXRProjectGameModeBase::BeginPlay()
{

	//
    // LoginServer
    //GetNetMgr().GetPacketReceiveDelegate(ENetworkSCOpcode::kCreateAccountResult)->BindUObject(
    //    this, &AXRProjectGameModeBase::HandleCreateAccountResult);
	//
    //GetNetMgr().GetPacketReceiveDelegate(ENetworkSCOpcode::kLoginResult)->BindUObject(
    //    this, &AXRProjectGameModeBase::HandleCreateAccountResult);


    // LobbyServer

    //GetNetMgr().Connect("127.0.0.1", 12221, nullptr);


	//GetNetMgr().Connect("127.0.0.1", 12221,
	//	[this]() {
	//	/*OutputStream out;
	//	out.WriteOpcode(ENetworkCSOpcode::kRequestSpawn);
	//	out.CompletePacketBuild();
	//	GetNetMgr().SendPacket(out);*/
	//});


	////ZoneServer
	//GetNetMgr().GetPacketReceiveDelegate(ENetworkSCOpcode::kCharacterSpawn)->BindUObject(
	//	this, &AXRProjectGameModeBase::HandleCharacterSpawn);
	//GetNetMgr().GetPacketReceiveDelegate(ENetworkSCOpcode::kPossessCharacter)->BindUObject(
	//	this, &AXRProjectGameModeBase::HandlePossess);
	//GetNetMgr().GetPacketReceiveDelegate(ENetworkSCOpcode::kCharacterPos)->BindUObject(
	//	this, &AXRProjectGameModeBase::HandleCharacterMove);
	//GetNetMgr().GetPacketReceiveDelegate(ENetworkSCOpcode::kEventRecv)->BindUObject(
	//	this, &AXRProjectGameModeBase::EventRecv);



}

void AXRProjectGameModeBase::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
    Super::EndPlay(EndPlayReason);
    GetNetMgr().Close();
}

void AXRProjectGameModeBase::Tick(float DeltaSeconds)
{
    Super::Tick(DeltaSeconds);
    GetNetMgr().Update();
}

void AXRProjectGameModeBase::HandleCreateAccountResult(InputStream& input)
{
}

void AXRProjectGameModeBase::HandleLoginResult(InputStream& input)
{
}

void AXRProjectGameModeBase::HandleCharacterSpawn(InputStream& input)
{
	/*FRotator rotator{ 0.f, 0.f, 0.f };
	std::string uuid;
	uuid = input.ReadCString();
	FVector vec;
	vec.X = input.ReadFloat32();
	vec.Y = input.ReadFloat32();
	vec.Z = 90.0f;
	AActor* actor = GetWorld()->SpawnActor(
		AXRPlayerCharacter::StaticClass(),
		&vec,
		&rotator
	);
	auto player = Cast<AXRPlayerCharacter>(actor);
	if (player) {
		FString fuuid = FString(uuid.c_str());
		Players.Emplace(fuuid, player);
	}*/
}

void AXRProjectGameModeBase::HandlePossess(InputStream& input)
{
	/*std::string uuid; input >> uuid;
	FString fuuid(uuid.c_str());

	my_uuid = fuuid;

	AXRPlayerCharacter** pplayer = Players.Find(fuuid);
	if (pplayer) {
		AXRPlayerCharacter* player = *pplayer;
		GetWorld()->GetPlayerControllerIterator()->Get()->Possess(player);
	}*/
}

void AXRProjectGameModeBase::HandleCharacterMove(InputStream& input)
{
	/*std::string uuid; input >> uuid;
	FString fuuid(uuid.c_str());

	if (my_uuid != fuuid) {
		AXRPlayerCharacter** pplayer = Players.Find(fuuid);
		if (pplayer) {
			AXRPlayerCharacter* player = *pplayer;

			float x = input.ReadFloat32();
			float y = input.ReadFloat32();
			float z = input.ReadFloat32();
			FVector pos = FVector(x, y, z);
			FRotator rot = input.ReadFRotator();

			player->SetRemoteLocation(pos);
			player->SetRemoteRotation(rot);

			GEngine->AddOnScreenDebugMessage(3, 5.0f, FColor::Blue, FString::Printf(TEXT("Recv Location : %s"), *pos.ToString()));
			GEngine->AddOnScreenDebugMessage(4, 5.0f, FColor::Blue, FString::Printf(TEXT("Recv Rotator : %s"), *rot.ToString()));

		}
	}*/
}

void AXRProjectGameModeBase::EventRecv(InputStream& input)
{
	/*std::string uuid; input >> uuid;
	FString fuuid(uuid.c_str());

	if (my_uuid != fuuid) {
		AXRPlayerCharacter** pplayer = Players.Find(fuuid);
		if (pplayer) {
			AXRPlayerCharacter* player = *pplayer;
			int eventid = input.ReadInt32();
			GEngine->AddOnScreenDebugMessage(5, 5.0f, FColor::Red, FString::Printf(TEXT("%s Player Event %d "), *player->GetName(), eventid));

		}
	}*/
}
