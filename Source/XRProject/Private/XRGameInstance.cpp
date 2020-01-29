// Fill out your copyright notice in the Description page of Project Settings.


#include "XRGameInstance.h"

void UXRGameInstance::Init()
{
	UE_LOG(LogTemp, Warning, TEXT("INCREDIBUILD"));
    NetworkManager = NewObject<UNetworkManager>();
    NetworkManager->StartThread();
	XRAssetManager = NewObject<UXRAssetMgr>();
	ItemManager = NewObject<UItemManager>();
	MapManager = NewObject<UMapManager>();
}

void UXRGameInstance::Shutdown()
{
    NetworkManager->StopThread();
}