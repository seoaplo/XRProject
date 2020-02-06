// Fill out your copyright notice in the Description page of Project Settings.


#include "BossCharacter.h"

ABossCharacter::ABossCharacter()
{

	bUseControllerRotationYaw = false;
	GetCharacterMovement()->bOrientRotationToMovement = true;
}