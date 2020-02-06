// Fill out your copyright notice in the Description page of Project Settings.
#include "TestGameMode.h"
#include "XRGameInstance.h"
#include "XRAIController.h"
#include "XRPlayerController.h"
#include "Engine/Engine.h"


ATestGameMode::ATestGameMode()
{
	PrimaryActorTick.bCanEverTick = true;
	PrimaryActorTick.bStartWithTickEnabled = true;
	DefaultPawnClass = APlayerCharacter::StaticClass();
}

ATestGameMode::~ATestGameMode()
{
}


void ATestGameMode::BeginPlay()
{

	Super::BeginPlay();
	
	auto GameInstance = Cast<UXRGameInstance>(GetWorld()->GetGameInstance());


	FVector Loc = FVector(100.0f, 100.0f, 100.0f);
	FRotator Rot = FRotator(0.0f, 0.0f, 0.0f);
	FActorSpawnParameters param;
	param.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn;
	AActor* actor = GetWorld()->SpawnActor
		(ANonePlayerCharacter::StaticClass(), &Loc, &Rot, param);

	ANonePlayerCharacter* Monster = Cast<ANonePlayerCharacter>(actor);




	Loc = FVector(100.0f, 0.0f, 200.0f);
	Rot = FRotator(0.0f, 0.0f, 0.0f);
	param.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn;
	AActor* actor2 = GetWorld()->SpawnActor
	(APlayerCharacter::StaticClass(), &Loc, &Rot, param);

	APlayerCharacter* Character = Cast<APlayerCharacter>(actor2);
	Character->TestInitialize();

	
}

void ATestGameMode::Tick(float deltatime)
{
	Super::Tick(deltatime);
}


void ATestGameMode::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	Super::EndPlay(EndPlayReason);

}
