// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include <vector>
#include "UserCharacter.h"
#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "CharacterSelectWidget.generated.h"

/**
 * 
 */
UCLASS()
class XRPROJECT_API UCharacterSelectWidget : public UUserWidget
{
	GENERATED_BODY()
public:
	std::vector<AUserCharacter*> CharacterList;
	UPROPERTY(EditDefaultsOnly, BlueprintReadwrite, Category = "C_Info")
		int64 CharacterCount;
public:
	UFUNCTION(BlueprintCallable)
		void AddCharacter(AUserCharacter* Character);
	UFUNCTION(BlueprintCallable)
		void DeleteCharacter(int Num);
	UFUNCTION(BlueprintCallable)
		AUserCharacter* GetCharacter(int Num);
	UFUNCTION(BlueprintCallable)
		void CharacterSelectionRequest(int32 Numder);
	UFUNCTION(BlueprintCallable)
		void CharacterCreateRequest(FText Name, int32 Sex);
	UFUNCTION(BlueprintCallable)
		void CharacterDeleteRequest(int32 Numder);
	UFUNCTION(BlueprintCallable)
		void ReturnLoginServer();
	UFUNCTION(BlueprintImplementableEvent)
		void CharacterLoginFail();
	UFUNCTION(BlueprintImplementableEvent)
		void CharacterCreateResult(bool bResult);
	UFUNCTION(BlueprintImplementableEvent)
		void CharacterDeleteResult(bool bResult);
	UFUNCTION(BlueprintImplementableEvent)
		void BlockButton();
	UFUNCTION(BlueprintImplementableEvent)
		void UpdateList();
};
