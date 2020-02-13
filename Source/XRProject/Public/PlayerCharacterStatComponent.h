// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "CharacterStatComponent.h"
#include "PlayerCharacterStatComponent.generated.h"

/**
 * 
 */
UCLASS()
class XRPROJECT_API UPlayerCharacterStatComponent : public UCharacterStatComponent
{
	GENERATED_BODY()
	
private:
	UPROPERTY(BlueprintReadOnly, Meta = (AllowPrivateAccess = true))
		float MaxStamina;
	UPROPERTY(Transient, BlueprintReadOnly, Meta = (AllowPrivateAccess = true))
		float CurrentStamina;
	UPROPERTY(BlueprintReadOnly, Meta = (AllowPrivateAccess = true))
		int32 MaxExp;
	UPROPERTY(Transient, BlueprintReadOnly, Meta = (AllowPrivateAccess = true))
		int32 CurrentExp;
	UPROPERTY(BlueprintReadOnly, Meta = (AllowPrivateAccess = true))
		int32 Level;
	UPROPERTY(BlueprintReadOnly, Meta = (AllowPrivateAccess = true))
		int32 STR;
	UPROPERTY(BlueprintReadOnly, Meta = (AllowPrivateAccess = true))
		int32 DEX;
	UPROPERTY(BlueprintReadOnly, Meta = (AllowPrivateAccess = true))
		int32 INT;
	UPROPERTY(BlueprintReadOnly, Meta = (AllowPrivateAccess = true))
		int32 Gender; //0 : Male , 1 : Female



public:

	UFUNCTION(BlueprintCallable, Category = "C_StatFunction")
		const float GetCurrentStamina() const;
	UFUNCTION(BlueprintCallable, Category = "C_StatFunction")
		void SetCurrentStamina(float Stamina);
	UFUNCTION(BlueprintCallable, Category = "C_StatFunction")
		void AddExp(int32 Value);
	UFUNCTION(BlueprintCallable, Category = "C_StatFunction")
		void SubtractExp(int32 Value);
	UFUNCTION(BlueprintCallable, Category = "C_StatFunction")
		int32 GetCurrentExp();
	UFUNCTION(BlueprintCallable, Category = "C_StatFunction")
		void SetCurrentExp(int32 Stamina);
	UFUNCTION(BlueprintCallable, Category = "C_StatFunction")
		int32 GetMaxExp();
	UFUNCTION(BlueprintCallable, Category = "C_StatFunction")
		void SetMaxExp(int32 Stamina);
	UFUNCTION(BlueprintCallable, Category = "C_StatFunction")
		/*Stamina µ¡¼À*/
		void AddStamina(float Value);
	UFUNCTION(BlueprintCallable, Category = "C_StatFunction")
		void SubtractStamina(float Value);
	UFUNCTION(BlueprintCallable, Category = "C_StatFunction")
		void SetMaxStamina(float Stamina);
	UFUNCTION(BlueprintCallable, Category = "C_StatFunction")
		float GetMaxStamina();
	UFUNCTION(BlueprintCallable, Category = "C_StatFunction")
		int32 GetLevel();
	UFUNCTION(BlueprintCallable, Category = "C_StatFunction")
		int GetSTR();
	UFUNCTION(BlueprintCallable, Category = "C_StatFunction")
		int GetDEX();
	UFUNCTION(BlueprintCallable, Category = "C_StatFunction")
		int GetINT();
	UFUNCTION(BlueprintCallable, Category = "C_StatFunction")
		int GetGender();
	UFUNCTION(BlueprintCallable, Category = "C_StatFunction")
		void SetLevel(int32 Level_);
	UFUNCTION(BlueprintCallable, Category = "C_StatFunction")
		void SetSTR(int32 STR_);
	UFUNCTION(BlueprintCallable, Category = "C_StatFunction")
		void SetDEX(int32 DEX_);
	UFUNCTION(BlueprintCallable, Category = "C_StatFunction")
		void SetINT(int32 INT_);
	UFUNCTION(BlueprintCallable, Category = "C_StatFunction")
		void SetGender(int32 Gender_);


	virtual bool GetStatDataFromServer(InputStream& input) override;
};
