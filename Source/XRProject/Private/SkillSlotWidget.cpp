// Fill out your copyright notice in the Description page of Project Settings.


#include "SkillSlotWidget.h"
#include "XRGameInstance.h"

bool USkillSlotWidget::IsEmpty()
{
	return !(bool)TargetSkill;
}

FString USkillSlotWidget::GetName()
{
	if(IsEmpty()) return FString();
	return TargetSkill->GetSkillName();
}

float USkillSlotWidget::GetMaxCooldown()
{
	if (IsEmpty()) return 0.0f;
	return TargetSkill->GetCoolTime();
}

float USkillSlotWidget::GetCurCooldown()
{
	if (IsEmpty()) return 0.0f;
	return TargetSkill->GetCoolTime();
}

float USkillSlotWidget::GetDamege()
{
	if (IsEmpty()) return 0.0f;
	return TargetSkill->GetDamageMagnification();
}

void USkillSlotWidget::UpdateIcon()
{
	if (IsEmpty()) return;
	UXRGameInstance* XRGI = Cast<UXRGameInstance>(GetWorld()->GetGameInstance());
	if (!XRGI) return;
	XRGI->ItemManager->GetIcon(Icon, TargetSkill->GetIconID());
}

void USkillSlotWidget::Play()
{
	if (!IsEmpty()) return;
}
