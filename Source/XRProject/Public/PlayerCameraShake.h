// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Camera/CameraShake.h"
#include "PlayerCameraShake.generated.h"

/**
 * 
 */


USTRUCT(Blueprinttype)
struct FCameraShakeInfo
{
	GENERATED_BODY()
public:
	UPROPERTY(EditInstanceOnly)
		float BlendInTime;
	UPROPERTY(EditInstanceOnly)
		float BlendOutTime;
	UPROPERTY(EditInstanceOnly)
		FVector AmplitudeMin;
	UPROPERTY(EditInstanceOnly)
		FVector AmplitudeMax;
	UPROPERTY(EditInstanceOnly)
		FVector FrequencyMin;
	UPROPERTY(EditInstanceOnly)
		FVector FrequencyMax;

	FCameraShakeInfo()
	{
		BlendInTime = 0.5f;
		BlendOutTime = 0.5f;
		AmplitudeMin = FVector(0.0f, 0.0f, 0.0f);
		AmplitudeMax = FVector(0.0f, 0.0f, 0.0f);
		FrequencyMin = FVector(0.0f, 0.0f, 0.0f);
		FrequencyMax = FVector(0.0f, 0.0f, 0.0f);
	}

};


UCLASS()
class XRPROJECT_API UPlayerCameraShake : public UCameraShake
{
	GENERATED_BODY()

public:
	UPlayerCameraShake();
	~UPlayerCameraShake();


public:
	void SetSmallShakeMode();
	void SetBigShakeMode();
		/*Duration 설정 불가능*/
	void SetCustomShakeMode(FCameraShakeInfo Info);
};
