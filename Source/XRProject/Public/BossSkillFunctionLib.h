// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "XRProject.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "BossSkillFunctionLib.generated.h"

/**
 * 
 */
UCLASS()
class XRPROJECT_API UBossSkillFunctionLib : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()
	

		UFUNCTION(BlueprintCallable, Category = "Mesh Merge", meta = (UnsafeDuringActorConstruction = "true"))
		static void FireSkillEvent();

};
