// Fill out your copyright notice in the Description page of Project Settings.


#include "NonePlayerCharacter.h"
#include "XRAIController.h"
#include "XRPlayerController.h"
#include "PlayerCharacter.h"
#include "BehaviorTree/BehaviorTreeComponent.h"
#include "BehaviorTree/BlackboardData.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Navigation/PathFollowingComponent.h"
#include "XRProjectGameModeBase.h"
#include "XRGameInstance.h"
#include "IngameGameMode.h"
#include "Perception/AISense_Damage.h"

ANonePlayerCharacter::ANonePlayerCharacter()
{


	EnermyStatComponent = CreateDefaultSubobject<UCharacterStatComponent>(TEXT("EnermyStat"));


	GetCharacterMovement()->RotationRate = FRotator(0.0f, 700.f, 0.0f);
	
	AIControllerClass = AXRAIController::StaticClass(); 
	AutoPossessAI = EAutoPossessAI::PlacedInWorldOrSpawned;





	GetCapsuleComponent()->SetCollisionProfileName(TEXT("NonePlayer"));
	GetCapsuleComponent()->SetGenerateOverlapEvents(true);

	GetMesh()->SetCollisionProfileName(TEXT("NPCMesh"));
	GetMesh()->SetGenerateOverlapEvents(false);

	GetMesh()->OnComponentBeginOverlap.AddDynamic(this, &ANonePlayerCharacter::AttackCheck);

}


void ANonePlayerCharacter::PostInitializeComponents()
{
	ABaseCharacter::PostInitializeComponents();

}

void ANonePlayerCharacter::BeginPlay()
{
	ACharacter::BeginPlay();

}


void ANonePlayerCharacter::Tick(float DeltaTime)
{
	ABaseCharacter::Tick(DeltaTime);
	if (CurrentLoadState == ECharacterLoadState::READY && CurrentLifeState == ECharacterLifeState::ALIVE)
	{
		auto ingameMode = Cast<UXRGameInstance>(GetGameInstance());
		if (ingameMode)
		{
			if (ingameMode->GetIsSuper())
			{
				AICon->RunAI();

				SumSec += DeltaTime;
				if (SumSec >= 0.2f) 
				{
					SumSec = 0.0f;

					auto Aicon = Cast<AXRAIController>(GetController());
					EPathFollowingStatus::Type followstatus = Aicon->GetMoveStatus();
					if (followstatus == EPathFollowingStatus::Type::Moving)
					{
						SendAction(1000, GetActorLocation(), GetActorRotation());
						//XRLOG(Warning, TEXT("%s Is Moving, %s "), *GetName(), *GetActorLocation().ToString());
					}
				}
			}
		}
	}
}

void ANonePlayerCharacter::PossessedBy(AController* Cntr)
{
	Super::PossessedBy(Cntr);
	AICon = Cast<AXRAIController>(GetController());


	bUseControllerRotationPitch = false;
	bUseControllerRotationRoll = false;
	bUseControllerRotationYaw = false;
	GetCharacterMovement()->RotationRate = FRotator(0.0f, 700.f, 0.0f);
	

	GetCharacterMovement()->bOrientRotationToMovement = true;
	GetCharacterMovement()->bUseControllerDesiredRotation = true;// 캐릭터 움직임 보간


	XRLOG(Warning, TEXT("%s PossessedBy %s"), *GetName(), *Cntr->GetName())
}

float ANonePlayerCharacter::TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser)
{
	float damage = Super::TakeDamage(DamageAmount, DamageEvent, EventInstigator, DamageCauser);

	UAISense_Damage::ReportDamageEvent(GetWorld(), this, EventInstigator->GetPawn(), damage, GetActorLocation(), GetActorLocation());
	EnermyStatComponent->SetCurrentHP(EnermyStatComponent->GetCurrentHP() - damage);
	XRLOG(Warning, TEXT("Monster TakeDamage : %f"), damage);
	


	ABaseCharacter* DamageCauserPlayer = Cast<ABaseCharacter>(EventInstigator->GetPawn());
	if (DamageCauserPlayer)
	{
	
		if (AggroList.Contains(DamageCauserPlayer))
		{
			AggroList[DamageCauserPlayer] += damage;
		}
		else
		{
			AggroList.Add(DamageCauserPlayer);
			AggroList[DamageCauserPlayer] = damage;
		}
	}
	auto aicon = Cast<AAIController>(GetController());
	if (aicon)
	{
		aicon->StopMovement();
	}
	auto npcAnim = Cast<UNonePlayerCharacterAnimInstance>(GetMesh()->GetAnimInstance());
	if (npcAnim)
	{
		if (!npcAnim->Montage_IsPlaying(npcAnim->NpcTakeDamageMontage))
		{
			if (npcAnim->IsAnyMontagePlaying())
			{
				npcAnim->StopAllMontages(0.05f);
			}
			GetMesh()->SetGenerateOverlapEvents(false);
			npcAnim->Montage_Play(npcAnim->NpcTakeDamageMontage);
		}
	}
	return DamageAmount;
}

void ANonePlayerCharacter::DetectTarget(const TArray<AActor*>& DetectingPawn)
{



}

void ANonePlayerCharacter::SetCharacterLoadState(ECharacterLoadState NewState)
{
	
	CurrentLoadState = NewState;

	switch (CurrentLoadState)
	{
	case ECharacterLoadState::PREINIT:
	{
		GEngine->AddOnScreenDebugMessage(1, 50.0f, FColor::Yellow, FString::Printf(TEXT("CurrentState : PREINIT")));
	

		SetCharacterLifeState(ECharacterLifeState::SPAWN);
		SetCharacterLoadState(ECharacterLoadState::LOADING);
		break;
	}
	case ECharacterLoadState::LOADING:
	{
		GEngine->AddOnScreenDebugMessage(1, 50.0f, FColor::Yellow, FString::Printf(TEXT("CurrentState : LOADING")));
		EnermyStatComponent->OnHPZero.AddDynamic(this, &ANonePlayerCharacter::OnDead);
		if (AICon)
		{
			AICon->LoadAI(BTID, BBID);
		}
		else
		{
			XRLOG(Warning, TEXT("LOST AI CONTROLLER"));
		}
		GetCharacterMovement()->MaxWalkSpeed = EnermyStatComponent->GetSpeed();
		
		NpcAnim = Cast<UNonePlayerCharacterAnimInstance>(GetMesh()->GetAnimInstance());
		
		SetSkelResource(SkelID, AnimBPID);
		OnNpcReady.Broadcast();
		break;
	}
	case ECharacterLoadState::READY:
	{
		GEngine->AddOnScreenDebugMessage(1, 50.0f, FColor::Yellow, FString::Printf(TEXT("CurrentState : READY")));
		NpcAnim = Cast<UNonePlayerCharacterAnimInstance>(GetMesh()->GetAnimInstance());
		
		for (int i = 0; i < NpcAnim->NpcAttackMontage.Num(); i++)
		{
			SetOnSkillQueue(i);
		}

		SetCharacterLifeState(ECharacterLifeState::ALIVE);




		break;
	}
	default:
		break;
	}
}

void ANonePlayerCharacter::SetCharacterLifeState(ECharacterLifeState NewState)
{
	CurrentLifeState = NewState;

	switch (CurrentLifeState)
	{
	case ECharacterLifeState::SPAWN:
	{
		GEngine->AddOnScreenDebugMessage(2, 50.0f, FColor::Blue, FString::Printf(TEXT("CurrentState : SPAWN")));

		break;
	}
	case ECharacterLifeState::ALIVE:
	{
		GEngine->AddOnScreenDebugMessage(2, 50.0f, FColor::Blue, FString::Printf(TEXT("CurrentState : Play")));

		break;
	}
	case ECharacterLifeState::DEAD:
	{
		GEngine->AddOnScreenDebugMessage(2, 50.0f, FColor::Blue, FString::Printf(TEXT("CurrentState : Dead")));
		SetActorEnableCollision(false);
		auto npcAnim = Cast<UNonePlayerCharacterAnimInstance>(GetMesh()->GetAnimInstance());
		AICon->StopAI();
		if (npcAnim)
		{
			npcAnim->Montage_Play(npcAnim->GetDeadMontage());
		}
		else
		{
			XRLOG(Warning, TEXT("NpcAnimInstance Lost"));
		}


		break;
	}
	default:
		break;
	}

}

void ANonePlayerCharacter::OnDead()
{
	SetCharacterLifeState(ECharacterLifeState::DEAD);
}

void ANonePlayerCharacter::GetNPCInfoFromTable(int32 NpcID)
{
	if (GetAssetMgr()->NPCDataTable != nullptr)
	{
		FMonsterTableRow* ResourceTableRow =
			GetAssetMgr()->NPCDataTable->FindRow<FMonsterTableRow>
			(FName(*(FString::FromInt(NpcID))), FString(""));
		if (ResourceTableRow)
		{
			XRLOG(Warning, TEXT("Finded NPC ID : %d  SkelID : %d  Name : %s "), NpcID, ResourceTableRow->MonsterSkeletalID, *ResourceTableRow->MonsterName);

			SkelID = ResourceTableRow->MonsterSkeletalID;
			AnimBPID = ResourceTableRow->MonsterAnimBP;
			BTID = ResourceTableRow->MonsterBT;
			BBID = ResourceTableRow->MonsterBB;

			EnermyStatComponent->SetMaxHP(ResourceTableRow->MonsterMaxHP);
			EnermyStatComponent->SetCurrentHP(EnermyStatComponent->GetMaxHP());
			EnermyStatComponent->SetAttack_Min(ResourceTableRow->MonsterAttackMin);
			EnermyStatComponent->SetAttack_Max(ResourceTableRow->MonsterAttackMax);
			EnermyStatComponent->SetAttack_Range(ResourceTableRow->MonsterAttackRange);
			EnermyStatComponent->SetAttack_Speed(ResourceTableRow->MonsterAttackSpeed);
			EnermyStatComponent->SetSpeed(ResourceTableRow->MonsterSpeed);
			EnermyStatComponent->SetCharacterName(ResourceTableRow->MonsterName);

			XRLOG(Warning, TEXT("NpcID %d Not Exist "), NpcID);
			FResourceLocalSize LocalTransForm = GetAssetMgr()->FindResourceSizeFromTable(SkelID);
			GetMesh()->SetRelativeTransform(LocalTransForm.LocalTransform);
			GetCapsuleComponent()->SetCapsuleHalfHeight(LocalTransForm.CapsuleHeight);
			GetCapsuleComponent()->SetCapsuleRadius(LocalTransForm.CapsuleRad);
			GetCharacterMovement()->MaxWalkSpeed = EnermyStatComponent->GetSpeed();
		}
	}


}

void ANonePlayerCharacter::NpcLoadStart(int32 npcID)
{
	GetNPCInfoFromTable(npcID);
	SetCharacterLoadState(ECharacterLoadState::PREINIT);
}

void ANonePlayerCharacter::AttackCheck(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	auto castPlayerCharacter = Cast<APlayerCharacter>(OtherActor);
	if (castPlayerCharacter)
	{
		auto PlayerCon = Cast<APlayerController>(castPlayerCharacter->GetController());
		if (PlayerCon)
		{
			auto FindListCharacter = AttackOverlapList.Find(castPlayerCharacter);
			if (!AttackOverlapList.IsValidIndex(FindListCharacter))
			{

				GEngine->AddOnScreenDebugMessage(-1, 5, FColor::Black, TEXT("PlayerHit"));
				AttackOverlapList.AddUnique(castPlayerCharacter);
				XRLOG(Warning, TEXT("OverlapPlayer"));
				OutputStream out;
				out.WriteOpcode(ENetworkCSOpcode::kMonsterHitCharacter);
				out << ObjectID;
				out << castPlayerCharacter->ObjectID;
				out << NpcAnim->NpcAttackMontage[CurrentAttackAction].SkilID;
				out << GetActorLocation();
				out << GetActorRotation();
				out.CompletePacketBuild();
				GetNetMgr().SendPacket(out);
			}
		}
	}
}

void ANonePlayerCharacter::SendAction(int32 ActionID, FVector Location, FRotator Rotator)
{
	OutputStream out;
	out.WriteOpcode(ENetworkCSOpcode::kNotifyMonsterAction);
	out << ObjectID;
	out << ActionID;
	out << Location;
	out << Rotator;
	out.CompletePacketBuild();
	GetNetMgr().SendPacket(out);

	//XRLOG(Warning, TEXT("Send to MonsterAction : (ObjectID : %d)(ActionID : %d)(Location : %s)"), ObjectID, ActionID, *Location.ToString());
}

void ANonePlayerCharacter::SendDamage(int32 ActionID, FVector Location, FRotator Rotator, AActor* OtherActor)
{
	auto castPlayerCharacter = Cast<APlayerCharacter>(OtherActor);
	if (castPlayerCharacter)
	{
		auto PlayerCon = Cast<APlayerController>(castPlayerCharacter->GetController());
		if (PlayerCon)
		{
				XRLOG(Warning, TEXT("OverlapPlayer"));
				OutputStream out;
				out.WriteOpcode(ENetworkCSOpcode::kMonsterHitCharacter);
				out << ObjectID;
				out << castPlayerCharacter->ObjectID;
				out << NpcAnim->NpcAttackMontage[CurrentAttackAction].SkilID;
				out << Location;
				out << Rotator;
				out.CompletePacketBuild();
				GetNetMgr().SendPacket(out);
		}
	}

}

void ANonePlayerCharacter::SetInBattle(bool battle)
{
	bInBattle = battle;
	SendAction(2000, FVector(bInBattle, 0, 0), FRotator(0, 0, 0));
}

void ANonePlayerCharacter::ExcuteRecvNpcAction(InputStream& input)
{
	auto ingameMode = Cast<UXRGameInstance>(GetGameInstance());
	if (ingameMode)
	{
		if (!ingameMode->GetIsSuper())
		{
			int32 ActionID = input.ReadInt32();
			FVector Location = input.ReadFVector();
			FRotator Rotator = input.ReadFRotator();

			XRLOG(Warning, TEXT("Recv MonsterAction : (ObjectID : %d)(ActionID : %d)(Location : %s)"), ObjectID, ActionID, *Location.ToString());
			if (ActionID < 1000)
			{

				GEngine->AddOnScreenDebugMessage(99, 5, FColor::Red, FString::FromInt(ActionID) );
				AttackOverlapList.Reset();
				//AICon->StopMovement();
				SetActorLocation(Location);
				
				SetActorRotation(Rotator);
				auto npcAnim = Cast<UNonePlayerCharacterAnimInstance>(GetMesh()->GetAnimInstance());
				if (npcAnim)
				{
					npcAnim->Montage_Play(npcAnim->NpcAttackMontage[ActionID].AttackAction);
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
		}
	}
}

void ANonePlayerCharacter::NpcTakeDamaged(float setHP, AController* EventInstigator, int32 AttackInstigatorID)
{
	float InputDamage = EnermyStatComponent->GetCurrentHP() - setHP;
	FDamageEvent evt;
	TakeDamage(InputDamage, evt, EventInstigator, EventInstigator->GetPawn());
}



void ANonePlayerCharacter::SetOnSkillQueue(int32 index)
{
	auto npcAnim = Cast<UNonePlayerCharacterAnimInstance>(GetMesh()->GetAnimInstance());
	FTimerHandle handle;
	GetWorld()->GetTimerManager().SetTimer(handle, FTimerDelegate::CreateLambda(
		[this, index]()
	{
		ReadySkillList.AddUnique(index);
	}
	), npcAnim->NpcAttackMontage[index].CoolTime, false);
}
