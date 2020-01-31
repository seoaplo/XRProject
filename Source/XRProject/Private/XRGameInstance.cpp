// Fill out your copyright notice in the Description page of Project Settings.


#include "XRGameInstance.h"

void UXRGameInstance::Init()
{
    NetworkManager = NewObject<UNetworkManager>();
    NetworkManager->StartThread();
    UNetworkManager::Instance = NetworkManager;
	XRAssetManager = NewObject<UXRAssetMgr>();
	ItemManager = NewObject<UItemManager>();
}

void UXRGameInstance::Shutdown()
{
    NetworkManager->StopThread();
    UNetworkManager::Instance = nullptr;
}