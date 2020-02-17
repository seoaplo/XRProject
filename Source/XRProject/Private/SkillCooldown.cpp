// Fill out your copyright notice in the Description page of Project Settings.


#include "SkillCooldown.h"
#include "XRGameInstance.h"

USkillCooldown::USkillCooldown()
{
	SkillID = -1;
	CurrentCoolTime = 0.0f;
	IsEnable = true;
	CoolTimeDecreaseSpeed = 1.0f;
}

USkillCooldown::~USkillCooldown()
{

}

void USkillCooldown::SetCD(int32 TargetSkillID, float CoolTime, UXRGameInstance* GI)
{
	this->SkillID = TargetSkillID;
	SkillCoolTime = CoolTime;
	this->GI = GI;
	CurrentCoolTime = CoolTime;
}

void USkillCooldown::SetTimer()
{
	GI->GetTimerManager().SetTimer(TimerHandle, this, &USkillCooldown::RepeatingFunc, kCoolDownDecreaseFrequency, true);
	IsEnable = false;
}

void USkillCooldown::RepeatingFunc()
{
	CurrentCoolTime -= CoolTimeDecreaseSpeed;
	//ÄðÅ¸ÀÓ Äü½½·Ô UIÀü¼Û
	if (CurrentCoolTime <= 0.0f)
	{
		GI->GetTimerManager().ClearTimer(TimerHandle);
		CurrentCoolTime = SkillCoolTime;
		IsEnable = true;
	}
}

int32 USkillCooldown::GetID()
{
	return SkillID;
}

bool USkillCooldown::GetIsEnable()
{
	return IsEnable;
}

float USkillCooldown::GetRemainCoolTime()
{
	if (!IsEnable)
		return CurrentCoolTime;
	else
		return 0.0f;
}