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
	ALIVE,
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
		const ECharacterLoadState GetCharacterLoadState() { return CurrentLoadState; }
	UFUNCTION(BlueprintCallable)
		const ECharacterLifeState GetCharacterLifeState() { return CurrentLifeState; }


	UFUNCTION(BlueprintCallable)
	virtual	void SetCharacterLoadState(ECharacterLoadState NewLoadState);
	UFUNCTION(BlueprintCallable)
	virtual	void SetCharacterLifeState(ECharacterLifeState NewLifeState);

	//UFUNCTION(BlueprintCallable)
	//virtual	UCharacterStatComponent* GetCharacterStat() { return StatComponent; };

	UFUNCTION(BlueprintCallable)
	virtual	void OnDead() { SetCharacterLifeState(ECharacterLifeState::DEAD); };


	UFUNCTION(BlueprintCallable)
	virtual	void SetSkelResource(int32 SkeletalID, int32 AnimBPID);


	UFUNCTION(BlueprintCallable)
	virtual	void SetRemoteLocation(FVector remoteLocation);
	UFUNCTION(BlueprintCallable)
	virtual	void SetRemoteRotation(FRotator remoteRotator);

	UPROPERTY()
		int64 ObjectID;


protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Meta = (AllowPrivateAccess = true))
		ECharacterLoadState			CurrentLoadState;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Meta = (AllowPrivateAccess = true))
		ECharacterLifeState			CurrentLifeState;

	UPROPERTY()
		FVector RemoteLocation;
	UPROPERTY()
		FRotator RemoteRotator;

private:
	//UPROPERTY(VisibleAnywhere,BlueprintReadOnly,Meta=(AllowPrivateAccess = true))
	//	UCharacterStatComponent*	StatComponent;


};



