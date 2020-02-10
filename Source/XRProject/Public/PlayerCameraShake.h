// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Camera/CameraShake.h"
#include "PlayerCameraShake.generated.h"

/**
 * 
 */

UCLASS()
class XRPROJECT_API UPlayerCameraShake : public UCameraShake
{
	GENERATED_BODY()

public:
	UPlayerCameraShake();
	~UPlayerCameraShake();

public:
	UPROPERTY(EditInstanceOnly)
	FVector2D PitchAmplitude;
	UPROPERTY(EditInstanceOnly)
	FVector2D PitchFrequency;
	UPROPERTY(EditInstanceOnly)
	FVector2D YawAmplitude;
	UPROPERTY(EditInstanceOnly)
	FVector2D YawFrequency;
	UPROPERTY(EditInstanceOnly)
	float Duration;

public:
	void SetSmallShakeMode();
	void SetBigShakeMode();
	void SetCustomShakeMode();

	
};
