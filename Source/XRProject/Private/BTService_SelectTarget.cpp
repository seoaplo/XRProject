// Fill out your copyright notice in the Description page of Project Settings.


#include "BTService_SelectTarget.h"
#include "NonePlayerCharacter.h"
#include "XRAIController.h"
#include "PlayerCharacter.h"
#include "BehaviorTree/BlackboardComponent.h"

UBTService_SelectTarget::UBTService_SelectTarget()
{

}
void UBTService_SelectTarget::TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	Super::TickNode(OwnerComp, NodeMemory, DeltaSeconds);

	APawn* ControllingPawn = OwnerComp.GetAIOwner()->GetPawn();
	AXRAIController* OwnerCon = Cast<AXRAIController>(OwnerComp.GetAIOwner());
	ANonePlayerCharacter* OwnerNpc = Cast<ANonePlayerCharacter>(ControllingPawn);
	if (OwnerNpc && OwnerCon)
	{
		TArray < AActor*> OutActors;
		OwnerCon->GetPerceptionComp()->GetKnownPerceivedActors(nullptr, OutActors);
		for (int i = 0; i < OutActors.Num(); i++)
		{
			APlayerCharacter* DetectedPlayer = Cast<APlayerCharacter>(OutActors[i]);
			if (DetectedPlayer)
			{
				if (DetectedPlayer->GetCharacterLifeState() != ECharacterLifeState::DEAD)
				{
					APlayerCharacter* Target = Cast<APlayerCharacter>(OwnerComp.GetBlackboardComponent()->GetValueAsObject(OwnerCon->TargetKey));
					if (Target)
					{
						if (Target->GetDistanceTo(OwnerNpc) < OutActors[i]->GetDistanceTo(OwnerNpc))
						{
							OwnerComp.GetBlackboardComponent()->SetValueAsObject(OwnerCon->TargetKey, OutActors[i]);
							return;
						}
					}
					OwnerComp.GetBlackboardComponent()->SetValueAsObject(OwnerCon->TargetKey, OutActors[i]);
					return;
					
				}
			}
		}
		OwnerComp.GetBlackboardComponent()->SetValueAsObject(OwnerCon->TargetKey, nullptr);
		return;
		//아무것도 없을때 타겟 클리어
	}
}