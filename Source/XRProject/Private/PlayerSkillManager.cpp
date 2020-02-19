// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerSkillManager.h"
#include "PlayerSkill.h"
#include "SkillCooldown.h"
#include "XRGameInstance.h"

UPlayerSkillManager::UPlayerSkillManager()
{
	FString SkillDataPath = TEXT("DataTable'/Game/Resources/DataTable/PlayerSkillTable.PlayerSkillTable'");
	static ConstructorHelpers::FObjectFinder<UDataTable> DT_MUSASKILL(*SkillDataPath);
	if (DT_MUSASKILL.Succeeded()) SkillDataTable = DT_MUSASKILL.Object;
	//CurrentInstance = GI;
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
		USkill_Berserk* Bsk = NewObject<USkill_Berserk>();
		Bsk->SetSkillName(SkillInfo->SkillName);
		Bsk->SetDamageMagnification(SkillInfo->DamageMagnification);
		Bsk->SetIsKnockBack(SkillInfo->IsKnockBack);
		Bsk->SetRequireStamina(SkillInfo->RequireStamina);
		Bsk->SetCoolTime(SkillInfo->CoolTime);
		Bsk->SetIconID(SkillInfo->IconID);
		Bsk->SetID(ID);

		return Bsk;
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

int32 UPlayerSkillManager::FindSkillFromCooldownList(int32 ID)
{
	for (int ii = 0 ; ii < CoolDownList.Num(); ii++)
	{
		if (CoolDownList[ii]->GetID() == ID)
		{
			XRLOG(Warning, TEXT("Skill CoolD Found, Index : %d"), ii);
			return ii;
		}
	}
	return -1;
}

UPlayerSkill * UPlayerSkillManager::FindSkillFromListByName(TArray<UPlayerSkill*>& SkillList, FString& Name)
{
	//UPlayerSkill* Skill = *(SkillList.FindByKey(Name));
	for (UPlayerSkill* Skill : SkillList)
	{
		if (Skill->GetSkillName() == Name)
		{
			return Skill;
		}
	}

	XRLOG(Warning, TEXT("Skill not found"));

	check(false);

	return nullptr;
}

void UPlayerSkillManager::AddSkill(UPlayerSkill* Skill, bool bNeedCheckDuplication)
{
	if (bNeedCheckDuplication)
	{
		for (UPlayerSkill* CurSkill : SkillListForPlalyer)
		{
			if (CurSkill == Skill)
				return;
		}
	}

	if (Skill == nullptr)
		return;

	SkillListForPlalyer.Add(Skill);

}

UDataTable* UPlayerSkillManager::GetSkillDataTable()
{
	return SkillDataTable;
}


void UPlayerSkillManager::AddSkillToCooldownList(UPlayerSkill* Skill, bool AutoSetTimer)
{
	check(Skill);

	USkillCooldown* Cooldown = NewObject<USkillCooldown>();
	Cooldown->SetCD(Skill->GetID(), Skill->GetCoolTime(), CurrentInstance);
	int32 Idx = CoolDownList.Add(Cooldown);

	if (AutoSetTimer)
	{
		CoolDownList[Idx]->SetTimer();
	}
}

void UPlayerSkillManager::SetGameInstance(UXRGameInstance* GI)
{
	CurrentInstance = GI;
}