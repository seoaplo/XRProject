// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "NetworkManager.h"
#include "XRAssetMgr.h"
#include "XRGameInstance.generated.h"

/**
 * 
 */
UCLASS()
class XRPROJECT_API UXRGameInstance : public UGameInstance
{
	GENERATED_BODY()
public:
    virtual void Init() override;
    virtual void Shutdown() override;

    FORCEINLINE UNetworkManager& GetNetworkManager() {
        return *NetworkManager;

    }

};

#define GetNetMgr Cast<UXRGameInstance>(GetGameInstance())->GetNetworkManager
#define GetAssetMgr Cast<UXRGameInstance>(GetGameInstance())->GetXRAssetMgr
