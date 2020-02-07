// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "PlayerCharacter.h"
#include "SlotWidget.h"
#include "Blueprint/UserWidget.h"
#include "CharacterInfoWidget.generated.h"

/**
 * 
 */
UCLASS()
class XRPROJECT_API UCharacterInfoWidget : public UUserWidget
{
	GENERATED_BODY()
public:
	UCharacterInfoWidget(const FObjectInitializer& ObjectInitializer);
	static UCharacterInfoWidget* GetInstance() { return CharacterInfoInstance; }
private:
	static UCharacterInfoWidget* CharacterInfoInstance;
	USlotWidget* BodySlot;
	USlotWidget* HandSlot;
	USlotWidget* FootSlot;
	USlotWidget* WeaponSlot;
	bool bIsVisible;
public:
	USlotWidget* Slot[4];
	UFUNCTION(BlueprintCallable, Category = "MG_Function")
		void SetVisible(bool IsVisible);
	UFUNCTION(BlueprintCallable, Category = "MG_Function")
		bool GetVisible();
	UFUNCTION(BlueprintCallable, Category = "MG_Function")
		void SwitchVisible();
	UFUNCTION(BlueprintCallable, Category = "MG_Function")
		void SetSlot(USlotWidget* Body, USlotWidget* Hand, USlotWidget* Foot, USlotWidget* Weapon);
};
