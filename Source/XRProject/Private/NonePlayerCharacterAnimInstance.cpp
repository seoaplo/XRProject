// Fill out your copyright notice in the Description page of Project Settings.


#include "NonePlayerCharacterAnimInstance.h"
#include "NonePlayerCharacter.h"

void UNonePlayerCharacterAnimInstance::AnimNotify_AttackColliOn()
{
	XRLOG(Warning, TEXT("AttackColliOn"));
	auto npcChar = Cast<ANonePlayerCharacter>(TryGetPawnOwner());
	npcChar->GetMesh()->SetGenerateOverlapEvents(true);
}

void UNonePlayerCharacterAnimInstance::AnimNotify_AttackColliOff()
{
	XRLOG(Warning, TEXT("AttackColliOff"));
	auto npcChar = Cast<ANonePlayerCharacter>(TryGetPawnOwner());
	npcChar->GetMesh()->SetGenerateOverlapEvents(false);
}
