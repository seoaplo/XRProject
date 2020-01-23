// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "XRAssetMgr.h"
#include "TestGameInstance_AssetMgr.generated.h"

/**
 * 
 */
UCLASS()
class XRPROJECT_API UTestGameInstance_AssetMgr : public UGameInstance
{
	GENERATED_BODY()

public:
	void Init() override;
	UXRAssetMgr* GetXRAssetMgr() { return XRAssetManager;  }

private:
	UPROPERTY(VisibleAnywhere, Meta = (AllowPrivateAccess = true))
	UXRAssetMgr* XRAssetManager;
	
	
};
