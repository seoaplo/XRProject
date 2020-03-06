// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerCharacterAnimInstance.h"
#include "AccountManager.h"
#include "XRGameInstance.h"
#include "PlayerSkill.h"
#include "playerSkillTimeDuration.h"
#include "PlayerCharacter.h"

UPlayerCharacterAnimInstance::UPlayerCharacterAnimInstance()
{
	CharacterSpeed = 0.0f;
	bIsAttack = false;
	bIsRolling = false;
	bIsSprint = false;
	bIsCharacterDead = false;
	bIsHit = false;
	bIsOverallRollAnimPlaying = false;
	FemaleMontageList.Reserve(7);
	MaleMontageList.Reserve(7);

	static ConstructorHelpers::FObjectFinder<UAnimMontage>
		ATTACK_MONTAGE(TEXT("AnimMontage'/Game/Resources/Character/PlayerCharacter/Animation/AttackMontage.AttackMontage'"));
	static ConstructorHelpers::FObjectFinder<UAnimMontage>
		FEMALE_ATTACK_MONTAGE(TEXT("AnimMontage'/Game/Resources/Character/PlayerCharacter/Animation/Female/FemaleAttackMontage.FemaleAttackMontage'"));
	MaleMontageList.Add(ATTACK_MONTAGE.Object);
	FemaleMontageList.Add(FEMALE_ATTACK_MONTAGE.Object);


	static ConstructorHelpers::FObjectFinder<UAnimMontage>
		ATTACK_MONTAGE_ONLYPLAY(TEXT("AnimMontage'/Game/Resources/Character/PlayerCharacter/Animation/AttackMontageOnlyPlay.AttackMontageOnlyPlay'"));
	static ConstructorHelpers::FObjectFinder<UAnimMontage>
		FEMALE_ATTACK_MONTAGE_ONLYPLAY
		(TEXT("AnimMontage'/Game/Resources/Character/PlayerCharacter/Animation/Female/FemaleAttackMontageOnlyPlay.FemaleAttackMontageOnlyPlay'"));
	MaleMontageList.Add(ATTACK_MONTAGE_ONLYPLAY.Object);
	FemaleMontageList.Add(FEMALE_ATTACK_MONTAGE_ONLYPLAY.Object);

	static ConstructorHelpers::FObjectFinder<UAnimMontage>
		MOVE_MONTAGE_ONLYPLAY(TEXT("AnimMontage'/Game/Resources/Character/PlayerCharacter/Animation/MoveMontageOnlyPlay.MoveMontageOnlyPlay'"));
	static ConstructorHelpers::FObjectFinder<UAnimMontage>
		FEMALE_MOVE_MONTAGE_ONLYPLAY
		(TEXT("AnimMontage'/Game/Resources/Character/PlayerCharacter/Animation/Female/FemaleMoveMontageOnlyPlay.FemaleMoveMontageOnlyPlay'"));
	MaleMontageList.Add(MOVE_MONTAGE_ONLYPLAY.Object);
	FemaleMontageList.Add(FEMALE_MOVE_MONTAGE_ONLYPLAY.Object);


	static ConstructorHelpers::FObjectFinder<UAnimMontage>
		HIT_MONTAGE(TEXT("AnimMontage'/Game/Resources/Character/PlayerCharacter/Animation/HitMontage.HitMontage'"));
	static ConstructorHelpers::FObjectFinder<UAnimMontage>
		FEMALE_HIT_MONTAGE(TEXT("AnimMontage'/Game/Resources/Character/PlayerCharacter/Animation/Female/FemaleHitMontage.FemaleHitMontage'"));
	MaleMontageList.Add(HIT_MONTAGE.Object);
	FemaleMontageList.Add(FEMALE_HIT_MONTAGE.Object);


	static ConstructorHelpers::FObjectFinder<UAnimMontage>
		ROLL_MONTAGE(TEXT("AnimMontage'/Game/Resources/Character/PlayerCharacter/Animation/RollMontage.RollMontage'"));
	static ConstructorHelpers::FObjectFinder<UAnimMontage>
		FEMALE_ROLL_MONTAGE(TEXT("AnimMontage'/Game/Resources/Character/PlayerCharacter/Animation/Female/FemaleRollMontage.FemaleRollMontage'"));
	MaleMontageList.Add(ROLL_MONTAGE.Object);
	FemaleMontageList.Add(FEMALE_ROLL_MONTAGE.Object);

	static ConstructorHelpers::FObjectFinder<UAnimMontage>
		SKILL_MONTAGE(TEXT("AnimMontage'/Game/Resources/Character/PlayerCharacter/Animation/SkillMontage.SkillMontage'"));
	static ConstructorHelpers::FObjectFinder<UAnimMontage>
		FEMALE_SKILL_MONTAGE(TEXT("AnimMontage'/Game/Resources/Character/PlayerCharacter/Animation/Female/FemaleSkillMontage.FemaleSkillMontage'"));
	MaleMontageList.Add(SKILL_MONTAGE.Object);
	FemaleMontageList.Add(FEMALE_SKILL_MONTAGE.Object);


}

UPlayerCharacterAnimInstance::~UPlayerCharacterAnimInstance()
{

}

void UPlayerCharacterAnimInstance::SetOwnerCharacter(APlayerCharacter * MyCharacter)
{
	this->MyCharacter = MyCharacter;
}

void UPlayerCharacterAnimInstance::NativeUpdateAnimation(float DeltaSeconds)
{
	Super::NativeUpdateAnimation(DeltaSeconds);
	
	if (MyCharacter)
	{
		bIsMove = MyCharacter->bIsMove;
		bIsRolling = MyCharacter->bIsRolling;
		bIsSprint = MyCharacter->bIsSprint;
		bIsCharacterDead = MyCharacter->bIsCharacterDead;
		bIsHit = MyCharacter->bIsHit;
		bIsOverallRollAnimPlaying = MyCharacter->bIsOverallRollAnimPlaying;
	}

}

void UPlayerCharacterAnimInstance::AnimNotify_CheckNextComboValid()
{
	if (Delegate_CheckNextCombo.IsBound())
		Delegate_CheckNextCombo.Execute();
}
void UPlayerCharacterAnimInstance::AnimNotify_AttackStart()
{
	if (Delegate_CharacterAttackMoveStart.IsBound())
		Delegate_CharacterAttackMoveStart.Execute();
}
void UPlayerCharacterAnimInstance::AnimNotify_AttackEnd()
{
	if (Delegate_CharacterAttackMoveEnd.IsBound())
		Delegate_CharacterAttackMoveEnd.Execute();

	if (MyCharacter->bIsSprint)
		MyCharacter->GetCharacterMovement()->MaxWalkSpeed = kSprintMovementSpeed;
}
void UPlayerCharacterAnimInstance::AnimNotify_RollingEnd()
{
	//구르는 동작의 끝(애니메이션의 끝이 아님)
	if (IsValid(MyCharacter))
	{
		MyCharacter->bIsRolling = false;
		bIsRolling = false;
		MyCharacter->SetNormalCapsuleMode();

		if(MyCharacter->bIsSprint)
			MyCharacter->GetCharacterMovement()->MaxWalkSpeed = kSprintMovementSpeed;

	}
}
void UPlayerCharacterAnimInstance::AnimNotify_HitMotionEnd()
{
	MyCharacter->bIsHit = false;
	bIsHit = false;
	MyCharacter->GetCharacterMovement()->MaxAcceleration = kMaxMovementAcceleration;
	MyCharacter->GetCharacterMovement()->MaxWalkSpeed = kNormalMovementSpeed;
}

void UPlayerCharacterAnimInstance::AnimNotify_RemoteRollingEnd()
{
	MyCharacter->bIsRolling = false; 
	bIsRolling = false;

	MyCharacter->GetCharacterMovement()->MaxAcceleration = kNormalMovementAcceleration;
	MyCharacter->GetCharacterMovement()->MaxWalkSpeed = kNormalMovementSpeed;
}

void UPlayerCharacterAnimInstance::AnimNotify_RemoteRollingAllEnd()
{
	bIsOverallRollAnimPlaying = false;
	MyCharacter->bIsOverallRollAnimPlaying = false;

}
void UPlayerCharacterAnimInstance::PlayAttackMontage()
{
	float ret = Montage_Play(AttackMontage, 1.0f);
	UE_LOG(LogTemp, Warning, TEXT("%f"), ret);
}

void UPlayerCharacterAnimInstance::PlayAttackOnlyPlayMontage()
{
	float ret = Montage_Play(AttackMontageOnlyPlay, 1.0f);
}

void UPlayerCharacterAnimInstance::PlayMoveOnlyPlayMontage()
{
	float ret = Montage_Play(MoveMontageOnlyPlay, 1.0f);
}

void UPlayerCharacterAnimInstance::PlayHitMontage()
{
	float ret = Montage_Play(HitMontage, 1.0f);
}

void UPlayerCharacterAnimInstance::PlayRollMontage()
{
	float ret = Montage_Play(RollMontage, 1.0f);
	UE_LOG(LogTemp, Warning, TEXT("%f"), ret);
}

void UPlayerCharacterAnimInstance::StopAttackMontage()
{
	Montage_Stop(0.0f, AttackMontage);
}

void UPlayerCharacterAnimInstance::JumpToComboMontageSection(int32 Section)
{
	check((Section <= 4));
	Montage_JumpToSection(FName(*FString::Printf(TEXT("combo%d"), Section)));
}

void UPlayerCharacterAnimInstance::JumpToMoveMontageSection(FString Section)
{
	Montage_JumpToSection(FName(*Section));
}

void UPlayerCharacterAnimInstance::PlaySkillMontage()
{
	float ret = Montage_Play(SkillMontage, 1.0f);
}

void UPlayerCharacterAnimInstance::JumpToSkillMonatgeSection(FString Section)
{
	Montage_JumpToSection(FName(*Section));
}

void UPlayerCharacterAnimInstance::AnimNotify_GaiaHit()
{
	if (IsValid(MyCharacter))
	{
		if (Delegate_GaiaCrushEnd.IsBound())
			Delegate_GaiaCrushEnd.Execute(MyCharacter);
	}
	else
		check(false);
	MyCharacter->SetbIsSkillMove(false);
}

void UPlayerCharacterAnimInstance::AnimNotify_GaiaEnd()
{
	if (IsValid(MyCharacter))
	{
		if (Delegate_GaiaCrushProcessEnd.IsBound())
			Delegate_GaiaCrushProcessEnd.Execute(MyCharacter);
	}
	else
		check(false);
	MyCharacter->SetbIsSkillPlaying(false);
}

void UPlayerCharacterAnimInstance::AnimNotify_BerserkEnd()
{
	if (IsValid(MyCharacter))
	{
		if (Delegate_BerserkEnd.IsBound())
			Delegate_BerserkEnd.Execute(MyCharacter);
	}
	else
		check(false);

	MyCharacter->SetbIsSkillPlaying(false);
	UParticleSystemComponent* Comp = MyCharacter->GetParticleComponentByName(TEXT("BerserkLoop"));
	Comp->SetActive(true);
}

void UPlayerCharacterAnimInstance::AnimNotify_NormalTrail()
{
	int32 ComboCount = MyCharacter->GetComboCount();
	UParticleSystemComponent* Comp = nullptr;

	FName aaa("FxBottom");

	//SwordTrail 지속시간
	auto GI = Cast<UXRGameInstance>(MyCharacter->GetGameInstance());
	FString Fstr = "Berserk";
	UPlayerSkill* Bersk = GI->GetPlayerSkillManager()->FindSkillFromListByName(GI->GetPlayerSkillManager()->SkillListForPlalyer, Fstr);
	int32 RetNum = GI->GetPlayerSkillManager()->FindSkillFromTimeDurationList(Bersk->GetID());

	//int32 TDNum = GI->GetPlayerSkillManager()->TimeDurationList[RetNum];

	UPlayerSkillTimeDuration* TD = nullptr;

	if(RetNum != -1)
		TD= GI->GetPlayerSkillManager()->TimeDurationList[RetNum];

	if (TD != nullptr && TD->GetbSkillRemainTime())
	{
		Comp = MyCharacter->GetParticleComponentByName(TEXT("SwordTrailBerserkNormal"));
	}
	else
		Comp = MyCharacter->GetParticleComponentByName(TEXT("SwordTrailNormal"));
	

	switch (ComboCount)
	{		
		case 1:
			Comp->SetActive(false);
			Comp->AttachToComponent(MyCharacter->Equipments.BodyComponent, 
				FAttachmentTransformRules::KeepRelativeTransform , MyCharacter->GetComboSocket().FXCombo1);
			Comp->SetActive(true);
			break;
		case 2:
			Comp->SetActive(false);
			Comp->AttachToComponent(MyCharacter->Equipments.BodyComponent,
				FAttachmentTransformRules::KeepRelativeTransform, MyCharacter->GetComboSocket().FXCombo2);
			Comp->SetActive(true);
			break;
		case 3:
			Comp->SetActive(false);
			Comp->AttachToComponent(MyCharacter->Equipments.BodyComponent,
				FAttachmentTransformRules::KeepRelativeTransform, MyCharacter->GetComboSocket().FXCombo3);
			Comp->SetActive(true);
			break;
	}
}

void UPlayerCharacterAnimInstance::AnimNotify_FinalTrail()
{
	//SwordTrail 지속시간
	auto GI = Cast<UXRGameInstance>(MyCharacter->GetGameInstance());
	FString Fstr = "Berserk";
	UPlayerSkill* Bersk = GI->GetPlayerSkillManager()->FindSkillFromListByName(GI->GetPlayerSkillManager()->SkillListForPlalyer, Fstr);
	int32 RetNum = GI->GetPlayerSkillManager()->FindSkillFromTimeDurationList(Bersk->GetID());

	//int32 TDNum = GI->GetPlayerSkillManager()->FindSkillFromTimeDurationList(ID);
	UPlayerSkillTimeDuration* TD = nullptr;
	if (RetNum != -1)
		TD = GI->GetPlayerSkillManager()->TimeDurationList[RetNum];

	const int32 FinalCombo = 4;
	UParticleSystemComponent* Comp = nullptr;

	if (TD != nullptr && TD->GetbSkillRemainTime())
	{
		Comp = MyCharacter->GetParticleComponentByName(TEXT("SwordTrailBerserkFinal"));
	}
	else
		Comp = MyCharacter->GetParticleComponentByName(TEXT("SwordTrailFinal"));

	Comp->SetActive(false);
	Comp->AttachToComponent(MyCharacter->Equipments.BodyComponent,
		FAttachmentTransformRules::KeepRelativeTransform, MyCharacter->GetComboSocket().FXCombo4);
	Comp->SetActive(true);
}


void UPlayerCharacterAnimInstance::AnimNotify_BerserkEffon()
{
	UParticleSystemComponent* Comp = MyCharacter->GetParticleComponentByName(TEXT("BerserkStart"));
	Comp->SetActive(true);
}

void UPlayerCharacterAnimInstance::AnimNotify_InvisibleOn()
{
	if (IsValid(MyCharacter))
	{
		MyCharacter->SetbIsInvisible(true);
	}
}

void UPlayerCharacterAnimInstance::AnimNotify_InvisibleOff()
{
	if (IsValid(MyCharacter))
	{
		MyCharacter->SetbIsInvisible(false);
	}
}

void UPlayerCharacterAnimInstance::AnimNotify_KnockBackMoveEnd()
{
	if (IsValid(MyCharacter))
	{
		MyCharacter->SetbIsKnockBackMoving(false);
		MyCharacter->GetCharacterMovement()->bOrientRotationToMovement = true;
	}
}

void UPlayerCharacterAnimInstance::InitializeMontage(bool IsMale)
{
	if (IsMale)
	{
		AttackMontage			= MaleMontageList[0];
		AttackMontageOnlyPlay	= MaleMontageList[1];
		MoveMontageOnlyPlay		= MaleMontageList[2];
		HitMontage			    = MaleMontageList[3];
		RollMontage				= MaleMontageList[4];
		SkillMontage		    = MaleMontageList[5];
	}
	else
	{
		AttackMontage			= FemaleMontageList[0];
		AttackMontageOnlyPlay	= FemaleMontageList[1];
		MoveMontageOnlyPlay		= FemaleMontageList[2];
		HitMontage				= FemaleMontageList[3];
		RollMontage				= FemaleMontageList[4];
		SkillMontage			= FemaleMontageList[5];
	}
}

void UPlayerCharacterAnimInstance::AnimNotify_GargiSound()
{
	FString HitSound;

	int32 Rand = FMath::RandRange(0, 3);
	switch (Rand)
	{
	case 0:
		HitSound = "AirGargi1";
		break;
	case 1:
		HitSound = "AirGargi2";
		break;
	case 2:
		HitSound = "AirGargi3";
		break;
	default:
		HitSound = "AiRGargi4";
		break;
	}
	int32 idx = MyCharacter->CurGameInstance->GetSoundIdxByName(HitSound);
	UAudioComponent* SoundComp = MyCharacter->CurGameInstance->GetAudioComponentByIdx(idx);
	UGameplayStatics::PlaySoundAtLocation(GetWorld(), SoundComp->Sound, MyCharacter->GetActorLocation(),
		1.0f, 1.0f, 0.0f, MyCharacter->Attenuation);
}