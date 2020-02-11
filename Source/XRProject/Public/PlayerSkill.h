// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "Engine/DataTable.h"
#include "PlayerSkill.generated.h"

class APlayerCharacter;


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
	int32			SkillID;
	FString			SkillName;
	float			DamageMagnification;
	int32			IsKnockBack;
	float			RequireStamina;
	float			CoolTime;
	int32			IconID;

public:
	virtual void Play(APlayerCharacter* Character);
	virtual bool ConditionCheck(APlayerCharacter* Character);

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

};


UCLASS()
class XRPROJECT_API USkill_GaiaCrush : public UPlayerSkill
{
	//ETC1 : 비행거리(MoveDistance) || ETC2 : 원형범위(AffectRadius)
	GENERATED_BODY()

public:
	USkill_GaiaCrush();
	~USkill_GaiaCrush();

private:
	float MoveDistance; //공중도약시 이동거리
	float AffectRadius; //공격이 미치는 원형 범위

public:
	virtual void Play(APlayerCharacter* Character) override;
	virtual bool ConditionCheck(APlayerCharacter* Character) override;
	//void Set();

public:
	void SetMoveDistance(float MoveDistance);
	void SetAffectRadius(float AffectRadius);
	float GetMoveDistance();
	float GetAffectRadius();
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
		FString ETC1;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "C_SkillRes")
		FString ETC2;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "C_SkillRes")
		FString ETC3;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "C_SkillRes")
		FString ETC4;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "C_SkillRes")
		FString ETC5;
};
