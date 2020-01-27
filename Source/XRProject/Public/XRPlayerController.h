// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "XRProject.h"
#include "GameFramework/PlayerController.h"
#include "XRPlayerController.generated.h"

/**
 * 
 */
UCLASS()
class XRPROJECT_API AXRPlayerController : public APlayerController
{
	GENERATED_BODY()
	
public:
	UFUNCTION(BlueprintCallable)
		FORCEINLINE bool IsSpuer() const { return bIsSuper; }
	UFUNCTION(BlueprintCallable)
		void SetSpuer() { bIsSuper = true; }

protected:
private:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Meta = (AllowPrivateAccess = true))
		bool bIsSuper= false;

};