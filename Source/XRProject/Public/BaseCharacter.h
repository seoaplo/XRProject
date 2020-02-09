// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "XRProject.h"
#include "GameFramework/Character.h"
#include "CharacterStatComponent.h"
#include "Engine/DataTable.h"
#include "BaseCharacter.generated.h"



/*
//작성자 조재진
*/
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


struct FCharacterSizeInfo
{
public:
	FTransform LocalTransform;
	float CapsuleHeight;
	float CapsuleRad;
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
	
	class UDataTable* CharacterDataTable;

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

	FCharacterSizeInfo FindCharacterSizeFromDataTable(int32 ID);

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



USTRUCT(BlueprintType)
struct FCharacterResource : public FTableRowBase
{
	GENERATED_BODY()
public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "C_CharacterResource")
		FString Name;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "C_CharacterResource")
		FString LocalTransform;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "C_CharacterResource")
		float CapsuleHeight;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "C_CharacterResource")
		float CapsuleRad;
};
