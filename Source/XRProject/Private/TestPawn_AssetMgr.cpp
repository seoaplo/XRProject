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
	//게임 인스턴스를 에셋 매니저를 가지고있는 인스턴스로 캐스팅

	FSoftObjectPath AssetPath =
		TestGameInstance->GetXRAssetMgr()->FindResourceFromDataTable(1000001);
	//리소스 ID로 데이터 테이블에서 에셋 경로를 가져온다


	FStreamableDelegate ResultCallback;
	ResultCallback.BindUObject(this, &ATestPawn_AssetMgr::ResourceASyncLoadComplete, AssetPath);
	//해당 경로의 에셋이 로드가 완료됐을때, 이후 작업을 실행할 함수를 등록한다

	//따로 함수를 작성하지 않고 람다로 하는 방법

	//ResultCallback.BindLambda([AssetPath,this]()
	//{ 
	//	TSoftObjectPtr<USkeletalMesh> LoadedMesh(AssetPath);
	//	TestMash->SetSkeletalMesh(LoadedMesh.Get());
	//	XRLOG(Warning, TEXT("MeshLoadComplete")); 
	//});

	//해당 경로의 에셋을 비동기 로딩하고 완료시 ResultCallback에 등록된 함수의 로직을 실행한다
	TestGameInstance->GetXRAssetMgr()->ASyncLoadAssetFromPath(AssetPath, ResultCallback);





	FSoftObjectPath AssetPath2 =
		TestGameInstance->GetXRAssetMgr()->FindResourceFromDataTable(3000001);

	FStreamableDelegate ResultCallback2;
	ResultCallback2.BindLambda([AssetPath2, this]()
	{
		//애니메이션 블루프린트의 경우엔 TSoftClassPtr로 변환한다
		TSoftClassPtr<UAnimInstance> LoadedAnim(AssetPath2);
		TestMash->SetAnimInstanceClass(LoadedAnim.Get());
		XRLOG(Warning, TEXT("AnimBlueprint Load Complete"));
		//*** 클래스로 사용하는 에셋의 경우 에셋 경로 끝에 반드시  _C가 붙어야한다 ***///
	});

	TestGameInstance->GetXRAssetMgr()->ASyncLoadAssetFromPath(AssetPath2, ResultCallback2);





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
	//비동기 로딩이 완료되고 원하는 유형의 클래스 ptr로 변환해 리소스를 적용한다 

	XRLOG(Warning, TEXT("MeshLoadComplete"));
}

