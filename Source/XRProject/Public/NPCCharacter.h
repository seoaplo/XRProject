// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "CharacterStatComponent.h"
#include "NPCCharacter.generated.h"

enum class ENPCType
{
	Alliance,
	Enermy,
};


UCLASS()
class XRPROJECT_API ANPCCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	ANPCCharacter();

protected:
	virtual void BeginPlay() override;
public:	
	virtual void Tick(float DeltaTime) override;
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

public:
	UCharacterStatComponent* GetCharacterStat() { return CharacterStat; }

private:
	UPROPERTY(VisibleAnywhere,BlueprintReadOnly,meta = (AllowPrivateAccess = true))
	UCharacterStatComponent* CharacterStat;


	
	
};




