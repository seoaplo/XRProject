// Fill out your copyright notice in the Description page of Project Settings.


#include "BTService_SelectTarget.h"
#include "NonePlayerCharacter.h"
#include "XRAIController.h"
#include "PlayerCharacter.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Perception/AISenseConfig_Damage.h"

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
		APlayerCharacter* Target = Cast<APlayerCharacter>(OwnerComp.GetBlackboardComponent()->GetValueAsObject(OwnerCon->TargetKey));

		for (int i = 0; i < OutActors.Num(); i++)
		{
			APlayerCharacter* DetectedPlayer = Cast<APlayerCharacter>(OutActors[i]);
			if (DetectedPlayer)
			{
				if (DetectedPlayer->GetCharacterLifeState() != ECharacterLifeState::DEAD)
				{

					if (Target)
					{
						if (Target->GetDistanceTo(OwnerNpc) > OutActors[i]->GetDistanceTo(OwnerNpc))
						{
							Target = DetectedPlayer;
							OwnerComp.GetBlackboardComponent()->SetValueAsObject(OwnerCon->TargetKey,Target);
						}
					}
					else
					{
						Target = DetectedPlayer;
						OwnerComp.GetBlackboardComponent()->SetValueAsObject(OwnerCon->TargetKey, Target);
					}
				}
			}
		}
		OwnerComp.GetBlackboardComponent()->SetValueAsObject(OwnerCon->TargetKey, Target);
		//아무것도 없을때 타겟 클리어
	}
}