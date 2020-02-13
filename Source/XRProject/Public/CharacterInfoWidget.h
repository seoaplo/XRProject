// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "PlayerCharacter.h"
#include "SlotWidget.h"
#include "Blueprint/UserWidget.h"
#include "CharacterInfoWidget.generated.h"

UENUM(BlueprintType)
enum class EStateType : uint8
{
	Name			UMETA(DisplayName = "Name"),
	LEVEL			UMETA(DisplayName = "LEVEL"),
	Gender			UMETA(DisplayName = "Gender"),
	MaxHP			UMETA(DisplayName = "MaxHP"),
	MaxStamina		UMETA(DisplayName = "MaxStamina"),
	ATK_Min			UMETA(DisplayName = "ATK_Min"),
	ATK_Max			UMETA(DisplayName = "ATK_Max"),
	ATK_Range		UMETA(DisplayName = "ATK_Range"),
	ATK_Speed		UMETA(DisplayName = "ATK_Speed"),
	DEF				UMETA(DisplayName = "DEF"),
	STR				UMETA(DisplayName = "STR"),
	DEX				UMETA(DisplayName = "DEX"),
	INT				UMETA(DisplayName = "INT"),
	SPEED			UMETA(DisplayName = "SPEED"),
};

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
public:
	void SetSlotInfo();
	UFUNCTION(BlueprintCallable, Category = "MG_Function")
		void SetVisible(bool IsVisible);
	UFUNCTION(BlueprintCallable, Category = "MG_Function")
		bool GetVisible();
	UFUNCTION(BlueprintCallable, Category = "MG_Function")
		void SwitchVisible();
	UFUNCTION(BlueprintCallable, Category = "MG_Function")
		void SetSlot(USlotWidget* Body, USlotWidget* Hand, USlotWidget* Foot, USlotWidget* Weapon);
	UFUNCTION(BlueprintCallable, Category = "MG_Function")
		FText GetTargetState(EStateType Type);
	UFUNCTION(BlueprintImplementableEvent)
		void SetCharacterInfo();
};
