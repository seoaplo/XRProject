// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "XRProject.h"
#include "Animation/AnimInstance.h"

#include "NonePlayerCharacterAnimInstance.generated.h"

/*
//작성자 조재진
*/
UCLASS()
class XRPROJECT_API UNonePlayerCharacterAnimInstance : public UAnimInstance
{
	GENERATED_BODY()



public:
	virtual void NativeUpdateAnimation(float DeltaSeconds) override;
protected:
private:


	//사용자 정의//

public:

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Meta = (AllowPrivateAccess = true))
		float CurrentSpeed;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Meta = (AllowPrivateAccess = true))
		bool bInBattle;



	UPROPERTY(BlueprintReadWrite, EditAnywhere, Meta = (AllowPrivateAccess = true))
		TArray<UAnimMontage*> NpcAttackMontage;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Meta = (AllowPrivateAccess = true))
		UAnimMontage* NpcTakeDamageMontage;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Meta = (AllowPrivateAccess = true))
		UAnimMontage* NpcDeadMontage;
	UFUNCTION(BlueprintCallable)
		UAnimMontage* GetDeadMontage() { return NpcDeadMontage; }
	
	UFUNCTION()
		void AnimNotify_AttackColliOn();

	UFUNCTION()
		void AnimNotify_AttackColliOff();


protected:

private:



};
