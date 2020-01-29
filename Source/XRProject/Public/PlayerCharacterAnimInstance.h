// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AccountManager.h"
#include "PlayerCharacter.h"
#include "Animation/AnimInstance.h"
#include "PlayerCharacterAnimInstance.generated.h"

/**
 * 
 */
UCLASS()
class XRPROJECT_API UPlayerCharacterAnimInstance : public UAnimInstance
{
	GENERATED_BODY()

public:
	UPlayerCharacterAnimInstance();
	virtual ~UPlayerCharacterAnimInstance();
private:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "C_PlayerAnim", Meta = (AllowPrivateAccess = true))
		float CharacterSpeed;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "C_PlayerAnim", Meta = (AllowPrivateAccess = true))
		bool bIsAttack;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "C_PlayerAnim", Meta = (AllowPrivateAccess = true))
		APlayerCharacter* MyCharacter;

public:
	virtual void NativeUpdateAnimation(float DeltaSeconds) override;

};
