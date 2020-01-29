// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerCharacterAnimInstance.h"

UPlayerCharacterAnimInstance::UPlayerCharacterAnimInstance()
{
	CharacterSpeed = 0.0f;
	bIsAttack = false;
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
