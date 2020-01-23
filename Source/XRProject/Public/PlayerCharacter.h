// Fill out your copyright notice in the Description page of Project Settings.
/*
	작성자 : 박수찬
*/
#pragma once

#include "EngineMinimal.h"
#include "UserCharacter.h"
#include "ItemEquipment.h"
#include "ItemWeapon.h"
#include "PlayerCharacter.generated.h"

UENUM()
enum class EEquipmentsType : uint8
{
	HAIR,
	FACE,
	BODY,
	HANDS,
	LEGS,
	WEAPON,
	SUBWEAPON,
};

USTRUCT(BlueprintType)
struct FEquipment
{
	GENERATED_USTRUCT_BODY()
public:
	/*  */
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "C_Equipment")
		USkeletalMeshComponent* FaceComponent;
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "C_Equipment")
		USkeletalMeshComponent* HairComponent;
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "C_Equipment")
		USkeletalMeshComponent* BodyComponent;
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "C_Equipment")
		USkeletalMeshComponent* HandsComponent;
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "C_Equipment")
		USkeletalMeshComponent* LegsComponent;
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "C_Equipment")
		USkeletalMeshComponent* WeaponComponent;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "C_EquipmentItem")
		UItemEquipment* FaceItem;
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "C_EquipmentItem")
		UItemEquipment* HairItem;
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "C_EquipmentItem")
		UItemEquipment* BodyItem;
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "C_EquipmentItem")
		UItemEquipment* HandsItem;
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "C_EquipmentItem")
		UItemEquipment* LegsItem;
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "C_EquipmentItem")
		UItemWeapon* WeaponItem;

};


/**
 * 
 */
UCLASS()
class XRPROJECT_API APlayerCharacter : public AUserCharacter
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
		bool IsMale; //성별 체크를 위한 bool값.
	UPROPERTY()
		FEquipment Equipments;
	UPROPERTY(EditInstanceOnly, Category = "C_Camera")
		class UCameraComponent* CameraComponent;
	UPROPERTY(EditInstanceOnly, Category = "C_Camera")
		class USpringArmComponent* SpringArmComponent;

	
private:
	/*TEST*/
	FRotator DeltaRotation;
	FRotator AdditionalRotationValue;
	FVector SpringArmLocation;
	bool bForwardKeyIsNeutral;
public:
	virtual void Tick(float deltatime) override;
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

public:
	void MoveForward(float Value);
	void MoveRight(float Value);
	
	UFUNCTION()
		void ChangeEquipment(EEquipmentsType Types, UItem* Item);
};
