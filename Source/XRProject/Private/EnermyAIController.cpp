// Fill out your copyright notice in the Description page of Project Settings.


#include "EnermyAIController.h"
#include "TestGameInstance_AssetMgr.h"
#include "XRAssetMgr.h"
#include "NavigationSystem.h"
#include "Blueprint/AIBlueprintHelperLibrary.h"
#include "BehaviorTree/BehaviorTree.h"
#include "BehaviorTree/BlackboardData.h"
#include "BehaviorTree/BlackboardComponent.h"

const FName AEnermyAIController::TargetKey("Target");
void AEnermyAIController::OnPossess(APawn* InPawn)
{
	auto TestGameInstance = Cast<UTestGameInstance_AssetMgr>(GetGameInstance());

	FSoftObjectPath AssetPath =
		TestGameInstance->GetXRAssetMgr()->FindResourceFromDataTable(8000);
	FStreamableDelegate ResultCallback;
	ResultCallback.BindLambda([AssetPath, this]()
	{
		TSoftObjectPtr<UBlackboardData> LoadedBB(AssetPath);
		BBAsset = LoadedBB.Get();
		XRLOG(Warning, TEXT("BlackBoard Load Complete"));

	});
	TestGameInstance->GetXRAssetMgr()->ASyncLoadAssetFromPath(AssetPath, ResultCallback);



	FSoftObjectPath AssetPath2 =
		TestGameInstance->GetXRAssetMgr()->FindResourceFromDataTable(8100);
	FStreamableDelegate ResultCallback2;
	ResultCallback2.BindLambda([AssetPath2, this]()
	{
		TSoftObjectPtr<UBehaviorTree> LoadedBT(AssetPath2);
		BTAsset = LoadedBT.Get();
		XRLOG(Warning, TEXT("BehaviorTree Load Complete"));

		RunAI();
	});
	TestGameInstance->GetXRAssetMgr()->ASyncLoadAssetFromPath(AssetPath2, ResultCallback2);

}


void AEnermyAIController::OnUnPossess()
{
}

void AEnermyAIController::RunAI()
{
	if (UseBlackboard(BBAsset, Blackboard))
	{
		Blackboard->SetValueAsVector(FName("Pos"), FVector(0,0,0));
		if (!RunBehaviorTree(BTAsset))
		{
			XRLOG(Error, TEXT("AIController couldn't run behavior tree!"));
		}

		

	}
}

void AEnermyAIController::StopAI()
{
	auto BehaviorTreeComponent = Cast<UBehaviorTreeComponent>(BrainComponent);
	if (nullptr != BehaviorTreeComponent)
	{
		BehaviorTreeComponent->StopTree(EBTStopMode::Safe);
	}
}
