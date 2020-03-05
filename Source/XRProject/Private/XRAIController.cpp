// Fill out your copyright notice in the Description page of Project Settings.


#include "XRAIController.h"
#include "XRGameInstance.h"
#include "NavigationSystem.h"
#include "BehaviorTree/BehaviorTree.h"
#include "BehaviorTree/BlackboardData.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "NonePlayerCharacter.h"
#include "Perception/AISenseConfig_Sight.h"
#include "Perception/AISenseConfig_Damage.h"

const FName AXRAIController::HomePosKey(TEXT("HomePos"));
const FName AXRAIController::PatrolPosKey(TEXT("PatrolPos"));
const FName AXRAIController::TargetKey(TEXT("Target"));

AXRAIController::AXRAIController()
{
	SetGenericTeamId(FGenericTeamId(5));
	
	
	EnermyPerceptionComponent = CreateDefaultSubobject<UAIPerceptionComponent>(TEXT("EnermySensing"));
	SightConfig = CreateOptionalDefaultSubobject<UAISenseConfig_Sight>(TEXT("Sight Config"));
	SightConfig->SightRadius = 500.f;
	SightConfig->LoseSightRadius = 500.f + 50.f;
	SightConfig->PeripheralVisionAngleDegrees = 180.f;
	SightConfig->SetMaxAge(10.f);


	EnermyPerceptionComponent->ConfigureSense(*SightConfig);

	DamageConfig = CreateOptionalDefaultSubobject<UAISenseConfig_Damage>(TEXT("Damage Config"));
	DamageConfig->SetMaxAge(10.f);

	EnermyPerceptionComponent->ConfigureSense(*DamageConfig);
	bSetControlRotationFromPawnOrientation = false;

	EnermyPerceptionComponent->OnPerceptionUpdated.AddDynamic(this, &AXRAIController::TestPrint);

}

void AXRAIController::OnPossess(APawn* InPawn)
{
	Super::OnPossess(InPawn);
	auto NpcCharacter = Cast<ANonePlayerCharacter>(InPawn);
	SetPerceptionComponent(*EnermyPerceptionComponent);


}

void AXRAIController::OnUnPossess()
{
	Super::OnUnPossess();
}

void AXRAIController::RunAI()
{
	auto BehaviorTreeComponent = Cast<UBehaviorTreeComponent>(BrainComponent);
	if (BehaviorTreeComponent)
	{
		if (BehaviorTreeComponent->IsRunning())
		{
			return;
		}
	}

	if (UseBlackboard(BBAsset, Blackboard))
	{
		Blackboard->SetValueAsVector(HomePosKey, GetPawn()->GetActorLocation());
		if (!RunBehaviorTree(BTAsset))
		{
			XRLOG(Error, TEXT("AIController couldn't run behavior tree!"));
		}
		XRLOG(Warning, TEXT("BehaviorTree Start Run"));
	}


}

void AXRAIController::StopAI()
{
	auto BehaviorTreeComponent = Cast<UBehaviorTreeComponent>(BrainComponent);
	if (BehaviorTreeComponent)
	{
		if (BehaviorTreeComponent->IsRunning())
		{
			BehaviorTreeComponent->StopTree(EBTStopMode::Safe);
		}
	}
}

void AXRAIController::LoadAI(int32 BTID, int32 BBID)
{
	FSoftObjectPath BTPath = GetAssetMgr()->FindResourceFromDataTable(BTID);
	FStreamableDelegate BTResultCallback;
	BTResultCallback.BindLambda([BTPath, this]()
	{
		TSoftObjectPtr<UBehaviorTree> LoadedBT(BTPath);
		BTAsset = LoadedBT.Get();
		XRLOG(Warning, TEXT("BTLoadComplete"));
	});
	GetAssetMgr()->ASyncLoadAssetFromPath(BTPath, BTResultCallback);

	FSoftObjectPath BBPath = GetAssetMgr()->FindResourceFromDataTable(BBID);
	FStreamableDelegate BBResultCallback;
	BBResultCallback.BindLambda([BBPath, this]()
	{
		TSoftObjectPtr<UBlackboardData> LoadedBB(BBPath);
		BBAsset = LoadedBB.Get();
		XRLOG(Warning, TEXT("BBLoadComplete"));
	});
	GetAssetMgr()->ASyncLoadAssetFromPath(BBPath, BBResultCallback);
}

void AXRAIController::OnRepeatTimer()
{
}

void AXRAIController::TestPrint(const TArray<AActor*>& recvActor)
{

	for (auto list : recvActor)
	{
		XRLOG(Warning, TEXT("AI Perception TestPrint : %s"), *list->GetName());
	}
}

ETeamAttitude::Type AXRAIController::GetTeamAttitudeTowards(const AActor& Other) const
{

	if (const APawn* OtherPawn = Cast<APawn>(&Other)) {
		if (const IGenericTeamAgentInterface* TeamAgent = Cast<IGenericTeamAgentInterface>(OtherPawn->GetController()))
		{
			FGenericTeamId OtherTeamID = TeamAgent->GetGenericTeamId();
			uint8 teamID = OtherTeamID.GetId();
			if (teamID == 5) {
				return ETeamAttitude::Friendly;
			}
			else if (teamID == 10)
			{
				return ETeamAttitude::Hostile;
			}
		}
	}
	return ETeamAttitude::Hostile;

}
