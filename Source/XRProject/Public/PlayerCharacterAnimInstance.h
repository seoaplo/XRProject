// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "PlayerCharacterAnimInstance.generated.h"

class APlayerCharacter;


DECLARE_DELEGATE(FCheckNextComboValid)

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

public:
	FCheckNextComboValid Delegate_CheckNextCombo;

public:
	UPROPERTY(VisibleDefaultsOnly, BlueprintReadonly, Category = "C_Montage", Meta = (AllowPrivateAccess = true))
		UAnimMontage* AttackMontage;
	UPROPERTY(VisibleDefaultsOnly, BlueprintReadonly, Category = "C_Montage", Meta = (AllowPrivateAccess = true))
		UAnimMontage* AttackMontageOnlyPlay;
	UPROPERTY(VisibleDefaultsOnly, BlueprintReadonly, Category = "C_Montage", Meta = (AllowPrivateAccess = true))
		UAnimMontage* MoveMontageOnlyPlay;
	UPROPERTY(VisibleDefaultsOnly, BlueprintReadonly, Category = "C_Montage", Meta = (AllowPrivateAccess = true))
		UAnimMontage* HitMontage;
	UPROPERTY(VisibleDefaultsOnly, BlueprintReadonly, Category = "C_Montage", Meta = (AllowPrivateAccess = true))
		UAnimMontage* RollMontage;


private:
	UPROPERTY(EditInstanceOnly, BlueprintReadOnly, Category = "C_PlayerAnim", Meta = (AllowPrivateAccess = true))
		float CharacterSpeed;
	UPROPERTY(EditInstanceOnly, BlueprintReadOnly, Category = "C_PlayerAnim", Meta = (AllowPrivateAccess = true))
		bool bIsAttack;
	UPROPERTY(EditInstanceOnly, BlueprintReadOnly, Category = "C_PlayerAnim", Meta = (AllowPrivateAccess = true))
		bool bIsMove;
	UPROPERTY(EditInstanceOnly, BlueprintReadOnly, Category = "C_PlayerAnim", Meta = (AllowPrivateAccess = true))
		bool bIsRolling;
	UPROPERTY(EditInstanceOnly, BlueprintReadOnly, Category = "C_PlayerAnim", Meta = (AllowPrivateAccess = true))
		bool bIsOverallRollAnimPlaying;
	UPROPERTY(EditInstanceOnly, BlueprintReadOnly, Category = "C_PlayerAnim", Meta = (AllowPrivateAccess = true))
		bool bIsSprint;
	UPROPERTY(EditInstanceOnly, BlueprintReadOnly, Category = "C_PlayerAnim", Meta = (AllowPrivateAccess = true))
		bool bIsCharacterDead;
	UPROPERTY(EditInstanceOnly, BlueprintReadWrite, Category = "C_PlayerAnim", Meta = (AllowPrivateAccess = true))
		bool bIsHit;
	UPROPERTY(VisibleDefaultsOnly, BlueprintReadonly, Category = "C_Montage", Meta = (AllowPrivateAccess = true))
		TArray<UAnimMontage*> FemaleMontageList;

	APlayerCharacter* MyCharacter;


public:
	virtual void NativeUpdateAnimation(float DeltaSeconds) override;

public:
	UFUNCTION()
		void AnimNotify_CheckNextComboValid();
	UFUNCTION()
		void AnimNotify_RollingEnd();
	UFUNCTION()
		void AnimNotify_HitMotionEnd();
	UFUNCTION()
		void AnimNotify_RemoteRollingEnd();
	UFUNCTION()
		void AnimNotify_RemoteRollingAllEnd();

	void PlayAttackMontage();
	void PlayAttackOnlyPlayMontage();
	void PlayMoveOnlyPlayMontage();
	void StopAttackMontage();
	void PlayHitMontage();
	void PlayRollMontage();
	void SetOwnerCharacter(APlayerCharacter* MyCharacter);

	void JumpToComboMontageSection(int32 Section);
	void JumpToMoveMontageSection(FString Section);


};
