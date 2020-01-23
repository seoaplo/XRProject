// Fill out your copyright notice in the Description page of Project Settings.


#include "TestPawn_AssetMgr.h"
#include "TestGameInstance_AssetMgr.h"

// Sets default values
ATestPawn_AssetMgr::ATestPawn_AssetMgr()
{
 	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	TestMash = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("SkeletalMesh"));

	SetRootComponent(TestMash);
}

// Called when the game starts or when spawned
void ATestPawn_AssetMgr::BeginPlay()
{
	Super::BeginPlay();
	auto TestGameInstance = Cast<UTestGameInstance_AssetMgr>(GetGameInstance());
	
	FSoftObjectPath AssetPath = TestGameInstance->XRAssetManager->FindResourceFromDataTable(1000001);
	
	
	FStreamableDelegate ResultCallback;
	ResultCallback.BindUObject(this,&ATestPawn_AssetMgr::ResourceASyncLoadComplete, AssetPath);
	//ResultCallback.BindLambda([AssetPath,this]()
	//{ 
	//	TSoftObjectPtr<USkeletalMesh> LoadedMesh(AssetPath);
	//	TestMash->SetSkeletalMesh(LoadedMesh.Get());
	//	XRLOG(Warning, TEXT("MeshLoadComplete")); 
	//});



	TestGameInstance->XRAssetManager->ASyncLoadAssetFromPath(AssetPath.ToString(), AssetPath, ResultCallback);
	

	
	
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

void ATestPawn_AssetMgr::ResourceASyncLoadComplete(FSoftObjectPath AssetPath)
{
	TSoftObjectPtr<USkeletalMesh> LoadedMesh(AssetPath);
	TestMash->SetSkeletalMesh(LoadedMesh.Get());
	XRLOG(Warning, TEXT("MeshLoadComplete"));
}

