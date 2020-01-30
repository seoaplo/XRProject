// Fill out your copyright notice in the Description page of Project Settings.


#include "XRAIController.h"
#include "NavigationSystem.h"
#include "BehaviorTree/BehaviorTree.h"
#include "BehaviorTree/BlackboardData.h"
#include "BehaviorTree/BlackboardComponent.h"



const FName AXRAIController::HomePosKey(TEXT("HomePos"));
const FName AXRAIController::PatrolPosKey(TEXT("PatrolPos"));
const FName AXRAIController::TargetKey(TEXT("Target"));

AXRAIController::AXRAIController()
{

	static ConstructorHelpers::FObjectFinder<UBlackboardData> BBObject(TEXT("BlackboardData'/Game/Resources/AI/BB_Enermy.BB_Enermy'"));
	if (BBObject.Succeeded())
	{
		BBAsset = BBObject.Object;
	}

	static ConstructorHelpers::FObjectFinder<UBehaviorTree> BAObject(TEXT("BehaviorTree'/Game/Resources/AI/BT_Enermy.BT_Enermy'"));
	if (BAObject.Succeeded())
	{
		BTAsset = BAObject.Object;
	}

}

void AXRAIController::OnPossess(APawn* InPawn)
{
	Super::OnPossess(InPawn);
}

void AXRAIController::OnUnPossess()
{
	Super::OnUnPossess();
}

void AXRAIController::RunAI()
{
	if (UseBlackboard(BBAsset, Blackboard))
	{
		Blackboard->SetValueAsVector(HomePosKey, GetPawn()->GetActorLocation());
		if (!RunBehaviorTree(BTAsset))
		{
			XRLOG(Error, TEXT("AIController couldn't run behavior tree!"));
		}
	}

}

void AXRAIController::StopAI()
{
	auto BehaviorTreeComponent = Cast<UBehaviorTreeComponent>(BrainComponent);
	if (nullptr != BehaviorTreeComponent)
	{
		BehaviorTreeComponent->StopTree(EBTStopMode::Safe);
	}
}

void AXRAIController::OnRepeatTimer()
{
}
