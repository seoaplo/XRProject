// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/TriggerSphere.h"
#include "PotalTrigger.generated.h"

/**
 * 제작자 : 서승석
 */


DECLARE_DELEGATE_OneParam(PotalTrigger, AActor*)
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
	void SetSize(float SphereRadius);
public:
	int32_t MapID;
	PotalTrigger BeginOverlapDelegate;
	PotalTrigger EndOverlapDelegate;
};
