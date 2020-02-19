// Fill out your copyright notice in the Description page of Project Settings.


#include "XRPlayerController.h"
#include "IngameGameMode.h"
#include "QuickBar.h"


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

void AXRPlayerController::QuickSlotAction0()
{
	if (UQuickBar::GetInstance())
		UQuickBar::GetInstance()->list[0]->PushButton();
}

void AXRPlayerController::QuickSlotAction1()
{
	if(UQuickBar::GetInstance())
	UQuickBar::GetInstance()->list[1]->PushButton();
}

void AXRPlayerController::QuickSlotAction2()
{
	if (UQuickBar::GetInstance())
		UQuickBar::GetInstance()->list[2]->PushButton();
}

void AXRPlayerController::QuickSlotAction3()
{
	if (UQuickBar::GetInstance())
		UQuickBar::GetInstance()->list[3]->PushButton();
}

void AXRPlayerController::QuickSlotAction4()
{
	if (UQuickBar::GetInstance())
		UQuickBar::GetInstance()->list[4]->PushButton();
}


void AXRPlayerController::QuickSlotAction5()
{
	if (UQuickBar::GetInstance())
		UQuickBar::GetInstance()->list[5]->PushButton();
}


void AXRPlayerController::QuickSlotAction6()
{
	if (UQuickBar::GetInstance())
		UQuickBar::GetInstance()->list[6]->PushButton();
}


void AXRPlayerController::QuickSlotAction7()
{
	if (UQuickBar::GetInstance())
		UQuickBar::GetInstance()->list[7]->PushButton();
}


void AXRPlayerController::QuickSlotAction8()
{
	if (UQuickBar::GetInstance())
		UQuickBar::GetInstance()->list[8]->PushButton();
}


void AXRPlayerController::QuickSlotAction9()
{
	if (UQuickBar::GetInstance())
		UQuickBar::GetInstance()->list[9]->PushButton();
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

	InputComponent->BindAction(TEXT("QuickSlotNum1"), IE_Pressed, this, &AXRPlayerController::QuickSlotAction0);
	InputComponent->BindAction(TEXT("QuickSlotNum2"), IE_Pressed, this, &AXRPlayerController::QuickSlotAction1);
	InputComponent->BindAction(TEXT("QuickSlotNum3"), IE_Pressed, this, &AXRPlayerController::QuickSlotAction2);
	InputComponent->BindAction(TEXT("QuickSlotNum4"), IE_Pressed, this, &AXRPlayerController::QuickSlotAction3);
	InputComponent->BindAction(TEXT("QuickSlotNum5"), IE_Pressed, this, &AXRPlayerController::QuickSlotAction4);
	InputComponent->BindAction(TEXT("QuickSlotNum6"), IE_Pressed, this, &AXRPlayerController::QuickSlotAction5);
	InputComponent->BindAction(TEXT("QuickSlotNum7"), IE_Pressed, this, &AXRPlayerController::QuickSlotAction6);
	InputComponent->BindAction(TEXT("QuickSlotNum8"), IE_Pressed, this, &AXRPlayerController::QuickSlotAction7);
	InputComponent->BindAction(TEXT("QuickSlotNum9"), IE_Pressed, this, &AXRPlayerController::QuickSlotAction8);
	InputComponent->BindAction(TEXT("QuickSlotNum10"), IE_Pressed, this, &AXRPlayerController::QuickSlotAction9);

}

void AXRPlayerController::TemporaryEnterZone()
{
	if (EnterZoneFunc != nullptr) EnterZoneFunc();
}
