// Fill out your copyright notice in the Description page of Project Settings.


#include "BaseCharacter.h"
#include "Engine.h"
#include "XRGameInstance.h"
#include "XRAssetMgr.h"
#include "XRAIController.h"

// Sets default values
ABaseCharacter::ABaseCharacter()
{
	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	//StatComponent = CreateDefaultSubobject<UCharacterStatComponent>(TEXT("CharacterStat"));
	FString CharacterDataPath = TEXT("DataTable'/Game/Resources/DataTable/CharacterData.CharacterData'");

	static ConstructorHelpers::FObjectFinder<UDataTable> DT_CHAR(*CharacterDataPath);
	if (DT_CHAR.Succeeded()) CharacterDataTable = DT_CHAR.Object;
}

// Called when the game starts or when spawned 
void ABaseCharacter::BeginPlay()
{
	Super::BeginPlay();
	//StatComponent->OnHPZero.AddDynamic(this, &ABaseCharacter::OnDead);
	SetCharacterLoadState(ECharacterLoadState::PREINIT);
}

void ABaseCharacter::PostInitializeComponents()
{
	Super::PostInitializeComponents();
}

// Called every frame
void ABaseCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);


}

// Called to bind functionality to input
void ABaseCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

void ABaseCharacter::SetCharacterLoadState(ECharacterLoadState NewLoadState)
{
	CurrentLoadState = NewLoadState;

	switch (CurrentLoadState)
	{
	case ECharacterLoadState::PREINIT:
	{

		SetCharacterLoadState(ECharacterLoadState::LOADING);
		break;
	}
	case ECharacterLoadState::LOADING:
	{
		
		int32 SkeletalID = 1000001;
		int32 AnimBPID = 3000001;
		
		SetSkelResource(SkeletalID, AnimBPID);
		break;
	}
	case ECharacterLoadState::READY:
	{

		AIControllerClass = AXRAIController::StaticClass();
		GetController()->Possess(this);
		break;
	}
	default:
		break;
	}

}

void ABaseCharacter::SetCharacterLifeState(ECharacterLifeState NewLifeState)
{

	CurrentLifeState = NewLifeState;

	switch (CurrentLifeState)
	{
	case ECharacterLifeState::SPAWN:
	{

		
		break;
	}
	case ECharacterLifeState::ALIVE:
	{	
		
		break;
	}
	case ECharacterLifeState::DEAD:
	{	
		break;
	}
	default:
		break;
	}

}

void ABaseCharacter::SetSkelResource(int32 SkeletalID, int32 AnimBPID)
{
	FSoftObjectPath AssetPath =
		GetAssetMgr()->FindResourceFromDataTable(SkeletalID);
	FStreamableDelegate ResultCallback;
	ResultCallback.BindLambda([AssetPath, this]()
	{
		TSoftObjectPtr<USkeletalMesh> LoadedMesh(AssetPath);


		GetMesh()->SetSkeletalMesh(LoadedMesh.Get());
		XRLOG(Warning, TEXT("MeshLoadComplete"));
		//SetCharacterLoadState(ECharacterLoadState::READY);
	});
	GetAssetMgr()->ASyncLoadAssetFromPath(AssetPath, ResultCallback);


	FSoftObjectPath AssetPath2 =
		GetAssetMgr()->FindResourceFromDataTable(AnimBPID);
	FStreamableDelegate ResultCallback2;
	ResultCallback2.BindLambda([AssetPath2, this]()
	{
		//애니메이션 블루프린트의 경우엔 TSoftClassPtr로 변환한다
		TSoftClassPtr<UAnimInstance> LoadedAnim(AssetPath2);
		GetMesh()->SetAnimInstanceClass(LoadedAnim.Get());
		XRLOG(Warning, TEXT("AnimBlueprint Load Complete"));
		SetCharacterLoadState(ECharacterLoadState::READY);
		//*** 클래스로 사용하는 에셋의 경우 에셋 경로 끝에 반드시  _C가 붙어야한다 ***/// 
	});
	GetAssetMgr()->ASyncLoadAssetFromPath(AssetPath2, ResultCallback2);
}

void ABaseCharacter::SetRemoteLocation(FVector remoteLocation)
{
	RemoteLocation = remoteLocation;
}

void ABaseCharacter::SetRemoteRotation(FRotator remoteRotator)
{
	RemoteRotator = remoteRotator;
}

FCharacterSizeInfo ABaseCharacter::FindCharacterSizeFromDataTable(int32 ID)
{
	FCharacterSizeInfo Out;
	if (CharacterDataTable)
	{
		FCharacterResource* TableRow = CharacterDataTable->FindRow<FCharacterResource>
			(FName(*(FString::FromInt(ID))), FString(""));
		if (TableRow)
		{
			Out.LocalTransform.InitFromString(TableRow->LocalTransform);
			Out.CapsuleHeight = TableRow->CapsuleHeight;
			Out.CapsuleRad = TableRow->CapsuleRad;

			return Out;
		}
	}
	check(false);
	return Out;
}

