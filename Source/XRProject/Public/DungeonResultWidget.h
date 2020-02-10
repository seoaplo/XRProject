// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include <vector>
#include "Blueprint/UserWidget.h"
#include "NetworkManager.h"
#include "ItemManager.h"
#include "DungeonResultWidget.generated.h"


/**
 * 
 */
UCLASS()
class XRPROJECT_API UDungeonResultWidget : public UUserWidget
{
	GENERATED_BODY()
public:
	UPROPERTY(EditAnywhere, BlueprintReadwrite, Category = "C_Info")
		int Gold;
	UPROPERTY(EditAnywhere, BlueprintReadwrite, Category = "C_Info")
		int Exp;
	UPROPERTY(EditAnywhere, BlueprintReadwrite, Category = "C_Info")
		TArray<UItem*> ItemList;
public:
	void GetReward(class InputStream& input);

};
