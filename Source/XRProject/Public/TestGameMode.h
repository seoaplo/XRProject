// Fill out your copyright notice in the Description page of Project Settings.
#pragma once
#include "CoreMinimal.h"
#include "AccountManager.h"
#include "XRGameInstance.h"
#include "PlayerCharacter.h"
#include "XRProjectGameModeBase.h"
#include "TestGameMode.generated.h"

/**
 * 
 */
UCLASS()
class XRPROJECT_API ATestGameMode : public AXRProjectGameModeBase
{
	GENERATED_BODY()

public:
	ATestGameMode();
	virtual ~ATestGameMode();

public:
	virtual void BeginPlay() override;
	virtual void Tick(float deltatime) override;
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason);

};