// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "XRProject.h"
#include "NonePlayerCharacter.h"
#include "BossCharacter.generated.h"

USTRUCT(Atomic, BlueprintType)
struct FBossAttack
{
	GENERATED_BODY()
public:
	UPROPERTY(EditAnywhere)
		UAnimMontage* AttackAction;
	UPROPERTY(EditAnywhere)
		float AttackRange;
	UPROPERTY(EditAnywhere)
		float AttackAngle;

};

/**
 * 작성자 조재진
 */
UCLASS()
class XRPROJECT_API ABossCharacter : public ANonePlayerCharacter
{
	GENERATED_BODY()


public:
	ABossCharacter();
protected:
private:




	//사용자 정의///
public:
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
		TArray<FBossAttack> BossAttackList;

protected:
private:


};
