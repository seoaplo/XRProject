// Fill out your copyright notice in the Description page of Project Settings.


#include "MapManager.h"

bool UMapManager::Init(UWorld* world, UNetworkManager& networkmanager)
{

	if (world == nullptr) return false;

	World = world;
	//SpawnPlayer(1, FVector(-6370.0, 850.0, 480.0), FRotator(0, 0, 0));
	//PossessPlayer(1, FVector(-6370.0, 850.0, 480.0), FRotator(0, 0, 0));
	return true;
}
bool UMapManager::Clear()
{
	CharacterList.Reset();
	MonsterList.Reset();

	return true;
}

// ¸Ê¿¡ ÀÔÀå
void UMapManager::WriteMapDataFromServer(InputStream& input)
{
	int32_t characterlistsize = 0;
	int32_t monsterlistsize = 0;

	input >> characterlistsize;
	input >> monsterlistsize;

	for (int iCount = 0; iCount < characterlistsize; iCount++)
	{
		int64_t ObjectID;
		FVector PlayerLocation;
		FRotator PlayerRotator;
		input >> ObjectID;
		input >> PlayerLocation;
		input >> PlayerRotator;

		SpawnPlayer(ObjectID, PlayerLocation, PlayerRotator);
	}

	for (int iCount = 0; iCount < characterlistsize; iCount++)
	{
		int64_t ObjectID;
		FVector MonsterLocation;
		FRotator MonsterRotator;
		input >> ObjectID;
		input >> MonsterLocation;
		input >> MonsterRotator;

		SpawnMonster(ObjectID, MonsterLocation, MonsterRotator);
	}
}

bool UMapManager::SpawnMonster(int64_t objectid, FVector position, FRotator rotator)
{
	AActor* actor =
		World->SpawnActor
		(ANonePlayerCharacter::StaticClass(), &position, &rotator);

	ANonePlayerCharacter* Monster = Cast<ANonePlayerCharacter>(actor);
	if (Monster)
	{
		ANonePlayerCharacter* CheckMonster = MonsterList.FindOrAdd(objectid);
		if (CheckMonster == nullptr)
		{
			CheckMonster = Monster;
		}
		else
		{
			Monster->Destroy();
			CheckMonster->SetActorLocation(position);
			CheckMonster->SetActorRotation(rotator);
		}
		return true;
	}
	else return false;
}
bool UMapManager::PossessPlayer(int64_t objectid, FVector position, FRotator rotator)
{
	APlayerCharacter* CheckPlayer = CharacterList.FindRef(objectid);
	if (CheckPlayer == nullptr) return false;
	World->GetPlayerControllerIterator()->Get()->Possess(CheckPlayer);
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
		APlayerCharacter** CheckPlayer = &CharacterList.FindOrAdd(objectid);
		if (*CheckPlayer == nullptr)
		{
			*CheckPlayer = Player;
		}
		else
		{
			Player->Destroy();
			(*CheckPlayer)->SetActorLocation(position);
			(*CheckPlayer)->SetActorRotation(rotator);
		}
		return true;
	}
	else return false;
}
