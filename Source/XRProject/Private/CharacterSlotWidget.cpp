// Fill out your copyright notice in the Description page of Project Settings.


#include "CharacterSlotWidget.h"
#include "CharacterSelectSceneGameMode.h"





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
	return int64();
}

FString UCharacterSlotWidget::GetCharacterName()
{
	return FString();
}
