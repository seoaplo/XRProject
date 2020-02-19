// Fill out your copyright notice in the Description page of Project Settings.


#include "SkillWindowWidget.h"

void USkillWindowWidget::SetVisible(bool IsVisible)
{
	bIsVisible = IsVisible;
	if (bIsVisible)
	{
		SetVisibility(ESlateVisibility::Visible);
	}
	else
	{
		SetVisibility(ESlateVisibility::Hidden);
	}
}

bool USkillWindowWidget::GetVisible()
{
	return bIsVisible;
}

void USkillWindowWidget::SwitchVisible()
{
	SetVisible(!bIsVisible);
}

void USkillWindowWidget::CreateSkillList()
{
	if (!ListBox) return;
	UXRGameInstance* XRGI = Cast<UXRGameInstance>(GetWorld()->GetGameInstance());
	if (!XRGI) return;
	UPlayerSkillManager* SM = XRGI->GetPlayerSkillManager();
	if (!SM) return;
	for (int i = 0; i < SM->SkillListForPlalyer.Num(); i++)
	{
		USkillPanelWidget* NewSKillPanel = CreateWidget<USkillPanelWidget>(GetWorld(), SkillPanel);
		if (!NewSKillPanel) return;
		ListBox->AddChild(NewSKillPanel);
		NewSKillPanel->SkillSlot->TargetSkill = SM->SkillListForPlalyer[i];
		NewSKillPanel->SkillSlot->UpdateIcon();
		NewSKillPanel->Update();
	}
}
