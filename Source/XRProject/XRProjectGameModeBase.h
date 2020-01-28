// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "XRProjectGameModeBase.generated.h"

/**
 * 
 */
UCLASS()
class XRPROJECT_API AXRProjectGameModeBase : public AGameModeBase
{
	GENERATED_BODY()
	
public:
    AXRProjectGameModeBase();

    virtual void BeginPlay() override;
    virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;
    virtual void Tick(float DeltaSeconds) override;
private:
    void HandleCreateAccountResult(class InputStream& input);
    void HandleLoginResult(class InputStream& input);


	void HandleCharacterSpawn(class InputStream& input);
	void HandlePossess(class InputStream& input);
	void HandleCharacterMove(class InputStream& input);
	void EventRecv(class InputStream& input);

	TMap<FString, class AXRPlayerCharacter*> Players;

	FString my_uuid;


};
