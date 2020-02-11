// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerSkill.h"
#include "PlayerCharacter.h"

UPlayerSkill::UPlayerSkill()
{


}

UPlayerSkill::~UPlayerSkill()
{

}

void UPlayerSkill::Play(APlayerCharacter* Character)
{
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
	UPlayerCharacterAnimInstance* MyAnimInst = Character->MyAnimInstance;
	if (!MyAnimInst)
		check(false);

	FString GaiaStr = "GaiaCrush";

	MyAnimInst->PlaySkillMontage();
	MyAnimInst->JumpToSkillMonatgeSection(GaiaStr);
}

bool USkill_GaiaCrush::ConditionCheck(APlayerCharacter * Character)
{
	//Stamina Check
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
