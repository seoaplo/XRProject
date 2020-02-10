// Fill out your copyright notice in the Description page of Project Settings.
/*
	�ۼ��� : �ڼ���
*/
#pragma once

#include "Engine.h"
#include "BaseCharacter.h"
#include "ItemEquipment.h"
#include "PlayerCharacterAnimInstance.h"
#include "PlayerCharacterStatComponent.h"
#include "Perception/AIPerceptionStimuliSourceComponent.h"
#include "UserWidget.h"
#include "HealthBarWidget.h"
#include "XRPlayerController.h"
#include "PlayerCameraShake.h"
#include "PlayerCharacter.generated.h"

class ANonePlayerCharacter;

UENUM()
enum class EEquipmentsType : uint8
{
	BODY,
	HANDS,
	LEGS,
	WEAPON,
};
UENUM()
enum class EPartsType : uint8
{
	HAIR,
	FACE,
	NUDEBODY,
	NUDEHAND,
	NUDELEG,
};

USTRUCT(BlueprintType)
struct FEquipment
{
	GENERATED_USTRUCT_BODY()
public:
	/*  */
	//UPROPERTY(Category = Character, VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	UPROPERTY(EditInstanceOnly, BlueprintReadWrite, Category = "C_Equipment")
		class USkeletalMeshComponent* BodyComponent;
	UPROPERTY(EditInstanceOnly, BlueprintReadWrite, Category = "C_Equipment")
		class USkeletalMeshComponent* HandsComponent;
	UPROPERTY(EditInstanceOnly, BlueprintReadWrite, Category = "C_Equipment")
		class USkeletalMeshComponent* LegsComponent;
	UPROPERTY(EditInstanceOnly, BlueprintReadWrite, Category = "C_Equipment")
		class UStaticMeshComponent* WeaponComponent;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "C_EquipmentItem")
		UItemEquipment* BodyItem;
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "C_EquipmentItem")
		UItemEquipment* HandsItem;
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "C_EquipmentItem")
		UItemEquipment* LegsItem;
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "C_EquipmentItem")
		UItemEquipment* WeaponItem;

};


/**
 *
 */
UCLASS()
class XRPROJECT_API APlayerCharacter : public ABaseCharacter
{
	GENERATED_BODY()

public:
	APlayerCharacter();
	virtual ~APlayerCharacter();

	const int32 kMaxComboCount = 4;
	const float kSprintMovementSpeed = 750.0f;
	const float kNormalMovementSpeed = 450.0f;

public:
	UPROPERTY(EditInstanceOnly, Category = "Variable")
		float RotateSpeed;
	UPROPERTY(EditInstanceOnly, Category = "Variable")
		float SpringArmLength;
	UPROPERTY(EditInstanceOnly, Category = "Variable")
		float MovementSpeed;
	UPROPERTY()
		bool bIsMale; //���� üũ�� ���� bool��.


	UPROPERTY(EditAnywhere)
		FEquipment Equipments;
	UPROPERTY(EditInstanceOnly, Category = "C_Collision")
		class UCapsuleComponent* HitCapsule;
	UPROPERTY(EditInstanceOnly, Category = "C_Camera")
		class UWidgetComponent* NameTag;
	UPROPERTY(EditInstanceOnly, Category = "C_Camera")
		class UCameraComponent* CameraComponent;
	UPROPERTY(EditInstanceOnly, Category = "C_Camera")
		class USpringArmComponent* SpringArmComponent;
	UPROPERTY(EditInstanceOnly, BlueprintReadWrite, Category = "C_Parts")
		class USkeletalMeshComponent* FaceComponent;
	UPROPERTY(EditInstanceOnly, BlueprintReadWrite, Category = "C_Parts")
		class USkeletalMeshComponent* HairComponent;
	UPROPERTY(EditInstanceOnly, BlueprintReadWrite, Category = "C_AnimInstance")
		TSubclassOf<UAnimInstance> AnimInstance;
	UPROPERTY(EditInstanceOnly, BlueprintReadWrite, Category = "C_AnimInstance")
		TSubclassOf<UAnimInstance> FemaleAnimInstance;
	UPROPERTY(EditInstanceOnly, BlueprintReadWrite, Category = "C_AnimInstance")
		TSubclassOf<UAnimInstance> RemoteAnimInstance;
	UPROPERTY(EditInstanceOnly, BlueprintReadWrite, Category = "C_AnimInstance")
		TSubclassOf<UAnimInstance> FemaleRemoteAnimInstance;
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "C_AnimInstance")
		UPlayerCharacterAnimInstance* MyAnimInstance;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera)
		float BaseTurnRate;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera)
		float BaseLookUpRate;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "C_CharacterStatus")
		UPlayerCharacterStatComponent* PlayerStatComp;

	UPROPERTY(EditInstanceOnly, BlueprintReadWrite, Category = "C_TEST")
		FVector ScaleVector;
	UPROPERTY(EditInstanceOnly, BlueprintReadWrite, Category = "C_COLLISION")
		FVector2D CapsuleSize;
	UPROPERTY(EditInstanceOnly, BlueprintReadWrite, Category = "C_COLLLISION")
		FVector2D RollingHitCapsuleSize;
	UPROPERTY(EditInstanceOnly, BlueprintReadWrite, Category = "C_VALUE")
		float RollingCapsuleOffset;
	UPROPERTY(EditInstanceOnly, BlueprintReadWrite, Category = "C_LOCATION")
		FVector MeshLocationVector;
	UPROPERTY(EditInstanceOnly, BlueprintReadWrite, Category = "C_LOCATION")
		FVector WeaponScaleVector;
	UPROPERTY(EditInstanceOnly, BlueprintReadWrite, Category = "C_LOCATION")
		FVector NameTagLocation;
	UPROPERTY(EditAnywhere)
		TSubclassOf<UCameraShake> MyShake;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "C_CharacterStatus")
		UAIPerceptionStimuliSourceComponent* PlayerAIPerceptionStimul;
	UPROPERTY(EditInstanceOnly, BlueprintReadOnly, Category = "C_TEST")
		FCameraShakeInfo ShakeInfo;

public:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "C_Character", Meta = (AllowPrivateAccess = true))
		bool bIsAttack;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "C_Character", Meta = (AllowPrivateAccess = true))
		/*순수하게 구르는 동작만 수행하는가? : 구르기 시 액터 이동을 체크하기 위해 사용*/
		bool bIsRolling;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "C_Character", Meta = (AllowPrivateAccess = true))
		/*구르기 전체 동작을 수행중인가? true : false*/
		bool bIsOverallRollAnimPlaying;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "C_Character", Meta = (AllowPrivateAccess = true))
		bool bIsSprint;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "C_Character", Meta = (AllowPrivateAccess = true))
		bool bSavedCombo;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "C_Character", Meta = (AllowPrivateAccess = true))
		int32 ComboCount;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "C_Character", Meta = (AllowPrivateAccess = true))
		bool bIsMove;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "C_Character", Meta = (AllowPrivateAccess = true))
		bool bIsCharacterDead;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "C_Character", Meta = (AllowPrivateAccess = true))
		bool bIsHit;

	/*TEST*/
private:
	FRotator DeltaRotation;
	FRotator AdditionalRotationValue;
	FVector SpringArmLocation;
	bool bForwardKeyIsNeutral;
	std::vector<ANonePlayerCharacter*> AttackOverlapList;
	int32 CurrentComboCount;
	bool bIsPlayer;
	bool bInitialized;
	bool bIsTestMode;
	bool bIsMouseShow;
	int32 CurrentAttackID;
	float ForwardValue; //앞 방향키를 누르고 있는가(-1~1)
	float RightValue;  // 오른쪽 방향키를 누르고 있는가?(-1~1)
	float SumSec = 0;


public:
	virtual void Tick(float Deltatime) override;
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	virtual void PostInitializeComponents() override;
	virtual void BeginPlay() override;
	virtual void PossessedBy(AController* controller) override;


public:
	void InitializeCharacter(bool bIsPlayerCharacter, CharacterData& Data);
	void ChangePartsById(EPartsType Type, int32 ID);
	void ChangeEquipment(UItem* Item, USkeletalMesh* SkMesh);
	void ChangeEquipment(UItem* Item, UStaticMesh* SmMesh);
	void PseudoChangeEquipmentWithoutMesh(UItem * Item);//아이템 메시를 포함하지 않고, 정보만 일단 업데이트하는 함수
	void ChangePartsComponentsMesh(EPartsType Type, FSoftObjectPath PartAsset);
	void SetIsPlayer(bool is);
	bool GetIsPlayer();
	bool GetIsTestMode();
	void SetRollingCapsuleMode(); //구를때 모드 설정. 캡슐뿐아니라 이동속도도 관장함
	void SetNormalCapsuleMode(); //구른 뒤에 모드 설정. 캡슐뿐아니라 이동속도도 관장함

	bool GetbIsRolling();
	bool GetbIsOverallRollAnimPlaying();

	void TestPlay();

	UItemEquipment* GetEquippedItem(EEquipmentsType Type);
	void SetEquippedItem(EEquipmentsType Type, UItemEquipment* Item);

	float TakeDamage(float Damage, struct FDamageEvent const& DamageEvent, class AController* EventInstigator,
		class AActor* DamageCauser) override;

	FVector2D GetNormalCapsuleSize();
	FVector2D GetRollingCapsuleSize();
	
	UFUNCTION()
		void OnOverlapBegin(class UPrimitiveComponent* OverlappedComp, class AActor* OtherActor,
			class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
	UFUNCTION()
		void OnMyMontageEnded(UAnimMontage* Montage, bool bInterrupted);
	UFUNCTION()
		void ContinueCombo();
	UFUNCTION()
		void LoadPartsComplete(FSoftObjectPath AssetPath, EPartsType Type);
	UFUNCTION(BlueprintCallable, Category ="C_TEST")
		void TestInitialize();

		virtual	void OnDead() override;

private:
	void MoveForward(float Value);
	void MoveRight(float Value);
	void TurnAtRate(float Rate);
	void LookUpAtRate(float Rate);
	void Attack();
	void Roll();
	void Sprint();
	void SprintEnd();
	void ToggleMouseCursor();

};

