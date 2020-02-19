// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "XRProject.h"
#include "GameFramework/PlayerController.h"
#include "GenericTeamAgentInterface.h"
#include "InGameMainWidget.h"
#include "XRPlayerController.generated.h"

/**
 *
 */

class UMapManager;

UCLASS()
class XRPROJECT_API AXRPlayerController : public APlayerController, public IGenericTeamAgentInterface
{
	GENERATED_BODY()

public:

	virtual void	BeginPlay() override;
	virtual void	SetupInputComponent()override;
	void			TemporaryEnterZone();
public:
	void SetEnterZoneFunc(void(*Func)()) { EnterZoneFunc = Func; }
public:
	UFUNCTION(BlueprintCallable)
		FORCEINLINE bool IsSpuer() const { return bIsSuper; }
	UFUNCTION(BlueprintCallable)
		void SetSpuer() { bIsSuper = true; }
protected:
private:
	void(*EnterZoneFunc)();
	
private:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Meta = (AllowPrivateAccess = true))

		bool bIsSuper = false;

	UPROPERTY()
		UInGameMainWidget* MainWidget;
		TSubclassOf<UInGameMainWidget> WidgetClass;

	FGenericTeamId TeamId;
	FGenericTeamId GetGenericTeamId() const;

	void OpenInventory();
	void OpenPartyMatching();
	void OpenCharacterInfo();
	void OpenOptionWindow();
	void FocusChattingWindow();

	void QuickSlotAction0();
	void QuickSlotAction1();
	void QuickSlotAction2();
	void QuickSlotAction3();
	void QuickSlotAction4();
	void QuickSlotAction5();
	void QuickSlotAction6();
	void QuickSlotAction7();
	void QuickSlotAction8();
	void QuickSlotAction9();
};
