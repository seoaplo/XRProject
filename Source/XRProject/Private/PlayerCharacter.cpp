// Fill out your copyright notice in the Description page of Project Settings.

#include "PlayerCharacter.h"
#include "ItemManager.h"
#include "XRGameInstance.h"
#include "Animation/AnimBlueprint.h"
#include "AccountManager.h"
#include "Components/InputComponent.h"
#include "Perception/AISenseConfig_Sight.h"
#include "NonePlayerCharacter.h"
APlayerCharacter::APlayerCharacter()
{
	
	PrimaryActorTick.bCanEverTick = true;
	PrimaryActorTick.bStartWithTickEnabled = true;

	PlayerStatComp = CreateDefaultSubobject<UPlayerCharacterStatComponent>(TEXT("CharacterStat"));
	PlayerStatComp->OnHPZero.AddDynamic(this, &ABaseCharacter::OnDead);

	static ConstructorHelpers::FClassFinder<UAnimInstance> AnimBP
	(TEXT("AnimBlueprint'/Game/Blueprint/Character/ABP_PlayerCharacter.ABP_PlayerCharacter_C'"));

	static ConstructorHelpers::FClassFinder<UAnimInstance> RemoteAnimBP
	(TEXT("AnimBlueprint'/Game/Blueprint/Character/ABP_RemoteCharacter.ABP_RemoteCharacter_C'"));
	
	if (AnimBP.Succeeded())
	{
		AnimInstance = AnimBP.Class;
	}
	else
		check(false);

	if (RemoteAnimBP.Succeeded())
	{
		RemoteAnimInstance = AnimBP.Class;
	}
	else
		check(false);

	BaseTurnRate = 45.f;
	BaseLookUpRate = 45.f;

	GetCapsuleComponent()->SetVisibility(true);
	GetCapsuleComponent()->bHiddenInGame = false;


	SpringArmLength = 300.0f;
	RotateSpeed = 1000.0f;
	MovementSpeed = kNormalMovementSpeed;


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
	GetCharacterMovement()->MaxWalkSpeed = kNormalMovementSpeed;

	CameraComponent->bUsePawnControlRotation = false;



	FName HairSocket("HairSocket");
	FName FaceSocket("FaceSocket");
	FName WeaponSocket("WeaponSocket");

	HairComponent = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("Hair"));
	FaceComponent = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("Face"));

	static ConstructorHelpers::FObjectFinder<USkeletalMesh>
		INVISIBLE_MESH
		(TEXT("SkeletalMesh'/Game/Resources/Character/PlayerCharacter/Mesh/CommonSkeleton/SK_Character_human_male_skeleton.SK_Character_human_male_skeleton'"));
	static ConstructorHelpers::FObjectFinder<USkeletalMesh>
		TESTMESH
		(TEXT("SkeletalMesh'/Game/Resources/Character/PlayerCharacter/Mesh/Body/SK_Character_human_male_body_common.SK_Character_human_male_body_common'"));
	static ConstructorHelpers::FObjectFinder<USkeletalMesh>
		FIRSTBODYMESH
		(TEXT("SkeletalMesh'/Game/Resources/Character/PlayerCharacter/Mesh/Body/SK_Character_human_male_body_common.SK_Character_human_male_body_common'"));

	GetMesh()->SetSkeletalMesh(INVISIBLE_MESH.Object);
	FaceComponent->SetSkeletalMesh(FIRSTBODYMESH.Object);

	Equipments.BodyComponent = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("Body"));
	Equipments.BodyComponent->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	Equipments.LegsComponent = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("Legs"));
	Equipments.LegsComponent->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	Equipments.HandsComponent = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("Hands"));
	Equipments.HandsComponent->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	Equipments.WeaponComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Weapon"));
	Equipments.WeaponComponent->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	GetMesh()->SetCollisionEnabled(ECollisionEnabled::QueryOnly);

	Equipments.BodyComponent->SetupAttachment(GetMesh());
	Equipments.LegsComponent->SetupAttachment(GetMesh());
	Equipments.HandsComponent->SetupAttachment(GetMesh());
	Equipments.WeaponComponent->SetupAttachment(GetMesh());
	FaceComponent->AttachToComponent(Equipments.BodyComponent, FAttachmentTransformRules::KeepRelativeTransform, FaceSocket);
	HairComponent->AttachToComponent(FaceComponent, FAttachmentTransformRules::KeepRelativeTransform, HairSocket);
	Equipments.WeaponComponent->AttachToComponent(Equipments.BodyComponent, FAttachmentTransformRules::KeepRelativeTransform, WeaponSocket);

	Equipments.BodyComponent->SetSkeletalMesh(FIRSTBODYMESH.Object);
	//Equipments.BodyComponent->SetAnimInstanceClass(AnimBP.Class);

	Equipments.LegsComponent->SetMasterPoseComponent(Equipments.BodyComponent);
	Equipments.HandsComponent->SetMasterPoseComponent(Equipments.BodyComponent);

	ScaleVector = FVector(3.85f, 3.85f, 3.85f);
	CapsuleSize = FVector2D(90.0f, 34.0f);
	MeshLocationVector = FVector(0.0f, 0.0f, -90.0f);
	WeaponScaleVector = FVector(0.4f, 0.4f, 0.4f);

	this->GetMesh()->SetRelativeScale3D(ScaleVector);
	this->GetCapsuleComponent()->SetCapsuleHalfHeight(CapsuleSize.X);
	this->GetCapsuleComponent()->SetCapsuleRadius(CapsuleSize.Y);
	this->GetMesh()->SetRelativeLocation(MeshLocationVector);
	Equipments.WeaponComponent->SetRelativeScale3D(WeaponScaleVector);

	ComboCount = 1;
	bIsMove = false;
	bIsRolling = false;
	bIsAttack = false;
	bIsSprint = false;
	bIsCharacterDead = false;
	bIsHit = false;
	bIsPlayer = false;
	bInitialized = false;

#pragma region TESTCODE

	/* 주의 : 남자임을 가정하고 테스트 중 */
	bIsMale = true;

#pragma endregion
	AutoPossessAI = EAutoPossessAI::PlacedInWorldOrSpawned;


	PlayerAIPerceptionStimul = CreateDefaultSubobject<UAIPerceptionStimuliSourceComponent>(TEXT("AIPerceptionStimulSource"));
}

APlayerCharacter::~APlayerCharacter()
{

}

void APlayerCharacter::Tick(float deltatime)
{
	Super::Tick(deltatime);
	ABaseCharacter::Tick(deltatime);

	if (Cast<APlayerController>(GetController()))
	{
		SumSec += deltatime;
		if (SumSec >= 0.1f) {
			SumSec = 0.0f;

			if (GetCharacterMovement()->Velocity.Size() > KINDA_SMALL_NUMBER)
			{
				OutputStream out;
				out.WriteOpcode(ENetworkCSOpcode::kNotifyCurrentChrPosition);
				out << 999;
				out << GetActorLocation();
				out << GetActorRotation();
				GEngine->AddOnScreenDebugMessage(1, 5.0f, FColor::Yellow, FString::Printf(TEXT("Send Location : %s"), *GetActorLocation().ToString()));
				GEngine->AddOnScreenDebugMessage(2, 5.0f, FColor::Yellow, FString::Printf(TEXT("Send Rotator : %s"), *GetActorRotation().ToString()));
				out.CompletePacketBuild();
				GetNetMgr().SendPacket(out);

				bIsMove = true;
			}
			else
			{
				if (bIsMove == true)
				{
					OutputStream out;
					out.WriteOpcode(ENetworkCSOpcode::kRequestCharacterWait);
					out << this->ObjectID;
					out << this->GetActorLocation();
					out.CompletePacketBuild();
					GetNetMgr().SendPacket(out);
				}

				bIsMove = false;
			}
		}
		GEngine->AddOnScreenDebugMessage(10, 5.0f, FColor::Yellow, FString::Printf(TEXT("Velocity : %s"), *GetCharacterMovement()->Velocity.ToString()));
	}
	else
	{
		if (GetCharacterMovement()->Velocity.Size() > KINDA_SMALL_NUMBER)
			bIsMove = true;
		else	
			bIsMove = false;
	}

	GEngine->AddOnScreenDebugMessage(3, 5.0f, FColor::Red, FString::Printf(TEXT("MoveSpeed : %s"), *FString::SanitizeFloat(GetCharacterMovement()->Velocity.Size())));
	Equipments.WeaponComponent->SetRelativeScale3D(WeaponScaleVector);



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
	PlayerInputComponent->BindAction("Attack", IE_Pressed, this, &APlayerCharacter::Attack);
	PlayerInputComponent->BindAction("Roll", IE_Pressed, this, &APlayerCharacter::Roll);
	PlayerInputComponent->BindAction("Sprint", IE_Pressed, this, &APlayerCharacter::Sprint);
	PlayerInputComponent->BindAction("Sprint", IE_Released, this, &APlayerCharacter::SprintEnd);

}

void APlayerCharacter::PostInitializeComponents()
{
	Super::PostInitializeComponents();


	PlayerAIPerceptionStimul->bAutoRegister = true;
	PlayerAIPerceptionStimul->RegisterForSense(UAISense_Sight::StaticClass());


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

	auto GameInstance = Cast < UXRGameInstance >(GetGameInstance());
	
	GameInstance->ItemManager->BuildItem(EItemType::EQUIPMENT, 3020001, GetWorld(), this);
	GameInstance->ItemManager->BuildItem(EItemType::EQUIPMENT, 3120001, GetWorld(), this);
	GameInstance->ItemManager->BuildItem(EItemType::EQUIPMENT, 3220001, GetWorld(), this);
	GameInstance->ItemManager->BuildItem(EItemType::EQUIPMENT, 3300001, GetWorld(), this);
	ChangePartsById(EPartsType::HAIR, 110);
	ChangePartsById(EPartsType::FACE, 120);


}

void APlayerCharacter::MoveForward(float Value)
{
	if (bIsAttack || bIsRolling || bIsHit || bIsCharacterDead )
		return;

	if ((Controller != NULL) && (Value != 0.0f))
	{
		if (bInitialized == false)
			check(false);

		const FRotator Rotation = Controller->GetControlRotation();
		const FRotator YawRotation(0, Rotation.Yaw, 0);

		const FVector Direction = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
		AddMovementInput(Direction, Value);
	}
}

void APlayerCharacter::MoveRight(float Value)
{
	if (bIsAttack || bIsRolling || bIsHit || bIsCharacterDead)
		return;

	if ((Controller != NULL) && (Value != 0.0f))
	{
		const FRotator Rotation = Controller->GetControlRotation();
		const FRotator YawRotation(0, Rotation.Yaw, 0);

		const FVector Direction = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);
		AddMovementInput(Direction, Value);
	}
}


void APlayerCharacter::ChangePartsById(EPartsType Type, int32 ID)
{
	auto GameInstance = Cast<UXRGameInstance>(UGameplayStatics::GetGameInstance(GetWorld()));

	FPartsResource* PartResourceTable = GameInstance->ItemManager->PartsDataTable->
		FindRow<FPartsResource>(*(FString::FromInt(ID)), TEXT("t"));

	if (Type == EPartsType::HAIR)
	{
		//헤어파츠
		FSoftObjectPath HairAssetPath = nullptr;
		HairAssetPath = GameInstance->GetXRAssetMgr()->FindResourceFromDataTable(PartResourceTable->ResourceID);
		FStreamableDelegate HairAssetLoadDelegate;
		HairAssetLoadDelegate = FStreamableDelegate::CreateUObject(this, &APlayerCharacter::LoadPartsComplete,
			HairAssetPath, EPartsType::HAIR);
		GameInstance->GetXRAssetMgr()->ASyncLoadAssetFromPath(HairAssetPath, HairAssetLoadDelegate);
	}
	else if (Type == EPartsType::FACE)
	{
		//페이스 파츠
		FSoftObjectPath FaceAssetPath = nullptr;
		FaceAssetPath = GameInstance->GetXRAssetMgr()->FindResourceFromDataTable(PartResourceTable->ResourceID);
		FStreamableDelegate FaceAssetLoadDelegate;
		FaceAssetLoadDelegate = FStreamableDelegate::CreateUObject(this, &APlayerCharacter::LoadPartsComplete,
			FaceAssetPath, EPartsType::FACE);
		GameInstance->GetXRAssetMgr()->ASyncLoadAssetFromPath(FaceAssetPath, FaceAssetLoadDelegate);
	}
	else
	{
		FSoftObjectPath ETCAssetPath = nullptr;
		ETCAssetPath = GameInstance->GetXRAssetMgr()->FindResourceFromDataTable(PartResourceTable->ResourceID);
		FStreamableDelegate ETCAssetLoadDelegate;
		ETCAssetLoadDelegate = FStreamableDelegate::CreateUObject(this, &APlayerCharacter::LoadPartsComplete,
			ETCAssetPath, Type);

		GameInstance->GetXRAssetMgr()->ASyncLoadAssetFromPath(ETCAssetPath, ETCAssetLoadDelegate);

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
	}

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
	}
}

void APlayerCharacter::ChangeEquipment(UItem * Item, UStaticMesh* SmMesh)
{
	UItemEquipment* EquipItem = Cast<UItemEquipment>(Item);

	if (EquipItem == nullptr)
		check(false);

	EEquipmentsType Types = EEquipmentsType::WEAPON;

	Equipments.WeaponItem = EquipItem;
	Equipments.WeaponComponent->SetStaticMesh(SmMesh);
}

void APlayerCharacter::ChangePartsComponentsMesh(EPartsType Type, FSoftObjectPath PartAsset)
{
	TSoftObjectPtr<USkeletalMesh> LoadedMesh(PartAsset);

	if (Type == EPartsType::HAIR)
	{
		HairComponent->SetSkeletalMesh(LoadedMesh.Get());
	}
	else if (Type == EPartsType::FACE)
	{
		FaceComponent->SetSkeletalMesh(LoadedMesh.Get());
	}
	else if (Type == EPartsType::NUDEBODY)
	{
		Equipments.BodyComponent->SetSkeletalMesh(LoadedMesh.Get());
	}
	else if (Type == EPartsType::NUDEHAND)
	{
		Equipments.HandsComponent->SetSkeletalMesh(LoadedMesh.Get());
	}
	else if (Type == EPartsType::NUDELEG)
	{
		Equipments.LegsComponent->SetSkeletalMesh(LoadedMesh.Get());
	}
}

float APlayerCharacter::TakeDamage(float Damage, FDamageEvent const & DamageEvent, AController * EventInstigator, AActor * DamageCauser)
{
	Super::TakeDamage(Damage, DamageEvent, EventInstigator, DamageCauser);

	XRLOG(Warning, TEXT("Player SetHP  : %f"), Damage);
	ANonePlayerCharacter* NPC = nullptr;
	if (DamageCauser != nullptr)
	{
		NPC = Cast<ANonePlayerCharacter>(DamageCauser);
	}

	if (NPC == nullptr)
		check(false);

	if (PlayerStatComp->GetCurrentHP() < 0.0f)
	{
		SetCharacterLifeState(ECharacterLifeState::DEAD);
		bIsCharacterDead = true;
		//Dead 재생
	}
	else
	{
		PlayerStatComp->SetCurrentHP(PlayerStatComp->GetCurrentHP() - Damage);
		bIsHit = true;
		MyAnimInstance->PlayHitMontage();

		UE_LOG(LogTemp, Warning, TEXT("Damaged By %s, Damage : %f"), *(NPC->GetName()), Damage);
		//Hit 재생
	}
	return 0.0f;
}

void APlayerCharacter::Attack()
{
	if (bIsRolling == true || bIsHit)
		return;

	AttackOverlapList.clear(); //Overlap list 초기화

	//first
	if (bIsAttack == false)
	{
		bIsAttack = true;
		MyAnimInstance->PlayAttackMontage();
		
		OutputStream out;
		out.WriteOpcode(ENetworkCSOpcode::kCharacterAttack);
		out << ComboCount;
		out << GetActorLocation();
		out << GetActorRotation();
		out.CompletePacketBuild();
		GetNetMgr().SendPacket(out);

	}
	else
		bSavedCombo = true;



}

void APlayerCharacter::Roll()
{
	bIsRolling = true;
}

void APlayerCharacter::Sprint()
{
	GetCharacterMovement()->MaxWalkSpeed = kSprintMovementSpeed;
	bIsSprint = true;

	if (Cast<APlayerController>(GetController()))
	{
		OutputStream out;
		out.WriteOpcode(ENetworkCSOpcode::kRequestCharacterSprint);
		out << this->ObjectID;
		out.CompletePacketBuild();
		GetNetMgr().SendPacket(out);

		UE_LOG(LogTemp, Warning, TEXT("CharacterSprint Send"));
	}
}

void APlayerCharacter::SprintEnd()
{
	GetCharacterMovement()->MaxWalkSpeed = kNormalMovementSpeed;
	bIsSprint = false;

	UE_LOG(LogTemp, Warning, TEXT("Sprint END"));
}

void APlayerCharacter::InitializeCharacter(bool bIsPlayerCharacter, CharacterData & Data)
{
	bInitialized = true;

	bIsPlayer = bIsPlayerCharacter;

	auto GameInstance = Cast<UXRGameInstance>(GetGameInstance());

	if (bIsPlayerCharacter)
	{
		Equipments.BodyComponent->SetAnimInstanceClass(AnimInstance);
		MyAnimInstance = Cast<UPlayerCharacterAnimInstance>(Equipments.BodyComponent->GetAnimInstance());
		MyAnimInstance->Delegate_CheckNextCombo.BindUFunction(this, FName("ContinueCombo"));
		MyAnimInstance->OnMontageEnded.AddDynamic(this, &APlayerCharacter::OnMyMontageEnded);
	}
	else
	{
		Equipments.BodyComponent->SetAnimInstanceClass(RemoteAnimInstance);
		MyAnimInstance = Cast<UPlayerCharacterAnimInstance>(Equipments.BodyComponent->GetAnimInstance());
	}
		MyAnimInstance->SetOwnerCharacter(this);
	
		ObjectID = Data.ObjectID;
		PlayerStatComp->SetCurrentHP(Data.Current_HP);
		PlayerStatComp->SetMaxHP(Data.Max_HP);
		PlayerStatComp->SetAttack_Min(Data.Attack_Min);
		PlayerStatComp->SetAttack_Max(Data.Attack_Max);
		PlayerStatComp->SetAttack_Range(Data.Attack_Range);
		PlayerStatComp->SetAttack_Speed(Data.Attack_Speed);
		PlayerStatComp->SetSpeed(Data.Speed);
		PlayerStatComp->SetDefence(Data.Defence);
		PlayerStatComp->SetLevel(Data.Level);
		PlayerStatComp->SetGender(Data.Gender);
		PlayerStatComp->SetSTR(Data.STR);
		PlayerStatComp->SetDEX(Data.DEX);
		PlayerStatComp->SetINT(Data.INT);
		PlayerStatComp->SetCurrentStamina(Data.CurrentStamina);
		PlayerStatComp->SetMaxStamina(Data.MaxStamina);

		for (int ii = 0; ii < Data.kEquipmentArraySize; ii++)
		{
			/*맨몸일 때 */
			if (Data.EquipArray[ii].ID == -1)
			{
				//FEquipmentTableResource* EqTable;

				const int32 kMalePrimaryBody = 130;
				const int32 kMalePrimaryHand = 140;
				const int32 kMalePrimaryLeg = 150;
				const int32 kMalePrimaryWeapon = 3300001;

				switch (ii)
				{
				case 0:
					ChangePartsById(EPartsType::NUDEBODY, kMalePrimaryBody);
					break;
				case 1:
					ChangePartsById(EPartsType::NUDEHAND, kMalePrimaryHand);
					break;
				case 2:
					ChangePartsById(EPartsType::NUDELEG, kMalePrimaryLeg);
					break;
				case 3:
					GameInstance->ItemManager->BuildItem(EItemType::EQUIPMENT, kMalePrimaryWeapon,
						GameInstance->GetWorld(), this);
					break;
				}
			}
			else
				GameInstance->ItemManager->BuildItem(EItemType::EQUIPMENT, Data.EquipArray[ii].ID, GetWorld(), this);
		}

}

void APlayerCharacter::OnMyMontageEnded(UAnimMontage* Montage, bool bInterrupted)
{

	if (MyAnimInstance->AttackMontage == Montage)
	{
		bIsAttack = false;
		bSavedCombo = false;
		ComboCount = 1;
	}
	else if (MyAnimInstance->HitMontage == Montage)
	{
		bIsHit = false;
	}
}


void APlayerCharacter::ContinueCombo()
{
	if (bSavedCombo)
	{
		if (ComboCount >= kMaxComboCount)
		{
			ComboCount = 1;
			return;
		}

		ComboCount++;
		MyAnimInstance->JumpToComboMontageSection(ComboCount);
		bSavedCombo = false;
		XRLOG(Warning, TEXT("CurrentCombo : %d"), ComboCount);


		OutputStream out;
		out.WriteOpcode(ENetworkCSOpcode::kCharacterAttack);
		out << ComboCount;
		out << GetActorLocation();
		out << GetActorRotation();
		out.CompletePacketBuild();
		GetNetMgr().SendPacket(out);

	}
}


void APlayerCharacter::LoadPartsComplete(FSoftObjectPath AssetPath, EPartsType Type)
{
	//TSoftObjectPtr<USkeletalMesh> LoadedMesh(AssetPath);
	//AccountManager::GetInstance().GetCurrentPlayerCharacter()->ChangePartsComponentsMesh(Type, LoadedMesh.Get());
	this->ChangePartsComponentsMesh(Type, AssetPath);

}

void APlayerCharacter::OnOverlapBegin(UPrimitiveComponent * OverlappedComp, AActor * OtherActor,
	UPrimitiveComponent * OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult & SweepResult)
{

	if (Cast<APlayerController>(GetController()))
	{
		ANonePlayerCharacter* NPC = Cast<ANonePlayerCharacter>(OtherActor);
		if (NPC)
		{

			for (ANonePlayerCharacter* FlagNpc : AttackOverlapList)
			{
				if (FlagNpc == NPC)
					return;
			}

			int64 EnemyID = NPC->ObjectID;
			int64 MyID = this->ObjectID;

			OutputStream out;
			out.WriteOpcode(ENetworkCSOpcode::kCharcterHitSuccess);
			out << MyID;
			out << EnemyID;
			out << this->GetActorLocation();
			out << this->GetActorRotation();
			out.CompletePacketBuild();
			GetNetMgr().SendPacket(out);

			AttackOverlapList.push_back(NPC);

		}
	}
}

void APlayerCharacter::SetIsPlayer(bool is)
{
	bIsPlayer = is;
}

bool APlayerCharacter::GetIsPlayer()
{
	return bIsPlayer;
}