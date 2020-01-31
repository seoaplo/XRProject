// Fill out your copyright notice in the Description page of Project Settings.

#include "PlayerCharacter.h"
#include "ItemManager.h"
#include "XRGameInstance.h"
#include "AccountManager.h"
#include "Components/InputComponent.h"

APlayerCharacter::APlayerCharacter()
{
	PrimaryActorTick.bCanEverTick = true;
	PrimaryActorTick.bStartWithTickEnabled = true;

	PlayerStatComp = CreateDefaultSubobject<UPlayerCharacterStatComponent>(TEXT("CharacterStat"));
	PlayerStatComp->OnHPZero.AddDynamic(this, &ABaseCharacter::OnDead);

	BaseTurnRate = 45.f;
	BaseLookUpRate = 45.f;

	GetCapsuleComponent()->SetVisibility(true);
	GetCapsuleComponent()->bHiddenInGame = false;

	SpringArmLength = 300.0f;
	RotateSpeed = 1000.0f;
	MovementSpeed = 1000.0f;

	SpringArmComponent = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArm"));
	SpringArmComponent->SetupAttachment(RootComponent);
	SpringArmComponent->TargetArmLength = SpringArmLength;
	SpringArmComponent->bUsePawnControlRotation = true;

	CameraComponent = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));
	CameraComponent->SetupAttachment(SpringArmComponent);

	bUseControllerRotationYaw = false; //컨트롤러 로테이션에 따라 캐릭터가 회전하는 bool. 해제해야 임의로 회전시킬 수 있음.
	bUseControllerRotationPitch = false; //컨트롤러 로테이션에 따라 캐릭터가 회전하는 bool. 해제해야 임의로 회전시킬 수 있음.
	bUseControllerRotationRoll = false; //컨트롤러 로테이션에 따라 캐릭터가 회전하는 bool. 해제해야 임의로 회전시킬 수 있음.

	GetCharacterMovement()->bOrientRotationToMovement = true;
	GetCharacterMovement()->RotationRate = FRotator(0.0f, RotateSpeed, 0.0f);
	GetCharacterMovement()->AirControl = 0.5f;
	GetCharacterMovement()->MaxWalkSpeed = MovementSpeed;

	CameraComponent->bUsePawnControlRotation = false;
	
	
	FName HairSocket("HairSocket");
	FName FaceSocket("FaceSocket");
	
	HairComponent = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("Hair"));
	FaceComponent = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("Face"));

	Equipments.BodyComponent = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("Body"));
	Equipments.LegsComponent = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("Legs"));
	Equipments.HandsComponent = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("Hands"));
	Equipments.BodyComponent->SetupAttachment(RootComponent);
	Equipments.LegsComponent->SetupAttachment(RootComponent);
	Equipments.HandsComponent->SetupAttachment(RootComponent);
	HairComponent->AttachToComponent(Equipments.BodyComponent, FAttachmentTransformRules::KeepRelativeTransform, HairSocket);
	FaceComponent->AttachToComponent(Equipments.BodyComponent, FAttachmentTransformRules::KeepRelativeTransform, FaceSocket);

	

#pragma region TESTCODE

	/* 주의 : 남자임을 가정하고 테스트 중 */
	bIsMale = true;

	//Equipments.BodyComponent = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("Body"));
	//Equipments.BodyComponent->SetupAttachment(RootComponent);
	auto Hair = 
		ConstructorHelpers::FObjectFinder
		<USkeletalMesh>
		(TEXT("SkeletalMesh'/Game/Resources/Character/PlayerCharacter/Mesh/Body/SK_Character_human_male_body_common.SK_Character_human_male_body_common'"));
	//Equipments.BodyComponent->SetSkeletalMesh(Mesh.Object);
#pragma endregion

}

APlayerCharacter::~APlayerCharacter()
{

}

void APlayerCharacter::Tick(float deltatime)
{
	ABaseCharacter::Tick(deltatime);
#pragma region TESTCODE
	GetCharacterMovement()->MaxWalkSpeed = MovementSpeed;
#pragma endregion
}

void APlayerCharacter::SetupPlayerInputComponent(UInputComponent * PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
	PlayerInputComponent->BindAxis("Forward", this, &APlayerCharacter::MoveForward);
	PlayerInputComponent->BindAxis("Right", this, &APlayerCharacter::MoveRight);
	PlayerInputComponent->BindAxis("LookUp", this, &APawn::AddControllerYawInput);
	PlayerInputComponent->BindAxis("TurnRate", this, &APlayerCharacter::TurnAtRate);
	PlayerInputComponent->BindAxis("Turn", this, &APawn::AddControllerPitchInput);
	PlayerInputComponent->BindAxis("LookUpRate", this, &APlayerCharacter::LookUpAtRate);
	PlayerInputComponent->BindAction("Attack", IE_Pressed,this, &APlayerCharacter::Attack);

}

void APlayerCharacter::PostInitializeComponents()
{
	Super::PostInitializeComponents();
	AnimInstance = Cast<UPlayerCharacterAnimInstance>(GetMesh()->GetAnimInstance());
	//AnimInstance->Delegate_CheckNextCombo.AddDynamic(this, )

}

void APlayerCharacter::PossessedBy(AController* controller)
{
	Super::PossessedBy(controller);
}

void APlayerCharacter::TurnAtRate(float Rate)
{
	AddControllerYawInput(Rate * BaseTurnRate * GetWorld()->GetDeltaSeconds());
}

void APlayerCharacter::LookUpAtRate(float Rate)
{
	AddControllerPitchInput(Rate * BaseLookUpRate * GetWorld()->GetDeltaSeconds());
}


void APlayerCharacter::BeginPlay()
{
	ABaseCharacter::BeginPlay();
	auto GameInstance = Cast < UXRGameInstance > (GetGameInstance());
	bool Ret = AccountManager::GetInstance().SetCurrentPlayerCharacter(this);
	check(Ret);
	GameInstance->ItemManager->BuildItem(EItemType::EQUIPMENT, 3020001, GetWorld(), this);
	GameInstance->ItemManager->BuildItem(EItemType::EQUIPMENT, 3120001, GetWorld(), this);
	GameInstance->ItemManager->BuildItem(EItemType::EQUIPMENT, 3220001, GetWorld(), this);

}

void APlayerCharacter::MoveForward(float Value)
{

	if ((Controller != NULL) && (Value != 0.0f))
	{
		const FRotator Rotation = Controller->GetControlRotation();
		const FRotator YawRotation(0, Rotation.Yaw, 0);

		const FVector Direction = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
		AddMovementInput(Direction, Value);
	}
}

void APlayerCharacter::MoveRight(float Value)
{
	if ((Controller != NULL) && (Value != 0.0f))
	{
		const FRotator Rotation = Controller->GetControlRotation();
		const FRotator YawRotation(0, Rotation.Yaw, 0);
		
		const FVector Direction = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);
		AddMovementInput(Direction, Value);
	}
}


void APlayerCharacter::ChangeEquipment(UItem * Item, USkeletalMesh* SkMesh)
{
	UItemEquipment* EquipItem = Cast<UItemEquipment>(Item);

	if (EquipItem == nullptr)
		check(false);

	EEquipmentsType Types;

	switch (EquipItem->DefaultInfo.Type)
	{
	case 0: { Types = EEquipmentsType::BODY; break; }
	case 1: { Types = EEquipmentsType::HANDS; break; }
	case 2: { Types = EEquipmentsType::LEGS; break; }
	case 3: { Types = EEquipmentsType::WEAPON; break; }
	}


	//클라의 아이템빌더에서 아이템이 이미 빌드되어 나왔다고 가정
	//현재 캐릭터가 남/여인지는 아마 GetPawn같은걸로 가져오면 될 듯
	//아이템빌더에서 애셋로드까지 되면 이 함수가 실행됨
	//웨폰이면 차라리 함수를 다르게 할까

	switch (Types)
	{
		case EEquipmentsType::BODY:
			Equipments.BodyItem = EquipItem;
			Equipments.BodyComponent->SetSkeletalMesh(SkMesh);
			break;
		case EEquipmentsType::HANDS:
			Equipments.HandsItem = EquipItem;
			Equipments.HandsComponent->SetSkeletalMesh(SkMesh);
			break;
		case EEquipmentsType::LEGS:
			Equipments.LegsItem = EquipItem;
			Equipments.LegsComponent->SetSkeletalMesh(SkMesh);
			break;
		case EEquipmentsType::WEAPON:
			Equipments.WeaponItem = EquipItem;
			Equipments.WeaponComponent->SetSkeletalMesh(SkMesh);
			break;
	}


}

void APlayerCharacter::ChangePartsComponentsMesh(EPartsType Type, USkeletalMesh * PartsMesh)
{
	if (Type == EPartsType::HAIR)
	{
		HairComponent->SetSkeletalMesh(PartsMesh);
	}
	else if (Type == EPartsType::FACE)
	{
		FaceComponent->SetSkeletalMesh(PartsMesh);
	}
}

void APlayerCharacter::Attack()
{
	if (bIsAttack == false)
	{
		bIsAttack = true;
		//AnimInstance->

	}

}
