// Fill out your copyright notice in the Description page of Project Settings.


#include "CharacterSlotWidget.h"
#include "CharacterSelectSceneGameMode.h"


UCharacterSlotWidget::UCharacterSlotWidget(const FObjectInitializer& ObjectInitializer) : UUserWidget(ObjectInitializer)
{
	TargetCharacter = nullptr;
}


void UCharacterSlotWidget::SendCurrentSelectSlot()
{
	AGameModeBase* GameModeBase = UGameplayStatics::GetGameMode(GetWorld());
	ACharacterSelectSceneGameMode* CSSGM = Cast<ACharacterSelectSceneGameMode>(GameModeBase);
	if (CSSGM != nullptr)
	{
		CSSGM->ChangeSelectedCharacter(MySlotNumber);
	}
}

int64 UCharacterSlotWidget::GetCharacterLevel()
{
	if (TargetCharacter)
	{
		int Level = TargetCharacter->PlayerStatComp->GetLevel();
		return Level;
	}
	return 0;
}

FString UCharacterSlotWidget::GetCharacterName()
{
	if (TargetCharacter)
	{
		FString Name = TargetCharacter->PlayerStatComp->GetCharacterName();
		return Name;
	}
	return FString();
}
