// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerCharacterAnimInstance.h"
#include "AccountManager.h"
#include "XRGameInstance.h"
#include "PlayerCharacter.h"

UPlayerCharacterAnimInstance::UPlayerCharacterAnimInstance()
{
	CharacterSpeed = 0.0f;
	bIsAttack = false;
	bIsRolling = false;
	bIsSprint = false;
	bIsCharacterDead = false;
	bIsHit = false;
	bIsOverallRollAnimPlaying = false;
	FemaleMontageList.Reserve(5);

	static ConstructorHelpers::FObjectFinder<UAnimMontage>
		ATTACK_MONTAGE(TEXT("AnimMontage'/Game/Resources/Character/PlayerCharacter/Animation/AttackMontage.AttackMontage'"));
	if (ATTACK_MONTAGE.Succeeded())
	{
		AttackMontage = ATTACK_MONTAGE.Object;
	}

	static ConstructorHelpers::FObjectFinder<UAnimMontage>
		ATTACK_MONTAGE_ONLYPLAY(TEXT("AnimMontage'/Game/Resources/Character/PlayerCharacter/Animation/AttackMontageOnlyPlay.AttackMontageOnlyPlay'"));
	if (ATTACK_MONTAGE_ONLYPLAY.Succeeded())
	{
		AttackMontageOnlyPlay = ATTACK_MONTAGE_ONLYPLAY.Object;
	}	

	static ConstructorHelpers::FObjectFinder<UAnimMontage>
		MOVE_MONTAGE_ONLYPLAY(TEXT("AnimMontage'/Game/Resources/Character/PlayerCharacter/Animation/MoveMontageOnlyPlay.MoveMontageOnlyPlay'"));
	if (MOVE_MONTAGE_ONLYPLAY.Succeeded())
	{
		MoveMontageOnlyPlay = MOVE_MONTAGE_ONLYPLAY.Object;
	}

	static ConstructorHelpers::FObjectFinder<UAnimMontage>
		HIT_MONTAGE(TEXT("AnimMontage'/Game/Resources/Character/PlayerCharacter/Animation/HitMontage.HitMontage'"));
	if (HIT_MONTAGE.Succeeded())
		HitMontage = HIT_MONTAGE.Object;

	static ConstructorHelpers::FObjectFinder<UAnimMontage>
		ROLL_MONTAGE(TEXT("AnimMontage'/Game/Resources/Character/PlayerCharacter/Animation/RollMontage.RollMontage'"));
	if (ROLL_MONTAGE.Succeeded())
		RollMontage = ROLL_MONTAGE.Object;


	static ConstructorHelpers::FObjectFinder<UAnimMontage>
		SKILL_MONTAGE(TEXT("AnimMontage'/Game/Resources/Character/PlayerCharacter/Animation/SkillMontage.SkillMontage'"));
	if (SKILL_MONTAGE.Succeeded())
		SkillMontage = SKILL_MONTAGE.Object;

}

UPlayerCharacterAnimInstance::~UPlayerCharacterAnimInstance()
{

}

void UPlayerCharacterAnimInstance::SetOwnerCharacter(APlayerCharacter * MyCharacter)
{
	this->MyCharacter = MyCharacter;
}

void UPlayerCharacterAnimInstance::NativeUpdateAnimation(float DeltaSeconds)
{
	Super::NativeUpdateAnimation(DeltaSeconds);
	
	if (MyCharacter)
	{
		bIsMove = MyCharacter->bIsMove;
		bIsRolling = MyCharacter->bIsRolling;
		bIsSprint = MyCharacter->bIsSprint;
		bIsCharacterDead = MyCharacter->bIsCharacterDead;
		bIsHit = MyCharacter->bIsHit;
		bIsOverallRollAnimPlaying = MyCharacter->bIsOverallRollAnimPlaying;
	}

}

void UPlayerCharacterAnimInstance::AnimNotify_CheckNextComboValid()
{
	if (Delegate_CheckNextCombo.IsBound())
		Delegate_CheckNextCombo.Execute();
}
void UPlayerCharacterAnimInstance::AnimNotify_AttackStart()
{
	if (Delegate_CharacterAttackMoveStart.IsBound())
		Delegate_CharacterAttackMoveStart.Execute();
}
void UPlayerCharacterAnimInstance::AnimNotify_AttackEnd()
{
	if (Delegate_CharacterAttackMoveEnd.IsBound())
		Delegate_CharacterAttackMoveEnd.Execute();

	if (MyCharacter->bIsSprint)
		MyCharacter->GetCharacterMovement()->MaxWalkSpeed = kSprintMovementSpeed;
}
void UPlayerCharacterAnimInstance::AnimNotify_RollingEnd()
{
	//구르는 동작의 끝(애니메이션의 끝이 아님)
	if (IsValid(MyCharacter))
	{
		MyCharacter->bIsRolling = false;
		bIsRolling = false;
		MyCharacter->SetNormalCapsuleMode();

		if(MyCharacter->bIsSprint)
			MyCharacter->GetCharacterMovement()->MaxWalkSpeed = kSprintMovementSpeed;

	}
}
void UPlayerCharacterAnimInstance::AnimNotify_HitMotionEnd()
{
	MyCharacter->bIsHit = false;
	bIsHit = false;
}

void UPlayerCharacterAnimInstance::AnimNotify_RemoteRollingEnd()
{
	MyCharacter->bIsRolling = false; 
	bIsRolling = false;

	MyCharacter->GetCharacterMovement()->MaxAcceleration = kNormalMovementAcceleration;
	MyCharacter->GetCharacterMovement()->MaxWalkSpeed = kNormalMovementSpeed;
}

void UPlayerCharacterAnimInstance::AnimNotify_RemoteRollingAllEnd()
{
	bIsOverallRollAnimPlaying = false;
	MyCharacter->bIsOverallRollAnimPlaying = false;
}
void UPlayerCharacterAnimInstance::PlayAttackMontage()
{
	float ret = Montage_Play(AttackMontage, 1.f);
	UE_LOG(LogTemp, Warning, TEXT("%f"), ret);
}

void UPlayerCharacterAnimInstance::PlayAttackOnlyPlayMontage()
{
	float ret = Montage_Play(AttackMontageOnlyPlay, 1.f);
}

void UPlayerCharacterAnimInstance::PlayMoveOnlyPlayMontage()
{
	float ret = Montage_Play(MoveMontageOnlyPlay, 1.f);
}

void UPlayerCharacterAnimInstance::PlayHitMontage()
{
	float ret = Montage_Play(HitMontage, 1.f);
	UE_LOG(LogTemp, Warning, TEXT("%f"), ret);
}

void UPlayerCharacterAnimInstance::PlayRollMontage()
{
	float ret = Montage_Play(RollMontage, 1.f);
	UE_LOG(LogTemp, Warning, TEXT("%f"), ret);
}

void UPlayerCharacterAnimInstance::StopAttackMontage()
{
	Montage_Stop(0.0f, AttackMontage);
}

void UPlayerCharacterAnimInstance::JumpToComboMontageSection(int32 Section)
{
	check((Section <= 4));
	Montage_JumpToSection(FName(*FString::Printf(TEXT("combo%d"), Section)));
}

void UPlayerCharacterAnimInstance::JumpToMoveMontageSection(FString Section)
{
	Montage_JumpToSection(FName(*Section));
}

void UPlayerCharacterAnimInstance::PlaySkillMontage()
{
	float ret = Montage_Play(SkillMontage, 1.f);
}

void UPlayerCharacterAnimInstance::JumpToSkillMonatgeSection(FString Section)
{
	Montage_JumpToSection(FName(*Section));
}

void UPlayerCharacterAnimInstance::AnimNotify_GaiaHit()
{
	if (IsValid(MyCharacter))
	{
		if (Delegate_GaiaCrushEnd.IsBound())
			Delegate_GaiaCrushEnd.Execute(MyCharacter);
	}
	else
		check(false);
	MyCharacter->SetbIsSkillMove(false);
}

void UPlayerCharacterAnimInstance::AnimNotify_GaiaEnd()
{
	if (IsValid(MyCharacter))
	{
		if (Delegate_GaiaCrushProcessEnd.IsBound())
			Delegate_GaiaCrushProcessEnd.Execute(MyCharacter);
	}
	else
		check(false);
	MyCharacter->SetbIsSkillPlaying(false);
}

void UPlayerCharacterAnimInstance::AnimNotify_BerserkEnd()
{
	if (IsValid(MyCharacter))
	{
		if (Delegate_BerserkEnd.IsBound())
			Delegate_BerserkEnd.Execute(MyCharacter);
	}
	else
		check(false);

	MyCharacter->SetbIsSkillPlaying(false);
}