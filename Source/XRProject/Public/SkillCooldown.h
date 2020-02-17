// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "EngineMinimal.h"
#include "UObject/NoExportTypes.h"
#include "SkillCooldown.generated.h"

class UXRGameInstance;

const float kCoolDownDecreaseFrequency = 1.0f;

/**
 * 
 */
UCLASS()
class XRPROJECT_API USkillCooldown : public UObject
{
	GENERATED_BODY()

public:
	USkillCooldown();
	virtual ~USkillCooldown();
	UXRGameInstance* GI;

private:
	UPROPERTY()
	FTimerHandle TimerHandle;
	UPROPERTY()
	int32 SkillID;

	float SkillCoolTime; //½ºÅ³ max ÄðÅ¸ÀÓ
	float CurrentCoolTime;
	float CoolTimeDecreaseSpeed;
	bool IsEnable;

public:
	UFUNCTION()
		void SetCD(int32 TargetSkillID, float CoolTime, UXRGameInstance* GI);
	UFUNCTION()
		void RepeatingFunc();
	UFUNCTION()
		void SetTimer();

	int32 GetID();
	bool GetIsEnable();
	float GetRemainCoolTime();
};
