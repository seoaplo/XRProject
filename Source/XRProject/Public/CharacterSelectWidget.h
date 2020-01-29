// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include <vector>
#include "PlayerCharacter.h"
#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "CharacterSelectWidget.generated.h"

const int kMaxCharacterSlot = 5;

UCLASS()
class XRPROJECT_API UCharacterSelectWidget : public UUserWidget
{
	GENERATED_BODY()
public:
	UCharacterSelectWidget(const FObjectInitializer& ObjectInitializer);
	std::vector<APlayerCharacter*> CharacterList;
	UPROPERTY(EditDefaultsOnly, BlueprintReadwrite, Category = "C_Info")
		int64 CharacterCount;
public:
	UFUNCTION(BlueprintCallable)
		void AddCharacter(int Num, APlayerCharacter* Character);
	UFUNCTION(BlueprintCallable)
		void DeleteCharacter(int Num);
	UFUNCTION(BlueprintCallable)
		APlayerCharacter* GetCharacter(int Num);
	UFUNCTION(BlueprintCallable)
		void CharacterSelectionRequest(int Numder);
	UFUNCTION(BlueprintCallable)
		void CharacterCreateRequest(int SlotNum, FText Name, int FaceID, int HairID, int Gender);
	UFUNCTION(BlueprintCallable)
		void CharacterDeleteRequest(int Numder);
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
