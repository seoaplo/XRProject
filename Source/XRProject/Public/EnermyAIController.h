// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "EnermyAIController.generated.h"

/**
 * 
 */
UCLASS()
class XRPROJECT_API AEnermyAIController : public AAIController
{
	GENERATED_BODY()

protected:
	void OnPossess(APawn* InPawn) override;
	void OnUnPossess() override;
	


public:
	static const FName TargetKey;
	
	UFUNCTION(BlueprintCallable)
		void RunAI();
	UFUNCTION(BlueprintCallable)
		void StopAI();

private:
	UPROPERTY(VisibleAnywhere,BlueprintReadOnly, Meta = (AllowPrivateAccess = true))
		class UBehaviorTree* BTAsset;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Meta = (AllowPrivateAccess = true))
		class UBlackboardData* BBAsset;

};
