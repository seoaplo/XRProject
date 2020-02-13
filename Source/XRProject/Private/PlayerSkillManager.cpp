// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerSkillManager.h"
#include "XRGameInstance.h"

UPlayerSkillManager::UPlayerSkillManager()
{
	FString SkillDataPath = TEXT("DataTable'/Game/Resources/DataTable/PlayerSkillTable.PlayerSkillTable'");
	static ConstructorHelpers::FObjectFinder<UDataTable> DT_MUSASKILL(*SkillDataPath);
	if (DT_MUSASKILL.Succeeded()) SkillDataTable = DT_MUSASKILL.Object;
}

UPlayerSkillManager::~UPlayerSkillManager()
{

}

UPlayerSkill * UPlayerSkillManager::CreateSkillFromID(int32 ID)
{

	FPlayerSkillResources* SkillInfo = SkillDataTable->FindRow<FPlayerSkillResources>(FName(*(FString::FromInt(ID))), TEXT("ST"));
	
	check(SkillInfo);

	if (SkillInfo->SkillName == "GaiaCrush")
	{
		USkill_GaiaCrush* Gaia = NewObject<USkill_GaiaCrush>();
		Gaia->SetMoveDistance(FCString::Atof(*(SkillInfo->MoveDistance)));
		Gaia->SetAffectRadius(FCString::Atof(*(SkillInfo->AffectRadius)));

		Gaia->SetSkillName(SkillInfo->SkillName);
		Gaia->SetDamageMagnification(SkillInfo->DamageMagnification);
		Gaia->SetIsKnockBack(SkillInfo->IsKnockBack);
		Gaia->SetRequireStamina(SkillInfo->RequireStamina);
		Gaia->SetCoolTime(SkillInfo->CoolTime);
		Gaia->SetIconID(SkillInfo->IconID);
		Gaia->SetID(ID);
		return Gaia;
	}
	else if (SkillInfo->SkillName == "Berserk")
	{
		return nullptr;
	}

	return nullptr;
}

UPlayerSkill * UPlayerSkillManager::FindSkillFromList(TArray<UPlayerSkill*>& SkillList, int32 ID)
{
	for (UPlayerSkill* Skill : SkillList)
	{
		if (Skill->GetID() == ID)
			return Skill;
	}
	return nullptr;
}

UPlayerSkill * UPlayerSkillManager::FindSkillFromListByName(TArray<UPlayerSkill*>& SkillList, FString& Name)
{
	//UPlayerSkill* Skill = *(SkillList.FindByKey(Name));
	for (UPlayerSkill* Skill : SkillList)
	{
		if (Skill->GetSkillName() == Name)
			return Skill;
	}

	XRLOG(Warning, TEXT("Skill not found"));

	check(false);

	return nullptr;
}

void UPlayerSkillManager::AddSkill(TArray<UPlayerSkill*>& SkillList, UPlayerSkill* Skill, bool bNeedCheckDuplication)
{
	if (bNeedCheckDuplication)
	{
		for (UPlayerSkill* CurSkill : SkillList)
		{
			if (CurSkill == Skill)
				return;
		}
	}

	if (Skill == nullptr)
		return;

	SkillList.Add(Skill);
}

UDataTable* UPlayerSkillManager::GetSkillDataTable()
{
	return SkillDataTable;
}