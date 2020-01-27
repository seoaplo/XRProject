// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include <vector>
#include "UserCharacter.h"
#include "CharacterSelectWidget.h"
#include "CoreMinimal.h"
#include "XRProjectGameModeBase.h"
#include "CharacterSelectSceneGameMode.generated.h"

/**
 * 
 */
UCLASS()
class XRPROJECT_API ACharacterSelectSceneGameMode : public AXRProjectGameModeBase
{
	GENERATED_BODY()
public:
	ACharacterSelectSceneGameMode();
public:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "C_GameMode")
		TSubclassOf<UCharacterSelectWidget> LoginWidget;
	UPROPERTY()
		UCharacterSelectWidget* CurrentWidget;
public:
	std::vector<AUserCharacter> CharacterList; // 현재 접속한 계정의 캐릭터의 리스트(패킷으로 받아와야 한다.)
public:
	virtual void BeginPlay() override;
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;
	virtual void Tick(float DeltaSeconds) override;
public:
	void HandleCharacterCreateResult(class InputStream& input);
	void HandleCharacterSelectionResult(class InputStream& input);
	void HandleCharacterList(class InputStream& input);
};