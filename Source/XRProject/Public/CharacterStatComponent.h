// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "XRProject.h"
#include "Components/ActorComponent.h"
#include "InputStream.h"
#include "HealthBarWidget.h"
#include "CharacterStatComponent.generated.h"

/*
//ÀÛ¼ºÀÚ Á¶ÀçÁø
*/
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnCharacterStatChange);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnCharacterHPIsZero);


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class XRPROJECT_API UCharacterStatComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UCharacterStatComponent();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;
private:
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;


public:
	UFUNCTION(BlueprintCallable)
		const float GetCurrentHP() const;
	UFUNCTION(BlueprintCallable)
		void SetCurrentHP(float NewHP);
	UFUNCTION(BlueprintCallable)
		const float GetHPRatio() const;

	UFUNCTION(BlueprintCallable)
		const float GetMaxHP() const;
	UFUNCTION(BlueprintCallable)
		void SetMaxHP(float NewHP);

	UFUNCTION(BlueprintCallable)
		const float GetAttack_Min() const;
	UFUNCTION(BlueprintCallable)
		void SetAttack_Min(float NewAttack_Min);


	UFUNCTION(BlueprintCallable)
		const float GetAttack_Max() const;
	UFUNCTION(BlueprintCallable)
		void SetAttack_Max(float NewAttack_Max);


	UFUNCTION(BlueprintCallable)
		const float GetAttack_Range() const;
	UFUNCTION(BlueprintCallable)
		void SetAttack_Range(float NewAttack_Range);


	UFUNCTION(BlueprintCallable)
		const float GetAttack_Speed() const;
	UFUNCTION(BlueprintCallable)
		void SetAttack_Speed(float NewAttack_Speed);


	UFUNCTION(BlueprintCallable)
		const float GetSpeed() const;
	UFUNCTION(BlueprintCallable)
		void SetSpeed(float NewSpeed);


	UFUNCTION(BlueprintCallable)
		const float GetDefence()const;
	UFUNCTION(BlueprintCallable)
		void SetDefence(float NewDefence);


	UFUNCTION(BlueprintCallable)
		void SetCharacterName(FString NewName) { CharacterName = NewName; }
	UFUNCTION(BlueprintCallable)
		FString GetCharacterName() { return CharacterName; }

	virtual bool GetStatDataFromServer(InputStream& input);

	UFUNCTION(BlueprintCallable)
		/*HP µ¡¼À*/
		void AddHP(float Value);
	UFUNCTION(BlueprintCallable)
		void SubtractHP(float Value);


	UPROPERTY(BlueprintAssignable)
		FOnCharacterStatChange	OnStatChange;
	UPROPERTY(BlueprintAssignable)
		FOnCharacterHPIsZero	OnHPZero;

private:	
	// Called every frame

	UPROPERTY(Transient, BlueprintReadOnly, Meta = (AllowPrivateAccess = true))
		float Current_HP = 0.f;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Meta = (AllowPrivateAccess = true))
		float Max_HP = 0.f;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Meta = (AllowPrivateAccess = true))
		float Attack_Min = 0.f;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Meta = (AllowPrivateAccess = true))
		float Attack_Max = 0.f;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Meta = (AllowPrivateAccess = true))
		float Attack_Range = 0.f;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Meta = (AllowPrivateAccess = true))
		float Attack_Speed = 0.f;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Meta = (AllowPrivateAccess = true))
		float Speed = 0.f;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Meta = (AllowPrivateAccess = true))
		float Defence = 0.f;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Meta = (AllowPrivateAccess = true))
		FString CharacterName;
};
