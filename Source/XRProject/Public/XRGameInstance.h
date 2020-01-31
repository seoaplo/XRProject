// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "NetworkManager.h"
#include "XRAssetMgr.h"
#include "ItemManager.h"
#include "MapManager.h"
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
	FORCEINLINE UXRAssetMgr* GetXRAssetMgr() { return XRAssetManager; }

	UPROPERTY(VisibleAnywhere, Meta = (AllowPrivateAccess = true))
		UXRAssetMgr* XRAssetManager;
	
	UPROPERTY(VisibleAnywhere, Meta = (AllowPrivateAccess = true))
		UItemManager* ItemManager;

private:
	UPROPERTY(VisibleAnywhere, Meta = (AllowPrivateAccess = true))
		UNetworkManager* NetworkManager;
};

//넣었는데 문제있으면 말씀하십쇼 -수찬-
USTRUCT(BlueprintType)
struct FPartsResource : public FTableRowBase
{
	GENERATED_BODY()
public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "C_Resource")
	FString Name;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "C_Resource")
	int32 ResourceID;
};


#define GetNetMgr UNetworkManager::GetInstance
#define GetAssetMgr Cast<UXRGameInstance>(GetGameInstance())->GetXRAssetMgr
