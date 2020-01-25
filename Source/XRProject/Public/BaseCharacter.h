// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "XRProject.h"
#include "GameFramework/Character.h"
#include "CharacterStatComponent.h"
#include "BaseCharacter.generated.h"

UENUM()
enum class ECharacterLoadState : uint8
{
	PREINIT,
	LOADING,
	READY,
};

UENUM()
enum class ECharacterLifeState : uint8
{
	SPAWN,
	PLAY,
	DEAD
};



                               

UCLASS()
class XRPROJECT_API ABaseCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	ABaseCharacter();

protected:
	virtual void BeginPlay() override;
	virtual void PostInitializeComponents() override;
public:	
	virtual void Tick(float DeltaTime) override;
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

public:
	UFUNCTION(BlueprintCallable)
		void SetCharacterLoadState(ECharacterLoadState NewLoadState);
	UFUNCTION(BlueprintCallable)
		void SetCharacterLifeState(ECharacterLifeState NewLifeState);
	UFUNCTION(BlueprintCallable)
		UCharacterStatComponent* GetCharacterStat() { return StatComponent; };
	UFUNCTION(BlueprintCallable)
		void OnDead() { SetCharacterLifeState(ECharacterLifeState::DEAD); };


private:
	UPROPERTY(VisibleAnywhere,BlueprintReadOnly,Meta=(AllowPrivateAccess = true))
		UCharacterStatComponent*	StatComponent;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Meta = (AllowPrivateAccess = true))
		ECharacterLoadState			CurrentLoadState;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Meta = (AllowPrivateAccess = true))
		ECharacterLifeState			CurrentLifeState;

};



