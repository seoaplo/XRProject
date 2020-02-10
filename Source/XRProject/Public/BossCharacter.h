// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "XRProject.h"
#include "NonePlayerCharacter.h"
#include "BossCharacter.generated.h"

USTRUCT(Atomic, BlueprintType)
struct FBossSkill
{
	GENERATED_BODY()
public:
	UPROPERTY(BlueprintReadWrite,EditAnywhere)
		UAnimMontage* AttackAction;
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
		float AttackRange;
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
		float CoolTime;
};

UENUM(BlueprintType)
enum class EBossState : uint8
{
	Spawn,
	Idle,
	InBattle,
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
		TArray<FBossSkill> BossAttackList;
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
		TArray<int> ReadySkillList;

	UFUNCTION(BlueprintCallable)
		void SetOnSkillQueue(int32 index);


	UFUNCTION(BlueprintNativeEvent)
		void FireSkill();
	void FireSkill_Implementation();




	UPROPERTY(BlueprintReadWrite, EditAnywhere)
		int32 BossPhase;
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
		int32 CurrentBossPhase;

	UFUNCTION(BlueprintCallable)
		void SetCurrentBossPhase(int32 phase);

protected:
private:


	void SetCharacterLoadState(ECharacterLoadState NewState) override;

};
