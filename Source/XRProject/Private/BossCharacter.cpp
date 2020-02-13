// Fill out your copyright notice in the Description page of Project Settings.


#include "BossCharacter.h"
#include "XRAIController.h"
#include "Navigation/PathFollowingComponent.h"

ABossCharacter::ABossCharacter()
{
	bUseControllerRotationYaw = false;
	GetCharacterMovement()->bOrientRotationToMovement = true;
}

void ABossCharacter::SetOnSkillQueue(int32 index)
{
	FTimerHandle handle;
	GetWorld()->GetTimerManager().SetTimer(handle, FTimerDelegate::CreateLambda(
		[this,index]()
	{
		ReadySkillList.AddUnique(index);
	}
	), BossAttackList[index].CoolTime, false);
}

void ABossCharacter::FireSkill_Implementation()
{
}

void ABossCharacter::SetCurrentBossPhase(int32 phase)
{
	CurrentBossPhase = phase;
	ReadySkillList.Reset();
	


}

void ABossCharacter::SetCharacterLoadState(ECharacterLoadState NewState)
{
	Super::SetCharacterLoadState(NewState);
	switch (CurrentLoadState)
	{
	case ECharacterLoadState::PREINIT:
	{
		break;
	}
	case ECharacterLoadState::LOADING:
	{
		break;
	}
	case ECharacterLoadState::READY:
	{
		NpcAnim->OnSkillFire.AddDynamic(this, &ABossCharacter::FireSkill);
		break;
	}
	default:
		break;
	}
}