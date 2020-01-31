// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "PlayerCharacter.h"
#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "CharacterSlotWidget.generated.h"

/**
 * 
 */
UCLASS()
class XRPROJECT_API UCharacterSlotWidget : public UUserWidget
{
	GENERATED_BODY()
public:
	UCharacterSlotWidget(const FObjectInitializer& ObjectInitializer);
public:
	UPROPERTY(EditDefaultsOnly, BlueprintReadwrite, Category = "C_Info")
		APlayerCharacter* TargetCharacter;
	UPROPERTY(EditDefaultsOnly, BlueprintReadwrite, Category = "C_Info")
		int64 MySlotNumber;
public:
	UFUNCTION(BlueprintCallable)
		void SendCurrentSelectSlot();
	UFUNCTION(BlueprintCallable)
		int64 GetCharacterLevel();
	UFUNCTION(BlueprintCallable)
		FString GetCharacterName();

};
