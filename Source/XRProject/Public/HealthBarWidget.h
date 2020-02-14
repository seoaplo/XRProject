// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "HealthBarWidget.generated.h"

/**
 * 
 */
UCLASS()
class XRPROJECT_API UHealthBarWidget : public UUserWidget
{
	GENERATED_BODY()
public:
	UHealthBarWidget(const FObjectInitializer& ObjectInitializer);
public:
	static UHealthBarWidget* HealthBarInstance;
	static UHealthBarWidget* GetInatance() { return HealthBarInstance; };
protected:
	UPROPERTY(EditAnywhere, BlueprintReadwrite, Category = "C_Info")
		int TargetMaxHP;
	UPROPERTY(EditAnywhere, BlueprintReadwrite, Category = "C_Info")
		int TargetCurrentHP;
	UPROPERTY(EditAnywhere, BlueprintReadwrite, Category = "C_Info")
		int TargetBeforeHP;
	UPROPERTY(EditAnywhere, BlueprintReadwrite, Category = "C_Info")
		int TargetMaxStamina;
	UPROPERTY(EditAnywhere, BlueprintReadwrite, Category = "C_Info")
		int TargetCurrentStamina;
public:
	UFUNCTION(BlueprintCallable)
		void SetInatance();
	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable)
		void SetMaxHp(int PlayerMaxHP);
	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable)
		void ApplyHp(int PlayerCurrentHP);
	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable)
		void SetMaxStamina(int PlayerMaxStamina);
	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable)
		void ApplyStamina(int PlayerCurrentStamina);

};
