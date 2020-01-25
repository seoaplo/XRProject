// Fill out your copyright notice in the Description page of Project Settings.


#include "XRGameInstance.h"

void UXRGameInstance::Init()
{
    NetworkManager = NewObject<UNetworkManager>();
    NetworkManager->StartThread();
}

void UXRGameInstance::Shutdown()
{
    NetworkManager->StopThread();
}