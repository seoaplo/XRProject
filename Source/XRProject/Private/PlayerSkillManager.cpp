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

	UPlayerSkill* NewSkill = NewObject<UPlayerSkill>();

	
	NewSkill->SetSkillName(SkillInfo->SkillName);
	NewSkill->SetDamageMagnification(SkillInfo->DamageMagnification);
	NewSkill->SetIsKnockBack(SkillInfo->IsKnockBack);
	NewSkill->SetRequireStamina(SkillInfo->RequireStamina);
	NewSkill->SetCoolTime(SkillInfo->CoolTime);
	NewSkill->SetIconID(SkillInfo->IconID);
	NewSkill->SetID(ID);

	if (SkillInfo->SkillName == "GaiaCrush")
	{
		USkill_GaiaCrush* Gaia = Cast<USkill_GaiaCrush>(NewSkill);
		Gaia->SetMoveDistance(FCString::Atof(*(SkillInfo->ETC1)));
		Gaia->SetAffectRadius(FCString::Atof(*(SkillInfo->ETC2)));
		return Gaia;
	}
	else
		check(false);

	return NewSkill;
}

UPlayerSkill * UPlayerSkillManager::FindSkillFromList(TArray<UPlayerSkill*> SkillList, int32 ID)
{
	for (UPlayerSkill* Skill : SkillList)
	{
		if (Skill->GetID() == ID)
			return Skill;
	}

	return nullptr;
}

void UPlayerSkillManager::AddSkill(TArray<UPlayerSkill*> SkillList, UPlayerSkill* Skill, bool bNeedCheckDuplication = false)
{
	if (bNeedCheckDuplication)
	{
		for (UPlayerSkill* CurSkill : SkillList)
		{
			if (CurSkill == Skill)
				return;
		}
	}
	SkillList.Add(Skill);
}