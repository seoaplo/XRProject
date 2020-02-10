// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "NetworkManager.h"
#include "DungeonResultWidget.generated.h"

/**
 * 
 */
UCLASS()
class XRPROJECT_API UDungeonResultWidget : public UUserWidget
{
	GENERATED_BODY()
public:
	void GetReward(class InputStream& input);
};
