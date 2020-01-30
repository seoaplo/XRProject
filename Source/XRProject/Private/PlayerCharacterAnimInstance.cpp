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
	Super::NativeUpdateAnimation(DeltaSeconds);
	MyCharacter = AccountManager::GetInstance().GetCurrentPlayerCharacter();

	if (IsValid(MyCharacter))
	{
		//CharacterSpeed = MyCharacter->GetVelocity().Size();
		bIsMove = MyCharacter->bIsMove;
	}

}

void UPlayerCharacterAnimInstance::AnimNotify_CheckNextComboValid()
{
	if(Delegate_CheckNextCombo.IsBound())
		Delegate_CheckNextCombo.Execute();

}

void UPlayerCharacterAnimInstance::PlayAttackMontage()
{
	Montage_Play(AttackMontage, 1.f);
}

void UPlayerCharacterAnimInstance::StopAttackMontage()
{
}

void UPlayerCharacterAnimInstance::JumpToComboMontageSection(int32 Section)
{
	check((Section <= 4));
	Montage_JumpToSection(FName(*FString::Printf(TEXT("combo%d"), Section)));
}

void UPlayerCharacterAnimInstance::JumpToReloadMontageSection(int32 Section)
{
	check((Section <= 4));
	Montage_JumpToSection(FName(*FString::Printf(TEXT("combo%dR"), Section)));
}
