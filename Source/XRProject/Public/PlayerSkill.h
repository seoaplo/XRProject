// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "Engine/DataTable.h"
#include <vector>
#include "Engine/StaticMeshActor.h"
#include "PlayerSkill.generated.h"

class APlayerCharacter;
class ANonePlayerCharacter;

/**
 * 
 */
UCLASS()
class XRPROJECT_API UPlayerSkill : public UObject
{
	GENERATED_BODY()

public:
	UPlayerSkill();
	~UPlayerSkill();


protected:
	UPROPERTY(VisibleAnyWhere)
	int32			SkillID;
	UPROPERTY(VisibleAnyWhere)
	FString			SkillName;
	UPROPERTY(VisibleAnyWhere)
	float			DamageMagnification;
	UPROPERTY(VisibleAnyWhere)
	int32			IsKnockBack;
	UPROPERTY(VisibleAnyWhere)
	float			RequireStamina;
	UPROPERTY(VisibleAnyWhere)
	float			CoolTime;
	UPROPERTY(VisibleAnyWhere)
	int32			IconID;
	UPROPERTY()
	APlayerCharacter* OwnerPlayer;

public:
	virtual void Play(APlayerCharacter* Character);
	virtual bool End(APlayerCharacter* Character);
	virtual bool ConditionCheck(APlayerCharacter* Character);
	virtual bool CooldownCheck(APlayerCharacter* Character, UPlayerSkill* Skill);
public:
	void SetID(int32 ID) { SkillID = ID; }
	void SetSkillName(FString SkillName) { this->SkillName = SkillName; }
	void SetDamageMagnification(float Mag) { DamageMagnification = Mag; }
	void SetIsKnockBack(int32 IsKnockBack) { this->IsKnockBack = IsKnockBack; }
	void SetCoolTime(float CoolDown) { CoolTime = CoolDown; }
	void SetIconID(int32 ID) { IconID = ID; }
	void SetRequireStamina(float Stamina) { RequireStamina = Stamina; }

	int32 GetID() { return SkillID; }
	FString GetSkillName() { return SkillName; }
	float GetDamageMagnification() { return DamageMagnification; }
	int32 GetIsKnockBack() { return IsKnockBack; }
	float GetCoolTime() { return CoolTime; }
	int32 GetIconID() { return IconID; }
	float GetRequireStamina() { return RequireStamina; }
	//inline bool GetIsActiveMove() { return bIsActiveMove; }

protected:

};


UCLASS()
class XRPROJECT_API USkill_GaiaCrush : public UPlayerSkill
{
	const float kCircleDistance = 300.0f;
	//ETC1 : 비행거리(MoveDistance) || ETC2 : 원형범위(AffectRadius)
	GENERATED_BODY()

public:
	USkill_GaiaCrush();
	~USkill_GaiaCrush();

private:
	UPROPERTY(VisibleAnyWhere)
	float MoveDistance; //공중도약시 이동거리
	UPROPERTY(VisibleAnyWhere)
	float AffectRadius; //공격이 미치는 원형 범위
		UStaticMesh* RadMesh;

		std::vector<ANonePlayerCharacter*> AttackOverlapList;
	UPROPERTY()
		AStaticMeshActor* RadiusChecker;


public:
	UFUNCTION()
		virtual void Play(APlayerCharacter* Character) override;
	UFUNCTION()
		virtual bool End(APlayerCharacter* Character) override;
	UFUNCTION()
		virtual bool ConditionCheck(APlayerCharacter* Character) override;
	UFUNCTION()
		void RadiusOverlapEvent(UPrimitiveComponent * OverlappedComp, AActor * OtherActor,
		UPrimitiveComponent * OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult & SweepResult);
	UFUNCTION()
		void GaiaTargetCheck(APlayerCharacter* Character);
	//void Set();

public:
	void SetMoveDistance(float MoveDistance);
	void SetAffectRadius(float AffectRadius);
	float GetMoveDistance();
	float GetAffectRadius();
};

UCLASS()
class XRPROJECT_API USkill_Berserk : public UPlayerSkill
{
	GENERATED_BODY()

public:
	USkill_Berserk();
	~USkill_Berserk();


public:
	UFUNCTION()
		virtual void Play(APlayerCharacter* Character) override;
	UFUNCTION()
		virtual bool End(APlayerCharacter* Character) override;
	UFUNCTION()
		virtual bool ConditionCheck(APlayerCharacter* Character) override;

};


USTRUCT(BlueprintType)
struct FPlayerSkillResources : public FTableRowBase
{
	GENERATED_BODY()

public:
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "C_SkillRes")
		FString SkillName;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "C_SkillRes")
		FString SkillNameKorean;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "C_SkillRes")
		float DamageMagnification;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "C_SkillRes")
		int32 IsKnockBack;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "C_SkillRes")
		float RequireStamina;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "C_SkillRes")
		float CoolTime;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "C_SkillRes")
		int32 IconID;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "C_SkillRes")
		FString ToolTip;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "C_SkillRes")
		FString MoveDistance;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "C_SkillRes")
		FString AffectRadius;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "C_SkillRes")
		FString DMG_Increase;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "C_SkillRes")
		FString DEF_Increase;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "C_SkillRes")
		FString TimeDuration;
};
