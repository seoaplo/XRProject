// Fill out your copyright notice in the Description page of Project Settings.
/*
	작성자 : 박수찬
*/
#pragma once

#include "EngineMinimal.h"
#include "BaseCharacter.h"
#include "ItemEquipment.h"
#include "PlayerCharacterStatComponent.h"
#include "PlayerCharacter.generated.h"

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
};

USTRUCT(BlueprintType)
struct FEquipment
{
	GENERATED_USTRUCT_BODY()
public:
	/*  */

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "C_Equipment")
		USkeletalMeshComponent* BodyComponent;
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "C_Equipment")
		USkeletalMeshComponent* HandsComponent;
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "C_Equipment")
		USkeletalMeshComponent* LegsComponent;
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "C_Equipment")
		USkeletalMeshComponent* WeaponComponent;

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
		USkeletalMeshComponent* FaceComponent;
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "C_Parts")
		USkeletalMeshComponent* HairComponent;

public:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera)
		float BaseTurnRate;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera)
		float BaseLookUpRate;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "C_CharacterStatus")
		UPlayerCharacterStatComponent* PlayerStatComp;

private:
	/*TEST*/
	FRotator DeltaRotation;
	FRotator AdditionalRotationValue;
	FVector SpringArmLocation;
	bool bForwardKeyIsNeutral;
public:
	virtual void Tick(float Deltatime) override;
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	virtual void BeginPlay() override;
	virtual void PossessedBy(AController* controller) override;



public:
	void MoveForward(float Value);
	void MoveRight(float Value);
	void TurnAtRate(float Rate);
	void LookUpAtRate(float Rate);
	

	UFUNCTION()
		void ChangeEquipment(UItem* Item, USkeletalMesh* SkMesh);
	UFUNCTION()
		void ChangePartsComponentsMesh(EPartsType Type, USkeletalMesh* PartsMesh); //헤어, 페이스 등 파츠 교환
};
