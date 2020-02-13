// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "PlayerSkill.h"
#include "Engine/DataTable.h"
#include "UObject/NoExportTypes.h"
#include <initializer_list>
#include "PlayerSkillManager.generated.h"

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
public:
	UPROPERTY(VisibleAnywhere)
	TArray<UPlayerSkill*> SkillListForPlalyer;

	UPlayerSkill* CreateSkillFromID(int32 ID);
	UPlayerSkill* FindSkillFromList(TArray<UPlayerSkill*>& SkillList, int32 ID);
	UPlayerSkill* FindSkillFromListByName(TArray<UPlayerSkill*>& SkillList, FString& Name);
	void AddSkill(TArray<UPlayerSkill*>& SkillList, UPlayerSkill* Skill, bool bNeedCheckDuplication);
	UDataTable* GetSkillDataTable();

	
};
