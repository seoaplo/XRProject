// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "XRProject.h"
#include "BaseCharacter.h"
#include "CharacterStatComponent.h"
#include "Perception/AIPerceptionComponent.h"
#include "NonePlayerCharacter.generated.h"

UENUM()
enum class NPCType : uint8
{
	Alli,
	Enermy,
};

/**
 * 
 */
UCLASS()
class XRPROJECT_API ANonePlayerCharacter : public ABaseCharacter
{
	GENERATED_BODY()


		
		
		
		///오버라이드///
public:
	ANonePlayerCharacter();
	virtual void	BeginPlay()	override;
	virtual void	PostInitializeComponents()	override;
	virtual void	Tick(float DeltaTime)	override;
	virtual void	PossessedBy(AController* Cntr) override;
	virtual float	TakeDamage(float DamageAmount, struct FDamageEvent const& DamageEvent, class AController* EventInstigator, AActor* DamageCauser) override;

protected:
private:




	///사용자 정의 함수///
public:
	UFUNCTION()
		void DetectTarget(const TArray<AActor*>& DetectingPawn);

protected:
private:





	///사용자 정의 변수///
public:
	UFUNCTION(BlueprintCallable)
	ABaseCharacter* GetTarget() const { return Target; }
	UFUNCTION(BlueprintCallable)
	void SetTarget(ABaseCharacter* NewTarget) { Target = NewTarget; }

protected:
private:
	UPROPERTY(BlueprintReadOnly, VisibleAnywhere, Meta = (AllowPrivateAccess = true))
		TArray<ABaseCharacter*>		AggroList;
	UPROPERTY(BlueprintReadOnly,VisibleAnywhere,Meta =(AllowPrivateAccess = true))
		ABaseCharacter*				Target;
	UPROPERTY(BlueprintReadOnly, VisibleAnywhere, Meta = (AllowPrivateAccess = true))
		UCharacterStatComponent*	EnermyStatComponent;
	UPROPERTY(BlueprintReadOnly, VisibleAnywhere, Meta = (AllowPrivateAccess = true))
		UAIPerceptionComponent*		EnermyPerceptionComponent;
	
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Meta = (AllowPrivateAccess = true))
	bool bIsAttacking;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
		class UAISenseConfig_Sight* SightConfig;

};
