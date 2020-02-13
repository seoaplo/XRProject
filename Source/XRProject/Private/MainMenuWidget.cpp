#include "MainMenuWidget.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetSystemLibrary.h"

void UMainMenuWidget::SetVisible(bool IsVisible)
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

bool UMainMenuWidget::GetVisible()
{
	return bIsVisible;
}

void UMainMenuWidget::SwitchVisible()
{
	SetVisible(!bIsVisible);
}

void UMainMenuWidget::QuitGame()
{
	UKismetSystemLibrary::QuitGame(GetWorld(), GetWorld()->GetPlayerControllerIterator()->Get(), EQuitPreference::Quit, true);
}