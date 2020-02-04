// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/TriggerSphere.h"
#include "PotalTrigger.generated.h"

/**
 * 제작자 : 서승석
 */


DECLARE_DELEGATE_TwoParams(PotalTrigger, int64_t, AActor*)
UCLASS()
class XRPROJECT_API APotalTrigger : public ATriggerSphere
{
	GENERATED_BODY()
protected:
	virtual void BeginPlay() override;
public:
	APotalTrigger();

	// overlap begin function
	UFUNCTION()
		void OnOverlapBegin(class AActor* OverlappedActor, class AActor* OtherActor);

	// overlap end function
	UFUNCTION()
		void OnOverlapEnd(class AActor* OverlappedActor, class AActor* OtherActor);

public:
	int64_t ObjectID;
	PotalTrigger BeginOverlapDelegate;
	PotalTrigger EndOverlapDelegate;
};
