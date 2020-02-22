// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerSkill.h"
#include "PlayerCharacter.h"
#include "NonePlayerCharacter.h"
#include "MapManager.h"
#include "SkillCooldown.h"
#include "XRGameInstance.h"

UPlayerSkill::UPlayerSkill()
{
	
}

UPlayerSkill::~UPlayerSkill()
{

}



void UPlayerSkill::Play(APlayerCharacter* Character)
{
	if (OwnerPlayer == nullptr)
		OwnerPlayer = Character;
}

bool UPlayerSkill::End(APlayerCharacter* Character)
{
	return false;
}


bool UPlayerSkill::ConditionCheck(APlayerCharacter * Character)
{
	return false;
}

bool UPlayerSkill::CooldownCheck(APlayerCharacter * Character, UPlayerSkill * Skill)
{
	UXRGameInstance* GI = Cast<UXRGameInstance>(Character->GetWorld()->GetGameInstance());
	int32 Ret = GI->GetPlayerSkillManager()->FindSkillFromCooldownList(Skill->SkillID);
	if (Ret != -1) //쿨다운리스트에 해당 스킬이 있음. 즉 쿨다운이 돌고있거나, 이미 다 돌고 clear된 노드가 있는 상황
	{
		bool IsEnable = GI->GetPlayerSkillManager()->CoolDownList[Ret]->GetIsEnable();
		if (IsEnable == false)
		{
			XRLOG(Warning, TEXT("Skill CoolTime Still Remain %f"), GI->GetPlayerSkillManager()->CoolDownList[Ret]->GetRemainCoolTime());
			return false;
		}
		else //enable = true
		{
			GI->GetPlayerSkillManager()->CoolDownList[Ret]->SetTimer();
		}
	}
	return true;
}

USkill_GaiaCrush::USkill_GaiaCrush()
{

	static ConstructorHelpers::FObjectFinder<UStaticMesh>
		CMesh(TEXT("StaticMesh'/Game/Resources/Character/PlayerCharacter/Mesh/Skill/SkillOverlapCylinder.SkillOverlapCylinder'"));

	RadMesh = CMesh.Object;
	
}

USkill_GaiaCrush::~USkill_GaiaCrush()
{

}

void USkill_GaiaCrush::Play(APlayerCharacter* Character)
{

	if (OwnerPlayer == nullptr || OwnerPlayer != Character)
		OwnerPlayer = Character;

	
	if (OwnerPlayer->GetbIsRolling() || OwnerPlayer->GetbIsDead() || OwnerPlayer->GetbIsSkillPlaying() || 
		OwnerPlayer->bIsHit)
		return;

	if (!ConditionCheck(Character))
	{
		OwnerPlayer->SetbIsSkillPlaying(false);
		OwnerPlayer->SetbIsSkillMove(false);
		return;
	}

	if (!Character->MyAnimInstance->Delegate_GaiaCrushEnd.IsBound())
		Character->MyAnimInstance->Delegate_GaiaCrushEnd.BindUFunction(this, FName("GaiaTargetCheck"));

	if (!Character->MyAnimInstance->Delegate_GaiaCrushProcessEnd.IsBound())
		Character->MyAnimInstance->Delegate_GaiaCrushProcessEnd.BindUFunction(this, FName("End"));

	OwnerPlayer->SetbIsSkillPlaying(true);

	UPlayerCharacterAnimInstance* MyAnimInst = Character->MyAnimInstance;
	
	/*Attack 취소처리*/
	if (OwnerPlayer->GetbIsAttack())
	{
		OwnerPlayer->SetbIsAttack(false);
		OwnerPlayer->SetComboCount(0);
		OwnerPlayer->SetbSavedCombo(false);
		OwnerPlayer->EndMoveAttack();
	}


	if (!MyAnimInst)
		check(false);
	
	
	OutputStream out;
	out.WriteOpcode(ENetworkCSOpcode::kCharacterAction);
	out << 101;
	out << Character->GetActorLocation();
	out << Character->GetActorRotation();
	out.CompletePacketBuild();
	GetNetMgr().SendPacket(out);
	
	FString GaiaStr = "GaiaCrush";
	int32 Idx = MyAnimInst->SkillMontage->GetSectionIndex(FName(*GaiaStr));
	float length = MyAnimInst->SkillMontage->GetSectionLength(Idx);
	
	Character->GetCharacterMovement()->MaxWalkSpeed = MoveDistance / length;
	Character->GetCharacterMovement()->MaxAcceleration = kMaxMovementAcceleration;

	Character->SetbIsSkillMove(true);

	MyAnimInst->PlaySkillMontage();
	MyAnimInst->JumpToSkillMonatgeSection(GaiaStr);
}

void USkill_GaiaCrush::GaiaTargetCheck(APlayerCharacter* Character)
{
	FActorSpawnParameters param;
	param.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn;

	FVector Loc = Character->GetActorLocation() + Character->GetActorForwardVector() * kCircleDistance - FVector(0.0f, 0.0f, 30.0f);

	RadiusChecker = Character->GetWorld()->SpawnActor<AStaticMeshActor>(AStaticMeshActor::StaticClass(), 
		Loc,
		Character->GetActorRotation(), param);
	
	if (RadiusChecker)
	{
		RadiusChecker->SetActorScale3D(FVector(5.0f, 5.0f, 0.4f));
		RadiusChecker->SetMobility(EComponentMobility::Movable);
		RadiusChecker->GetStaticMeshComponent()->SetStaticMesh(RadMesh);
		RadiusChecker->GetStaticMeshComponent()->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
		RadiusChecker->GetStaticMeshComponent()->SetCollisionProfileName("PlayerWeapon");
		RadiusChecker->GetStaticMeshComponent()->SetGenerateOverlapEvents(true);
		RadiusChecker->GetStaticMeshComponent()->OnComponentBeginOverlap.AddDynamic(this, &USkill_GaiaCrush::RadiusOverlapEvent);
		RadiusChecker->GetStaticMeshComponent()->SetVisibility(false);
		RadiusChecker->SetActorRelativeLocation(RadiusChecker->GetActorLocation() +  FVector(0.0f, 0.0f, 0.1f));

		Character->SetbIsSkillMove(false);

	}
	else
		check(false);
}

bool USkill_GaiaCrush::End(APlayerCharacter* Character)
{
	if (IsValid(RadiusChecker))
	{
		RadiusChecker->GetStaticMeshComponent()->OnComponentBeginOverlap.Clear();
		RadiusChecker->Destroy();
	}
	AttackOverlapList.clear();

	OwnerPlayer->SetbIsSkillPlaying(false);
	
	if (OwnerPlayer->bIsSprint)
		OwnerPlayer->GetCharacterMovement()->MaxWalkSpeed = kSprintMovementSpeed;

	return true;
}
bool USkill_GaiaCrush::ConditionCheck(APlayerCharacter * Character)
{
	if (!CooldownCheck(Character, this))
		return false;

	if (Character->PlayerStatComp->GetCurrentStamina() >= GetRequireStamina())
	{ 
		
		return true;
	}
	return false;
}

void USkill_GaiaCrush::RadiusOverlapEvent(UPrimitiveComponent * OverlappedComp, AActor * OtherActor,
	UPrimitiveComponent * OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	//UXRGameInstance* Inst = Cast<UXRGameInstance>(OwnerPlayer->GetGameInstance());
	
	if (Cast<APlayerController>(OwnerPlayer->GetController()))
	{
		ANonePlayerCharacter* NPC = Cast<ANonePlayerCharacter>(OtherActor);
		if (NPC)
		{
			for (ANonePlayerCharacter* FlagNpc : AttackOverlapList)
			{
				if (FlagNpc == NPC)
					return;
			}

			int64 EnemyID = NPC->ObjectID;
			int64 MyID = OwnerPlayer->ObjectID;

			OutputStream out;
			out.WriteOpcode(ENetworkCSOpcode::kCharcterHitSuccess);
			out << MyID;
			out << EnemyID;
			out << 101;
			out << OwnerPlayer->GetActorLocation();
			out << OwnerPlayer->GetActorRotation();
			out.CompletePacketBuild();
			GetNetMgr().SendPacket(out);

			AttackOverlapList.push_back(NPC);
		}
	}
}

void USkill_GaiaCrush::SetMoveDistance(float MoveDistance)
{
	this->MoveDistance = MoveDistance;
}

void USkill_GaiaCrush::SetAffectRadius(float AffectRadius)
{
	this->AffectRadius = AffectRadius;
}

float USkill_GaiaCrush::GetMoveDistance()
{
	return MoveDistance;
}

float USkill_GaiaCrush::GetAffectRadius()
{
	return AffectRadius;
}


USkill_Berserk::USkill_Berserk()
{


}

USkill_Berserk::~USkill_Berserk()
{


}

void USkill_Berserk::Play(APlayerCharacter * Character)
{
	if (OwnerPlayer == nullptr || OwnerPlayer != Character)
		OwnerPlayer = Character;

	if (OwnerPlayer->GetbIsRolling() || OwnerPlayer->GetbIsDead() || OwnerPlayer->GetbIsSkillPlaying() || 
		OwnerPlayer->bIsHit)
		return;
	
	if (!ConditionCheck(Character))
	{
		OwnerPlayer->SetbIsSkillPlaying(false);
		return;
	}

	if (!Character->MyAnimInstance->Delegate_BerserkEnd.IsBound())
		Character->MyAnimInstance->Delegate_BerserkEnd.BindUFunction(this, FName("End"));
	
	OwnerPlayer->SetbIsSkillPlaying(true);

	UPlayerCharacterAnimInstance* MyAnimInst = Character->MyAnimInstance;

	/*Attack 취소처리*/
	if (OwnerPlayer->GetbIsAttack())
	{
		OwnerPlayer->SetbIsAttack(false);
		OwnerPlayer->SetComboCount(0);
		OwnerPlayer->SetbSavedCombo(false);
		OwnerPlayer->EndMoveAttack();
	}

	if (!MyAnimInst)
		check(false);

	FString BerserkStr = "Berserk";

	MyAnimInst->PlaySkillMontage();
	MyAnimInst->JumpToSkillMonatgeSection(BerserkStr);

}

bool USkill_Berserk::End(APlayerCharacter * Character)
{
	OwnerPlayer->SetbIsSkillPlaying(false);

	if (OwnerPlayer->bIsSprint)
		OwnerPlayer->GetCharacterMovement()->MaxWalkSpeed = kSprintMovementSpeed;

	OutputStream out;
	out.WriteOpcode(ENetworkCSOpcode::kCharacterAction);
	out << 102;
	out << Character->GetActorLocation();
	out << Character->GetActorRotation();
	out.CompletePacketBuild();
	GetNetMgr().SendPacket(out);
	return true;
}

bool USkill_Berserk::ConditionCheck(APlayerCharacter * Character)
{
	if (!CooldownCheck(Character, this))
		return false; 


	if (Character->PlayerStatComp->GetCurrentStamina() >= GetRequireStamina())
	{
		CooldownCheck(Character, this);
		return true;
	}
	return false;
}

