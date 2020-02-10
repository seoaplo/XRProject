// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "NonePlayerCharacter.h"
#include "BossHealthBar.generated.h"

/**
 * 
 */
UCLASS()
class XRPROJECT_API UBossHealthBar : public UUserWidget
{
	GENERATED_BODY()
public:
	UBossHealthBar(const FObjectInitializer& ObjectInitializer);
protected:
	UPROPERTY(EditAnywhere, BlueprintReadwrite, Category = "C_Info")
		ANonePlayerCharacter* Owner;
	UPROPERTY(EditAnywhere, BlueprintReadwrite, Category = "C_Info")
		int TargetMaxHP;
	UPROPERTY(EditAnywhere, BlueprintReadwrite, Category = "C_Info")
		int TargetCurrentHP;
	UPROPERTY(EditAnywhere, BlueprintReadwrite, Category = "C_Info")
		int TargetBeforeHP;
public:
	UFUNCTION(BlueprintCallable)
		void SetTargetNPC(ANonePlayerCharacter* Target);
	UFUNCTION(BlueprintCallable)
		void GetName(ANonePlayerCharacter* Target);
	UFUNCTION(BlueprintCallable)
		void SetMaxHp(int PlayerMaxHP);
	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable)
		void ApplyHp(int PlayerCurrentHP);
};
