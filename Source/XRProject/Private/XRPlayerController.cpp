// Fill out your copyright notice in the Description page of Project Settings.


#include "XRPlayerController.h"
#include "IngameGameMode.h"


void AXRPlayerController::BeginPlay()
{
	Super::BeginPlay();
	XRLOG(Warning, TEXT("PlayerController IS Ready"));
	SetGenericTeamId(FGenericTeamId(10));
	TeamId = FGenericTeamId(10);
	//MainWidget = CreateWidget<UInGameMainWidget>(GetWorld(), WidgetClass);
	
	
}

FGenericTeamId AXRPlayerController::GetGenericTeamId() const
{
	return TeamId;
}

void AXRPlayerController::OpenInventory()
{
	AIngameGameMode* Mode = Cast<AIngameGameMode>(GetWorld()->GetAuthGameMode());

	if(!MainWidget)
		MainWidget = Mode->CurrentWidget;

	MainWidget->Inventory->InventoryInstance->SwitchVisible();
}

void AXRPlayerController::OpenPartyMatching()
{
	AIngameGameMode* Mode = Cast<AIngameGameMode>(GetWorld()->GetAuthGameMode());

	if (!MainWidget)
		MainWidget = Mode->CurrentWidget;

	MainWidget->MatchingWindow->SwitchVisible();
}

void AXRPlayerController::OpenCharacterInfo()
{
	AIngameGameMode* Mode = Cast<AIngameGameMode>(GetWorld()->GetAuthGameMode());

	if (!MainWidget)
		MainWidget = Mode->CurrentWidget;

	MainWidget->CharacterInfo->SwitchVisible();
}

void AXRPlayerController::OpenOptionWindow()
{
	AIngameGameMode* Mode = Cast<AIngameGameMode>(GetWorld()->GetAuthGameMode());

	if (!MainWidget)
		MainWidget = Mode->CurrentWidget;

	MainWidget->MainMenu->SwitchVisible();
}

void AXRPlayerController::FocusChattingWindow()
{
	AIngameGameMode* Mode = Cast<AIngameGameMode>(GetWorld()->GetAuthGameMode());

	if (!MainWidget)
		MainWidget = Mode->CurrentWidget;

	MainWidget->ChatingBar->SendBoxFocus(this);

}


void AXRPlayerController::SetupInputComponent()
{
	Super::SetupInputComponent();
	InputComponent->BindAction(TEXT("TemporaryEnterZone"), IE_Pressed, this, &AXRPlayerController::TemporaryEnterZone);
	InputComponent->BindAction(TEXT("OpenInventory"), IE_Pressed, this, &AXRPlayerController::OpenInventory);
	InputComponent->BindAction(TEXT("OpenPartyMatching"), IE_Pressed, this, &AXRPlayerController::OpenPartyMatching);
	InputComponent->BindAction(TEXT("OpenCharacterInfo"), IE_Pressed, this, &AXRPlayerController::OpenCharacterInfo);
	InputComponent->BindAction(TEXT("OptionWindow"), IE_Pressed, this, &AXRPlayerController::OpenOptionWindow);
	InputComponent->BindAction(TEXT("FocusChattingWindow"), IE_Pressed, this, &AXRPlayerController::FocusChattingWindow);
}

void AXRPlayerController::TemporaryEnterZone()
{
	if (EnterZoneFunc != nullptr) EnterZoneFunc();
}
