// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "UIController.generated.h"

/**
 * 
 */
UCLASS()
class XRPROJECT_API AUIController : public APlayerController
{
	GENERATED_BODY()
public:
	AUIController();
public:
	UFUNCTION(BlueprintCallable, Category = "C_Function")
		void Test_SendChat(FText const& Chat);
	UFUNCTION(BlueprintCallable, Category = "C_Function")
		void Test_HP(int hp);
	UFUNCTION(BlueprintCallable, Category = "C_Function")
		void SendBoxFocusOrSend();
};
