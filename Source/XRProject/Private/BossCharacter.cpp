// Fill out your copyright notice in the Description page of Project Settings.


#include "BossCharacter.h"
#include "XRAIController.h"
#include "Navigation/PathFollowingComponent.h"
#include "NonePlayerCharacterAnimInstance.h"
#include "XRGameInstance.h"
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

void ABossCharacter::ExcuteRecvNpcAction(InputStream& input)
{

	auto ingameMode = Cast<UXRGameInstance>(GetGameInstance());
	if (ingameMode)
	{
		if (!ingameMode->GetIsSuper())
		{
			int32 ActionID = input.ReadInt32();
			FVector Location = input.ReadFVector();
			FRotator Rotator = input.ReadFRotator();

			//XRLOG(Warning, TEXT("Recv MonsterAction : (ObjectID : %d)(ActionID : %d)(Location : %s)"), ObjectID, ActionID, *Location.ToString());
			if (ActionID < 1000)
			{

				GEngine->AddOnScreenDebugMessage(99, 5, FColor::Red, FString::FromInt(ActionID));
				AttackOverlapList.Reset();
				//AICon->StopMovement();
				SetActorLocation(Location);

				SetActorRotation(Rotator);
				auto npcAnim = Cast<UNonePlayerCharacterAnimInstance>(GetMesh()->GetAnimInstance());
				if (npcAnim)
				{
					npcAnim->Montage_Play(npcAnim->NpcAttackMontage[ActionID]);
				}
			}
			else if (ActionID == 1000)
			{
				AICon->MoveToLocation(Location, 2, false, false);
				AICon->SetControlRotation(Rotator);
			}
			else if (ActionID == 2000)
			{
				if (FMath::IsNearlyZero(Location.X))
				{
					bInBattle = false;
				}
				else
				{
					bInBattle = true;
				}
			}
			else if (ActionID >= 3000 && ActionID < 4000)
			{
				GEngine->AddOnScreenDebugMessage(99, 5, FColor::Red, FString::FromInt(ActionID));
				AttackOverlapList.Reset();
				int32 SkillID = ActionID % 3000;
				SetActorLocation(Location);
				SetActorRotation(Rotator);
				auto npcAnim = Cast<UNonePlayerCharacterAnimInstance>(GetMesh()->GetAnimInstance());
				if (npcAnim)
				{
					npcAnim->Montage_Play(BossAttackList[SkillID].AttackAction);
				}
			}
			else if (ActionID >= 4000)
			{
				GEngine->AddOnScreenDebugMessage(99, 5, FColor::Red, FString::FromInt(ActionID));
				FTransform spawnTransform;
				spawnTransform.SetLocation(Location);
				spawnTransform.SetRotation(Rotator.Quaternion());
				RemoteSpawnActorQue.Add(spawnTransform);
			}
		}
	}

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