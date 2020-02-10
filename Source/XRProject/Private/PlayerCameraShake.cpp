// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerCameraShake.h"

UPlayerCameraShake::UPlayerCameraShake()
{
	OscillationDuration = 10.0f;
	OscillationBlendInTime = 0.05f;
	OscillationBlendOutTime = 0.05f;

	LocOscillation.X.Amplitude = FMath::RandRange(5.0f, 10.0f);
	LocOscillation.X.Frequency = FMath::RandRange(25.0f, 35.0f);
	LocOscillation.Y.Amplitude = FMath::RandRange(5.0f, 10.0f);
	LocOscillation.Y.Frequency = FMath::RandRange(25.0f, 35.0f);
}

UPlayerCameraShake::~UPlayerCameraShake()
{

}

void UPlayerCameraShake::SetSmallShakeMode()
{
}

void UPlayerCameraShake::SetBigShakeMode()
{
}

void UPlayerCameraShake::SetCustomShakeMode()
{
}
