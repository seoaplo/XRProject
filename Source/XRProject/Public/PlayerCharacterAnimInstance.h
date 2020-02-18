// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "PlayerCharacterAnimInstance.generated.h"

class APlayerCharacter;


DECLARE_DELEGATE(FCheckNextComboValid)
DECLARE_DELEGATE(FCheckCharacterAttackMoveStart)
DECLARE_DELEGATE(FCheckCharacterAttackMoveEnd)
DECLARE_DELEGATE_OneParam(FGaiaCrushEnd, APlayerCharacter*)
DECLARE_DELEGATE_OneParam(FGaiaCrushProcessEnd, APlayerCharacter*)
DECLARE_DELEGATE_OneParam(FBerserkEnd, APlayerCharacter*)



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
	FCheckCharacterAttackMoveStart Delegate_CharacterAttackMoveStart;
	FCheckCharacterAttackMoveEnd Delegate_CharacterAttackMoveEnd;
	FGaiaCrushEnd			Delegate_GaiaCrushEnd;
	FGaiaCrushProcessEnd	Delegate_GaiaCrushProcessEnd;
	FBerserkEnd				Delegate_BerserkEnd;
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
	UPROPERTY(VisibleDefaultsOnly, BlueprintReadonly, Category = "C_Montage", Meta = (AllowPrivateAccess = true))
		UAnimMontage* SkillMontage;


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
		void AnimNotify_AttackStart();
	UFUNCTION()
		void AnimNotify_AttackEnd();
	UFUNCTION()
		void AnimNotify_RollingEnd();
	UFUNCTION()
		void AnimNotify_HitMotionEnd();
	UFUNCTION()
		void AnimNotify_RemoteRollingEnd();
	UFUNCTION()
		void AnimNotify_RemoteRollingAllEnd();
	UFUNCTION()
		void AnimNotify_GaiaHit();
	UFUNCTION()
		void AnimNotify_GaiaEnd();
	UFUNCTION()
		void AnimNotify_BerserkEnd();
	UFUNCTION()
		void AnimNotify_NormalTrail();
	UFUNCTION()
		void AnimNotify_FinalTrail();
	UFUNCTION()
		void AnimNotify_BerserkEffon();

	void PlayAttackMontage();
	void PlaySkillMontage();

	void PlayAttackOnlyPlayMontage();
	void PlayMoveOnlyPlayMontage();
	void StopAttackMontage();
	void PlayHitMontage();
	void PlayRollMontage();
	void SetOwnerCharacter(APlayerCharacter* MyCharacter);

	void JumpToComboMontageSection(int32 Section);
	void JumpToMoveMontageSection(FString Section);
	void JumpToSkillMonatgeSection(FString Section);

};
