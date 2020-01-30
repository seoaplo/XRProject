// Fill out your copyright notice in the Description page of Project Settings.


#include "NonePlayerCharacter.h"
#include "XRAIController.h"
#include "XRPlayerController.h"
#include "PlayerCharacter.h"
#include "BehaviorTree/BlackboardData.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Navigation/PathFollowingComponent.h"
#include "Perception/AISenseConfig_Sight.h"
#include "Containers/Array.h"


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
	EnermyPerceptionComponent = CreateDefaultSubobject<UAIPerceptionComponent>(TEXT("EnermySensing"));
	SightConfig = CreateOptionalDefaultSubobject<UAISenseConfig_Sight>(TEXT("Sight Config"));
	AIControllerClass = AXRAIController::StaticClass();

	GetCharacterMovement()->bOrientRotationToMovement = true;
	GetCharacterMovement()->RotationRate = FRotator(0.0f, 700.f, 0.0f);


	SightConfig->SightRadius = 500.f;
	SightConfig->LoseSightRadius = 500.f+50.f;
	SightConfig->PeripheralVisionAngleDegrees = 75.f;
	SightConfig->SetMaxAge(5.f);

	SightConfig->DetectionByAffiliation.bDetectEnemies = true;
	SightConfig->DetectionByAffiliation.bDetectNeutrals = true;
	SightConfig->DetectionByAffiliation.bDetectFriendlies = true;

	EnermyPerceptionComponent->ConfigureSense(*SightConfig);
	AIControllerClass = AXRAIController::StaticClass();
}


void ANonePlayerCharacter::PostInitializeComponents()
{
	ABaseCharacter::PostInitializeComponents();
	EnermyPerceptionComponent->OnPerceptionUpdated.AddDynamic(this, &ANonePlayerCharacter::DetectTarget);

}

void ANonePlayerCharacter::BeginPlay()
{
	ABaseCharacter::BeginPlay();
	//SetCharacterLoadState(ECharacterLoadState::PREINIT);

}


void ANonePlayerCharacter::Tick(float DeltaTime)
{
	ABaseCharacter::Tick(DeltaTime);
}

void ANonePlayerCharacter::PossessedBy(AController* Cntr)
{
	Super::PossessedBy(Cntr);
	AICon = Cast<AXRAIController>(GetController());
	XRLOG(Warning,TEXT("%s PossessedBy %s"),*GetName(),*Cntr->GetName())
}

float ANonePlayerCharacter::TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser)
{
	Super::TakeDamage(DamageAmount, DamageEvent, EventInstigator, DamageCauser);
	XRLOG(Warning, TEXT("OnAttack"));
	return 0.f;
}

void ANonePlayerCharacter::DetectTarget(const TArray<AActor*>& DetectingPawn)
{
	if (AggroList.Num() < MAX_PARTY_MEMBER)
	{
		for (int DetectedNum = 0; DetectedNum < DetectingPawn.Num(); DetectedNum++)
		{
			XRLOG(Warning, TEXT("Detected  : %s"), *DetectingPawn[DetectedNum]->GetName());
			auto castTarget = Cast<APlayerCharacter>(DetectingPawn[DetectedNum]);
			if (castTarget)
			{
				AggroList.AddUnique(castTarget);
			}
			if (Target == nullptr)
			{
				Target = castTarget;
			}
			
		}
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
		SetCharacterLoadState(ECharacterLoadState::LOADING);
		SetCharacterLifeState(ECharacterLifeState::SPAWN);
		break;
	}
	case ECharacterLoadState::LOADING:
	{
		GEngine->AddOnScreenDebugMessage(1, 50.0f, FColor::Yellow, FString::Printf(TEXT("CurrentState : LOADING")));
		EnermyStatComponent->OnHPZero.AddDynamic(this, &ANonePlayerCharacter::OnDead);
		SetSkelResource(SkelID, AnimBPID);
		
		break;
	}
	case ECharacterLoadState::READY:
	{
		GEngine->AddOnScreenDebugMessage(1, 50.0f, FColor::Yellow, FString::Printf(TEXT("CurrentState : READY")));
		SetCharacterLifeState(ECharacterLifeState::ALIVE);
		auto playerCon = Cast<AXRPlayerController>(GetWorld()->GetFirstPlayerController());
		//if (playerCon && playerCon->IsSpuer())
		//{
		
				AICon->RunAI();
		//}
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
