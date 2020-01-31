// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerCharacterAnimInstance.h"
#include "AccountManager.h"
#include "PlayerCharacter.h"

UPlayerCharacterAnimInstance::UPlayerCharacterAnimInstance()
{
	CharacterSpeed = 0.0f;
	bIsAttack = false;


	static ConstructorHelpers::FObjectFinder<UAnimMontage> 
		ATTACK_MONTAGE(TEXT("AnimMontage'/Game/Resources/Character/PlayerCharacter/Animation/AttackMontage.AttackMontage'"));
	if (ATTACK_MONTAGE.Succeeded())
	{
		AttackMontage = ATTACK_MONTAGE.Object;
	}
}

UPlayerCharacterAnimInstance::~UPlayerCharacterAnimInstance()
{

}

void UPlayerCharacterAnimInstance::NativeUpdateAnimation(float DeltaSeconds)
{
	MyCharacter = AccountManager::GetInstance().GetCurrentPlayerCharacter();

	if (MyCharacter)
	{
		CharacterSpeed = MyCharacter->GetVelocity().Size();
	}

}

void UPlayerCharacterAnimInstance::AnimNotify_CheckNextComboValid()
{
	Delegate_CheckNextCombo.Broadcast();

}

void UPlayerCharacterAnimInstance::PlayAttackMontage()
{
	Montage_Play(AttackMontage, 1.f);
}

void UPlayerCharacterAnimInstance::JumpToComboMontageSection(int32 Section)
{
	check((Section <= 4));
	Montage_JumpToSection(FName(*FString::Printf(TEXT("Combo%d"), Section)));
}

void UPlayerCharacterAnimInstance::JumpToReloadMontageSection(int32 Section)
{
	check((Section <= 4));
	Montage_JumpToSection(FName(*FString::Printf(TEXT("Combo%dR"), Section)));
}
