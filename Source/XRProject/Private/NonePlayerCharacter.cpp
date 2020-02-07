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

ANonePlayerCharacter::ANonePlayerCharacter()
{
	static ConstructorHelpers::FObjectFinder<UDataTable> NPCDATATABLE(TEXT("DataTable'/Game/Resources/DataTable/MonsterTable.MonsterTable'"));
	if (NPCDATATABLE.Succeeded())
	{
		XRLOG(Warning, TEXT("Finded NPCTable"));
		NPCDataTable = NPCDATATABLE.Object;
	}
	else
	{
		XRLOG(Error, TEXT("Can't Find NPCTable"));
	}

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

	if (CurrentLoadState == ECharacterLoadState::READY)
	{
		auto ingameMode = Cast<UXRGameInstance>(GetGameInstance());
		if (ingameMode)
		{
			if (ingameMode->GetIsSuper())
			{
				AICon->RunAI();
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
	//GetCharacterMovement()->bOrientRotationToMovement = true;
	//GetCharacterMovement()->bUseControllerDesiredRotation = false;// 캐릭터 움직임 보간

	GetCharacterMovement()->bOrientRotationToMovement = false;
	GetCharacterMovement()->bUseControllerDesiredRotation = true;// 캐릭터 움직임 보간

	XRLOG(Warning, TEXT("%s PossessedBy %s"), *GetName(), *Cntr->GetName())
}

float ANonePlayerCharacter::TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser)
{
	Super::TakeDamage(DamageAmount, DamageEvent, EventInstigator, DamageCauser);
	EnermyStatComponent->SetCurrentHP(EnermyStatComponent->GetCurrentHP() - DamageAmount);

	auto npcAnim = Cast<UNonePlayerCharacterAnimInstance>(GetMesh()->GetAnimInstance());
	if (npcAnim)
	{
		if (!npcAnim->IsAnyMontagePlaying())
		{
			npcAnim->Montage_Play(npcAnim->NpcTakeDamageMontage);
		}
	}
	return DamageAmount;
}

void ANonePlayerCharacter::DetectTarget(const TArray<AActor*>& DetectingPawn)
{

	for (auto detec : DetectingPawn)
	{
		XRLOG(Warning, TEXT("%s"), *detec->GetName());
	}

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
			XRLOG(Warning, TEXT("LOST AICONTROLLER"));
		}
		GetCharacterMovement()->MaxWalkSpeed = EnermyStatComponent->GetSpeed();
		GetCapsuleComponent()->SetCapsuleHalfHeight(75.f);
		GetCapsuleComponent()->SetCapsuleRadius(16.5f);

		GetMesh()->SetRelativeLocation(FVector(0, 0, -70));
		GetMesh()->SetRelativeScale3D(FVector(3.f, 3.f, 3.f));
		
		NpcAnim = Cast<UNonePlayerCharacterAnimInstance>(GetMesh()->GetAnimInstance());
		
		SetSkelResource(SkelID, AnimBPID);
		break;
	}
	case ECharacterLoadState::READY:
	{
		GEngine->AddOnScreenDebugMessage(1, 50.0f, FColor::Yellow, FString::Printf(TEXT("CurrentState : READY")));
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
	if (NPCDataTable != nullptr)
	{
		FMonsterTableRow* ResourceTableRow =
			NPCDataTable->FindRow<FMonsterTableRow>
			(FName(*(FString::FromInt(NpcID))), FString(""));
		if (ResourceTableRow)
		{
			XRLOG(Warning, TEXT("Finded NPC ID : %d  SkelID : %d  Name : %s "), NpcID, ResourceTableRow->MonsterSkeletalID, *ResourceTableRow->MonsterName);

			SkelID = ResourceTableRow->MonsterSkeletalID;
			AnimBPID = ResourceTableRow->MonsterAnimBP;
			BTID = ResourceTableRow->MonsterBT;
			BBID = ResourceTableRow->MonsterBB;

			EnermyStatComponent->SetMaxHP(ResourceTableRow->MonsterMaxHP);
			EnermyStatComponent->SetAttack_Min(ResourceTableRow->MonsterAttackMin);
			EnermyStatComponent->SetAttack_Max(ResourceTableRow->MonsterAttackMax);
			EnermyStatComponent->SetAttack_Range(ResourceTableRow->MonsterAttackRange);
			EnermyStatComponent->SetAttack_Speed(ResourceTableRow->MonsterAttackSpeed);
			EnermyStatComponent->SetSpeed(ResourceTableRow->MonsterSpeed);
			EnermyStatComponent->SetCharacterName(ResourceTableRow->MonsterName);
		}
	}
	XRLOG(Warning, TEXT("NpcID %d Not Exist "), NpcID);

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
		auto FindListCharacter = AttackOverlapList.Find(castPlayerCharacter);
		if (!AttackOverlapList.IsValidIndex(FindListCharacter))
		{
			AttackOverlapList.AddUnique(castPlayerCharacter);
			XRLOG(Warning, TEXT("OverlapPlayer"));
			OutputStream out;
			out.WriteOpcode(ENetworkCSOpcode::kMonsterHitCharacter);
			out << ObjectID;
			out << castPlayerCharacter->ObjectID;
			out << 1;
			out << GetActorLocation();
			out << GetActorRotation();
			out.CompletePacketBuild();
			GetNetMgr().SendPacket(out);
		

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

	XRLOG(Warning, TEXT("Send to MonsterAction : (ObjectID : %d)(ActionID : %d)(Location : %s)"), ObjectID, ActionID, *Location.ToString());
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

			//XRLOG(Warning, TEXT("Recv MonsterAction : (ObjectID : %d)(ActionID : %d)(Location : %s)"), ObjectID, ActionID, *Location.ToString());
			if (ActionID < 1000)
			{
				AttackOverlapList.Reset();
				AICon->StopMovement();
				SetActorLocation(Location);
				
				SetActorRotation(Rotator);
				auto npcAnim = Cast<UNonePlayerCharacterAnimInstance>(GetMesh()->GetAnimInstance());
				if (npcAnim)
				{
					PlayAnimMontage(npcAnim->NpcAttackMontage[ActionID]);
				}
			}
			else if (ActionID == 1000)
			{
				AICon->MoveToLocation(Location, 2, false, false);
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


