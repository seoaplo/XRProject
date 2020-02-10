// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerCameraShake.h"

UPlayerCameraShake::UPlayerCameraShake()
{
	OscillationDuration = 0.5f;
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
	LocOscillation.X.Amplitude = FMath::RandRange(5.0f, 8.0f);
	LocOscillation.X.Frequency = FMath::RandRange(70.0f, 90.0f);
	LocOscillation.Y.Amplitude = FMath::RandRange(5.0f, 8.0f);
	LocOscillation.Y.Frequency = FMath::RandRange(70.0f, 90.0f);
}

void UPlayerCameraShake::SetBigShakeMode()
{
	LocOscillation.X.Amplitude = FMath::RandRange(13.0f, 20.0f);
	LocOscillation.X.Frequency = FMath::RandRange(70.0f, 90.0f);
	LocOscillation.Y.Amplitude = FMath::RandRange(13.0f, 20.0f);
	LocOscillation.Y.Frequency = FMath::RandRange(70.0f, 90.0f);
}

void UPlayerCameraShake::SetCustomShakeMode(FCameraShakeInfo Info)
{
	OscillationBlendInTime = Info.BlendInTime;
	OscillationBlendOutTime = Info.BlendOutTime;

	LocOscillation.X.Amplitude = FMath::RandRange(Info.AmplitudeMin.X, Info.AmplitudeMax.X);
	LocOscillation.X.Frequency = FMath::RandRange(Info.FrequencyMin.X, Info.FrequencyMax.X);
	LocOscillation.Y.Amplitude = FMath::RandRange(Info.AmplitudeMin.Y, Info.AmplitudeMax.Y);
	LocOscillation.Y.Frequency = FMath::RandRange(Info.FrequencyMin.Y, Info.FrequencyMin.Y);
	LocOscillation.Z.Amplitude = FMath::RandRange(Info.AmplitudeMin.Z, Info.AmplitudeMax.Z);
	LocOscillation.Z.Frequency = FMath::RandRange(Info.FrequencyMin.Z, Info.FrequencyMin.Z);
}
