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
		TSubclassOf<UAnimInstance> RemoteAnimInstance;
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
	UPROPERTY(EditInstanceOnly, BlueprintReadWrite, Category = "C_TEST")
		FVector2D CapsuleSize;
	UPROPERTY(EditInstanceOnly, BlueprintReadWrite, Category = "C_TEST")
		FVector MeshLocationVector;
	UPROPERTY(EditInstanceOnly, BlueprintReadWrite, Category = "C_TEST")
		FVector WeaponScaleVector;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "C_CharacterStatus")
		UAIPerceptionStimuliSourceComponent* PlayerAIPerceptionStimul;


public:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "C_Character", Meta = (AllowPrivateAccess = true))
		bool bIsAttack;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "C_Character", Meta = (AllowPrivateAccess = true))
		bool bIsRolling;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "C_Character", Meta = (AllowPrivateAccess = true))
		bool bIsSprint;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "C_Character", Meta = (AllowPrivateAccess = true))
		bool bSavedCombo; //�޺� �������̸�, �����޺��� ������ �� ����
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
public:
	virtual void Tick(float Deltatime) override;
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	virtual void PostInitializeComponents() override;
	virtual void BeginPlay() override;
	virtual void PossessedBy(AController* controller) override;

public:
	void MoveForward(float Value);
	void MoveRight(float Value);
	void TurnAtRate(float Rate);
	void LookUpAtRate(float Rate);
	void Attack();
	void Roll();
	void Sprint();
	void SprintEnd();
	void InitializeCharacter(bool bIsPlayerCharacter, CharacterData& Data);

	void ChangePartsById(EPartsType Type, int32 ID);
	void ChangeEquipment(UItem* Item, USkeletalMesh* SkMesh);
	void ChangeEquipment(UItem* Item, UStaticMesh* SmMesh);
	void ChangePartsComponentsMesh(EPartsType Type, FSoftObjectPath PartAsset); //���, ���̽� �� ���� ��ȯ
	
	void SetIsPlayer(bool is);
	bool GetIsPlayer();

	float TakeDamage(float Damage, struct FDamageEvent const& DamageEvent, class AController* EventInstigator,
		class AActor* DamageCauser) override;
	
	UFUNCTION()
		void OnOverlapBegin(class UPrimitiveComponent* OverlappedComp, class AActor* OtherActor,
			class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
	UFUNCTION()
		void OnMyMontageEnded(UAnimMontage* Montage, bool bInterrupted);
	UFUNCTION()
		void ContinueCombo();
	UFUNCTION()
		void LoadPartsComplete(FSoftObjectPath AssetPath, EPartsType Type);

	float SumSec = 0;
};
