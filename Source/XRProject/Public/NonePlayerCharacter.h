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




USTRUCT(BlueprintType)
struct FMonsterTableRow : public FTableRowBase
{
	GENERATED_BODY()
public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Resource")
		FString MonsterName;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Resource")
		int32 MonsterSkeletalID;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Resource")
		int32 MonsterAnimBP;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Resource")
		int32 MonsterBT;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Resource")
		int32 MonsterBB;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Resource")
		float MonsterMaxHP;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Resource")
		float MonsterAttackMin;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Resource")
		float MonsterAttackMax;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Resource")
		float MonsterAttackRange;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Resource")
		float MonsterAttackSpeed;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Resource")
		float MonsterAttackDefence;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Resource")
		float MonsterSpeed;
};



USTRUCT(BlueprintType)
struct FMonsterSkillTableRow : public FTableRowBase
{
	GENERATED_BODY()
public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Resource")
		FString MonsterSkillID;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Resource")
		float DamageRatio;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Resource")
		int32 SkillType;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Resource")
		float Radius;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Resource")
		float Range;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Resource")
		float Speed;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Resource")
		float LifeTime;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Resource")
		float CoolDown;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Resource")
		bool IsKnockBack;
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

	UPROPERTY()
		class UDataTable* NPCDataTable;


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
		virtual void SetInBattle(bool battle);

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
		TArray<APlayerCharacter*> AttackOverlapList;


	UPROPERTY(BlueprintReadWrite, EditAnywhere)
		UNonePlayerCharacterAnimInstance* NpcAnim;



	UPROPERTY(BlueprintAssignable)
	FOnNpcReadyState OnNpcReady;

	virtual void ExcuteRecvNpcAction(InputStream& input);




	UFUNCTION(BlueprintCallable)
		virtual void NpcTakeDamaged(float setHP, class AController* EventInstigator, int32 AttackInstigatorID);






protected:
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



	UPROPERTY(BlueprintReadOnly, VisibleAnywhere, Meta = (AllowPrivateAccess = true))
		class AXRAIController* AICon;


	UPROPERTY(BlueprintReadWrite, EditAnywhere, Meta = (AllowPrivateAccess = true))
		bool bIsAttacking;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
		class UAISenseConfig_Hearing* HearingConfig;








	float SumSec = 0.f;

};
