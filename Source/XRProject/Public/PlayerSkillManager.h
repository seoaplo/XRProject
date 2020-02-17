// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataTable.h"
#include "UObject/NoExportTypes.h"
#include "PlayerSkillManager.generated.h"

class USkillCooldown;
class UPlayerSkill;
class UXRGameInstance;

/**
 * 
 */
UCLASS()
class XRPROJECT_API UPlayerSkillManager : public UObject
{
	GENERATED_BODY()

public:
	UPlayerSkillManager();
	virtual ~UPlayerSkillManager();

private:
	class UDataTable* SkillDataTable;
	UXRGameInstance* CurrentInstance;
public:
	UPROPERTY(VisibleAnywhere)
		TArray<UPlayerSkill*> SkillListForPlalyer;
	UPROPERTY(VisibleAnywhere)
		TArray<USkillCooldown*> CoolDownList;

public:
	void SetGameInstance(UXRGameInstance* GI);
	UPlayerSkill* CreateSkillFromID(int32 ID);
	UPlayerSkill* FindSkillFromList(TArray<UPlayerSkill*>& SkillList, int32 ID);
	int32 FindSkillFromCooldownList(int32 ID);
	UPlayerSkill* FindSkillFromListByName(TArray<UPlayerSkill*>& SkillList, FString& Name);
	void AddSkill(UPlayerSkill* Skill, bool bNeedCheckDuplication);
	void AddSkillToCooldownList(UPlayerSkill* Skill, bool AutoSetTimer);
	UDataTable* GetSkillDataTable();

	
};
