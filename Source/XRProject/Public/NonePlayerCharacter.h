// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "XRProject.h"
#include "BaseCharacter.h"
#include "CharacterStatComponent.h"
#include "Engine/DataTable.h"
#include "GenericTeamAgentInterface.h"
#include "Perception/AIPerceptionComponent.h"
#include "PlayerCharacter.h"
#include "NetworkManager.h"
#include "NonePlayerCharacterAnimInstance.h"
#include "NonePlayerCharacter.generated.h"
/*
//작성자 조재진
*/


UENUM()
enum class NPCType : uint8
{
	Alli,
	Enermy,
};

USTRUCT(Atomic, BlueprintType)
struct FAggroContainer
{
	GENERATED_BODY()
public:
	UPROPERTY(EditAnywhere)
		ABaseCharacter* AggroCharacter;
	UPROPERTY(EditAnywhere)
		int32 AggroGaze = 0;
};






DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnNpcHitReactDelegate);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnNpcReadyState);



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
		virtual void DetectTarget(const TArray<AActor*>& DetectingPawn);
	virtual void SetCharacterLoadState(ECharacterLoadState NewState) override;
	virtual void SetCharacterLifeState(ECharacterLifeState NewState) override;
	virtual void OnDead() override;
	virtual void GetNPCInfoFromTable(int32 NpcID);

	UFUNCTION(BlueprintCallable)
		virtual void NpcLoadStart(int32 npcID);


protected:
private:





	///사용자 정의 변수///
public:
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
		bool bInBattle;
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
		bool bCanNextAttack;
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
		bool bCanHitReaction;



	UFUNCTION(BlueprintCallable)
		ABaseCharacter* GetTarget() const { return Target; }

	UFUNCTION(BlueprintCallable)
		virtual void SetTarget(ABaseCharacter* NewTarget) { Target = NewTarget; }



	UFUNCTION(BlueprintCallable)
		virtual void AttackCheck(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
			UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UFUNCTION(BlueprintCallable)
		virtual void SendAction(int32 ActionID, FVector Location, FRotator Rotator);

	UFUNCTION(BlueprintCallable)
		virtual void SendDamage(int32 ActionID, FVector Location, FRotator Rotator, AActor* OtherActor);

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
		TArray<int> ReadySkillList;


	UFUNCTION(BlueprintCallable)
		virtual void SetInBattle(bool battle);

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
		TArray<APlayerCharacter*> AttackOverlapList;


	UPROPERTY(BlueprintReadWrite, EditAnywhere)
		UNonePlayerCharacterAnimInstance* NpcAnim;





	UPROPERTY(BlueprintAssignable)
	FOnNpcReadyState OnNpcReady;

	virtual void ExcuteRecvNpcAction(InputStream& input);



	UFUNCTION(BlueprintCallable)
		void SetOnSkillQueue(int32 index);

	UFUNCTION(BlueprintCallable)
		virtual void NpcTakeDamaged(float setHP, class AController* EventInstigator, int32 AttackInstigatorID);



	UPROPERTY(BlueprintReadWrite,VisibleAnywhere)
	int32 CurrentAttackAction;


protected:

	UPROPERTY(BlueprintReadOnly, VisibleAnywhere, Meta = (AllowPrivateAccess = true))
		class AXRAIController* AICon;

private:
	UPROPERTY(BlueprintReadOnly, VisibleAnywhere, Meta = (AllowPrivateAccess = true))
		TMap<ABaseCharacter*,int>		AggroList;


	UPROPERTY(BlueprintReadOnly, VisibleAnywhere, Meta = (AllowPrivateAccess = true))
		ABaseCharacter* Target;
	UPROPERTY(BlueprintReadOnly, VisibleAnywhere, Meta = (AllowPrivateAccess = true))
		UCharacterStatComponent* EnermyStatComponent;


	UPROPERTY(BlueprintReadOnly, VisibleAnywhere, Meta = (AllowPrivateAccess = true))
		int32 SkelID;
	UPROPERTY(BlueprintReadOnly, VisibleAnywhere, Meta = (AllowPrivateAccess = true))
		int32 AnimBPID;
	UPROPERTY(BlueprintReadOnly, VisibleAnywhere, Meta = (AllowPrivateAccess = true))
		int32 BTID;
	UPROPERTY(BlueprintReadOnly, VisibleAnywhere, Meta = (AllowPrivateAccess = true))
		int32 BBID;



	UPROPERTY(BlueprintReadWrite, EditAnywhere, Meta = (AllowPrivateAccess = true))
		bool bIsAttacking;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
		class UAISenseConfig_Hearing* HearingConfig;

	float SumSec = 0.f;

};
