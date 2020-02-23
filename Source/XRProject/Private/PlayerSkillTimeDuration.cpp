// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerSkillTimeDuration.h"
#include "XRGameInstance.h"

UPlayerSkillTimeDuration::UPlayerSkillTimeDuration()
{
	SkillID = -1;
	CurrentRemainTime = 0.0f;
	bIsRemainingTime = false;
}

UPlayerSkillTimeDuration::~UPlayerSkillTimeDuration()
{

}

void UPlayerSkillTimeDuration::SetTD(int32 TargetSkillID, float TimeDuration, UXRGameInstance * GI)
{
	this->SkillID = TargetSkillID;
	this->SkillRemainTime = TimeDuration;
	this->GI = GI;
	this->CurrentRemainTime = TimeDuration;
}

void UPlayerSkillTimeDuration::RepeatingFunc()
{
	this->CurrentRemainTime -= kRemainTimeDecreaseFrequency;

	if (CurrentRemainTime <= 0.0f)
	{
		GI->GetTimerManager().ClearTimer(TimerHandle);
		CurrentRemainTime = SkillRemainTime;
		bIsRemainingTime = false;
	}
}

void UPlayerSkillTimeDuration::SetTimer()
{
	GI->GetTimerManager().SetTimer(TimerHandle, this, &UPlayerSkillTimeDuration::RepeatingFunc, kRemainTimeDecreaseFrequency, true);
	bIsRemainingTime = true;
}

int32 UPlayerSkillTimeDuration::GetID()
{
	return SkillID;
}

bool UPlayerSkillTimeDuration::GetbSkillRemainTime()
{
	return bIsRemainingTime;
}

float UPlayerSkillTimeDuration::GetRemainTime()
{
	if (bIsRemainingTime)
		return CurrentRemainTime;
	else
		return 0.0f;
}
