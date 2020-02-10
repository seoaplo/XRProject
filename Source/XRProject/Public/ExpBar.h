// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "ExpBar.generated.h"

/**
 * 
 */
UCLASS()
class XRPROJECT_API UExpBar : public UUserWidget
{
	GENERATED_BODY()
public:
	UPROPERTY(EditAnywhere, BlueprintReadwrite, Category = "C_Info")
		int TargetMaxExp;
	UPROPERTY(EditAnywhere, BlueprintReadwrite, Category = "C_Info")
		int TargetCurrentExp;
public:
	UFUNCTION(BlueprintCallable)
		void SetMaxExp(int Exp);
	UFUNCTION(BlueprintCallable)
		void SetCurrentExp(int Exp);
	UFUNCTION(BlueprintImplementableEvent)
		void Update();
};
