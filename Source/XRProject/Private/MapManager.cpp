// Fill out your copyright notice in the Description page of Project Settings.


#include "MapManager.h"

bool UMapManager::Init(UWorld* world)
{
	/*GetNetMgr().GetPacketReceiveDelegate(ENetworkSCOpcode::kCharacterSlotNotify)->
	BindUObject(this, &AIngameGameMode::PlayerCharacterInitializeFromServer);*/

	if (world == nullptr) return false;

	World = world;
	SpawnPlayer(0, FVector(-6350.0, 690.0, 540.0), FRotator(0, 0, 0));
	return true;
}

bool UMapManager::Clear()
{
	CharacterList.Reset();
	MonsterList.Reset();

	return true;
}
bool UMapManager::SpawnPlayer(int64_t objectid, FVector position, FRotator rotator)
{
	AActor* actor = 
		World->SpawnActor
		(APlayerCharacter::StaticClass(), &position, &rotator);

	APlayerCharacter* Player = Cast<APlayerCharacter>(actor);
	if (Player)
	{
		APlayerCharacter* CheckPlayer = CharacterList.FindOrAdd(objectid);
		if (CheckPlayer == nullptr)
		{
			CheckPlayer = Player;
		}
		else
		{
			Player->Destroy();
			CheckPlayer->SetActorLocation(position);
		}
		return true;
	}
	else return false;
}