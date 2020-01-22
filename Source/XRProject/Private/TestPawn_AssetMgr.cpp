// Fill out your copyright notice in the Description page of Project Settings.


#include "TestPawn_AssetMgr.h"
#include "TestGameInstance_AssetMgr.h"

// Sets default values
ATestPawn_AssetMgr::ATestPawn_AssetMgr()
{
 	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void ATestPawn_AssetMgr::BeginPlay()
{
	Super::BeginPlay();
	auto TestGameInstance = Cast<UTestGameInstance_AssetMgr>(GetGameInstance());
	
	FString AssetPath = TestGameInstance->XRAssetManager->FindResourceFromDataTable(1000001);
	FStreamableDelegate ResultCallback;
	ResultCallback.CreateLambda([this]() { XRLOG(Warning, TEXT("MeshLoadComplete")); });
	TestGameInstance->XRAssetManager->ASyncLoadAssetFromPath(AssetPath, ResultCallback);
	
	
}

// Called every frame
void ATestPawn_AssetMgr::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void ATestPawn_AssetMgr::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

