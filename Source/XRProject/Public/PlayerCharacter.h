// Fill out your copyright notice in the Description page of Project Settings.
/*
	작성자 : 박수찬
*/
#pragma once

#include "EngineMinimal.h"
#include "BaseCharacter.h"
#include "ItemEquipment.h"
#include "ItemWeapon.h"
#include "Engine/BlueprintGeneratedClass.h"
#include "PlayerCharacterAnimInstance.h"
#include "PlayerCharacterStatComponent.h"
#include "PlayerCharacter.generated.h"




UENUM()
enum class EEquipmentsType : uint8
{
	BODY,
	HANDS,
	LEGS,
	WEAPON,
	SUBWEAPON,
};
UENUM()
enum class EPartsType : uint8
{
	HAIR,
	FACE,
};

USTRUCT(BlueprintType)
struct FEquipment
{
	GENERATED_USTRUCT_BODY()
public:
	/*  */

	UPROPERTY(Category = Character, VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
		class USkeletalMeshComponent* BodyComponent;
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "C_Equipment")
		class USkeletalMeshComponent* HandsComponent;
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "C_Equipment")
		class USkeletalMeshComponent* LegsComponent;
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "C_Equipment")
		class USkeletalMeshComponent* WeaponComponent;
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "C_Equipment")
		class USkeletalMeshComponent* SubWeaponComponent;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "C_EquipmentItem")
		UItemEquipment* BodyItem;
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "C_EquipmentItem")
		UItemEquipment* HandsItem;
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "C_EquipmentItem")
		UItemEquipment* LegsItem;
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "C_EquipmentItem")
		UItemWeapon* WeaponItem;
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "C_EquipmentItem")
		UItemWeapon* SubWeaponItem;

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
public:
	UPROPERTY(EditInstanceOnly, Category = "Variable")
		float RotateSpeed;
	UPROPERTY(EditInstanceOnly, Category = "Variable")
		float SpringArmLength;
	UPROPERTY(EditInstanceOnly, Category = "Variable")
		float MovementSpeed;
	UPROPERTY()
		bool bIsMale; //성별 체크를 위한 bool값.

	UPROPERTY(EditAnywhere)
		FEquipment Equipments;
	UPROPERTY(EditInstanceOnly, Category = "C_Camera")
		class UCameraComponent* CameraComponent;
	UPROPERTY(EditInstanceOnly, Category = "C_Camera")
		class USpringArmComponent* SpringArmComponent;
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "C_Parts")
		class USkeletalMeshComponent* FaceComponent;
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "C_Parts")
		class USkeletalMeshComponent* HairComponent;
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "C_AnimInstance")
		TSubclassOf<UAnimInstance> AnimInstance;
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "C_AnimInstance")
		UPlayerCharacterAnimInstance* MyAnimInstance;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera)
		float BaseTurnRate;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera)
		float BaseLookUpRate;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "C_CharacterStatus")
		UPlayerCharacterStatComponent* PlayerStatComp;

public:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "C_Character")
		bool bIsAttack;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "C_Character")
		bool bSavedCombo; //콤보 진행중이며, 다음콤보를 진행할 수 있음
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "C_Character")
		int32 ComboCount;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "C_Character")
		bool bIsMove;
	/*TEST*/
private:
	FRotator DeltaRotation;
	FRotator AdditionalRotationValue;
	FVector SpringArmLocation;
	bool bForwardKeyIsNeutral;
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

	

	UFUNCTION()
		void ChangePartsById(EPartsType Type, int32 ID);
	UFUNCTION()
		void ChangeEquipment(UItem* Item, USkeletalMesh* SkMesh);
	UFUNCTION()
		void ChangePartsComponentsMesh(EPartsType Type, USkeletalMesh* PartsMesh); //헤어, 페이스 등 파츠 교환
	UFUNCTION()
		void Attack();
	UFUNCTION()
		void OnAttackMontageEnded(UAnimMontage* Montage, bool bInterrupted);
	UFUNCTION()
		void ContinueCombo();
	UFUNCTION()
		void LoadPartsComplete(FSoftObjectPath AssetPath, EPartsType Type);
};
