// Fill out your copyright notice in the Description page of Project Settings.

#include "PlayerCharacter.h"
#include "ItemManager.h"
#include "WidgetTree.h"
#include "Animation/AnimBlueprint.h"
#include "HealthBarWidget.h"
#include "AccountManager.h"
#include "Components/InputComponent.h"
#include "Components/WidgetComponent.h"
#include "Perception/AISenseConfig_Sight.h"
#include "Perception/AISenseConfig_Damage.h"
#include "HealthBarWidget.h"
#include "XRPlayerController.h"
#include "XRGameInstance.h"
#include "NonePlayerCharacter.h"
#include "NickNameWidget.h"

APlayerCharacter::APlayerCharacter()
{
	
	PrimaryActorTick.bCanEverTick = true;
	PrimaryActorTick.bStartWithTickEnabled = true;
	
	PlayerStatComp = CreateDefaultSubobject<UPlayerCharacterStatComponent>(TEXT("CharacterStat"));
	PlayerStatComp->OnHPZero.AddDynamic(this, &APlayerCharacter::OnDead);

	static ConstructorHelpers::FClassFinder<UAnimInstance> AnimBP
	(TEXT("AnimBlueprint'/Game/Blueprint/Character/ABP_PlayerCharacter.ABP_PlayerCharacter_C'"));
	
	static ConstructorHelpers::FClassFinder<UAnimInstance> FemaleAnimBP
	(TEXT("AnimBlueprint'/Game/Blueprint/Character/ABP_F_PlayerCharacter.ABP_F_PlayerCharacter_C'"));

	static ConstructorHelpers::FClassFinder<UAnimInstance> RemoteAnimBP
	(TEXT("AnimBlueprint'/Game/Blueprint/Character/ABP_RemoteCharacter.ABP_RemoteCharacter_C'"));

	static ConstructorHelpers::FClassFinder<UAnimInstance> RemoteFemaleAnimBP
	(TEXT("AnimBlueprint'/Game/Blueprint/Character/ABP_F_RemoteCharacter.ABP_F_RemoteCharacter_C'"));
	

	NameTag = CreateDefaultSubobject<UWidgetComponent>(TEXT("WidgetComponent"));
	NameTag->SetupAttachment(GetRootComponent());
	NameTag->SetWidgetSpace(EWidgetSpace::Screen);


	check(AnimBP.Succeeded());
	check(RemoteAnimBP.Succeeded());
	check(FemaleAnimBP.Succeeded());
	check(RemoteFemaleAnimBP.Succeeded());

	AnimInstance = AnimBP.Class;
	RemoteAnimInstance = AnimBP.Class;
	FemaleAnimInstance = FemaleAnimBP.Class;
	FemaleRemoteAnimInstance = RemoteFemaleAnimBP.Class;

	BaseTurnRate = 45.f;
	BaseLookUpRate = 45.f;

	ScaleVector = FVector(3.85f, 3.85f, 3.85f);
	CapsuleSize = FVector2D(90.0f, 34.0f);
	RollingHitCapsuleSize = FVector2D(45.0f, 34.0f);
	MeshLocationVector = FVector(0.0f, 0.0f, -90.0f);
	WeaponScaleVector = FVector(0.4f, 0.4f, 0.4f);
	RollingCapsuleOffset = 45.0f;

	HitCapsule = CreateDefaultSubobject<UCapsuleComponent>(TEXT("HitCapsule"));
	HitCapsule->SetVisibility(false);
	HitCapsule->bHiddenInGame = true;
	HitCapsule->SetCapsuleHalfHeight(CapsuleSize.X);
	HitCapsule->SetCapsuleRadius(CapsuleSize.Y);
	HitCapsule->SetGenerateOverlapEvents(true);
	HitCapsule->SetCollisionProfileName("Player");

	GetCapsuleComponent()->SetVisibility(true);
	GetCapsuleComponent()->bHiddenInGame = true;
	GetCapsuleComponent()->SetGenerateOverlapEvents(false);


	SpringArmLength = 350.0f;
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

	static ConstructorHelpers::FObjectFinder<UParticleSystem>
		SWORDTRAIL_NORMAL
		(TEXT("ParticleSystem'/Game/Resources/Effect/SwordTrail/P_PlayerSwordTrail.P_PlayerSwordTrail'"));
	static ConstructorHelpers::FObjectFinder<UParticleSystem>
		SWORDTRAIL_FINAL
		(TEXT("ParticleSystem'/Game/Resources/Effect/SwordTrail/P_PlayerSwordTrailFianl.P_PlayerSwordTrailFianl'"));
	static ConstructorHelpers::FObjectFinder<UParticleSystem>
		BLOOD_EFFECT
		(TEXT("ParticleSystem'/Game/Resources/Effect/Paticle/Blood_cloud_large.Blood_cloud_large'"));

	static ConstructorHelpers::FObjectFinder<UParticleSystem>
		ATTACK_EFFECT_1
		(TEXT("ParticleSystem'/Game/Resources/Effect/Paticle/P_SwordReact_1.P_SwordReAct_1'"));
	static ConstructorHelpers::FObjectFinder<UParticleSystem>
		ATTACK_EFFECT_2
		(TEXT("ParticleSystem'/Game/Resources/Effect/Paticle/P_SwordReact_2.P_SwordReAct_2'"));
	static ConstructorHelpers::FObjectFinder<UParticleSystem>
		ATTACK_EFFECT_3
		(TEXT("ParticleSystem'/Game/Resources/Effect/Paticle/P_SwordReact_3.P_SwordReAct_3'"));
	static ConstructorHelpers::FObjectFinder<UParticleSystem>
		ATTACK_EFFECT_4
		(TEXT("ParticleSystem'/Game/Resources/Effect/Paticle/P_SwordReact_4.P_SwordReAct_4'"));

	static ConstructorHelpers::FObjectFinder<UParticleSystem>
		BERSERK_EFFECT_START
		(TEXT("ParticleSystem'/Game/Resources/Effect/Paticle/P_BuffStart.P_BuffStart'"));
	static ConstructorHelpers::FObjectFinder<UParticleSystem>
		BERSERK_EFFECT_LOOP
		(TEXT("ParticleSystem'/Game/Resources/Effect/Paticle/P_BuffLoop.P_BuffLoop'"));


	static ConstructorHelpers::FObjectFinder<UObject>
		BLUR_MAT
		(TEXT("MaterialInstanceConstant'/Game/Resources/Character/PlayerCharacter/Material/RadialBlur_Inst.RadialBlur_Inst'"));


	//check(SWORDTRAIL_NORMAL.Succeeded());
	//check(SWORDTRAIL_FINAL.Succeeded());
	//check(BERSERK_EFFECT_START.Succeeded());
	//check(BERSERK_EFFECT_LOOP.Succeeded());
	//check(BLOOD_EFFECT.Succeeded());
	//check(ATTACK_EFFECT.Succeeded());


	BlurMaterial = Cast<UMaterialInstance>(BLUR_MAT.Object);
	DynamicBlurMaterial = UMaterialInstanceDynamic::Create(BlurMaterial, CameraComponent);
	CameraComponent->PostProcessSettings.AddBlendable(DynamicBlurMaterial, 1.0f);
	
	GetMesh()->SetSkeletalMesh(INVISIBLE_MESH.Object);
	FaceComponent->SetSkeletalMesh(FIRSTBODYMESH.Object);

	Equipments.BodyComponent = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("Body"));
	Equipments.BodyComponent->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	Equipments.BodyComponent->SetCollisionProfileName("Trigger");
	Equipments.BodyComponent->SetGenerateOverlapEvents(false);

	Equipments.LegsComponent = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("Legs"));
	Equipments.LegsComponent->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	Equipments.LegsComponent->SetGenerateOverlapEvents(false);

	Equipments.HandsComponent = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("Hands"));
	Equipments.HandsComponent->SetCollisionProfileName("Trigger");
	Equipments.HandsComponent->SetGenerateOverlapEvents(false);
	Equipments.HandsComponent->SetCollisionEnabled(ECollisionEnabled::QueryOnly);

	Equipments.WeaponComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Weapon"));
	Equipments.WeaponComponent->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	Equipments.WeaponComponent->SetCollisionProfileName("PlayerWeapon");

	GetMesh()->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	GetMesh()->SetGenerateOverlapEvents(false);

	Equipments.BodyComponent->SetupAttachment(GetMesh());
	Equipments.LegsComponent->SetupAttachment(GetMesh());
	Equipments.HandsComponent->SetupAttachment(GetMesh());
	Equipments.WeaponComponent->SetupAttachment(GetMesh());
	FaceComponent->AttachToComponent(Equipments.BodyComponent, FAttachmentTransformRules::KeepRelativeTransform, FaceSocket);
	HairComponent->AttachToComponent(FaceComponent, FAttachmentTransformRules::KeepRelativeTransform, HairSocket);
	Equipments.WeaponComponent->AttachToComponent(Equipments.BodyComponent, FAttachmentTransformRules::KeepRelativeTransform, WeaponSocket);
	Equipments.WeaponComponent->OnComponentBeginOverlap.AddDynamic(this, &APlayerCharacter::OnOverlapBegin);

	Equipments.BodyComponent->SetSkeletalMesh(FIRSTBODYMESH.Object);
	//Equipments.BodyComponent->SetAnimInstanceClass(AnimBP.Class);

	Equipments.LegsComponent->SetMasterPoseComponent(Equipments.BodyComponent);
	Equipments.HandsComponent->SetMasterPoseComponent(Equipments.BodyComponent);
	HitCapsule->SetupAttachment(RootComponent);

	this->GetMesh()->SetRelativeScale3D(ScaleVector);
	this->GetCapsuleComponent()->SetCapsuleHalfHeight(CapsuleSize.X);
	this->GetCapsuleComponent()->SetCapsuleRadius(CapsuleSize.Y);
	this->GetMesh()->SetRelativeLocation(MeshLocationVector);
	Equipments.WeaponComponent->SetRelativeScale3D(WeaponScaleVector);
	NameTagLocation = FVector(0.0f, 0.0f, 90.0f);

	SwordTrailNormal = CreateDefaultSubobject<UParticleSystemComponent>(TEXT("SwordTrailNormal"));
	SwordTrailFinal = CreateDefaultSubobject<UParticleSystemComponent>(TEXT("SwordTrailFinal"));
	BerserkBuffStart = CreateDefaultSubobject<UParticleSystemComponent>(TEXT("BerserkStart"));
	BerserkBuffLoop = CreateDefaultSubobject<UParticleSystemComponent>(TEXT("BerserkLoop"));
	UParticleSystemComponent* AttackEffect1 = CreateDefaultSubobject<UParticleSystemComponent>(TEXT("AttackEffect1"));
	UParticleSystemComponent* AttackEffect2 = CreateDefaultSubobject<UParticleSystemComponent>(TEXT("AttackEffect2"));
	UParticleSystemComponent* AttackEffect3 = CreateDefaultSubobject<UParticleSystemComponent>(TEXT("AttackEffect3"));
	UParticleSystemComponent* AttackEffect4 = CreateDefaultSubobject<UParticleSystemComponent>(TEXT("AttackEffect4"));
	BloodEffect = CreateDefaultSubobject<UParticleSystemComponent>(TEXT("BloodEffect"));
	SwordTrailNormal->bAutoActivate = false;
	SwordTrailFinal->bAutoActivate = false;

	SwordTrailNormal->SetTemplate(SWORDTRAIL_NORMAL.Object);
	SwordTrailFinal->SetTemplate(SWORDTRAIL_FINAL.Object);
	BerserkBuffStart->SetTemplate(BERSERK_EFFECT_START.Object);
	BerserkBuffLoop->SetTemplate(BERSERK_EFFECT_LOOP.Object);
	BloodEffect->SetTemplate(BLOOD_EFFECT.Object);

	AttackEffect1->SetTemplate(ATTACK_EFFECT_1.Object);
	AttackEffect2->SetTemplate(ATTACK_EFFECT_2.Object);
	AttackEffect3->SetTemplate(ATTACK_EFFECT_3.Object);
	AttackEffect4->SetTemplate(ATTACK_EFFECT_4.Object);

	BerserkBuffStart->AttachToComponent(Equipments.BodyComponent, FAttachmentTransformRules::KeepRelativeTransform,
		ComboParticleSocketName.FxBottom);
	BerserkBuffLoop->AttachToComponent(Equipments.BodyComponent, FAttachmentTransformRules::KeepRelativeTransform,
		ComboParticleSocketName.FxBottom);

	BerserkBuffStart->bAutoActivate = false;
	BerserkBuffLoop->bAutoActivate = false;

	ParticleArray.Add(SwordTrailNormal);
	ParticleArray.Add(SwordTrailFinal);
	ParticleArray.Add(BerserkBuffStart);
	ParticleArray.Add(BerserkBuffLoop);
	ParticleArray.Add(BloodEffect);

	AttackEffectList.Add(AttackEffect1);
	AttackEffectList.Add(AttackEffect2);
	AttackEffectList.Add(AttackEffect3);
	AttackEffectList.Add(AttackEffect4);

	ComboCount = 1;
	CurrentAttackID = -1;
	bIsMove = false;
	bIsRolling = false;
	bIsOverallRollAnimPlaying = false;
	bIsAttack = false;
	bIsSprint = false;
	bIsCharacterDead = false;
	bIsHit = false;
	bIsAttackMoving = false;
	bIsPlayer = false;
	bInitialized = false;
	bIsTestMode = false;
	bIsMouseShow = false;
	bIsMoveLocked = false;
	bIsInvisible = false;
	bIsPathFinding = false;
	ForwardValue = 0.0f;
	RightValue = 0.0f;
	KnockBackVector = FVector(0.0f, 0.0f, 0.0f);
	LocationSyncFailCount = 0;

	MyShake = UPlayerCameraShake::StaticClass();
	TestID = 2;

	AutoPossessAI = EAutoPossessAI::PlacedInWorldOrSpawned;


	PlayerAIPerceptionStimul = CreateDefaultSubobject<UAIPerceptionStimuliSourceComponent>(TEXT("AIPerceptionStimulSource"));
	AISenseDamage = CreateOptionalDefaultSubobject<UAISenseConfig_Damage>(TEXT("Damage Config"));
	PlayerAIPerceptionStimul->bAutoRegister = true;
	PlayerAIPerceptionStimul->RegisterForSense(AISenseDamage->GetSenseImplementation());


	/*TEST CODE*/
	AttackEffectRot = FRotator(0.0f, 0.0f, 0.0f);

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
	NameTag->SetRelativeLocation(NameTagLocation);
	
	if (bIsRolling || bIsAttackMoving || bIsSkillMove)
		AddMovementInput(GetActorForwardVector(), 1.0f, false);
	if(bIsKnockBackMoving)
	{
		FVector vVec = KnockBackVector;
		AddMovementInput(vVec, 1.0f, false);
	}

	if (bIsAttackMoving)
	{
		FRotator NextRot = FMath::RInterpConstantTo(GetActorRotation(), AttackNextRotation, deltatime, 1200.0f);
		SetActorRotation(NextRot);
	}

	float CurVelocity = GetCharacterMovement()->Velocity.Size();
	float DensityValue = CurVelocity / 750.f * 2.0f;

	if(CurVelocity > kNormalMovementSpeed + 1.1f && !bIsRolling && !bIsKnockBackMoving)
		DynamicBlurMaterial->SetScalarParameterValue(TEXT("Density"), DensityValue);
	else
		DynamicBlurMaterial->SetScalarParameterValue(TEXT("Density"), 1.0f);

}

void APlayerCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
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
	PlayerInputComponent->BindAction("ShowCursor", IE_Pressed, this, &APlayerCharacter::ToggleMouseCursor);
	PlayerInputComponent->BindAction("TEST", IE_Pressed, this, &APlayerCharacter::TestPlay);
	PlayerInputComponent->BindAction("WheelUp", IE_Pressed, this, &APlayerCharacter::WheelUp);
	PlayerInputComponent->BindAction("WheelDown", IE_Pressed, this, &APlayerCharacter::WheelDown);

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

	CurGameInstance = Cast<UXRGameInstance>(GetWorld()->GetGameInstance());
	//ChangePartsById(EPartsType::HAIR, 110);
	//ChangePartsById(EPartsType::FACE, 120);
}

void APlayerCharacter::MoveForward(float Value)
{
	ForwardValue = Value;
	if (bIsAttack || bIsOverallRollAnimPlaying || bIsHit || bIsCharacterDead || bIsSkillPlaying || bIsMoveLocked)
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
	RightValue = Value;
	
	if (bIsAttack || bIsOverallRollAnimPlaying || bIsHit || bIsCharacterDead || bIsSkillPlaying || bIsMoveLocked)
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
	
	FPartsResource* PartResourceTable = CurGameInstance->ItemManager->PartsDataTable->
		FindRow<FPartsResource>(*(FString::FromInt(ID)), TEXT("t"));

	if (Type == EPartsType::HAIR)
	{
		//헤어파츠
		FSoftObjectPath HairAssetPath = nullptr;
		HairAssetPath = CurGameInstance->GetXRAssetMgr()->FindResourceFromDataTable(PartResourceTable->ResourceID);
		FStreamableDelegate HairAssetLoadDelegate;
		HairAssetLoadDelegate = FStreamableDelegate::CreateUObject(this, &APlayerCharacter::LoadPartsComplete,
			HairAssetPath, EPartsType::HAIR);
		CurGameInstance->GetXRAssetMgr()->ASyncLoadAssetFromPath(HairAssetPath, HairAssetLoadDelegate);
	}
	else if (Type == EPartsType::FACE)
	{
		//페이스 파츠
		FSoftObjectPath FaceAssetPath = nullptr;
		FaceAssetPath = CurGameInstance->GetXRAssetMgr()->FindResourceFromDataTable(PartResourceTable->ResourceID);
		FStreamableDelegate FaceAssetLoadDelegate;
		FaceAssetLoadDelegate = FStreamableDelegate::CreateUObject(this, &APlayerCharacter::LoadPartsComplete,
			FaceAssetPath, EPartsType::FACE);
		CurGameInstance->GetXRAssetMgr()->ASyncLoadAssetFromPath(FaceAssetPath, FaceAssetLoadDelegate);
	}
	else
	{
		FSoftObjectPath ETCAssetPath = nullptr;
		ETCAssetPath = CurGameInstance->GetXRAssetMgr()->FindResourceFromDataTable(PartResourceTable->ResourceID);
		FStreamableDelegate ETCAssetLoadDelegate;
		ETCAssetLoadDelegate = FStreamableDelegate::CreateUObject(this, &APlayerCharacter::LoadPartsComplete,
			ETCAssetPath, Type);

		CurGameInstance->GetXRAssetMgr()->ASyncLoadAssetFromPath(ETCAssetPath, ETCAssetLoadDelegate);

		if (Type == EPartsType::NUDEBODY)
			Equipments.BodyItem = nullptr;
		else if (Type == EPartsType::NUDEHAND)
			Equipments.HandsItem = nullptr;
		else if (Type == EPartsType::NUDELEG)
			Equipments.LegsItem = nullptr;

	}


}

void APlayerCharacter::ChangeEquipment(UItem* Item, USkeletalMesh* SkMesh)
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

	USkeletalMesh* LegMesh = Equipments.LegsComponent->SkeletalMesh;
	USkeletalMesh* HandMesh = Equipments.HandsComponent->SkeletalMesh;

	switch (Types)
	{
	case EEquipmentsType::BODY:
		Equipments.BodyItem = EquipItem;
		Equipments.BodyComponent->SetSkeletalMesh(SkMesh); 
		Equipments.LegsComponent->SetSkeletalMesh(nullptr);
		Equipments.LegsComponent->SetSkeletalMesh(LegMesh);
		Equipments.HandsComponent->SetSkeletalMesh(nullptr);
		Equipments.HandsComponent->SetSkeletalMesh(HandMesh);
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

void APlayerCharacter::PseudoChangeEquipmentWithoutMesh(UItem* Item)
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

	switch (Types)
	{
	case EEquipmentsType::BODY:
		Equipments.BodyItem = EquipItem;
		break;
	case EEquipmentsType::HANDS:
		Equipments.HandsItem = EquipItem;
		break;
	case EEquipmentsType::LEGS:
		Equipments.LegsItem = EquipItem;
		break;
	case EEquipmentsType::WEAPON:
		Equipments.WeaponItem = EquipItem;
		break;
	}
}


void APlayerCharacter::ChangeEquipment(UItem* Item, UStaticMesh* SmMesh)
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


	USkeletalMesh* LegMesh = Equipments.LegsComponent->SkeletalMesh;
	USkeletalMesh* HandMesh = Equipments.HandsComponent->SkeletalMesh;

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
		Equipments.BodyItem = nullptr;
		Equipments.BodyComponent->SetSkeletalMesh(LoadedMesh.Get());
		Equipments.LegsComponent->SetSkeletalMesh(nullptr);
		Equipments.LegsComponent->SetSkeletalMesh(LegMesh);
		Equipments.HandsComponent->SetSkeletalMesh(nullptr);
		Equipments.HandsComponent->SetSkeletalMesh(HandMesh);
	}
	else if (Type == EPartsType::NUDEHAND)
	{
		Equipments.HandsItem = nullptr;
		Equipments.HandsComponent->SetSkeletalMesh(LoadedMesh.Get());
	}
	else if (Type == EPartsType::NUDELEG)
	{
		Equipments.LegsItem = nullptr;
		Equipments.LegsComponent->SetSkeletalMesh(LoadedMesh.Get());
	}
}

float APlayerCharacter::TakeDamage(float Damage, FXRDamageEvent& DamageEvent, AController* EventInstigator, AActor* DamageCauser)
{
	if (GetbIsInvisible())
	{
		XRLOG(Warning, TEXT("Now Invisible, Hit Ignored."));
		return -1.0f;
	}
	
	XRLOG(Warning, TEXT("Player SetHP  : %f"), Damage);
	ANonePlayerCharacter* NPC = nullptr;
	if (DamageCauser != nullptr)
	{
		NPC = Cast<ANonePlayerCharacter>(DamageCauser);
	}

	if (NPC == nullptr)
		check(false);

	if (bIsSkillPlaying)
		return -1.0f;

	PlayerStatComp->SetCurrentHP(Damage);
	bIsHit = true;
	
	
	if (DamageEvent.bIntensity == 1)
	{
		SetbIsInvisible(true);  
		ForceSkillStop();
		ForceAttackStop();
		ForceRollStop();
		MyAnimInstance->PlayHitMontage();
		MyAnimInstance->Montage_JumpToSection(FName(TEXT("BigHit")));
		ComboCount = 1;
		bSavedCombo = false;
		SetbIsKnockBackMoving(true);

		
		GetCharacterMovement()->MaxWalkSpeed = kKnockBackSpeed;
		GetCharacterMovement()->MaxAcceleration = kMaxMovementAcceleration;
		
		FVector VecToTarget = NPC->GetActorLocation() - GetActorLocation();
		FRotator AgainstMonster = FRotator(0.0f, 0.0f, 0.0f);

		AgainstMonster = FRotator(0.0f, (FRotationMatrix::MakeFromY(VecToTarget).Rotator().Yaw), 0.0f);
		SetActorRotation(AgainstMonster);
		KnockBackVector = -VecToTarget;
	}
	else if (!MyAnimInstance->Montage_IsPlaying(MyAnimInstance->AttackMontage) && !bIsSkillPlaying)
	{
		SetbIsInvisible(true);
		ForceSkillStop();
		ForceAttackStop();
		ForceRollStop();
		MyAnimInstance->PlayHitMontage();
		MyAnimInstance->Montage_JumpToSection(FName(TEXT("SmallHit")));
		ComboCount = 1;
		bSavedCombo = false;
	}
	else //공격 중, 스킬 중...
	{
		bIsHit = false;
	}


	if (MyShake)
	{
		auto CameraShake = GetWorld()->GetFirstPlayerController()->PlayerCameraManager->PlayCameraShake(MyShake, 1.0f);
		Cast<UPlayerCameraShake>(CameraShake)->SetSmallShakeMode();
	}

	/*동작 취소 처리*/
	bIsSprint = false;
	bIsMove = false;
	Equipments.WeaponComponent->SetCollisionEnabled(ECollisionEnabled::NoCollision);

	UE_LOG(LogTemp, Warning, TEXT("Damaged By %s, Damage : %f"), *(NPC->GetName()), Damage);

	return 0.0f;
}

FVector2D APlayerCharacter::GetNormalCapsuleSize()
{
	return CapsuleSize;
}

FVector2D APlayerCharacter::GetRollingCapsuleSize()
{
	return RollingHitCapsuleSize;
}

void APlayerCharacter::Attack()
{	
	if (bIsOverallRollAnimPlaying|| bIsRolling || bIsHit || bIsSkillPlaying)
		return;

	if (GetCharacterLifeState() == ECharacterLifeState::DEAD)
		return;

	ForceKnockbackStop();
	ForceSkillStop();

	//first
	if (bIsAttack == false)
	{
		AttackNextRotation = GetActorRotation(); //공격 시작시에, 액터로케이션과 Next로테이션을 동일하게 맞춤

		bIsAttack = true;
		ForceKnockbackStop();
		ForceSkillStop();
		MyAnimInstance->PlayAttackMontage();

		OutputStream out;
		out.WriteOpcode(ENetworkCSOpcode::kCharacterAction);
		out << ComboCount;
		out << GetActorLocation();
		out << GetActorRotation();
		out.CompletePacketBuild();
		GetNetMgr().SendPacket(out);
		CurrentAttackID = ComboCount;

	}
	else
		bSavedCombo = true;

	AttackOverlapList.clear(); //Overlap list 초기화

}

void APlayerCharacter::Roll()
{
	if (GetCharacterLifeState() == ECharacterLifeState::DEAD)
		return;
	if (bIsOverallRollAnimPlaying || bIsSkillPlaying || bIsHit)
		return;

	//후딜레이 동작에서 구르는지 체크
	if (bIsAttack)
	{
		FName CurrentSectionName = MyAnimInstance->Montage_GetCurrentSection(MyAnimInstance->AttackMontage);
		FString SecStr = CurrentSectionName.ToString();

		for (int ii = 1; ii <= kMaxComboCount; ii++)
		{
			FString CompStr = "Combo" + FString::FromInt(ii);
			if (SecStr == CompStr)
				return;
		}

		bIsAttackMoving = false;
		bIsAttack = false;
	}

	ForceSkillStop();
	ForceKnockbackStop();
	ForceAttackStop();

	bool bArrowKeyNotPressed = false;

	//무식한건 아는데 당장 생각이 안남
	float Yaw = GetYawFromArrowKeys(ForwardValue, RightValue, bArrowKeyNotPressed);

	const FRotator CameraForward = FRotator(0.0f, CameraComponent->GetComponentRotation().Yaw, 0.0f);

	if (bArrowKeyNotPressed)
		this->SetActorRotation(FRotator(0.0f, Yaw, 0.0f));
	else
		this->SetActorRotation(CameraForward + FRotator(0.0f, Yaw, 0.0f));

	bIsRolling = true;
	bIsOverallRollAnimPlaying = true;
	MyAnimInstance->PlayRollMontage();
	SetRollingCapsuleMode();
	
	if (Cast<APlayerController>(GetController()))
	{
		OutputStream out;
		out.WriteOpcode(ENetworkCSOpcode::kRequestCharacterRolling);
		out << this->ObjectID;
		out << this->GetActorRotation();
		out.CompletePacketBuild();
		GetNetMgr().SendPacket(out);
	}
}

void APlayerCharacter::Sprint()
{
	if (bIsOverallRollAnimPlaying || bIsSkillPlaying || bIsHit)
		return;

	bIsSprint = true;
	GetCharacterMovement()->MaxWalkSpeed = kSprintMovementSpeed;

	if (Cast<APlayerController>(GetController()))
	{
		OutputStream out;
		out.WriteOpcode(ENetworkCSOpcode::kRequestCharacterSprint);
		out << this->ObjectID;
		out.CompletePacketBuild();
		GetNetMgr().SendPacket(out);
	}

}

void APlayerCharacter::SprintEnd()
{
	GetCharacterMovement()->MaxWalkSpeed = kNormalMovementSpeed;
	bIsSprint = false;
}

void APlayerCharacter::InitializeCharacter(bool bIsPlayerCharacter, CharacterData& Data)
{
	bInitialized = true;

	bIsPlayer = bIsPlayerCharacter;

	auto MyGameInstance = Cast<UXRGameInstance>(GetGameInstance());

	ObjectID = Data.ObjectID;
	PlayerStatComp->SetMaxHP(Data.Max_HP);
	PlayerStatComp->SetCurrentHP(Data.Current_HP);
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
	PlayerStatComp->SetCurrentStamina(Data.Current_Stamina);
	PlayerStatComp->SetMaxStamina(Data.Max_Stamina);
	PlayerStatComp->SetCharacterName(Data.Name.c_str());
	PlayerStatComp->SetMaxExp(Data.Max_Exp);
	PlayerStatComp->SetCurrentStamina(Data.Current_Stamina);

	if (PlayerStatComp->GetGender() == 0)
		this->bIsMale = true;
	else
		this->bIsMale = false;

	/*TEST CODE*/
	//bIsMale = true;

	if (bIsPlayerCharacter)
	{
		if (bIsMale)
			Equipments.BodyComponent->SetAnimInstanceClass(AnimInstance);
		else
			Equipments.BodyComponent->SetAnimInstanceClass(FemaleAnimInstance);

		MyAnimInstance = Cast<UPlayerCharacterAnimInstance>(Equipments.BodyComponent->GetAnimInstance());
		MyAnimInstance->InitializeMontage(bIsMale);
		MyAnimInstance->Delegate_CheckNextCombo.BindUFunction(this, FName("ContinueCombo"));
		MyAnimInstance->Delegate_CharacterAttackMoveStart.BindUFunction(this, FName("StartMoveAttack"));
		MyAnimInstance->Delegate_CharacterAttackMoveEnd.BindUFunction(this, FName("EndMoveAttack"));
		MyAnimInstance->OnMontageEnded.AddDynamic(this, &APlayerCharacter::OnMyMontageEnded);
		UHealthBarWidget::GetInatance()->SetMaxHp(PlayerStatComp->GetMaxHP());
	}
	else
	{
		if (bIsMale)
			Equipments.BodyComponent->SetAnimInstanceClass(RemoteAnimInstance);
		else
			Equipments.BodyComponent->SetAnimInstanceClass(FemaleRemoteAnimInstance);

		MyAnimInstance = Cast<UPlayerCharacterAnimInstance>(Equipments.BodyComponent->GetAnimInstance());
		MyAnimInstance->InitializeMontage(bIsMale);
	}
		

	if (bIsMale)
	{
		ChangePartsById(EPartsType::HAIR, 110);
		ChangePartsById(EPartsType::FACE, 120);
	}
	else
	{
		ChangePartsById(EPartsType::HAIR, 210);
		ChangePartsById(EPartsType::FACE, 220);
	}

	MyAnimInstance->SetOwnerCharacter(this);

	for (int ii = 0; ii < Data.kEquipmentArraySize; ii++)
	{
		/*맨몸일 때 */
		if (Data.EquipArray[ii].ID == -1)
		{

			const int32 kMalePrimaryBody = 130;
			const int32 kMalePrimaryHand = 140;
			const int32 kMalePrimaryLeg = 150;
			const int32 kFemalePrimaryBody = 230;
			const int32 kFemalePrimaryHand = 240;
			const int32 kFemalePrimaryLeg = 250;
			const int32 kPrimaryWeapon = 3300001;

			int32 CurrentPrimaryBody;
			int32 CurrentPrimaryHand;
			int32 CurrentPrimaryLeg;

			if (bIsMale)
			{
				CurrentPrimaryBody = kMalePrimaryBody;
				CurrentPrimaryHand = kMalePrimaryHand;
				CurrentPrimaryLeg = kMalePrimaryLeg;
			}
			else
			{
				CurrentPrimaryBody = kFemalePrimaryBody;
				CurrentPrimaryHand = kFemalePrimaryHand;
				CurrentPrimaryLeg = kFemalePrimaryLeg;
			}

			switch (ii)
			{
			case 0:
				ChangePartsById(EPartsType::NUDEBODY, CurrentPrimaryBody);
				break;
			case 1:
				ChangePartsById(EPartsType::NUDEHAND, CurrentPrimaryHand);
				break;
			case 2:
				ChangePartsById(EPartsType::NUDELEG, CurrentPrimaryLeg);
				break;
			case 3:
				MyGameInstance->ItemManager->BuildItem(EItemType::EQUIPMENT, kPrimaryWeapon,
					MyGameInstance->GetWorld(), this);
				break;
			}
		}
		else
			MyGameInstance->ItemManager->BuildItem(EItemType::EQUIPMENT, Data.EquipArray[ii].ID, GetWorld(), this);
	}


	UNickNameWidget* NickNameWidget = CreateWidget<UNickNameWidget>(GetWorld(), UNickNameWidget::StaticClass());
	if (NickNameWidget)
	{
		NickNameWidget->SettingName(this->PlayerStatComp->GetCharacterName());
		NickNameWidget->SettingLevel(this->PlayerStatComp->GetLevel());
		NickNameWidget->SettingTitle(TEXT("Title Sample"));
		NameTag->SetWidget(NickNameWidget);
	}
	NameTag->SetRelativeLocation(NameTagLocation);
}

void APlayerCharacter::TestInitialize()
{
	bInitialized = true;
	bIsPlayer = true;
	bIsTestMode = true;
	bIsMale = true;

	auto MyGameInstance = Cast<UXRGameInstance>(GetGameInstance());

	ObjectID = 1000;
	PlayerStatComp->SetMaxHP(100);
	PlayerStatComp->SetCurrentHP(100);
	PlayerStatComp->SetAttack_Min(50);
	PlayerStatComp->SetAttack_Max(100);
	PlayerStatComp->SetAttack_Range(10);
	PlayerStatComp->SetAttack_Speed(10);
	PlayerStatComp->SetSpeed(10);
	PlayerStatComp->SetDefence(10);
	PlayerStatComp->SetLevel(1);
	PlayerStatComp->SetGender(0);
	PlayerStatComp->SetSTR(10);
	PlayerStatComp->SetSTR(10);
	PlayerStatComp->SetDEX(11);
	PlayerStatComp->SetINT(12);
	PlayerStatComp->SetCurrentStamina(100);
	PlayerStatComp->SetMaxStamina(100);
	PlayerStatComp->SetMaxExp(500);
	PlayerStatComp->SetCurrentExp(0);


	UPlayerSkillManager* SkillManager = MyGameInstance->GetPlayerSkillManager();
	SkillManager->AddSkill(SkillManager->CreateSkillFromID(101), true);
	SkillManager->AddSkill(SkillManager->CreateSkillFromID(102), true);

	Equipments.BodyComponent->SetAnimInstanceClass(AnimInstance);
	MyAnimInstance = Cast<UPlayerCharacterAnimInstance>(Equipments.BodyComponent->GetAnimInstance());
	MyAnimInstance->InitializeMontage(true); //Male
	MyAnimInstance->Delegate_CheckNextCombo.BindUFunction(this, FName("ContinueCombo"));
	MyAnimInstance->Delegate_CharacterAttackMoveStart.BindUFunction(this, FName("StartMoveAttack"));
	MyAnimInstance->Delegate_CharacterAttackMoveEnd.BindUFunction(this, FName("EndMoveAttack"));
	MyAnimInstance->OnMontageEnded.AddDynamic(this, &APlayerCharacter::OnMyMontageEnded);
	//UBarWidget::GetInatance()->SetMaxHp(PlayerStatComp->GetMaxHP());

	MyAnimInstance->SetOwnerCharacter(this);

	const int32 kMalePrimaryBody = 130;
	const int32 kMalePrimaryHand = 140;
	const int32 kMalePrimaryLeg = 150;
	const int32 kMalePrimaryWeapon = 3300001;

	//ChangePartsById(EPartsType::NUDEBODY, kMalePrimaryBody);

	//ChangePartsById(EPartsType::NUDEHAND, kMalePrimaryHand);

	//ChangePartsById(EPartsType::NUDELEG, kMalePrimaryLeg);

	MyGameInstance->ItemManager->BuildItem(EItemType::EQUIPMENT, kMalePrimaryWeapon,
		MyGameInstance->GetWorld(), this);

	MyGameInstance->ItemManager->BuildItem(EItemType::EQUIPMENT, 3040001,
		MyGameInstance->GetWorld(), this);
	MyGameInstance->ItemManager->BuildItem(EItemType::EQUIPMENT, 3140001,
		MyGameInstance->GetWorld(), this);
	MyGameInstance->ItemManager->BuildItem(EItemType::EQUIPMENT, 3240001,
		MyGameInstance->GetWorld(), this);

	AXRPlayerController* MyPlayerController = Cast<AXRPlayerController>(GetWorld()->GetPlayerControllerIterator()->Get());
	if (MyPlayerController == nullptr) return;
	MyPlayerController->Possess(this);

}

void APlayerCharacter::OnMyMontageEnded(UAnimMontage* Montage, bool bInterrupted)
{

	if (MyAnimInstance->AttackMontage == Montage)
	{
		bIsAttack = false;
		bSavedCombo = false;
		ComboCount = 1;
		Equipments.WeaponComponent->SetCollisionEnabled(ECollisionEnabled::NoCollision);

		if(bIsSprint)
			GetCharacterMovement()->MaxWalkSpeed = kSprintMovementSpeed;

	}
	else if (MyAnimInstance->HitMontage == Montage)
	{
		bIsHit = false;

		if (bIsSprint)
			GetCharacterMovement()->MaxWalkSpeed = kSprintMovementSpeed;
	}
	else if (MyAnimInstance->RollMontage == Montage)
	{
		bIsOverallRollAnimPlaying = false;

		OutputStream out;
		out.WriteOpcode(ENetworkCSOpcode::kNotifyCurrentChrPosition);
		out << 999;
		out << GetActorLocation();
		out << GetActorRotation();
		out.CompletePacketBuild();
		GetNetMgr().SendPacket(out);
	}
}

void APlayerCharacter::ContinueCombo()
{
	bIsAttackMoving = false;
	GetCharacterMovement()->MaxWalkSpeed = kNormalMovementSpeed;

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


		bool bArrowKeyNotPressed = false;
		float Yaw = GetYawFromArrowKeys(ForwardValue, RightValue, bArrowKeyNotPressed);
		const FRotator CameraForward = FRotator(0.0f, CameraComponent->GetComponentRotation().Yaw, 0.0f);

		if (bArrowKeyNotPressed)
		{
			const FRotator Rot = FRotator(0.0f, Yaw, 0.0f);
			AttackNextRotation = Rot;
		}
		else
		{
			const FRotator Rot = CameraForward + FRotator(0.0f, Yaw, 0.0f);
			AttackNextRotation = Rot;
		}


		OutputStream out;
		out.WriteOpcode(ENetworkCSOpcode::kCharacterAction);
		out << ComboCount;
		out << GetActorLocation();
		out << GetActorRotation();
		out.CompletePacketBuild();
		GetNetMgr().SendPacket(out);
	}
}

void APlayerCharacter::StartMoveAttack()
{
	bIsAttackMoving = true;
	GetCharacterMovement()->MaxWalkSpeed = kAttackMovementSpeed;
	Equipments.WeaponComponent->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
}
void APlayerCharacter::EndMoveAttack()
{
	bIsAttackMoving = false;
	GetCharacterMovement()->MaxWalkSpeed = kNormalMovementSpeed;

	OutputStream out;
	out.WriteOpcode(ENetworkCSOpcode::kNotifyCurrentChrPosition);
	out << 999;
	out << GetActorLocation();
	out << GetActorRotation();
	out.CompletePacketBuild();
	GetNetMgr().SendPacket(out);

	Equipments.WeaponComponent->SetCollisionEnabled(ECollisionEnabled::NoCollision);
}

void APlayerCharacter::LoadPartsComplete(FSoftObjectPath AssetPath, EPartsType Type)
{
	this->ChangePartsComponentsMesh(Type, AssetPath);
}

void APlayerCharacter::OnDead()
{
	if (GetCharacterLifeState() == ECharacterLifeState::DEAD)
		return;

	SetCharacterLifeState(ECharacterLifeState::DEAD);
	bIsCharacterDead = true;

	UE_LOG(LogTemp, Warning, TEXT("Character Is Dead!"));
}

void APlayerCharacter::OnOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (Cast<APlayerController>(GetController()))
	{
		ANonePlayerCharacter* NPC = Cast<ANonePlayerCharacter>(OtherActor);
		if (NPC)
		{

			for (ANonePlayerCharacter* FlagNpc : AttackOverlapList)
			{
				if (FlagNpc->GetName() == NPC->GetName())
					return;
			}
			///수정자 조재진///
//UGameplayStatics::ApplyDamage(NPC, 10.f, GetController(), this, UDamageType::StaticClass());
			//NPC->TakeDamage(1.f, FDamageEvent(), GetController(), this);
			/// 오프라인 공격 테스트용도 지워도 무상관///////


			FCollisionQueryParams Params(NAME_None, false, this);
			TArray<FHitResult> HitResult;
			FVector SwordBoxSize = FVector(15.0f, 15.0f, 72.0f);

			GetWorld()->SweepMultiByChannel(HitResult, Equipments.WeaponComponent->GetSocketLocation("SwordStart"),
				Equipments.WeaponComponent->GetSocketLocation("SwordEnd"), FQuat::Identity, ECollisionChannel::ECC_GameTraceChannel2,
				FCollisionShape::MakeSphere(72.0f), Params);

			FVector MiniBox = FVector(3.f, 3.f, 3.f);

			for (FHitResult& Rst : HitResult)
			{
				UCapsuleComponent* CapComp = Cast<UCapsuleComponent>(Rst.GetComponent());

				if (CapComp)
				{
					FVector TraceVec = Rst.TraceEnd - Rst.TraceStart;
					FVector TestCenter = (Rst.TraceStart  +
						Rst.TraceEnd) / 2;
					FQuat CapsuleRot = FRotationMatrix::MakeFromZ(TraceVec).ToQuat();
					float HalfHeight = (Rst.TraceEnd - Rst.TraceStart).Size() / 2;

					FVector HitLocation = Rst.Location;
					DrawDebugBox(GetWorld(), Rst.TraceStart, MiniBox, FColor::Red, false, 3.0f);
					DrawDebugBox(GetWorld(), Rst.TraceEnd, MiniBox, FColor::Blue, false, 3.0f);
					DrawDebugBox(GetWorld(), HitLocation, MiniBox, FColor::Green, false, 3.0f);
					DrawDebugCapsule(GetWorld(), TestCenter, HalfHeight, 15.0f, CapsuleRot, FColor::Emerald, false, 5.0f);
					
					int32 CurrentEffectNum = GetComboCount() - 1;

					FRotator SlashRot = FRotator(90.0f, 90.0f, 90.0f);
					FRotator BloodRot = FRotator(-90.0f, 0.0f, 0.0f);

					UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), AttackEffectList[CurrentEffectNum]->Template, HitLocation,
						SlashRot, true);
					UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), BloodEffect->Template, HitLocation,
						BloodRot, true);

				}
			}

			int64 EnemyID = NPC->ObjectID;
			int64 MyID = this->ObjectID;

			OutputStream out;
			out.WriteOpcode(ENetworkCSOpcode::kCharcterHitSuccess);
			out << MyID;
			out << EnemyID;
			out << CurrentAttackID;
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

bool APlayerCharacter::GetIsTestMode()
{
	return bIsTestMode;
}

//구를때 모드 설정. 캡슐뿐아니라 이동속도도 관장함
void APlayerCharacter::SetRollingCapsuleMode()
{
	HitCapsule->SetCapsuleHalfHeight(RollingHitCapsuleSize.X);
	HitCapsule->SetCapsuleRadius(RollingHitCapsuleSize.Y);
	HitCapsule->SetWorldLocation(GetCapsuleComponent()->GetComponentLocation() + FVector(0.0f, 0.0f, -RollingCapsuleOffset));

	/* 속도 설정 */
	GetCharacterMovement()->MaxAcceleration = kMaxMovementAcceleration;
	GetCharacterMovement()->MaxWalkSpeed = kRollingMovementSpeed;

}

//구른 뒤에 모드 설정. 캡슐뿐아니라 이동속도도 관장함
void APlayerCharacter::SetNormalCapsuleMode()
{
	HitCapsule->SetCapsuleHalfHeight(GetNormalCapsuleSize().X);
	HitCapsule->SetCapsuleRadius(GetNormalCapsuleSize().Y);
	//HitCapsule->SetRelativeLocation(FVector(0.0f, 0.0f, RollingCapsuleOffset));
	HitCapsule->SetWorldLocation(GetCapsuleComponent()->GetComponentLocation());

	/*속도 설정*/
	GetCharacterMovement()->MaxAcceleration = kMaxMovementAcceleration;
	GetCharacterMovement()->MaxWalkSpeed = kNormalMovementSpeed;
}

float APlayerCharacter::GetYawFromArrowKeys(float ForwardValue, float RightValue, bool& Out_ArrowKeyPressed)
{
	float Yaw = 0.0f;
	if (FMath::IsNearlyEqual(ForwardValue, 0.0f) && FMath::IsNearlyEqual(RightValue, 0.0f))
	{
		Yaw = GetActorRotation().Yaw;
		Out_ArrowKeyPressed = true;
	}
	else if (ForwardValue > 0.0f && FMath::IsNearlyEqual(RightValue, 0.0f))
		Yaw = 0.0f;
	else if (ForwardValue > 0.0f && RightValue > 0.0f)
		Yaw = 45.0f;
	else if (FMath::IsNearlyEqual(ForwardValue, 0.0f) && RightValue > 0.0f)
		Yaw = 90.0f;
	else if (ForwardValue < 0.0f && RightValue > 0.0f)
		Yaw = 135.0f;
	else if (ForwardValue < 0.0f && FMath::IsNearlyEqual(RightValue, 0.0f))
		Yaw = 180.0f;  //-180?
	else if (ForwardValue < 0.0f && RightValue < 0.0f)
		Yaw = -135.0f;
	else if (FMath::IsNearlyEqual(ForwardValue, 0.0f) && RightValue < 0.0f)
		Yaw = -90.0f;
	else if (ForwardValue > 0.0f && RightValue < 0.0f)
		Yaw = -45.0f;

	return Yaw;
}

void APlayerCharacter::SetbIsSkillMove(bool b)
{
	bIsSkillMove = b;
}

void APlayerCharacter::SetbIsSkillPlaying(bool b)
{
	bIsSkillPlaying = b;
}

bool APlayerCharacter::GetbIsRolling()
{
	return bIsRolling;
}

bool APlayerCharacter::GetbIsOverallRollAnimPlaying()
{
	return bIsOverallRollAnimPlaying;
}

bool APlayerCharacter::GetbIsSkillMove()
{
	return bIsSkillMove;
}

bool APlayerCharacter::GetbIsSkillPlaying()
{
	return bIsSkillPlaying;
}

bool APlayerCharacter::GetbIsDead()
{
	return bIsCharacterDead;
}

void APlayerCharacter::TestPlay()
{
	//FString Fstr = "Berserk";
	//UPlayerSkill* Skill = CurGameInstance->GetPlayerSkillManager()->
	//	FindSkillFromListByName(CurGameInstance->GetPlayerSkillManager()->SkillListForPlalyer, Fstr);

	//Skill->Play(this);

	FRotator SlashRot = FRotator(90.0f, 90.0f, 90.0f);

	int32 CurrentEffectNum = GetComboCount() - 1;

	UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), BloodEffect->Template, GetActorLocation(),
		SlashRot, true);
	UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), AttackEffectList[CurrentEffectNum]->Template, GetActorLocation(),
		AttackEffectRot, true);
}

UItemEquipment* APlayerCharacter::GetEquippedItem(EEquipmentsType Type)
{
	switch (Type)
	{
	case EEquipmentsType::BODY:
		return Equipments.BodyItem;
		break;
	case EEquipmentsType::HANDS:
		return Equipments.HandsItem;
		break;
	case EEquipmentsType::LEGS:
		return Equipments.LegsItem;
		break;
	case EEquipmentsType::WEAPON:
		return Equipments.WeaponItem;
		break;
	}

	check(false);
	return nullptr;
}

void APlayerCharacter::SetEquippedItem(EEquipmentsType Type, UItemEquipment* Item)
{
	const int32 kMalePrimaryBody = 130;
	const int32 kMalePrimaryHand = 140;
	const int32 kMalePrimaryLeg = 150;
	const int32 kMalePrimaryWeapon = 3300001;
	auto GameInstance = Cast<UXRGameInstance>(GetGameInstance());

	if (Item == nullptr)
	{
		if (Type == EEquipmentsType::BODY)
			ChangePartsById(EPartsType::NUDEBODY, kMalePrimaryBody);
		else if (Type == EEquipmentsType::HANDS)
			ChangePartsById(EPartsType::NUDEHAND, kMalePrimaryHand);
		else if (Type == EEquipmentsType::LEGS)
			ChangePartsById(EPartsType::NUDELEG, kMalePrimaryLeg);
		else if (Type == EEquipmentsType::WEAPON)
		{
			GameInstance->ItemManager->BuildItem(EItemType::EQUIPMENT, kMalePrimaryWeapon,
				GetWorld(), this);
		}
	}
	else
		GameInstance->ItemManager->BuildItem(Item, GetWorld(), this);

}

void APlayerCharacter::ToggleMouseCursor()
{
	int32 SizeX = 0;
	int32 SizeY = 0;
	GetWorld()->GetFirstPlayerController()->GetViewportSize(SizeX, SizeY);

	if (bIsMouseShow)
	{
		bIsMouseShow = false;
		GetWorld()->GetFirstPlayerController()->SetInputMode(FInputModeGameOnly());
		GetWorld()->GetFirstPlayerController()->bShowMouseCursor = false;
	}
	else
	{
		bIsMouseShow = true;
		GetWorld()->GetFirstPlayerController()->SetMouseLocation(SizeX / 2, SizeY / 2);
		GetWorld()->GetFirstPlayerController()->SetInputMode(FInputModeGameAndUI());
		GetWorld()->GetFirstPlayerController()->bShowMouseCursor = true;
	}

}

void APlayerCharacter::WheelUp()
{
	XRLOG(Warning, TEXT("WheelUp"));
	float len = FMath::Max(SpringArmComponent->TargetArmLength - kCameraWheelSpeed, static_cast<float>(kCameraWheelMinLimit));
	SpringArmComponent->TargetArmLength = len;
}

void APlayerCharacter::WheelDown()
{
	XRLOG(Warning, TEXT("WheelDown"));
	float len = FMath::Min(SpringArmComponent->TargetArmLength + kCameraWheelSpeed, static_cast<float>(kCameraWheelMaxLimit));
	SpringArmComponent->TargetArmLength = len;
}


void APlayerCharacter::SetbIsAttack(bool b)
{
	bIsAttack = b;
}

void APlayerCharacter::SetbIsInvisible(bool b)
{
	bIsInvisible = b;
}

void APlayerCharacter::SetbSavedCombo(bool b)
{
	bSavedCombo = b;
}

bool APlayerCharacter::GetbIsAttack()
{
	return bIsAttack;
}

bool APlayerCharacter::GetbSavedCombo()
{
	return bSavedCombo;
}

void APlayerCharacter::SetComboCount(int32 NextCount)
{
	ComboCount = NextCount;
}

void APlayerCharacter::SetKnockBackVector(FVector & Vec)
{
	KnockBackVector = Vec;
}

void APlayerCharacter::ForceAttackStop()
{
	bIsAttack = false;
	bIsAttackMoving = false;
	MyAnimInstance->Montage_Stop(0.1f, MyAnimInstance->AttackMontage);
	GetCharacterMovement()->MaxWalkSpeed = kNormalMovementSpeed;
}

void APlayerCharacter::ForceRollStop()
{
	bIsRolling = false;
	bIsOverallRollAnimPlaying = false;
	MyAnimInstance->Montage_Stop(0.1f, MyAnimInstance->RollMontage);
	GetCharacterMovement()->MaxWalkSpeed = kNormalMovementSpeed;
}

void APlayerCharacter::ForceSkillStop()
{
	bIsSkillMove = false;
	bIsSkillPlaying = false;
	MyAnimInstance->Montage_Stop(0.1f, MyAnimInstance->SkillMontage);
	GetCharacterMovement()->MaxWalkSpeed = kNormalMovementSpeed;
}

void APlayerCharacter::ForceKnockbackStop()
{
	bIsKnockBackMoving = false;
	MyAnimInstance->Montage_Stop(0.1f, MyAnimInstance->HitMontage);
}

void APlayerCharacter::LockCharacterMove(bool Lock)
{
	bIsMoveLocked = Lock;
}

void APlayerCharacter::AddLocationSyncFailCount()
{
	LocationSyncFailCount++;
}

void APlayerCharacter::SetbIsPathFinding(bool bPathFd)
{
	bIsPathFinding = bPathFd;
}

int32 APlayerCharacter::GetComboCount()
{
	return ComboCount;
}

int32 APlayerCharacter::GetLocationSyncFailCount()
{
	return LocationSyncFailCount;
}

bool APlayerCharacter::GetbIsPathFinding()
{
	return bIsPathFinding;
}

UParticleSystemComponent * APlayerCharacter::GetParticleComponentByName(FString FindStr)
{
	for (UParticleSystemComponent* Comp : ParticleArray)
	{
		if (Comp->GetName() == FindStr)
		{
			return Comp;
		}
	}

	check(false);
	return nullptr;
}

FComboSocket APlayerCharacter::GetComboSocket()
{
	return ComboParticleSocketName;
}

void APlayerCharacter::SetbIsKnockBackMoving(bool b)
{
	bIsKnockBackMoving = b;
}

bool APlayerCharacter::GetbIsKnockBackMoving()
{
	return bIsKnockBackMoving;
}

bool APlayerCharacter::GetbIsInvisible()
{
	return bIsInvisible;
}
