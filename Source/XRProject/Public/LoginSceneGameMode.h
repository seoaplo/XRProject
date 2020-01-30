// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "XRGameInstance.h"
#include "XRProjectGameModeBase.h"
#include "LoginWidget.h"
#include "PlayerCharacter.h"
#include "InputStream.h"
#include "AccountManager.h"
#include "LoginSceneGameMode.generated.h"

/**
 * 
 */
UCLASS()
class XRPROJECT_API ALoginSceneGameMode : public AXRProjectGameModeBase
{
	GENERATED_BODY()
public:
	ALoginSceneGameMode();
public:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "C_GameMode")
		TSubclassOf<ULoginWidget> LoginWidget;
	UPROPERTY()
		ULoginWidget* CurrentWidget;
public:
	virtual void BeginPlay() override;
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;
	virtual void Tick(float DeltaSeconds) override;
public:
	void HandleCreateAccountResult(class InputStream& input);
	void HandleLoginResult(class InputStream& input);
};
