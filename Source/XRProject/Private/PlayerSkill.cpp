// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerSkill.h"
#include "PlayerCharacter.h"
#include "XRGameInstance.h"

UPlayerSkill::UPlayerSkill()
{

}

UPlayerSkill::~UPlayerSkill()
{

}



void UPlayerSkill::Play(APlayerCharacter* Character)
{
}

bool UPlayerSkill::End(APlayerCharacter* Character)
{
	return false;
}


bool UPlayerSkill::ConditionCheck(APlayerCharacter * Character)
{
	return false;
}

USkill_GaiaCrush::USkill_GaiaCrush()
{
	
}

USkill_GaiaCrush::~USkill_GaiaCrush()
{

}

void USkill_GaiaCrush::Play(APlayerCharacter* Character)
{
	if (!Character->MyAnimInstance->Delegate_GaiaCrushEnd.IsBound())
		Character->MyAnimInstance->Delegate_GaiaCrushEnd.BindUFunction(this, FName("End"));

	UPlayerCharacterAnimInstance* MyAnimInst = Character->MyAnimInstance;
	if (!MyAnimInst)
		check(false);
	
	if (ConditionCheck(Character))
		Character->PlayerStatComp->SubtractStamina(GetRequireStamina());
	else
		return;
	
	FString GaiaStr = "GaiaCrush";
	int32 Idx = MyAnimInst->SkillMontage->GetSectionIndex(FName(*GaiaStr));
	float length = MyAnimInst->SkillMontage->GetSectionLength(Idx);

	Character->GetCharacterMovement()->MaxWalkSpeed = MoveDistance / length;
	Character->GetCharacterMovement()->MaxAcceleration = kMaxMovementAcceleration;
	Character->SetbIsSkillMove(true);

	MyAnimInst->PlaySkillMontage();
	MyAnimInst->JumpToSkillMonatgeSection(GaiaStr);
}

bool USkill_GaiaCrush::End(APlayerCharacter* Character)
{
	

	Character->SetbIsSkillMove(false);
	return true;
}

bool USkill_GaiaCrush::ConditionCheck(APlayerCharacter * Character)
{
	if (Character->PlayerStatComp->GetCurrentStamina() >= GetRequireStamina())
	{ 
		return true;
	}
	return false;
}

void USkill_GaiaCrush::SetMoveDistance(float MoveDistance)
{
	this->MoveDistance = MoveDistance;
}

void USkill_GaiaCrush::SetAffectRadius(float AffectRadius)
{
	this->AffectRadius = AffectRadius;
}

float USkill_GaiaCrush::GetMoveDistance()
{
	return MoveDistance;
}

float USkill_GaiaCrush::GetAffectRadius()
{
	return AffectRadius;
}
