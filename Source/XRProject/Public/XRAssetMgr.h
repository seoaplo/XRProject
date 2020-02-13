// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "XRProject.h"
#include "UObject/NoExportTypes.h"
#include "Engine/DataTable.h"
#include "Engine/AssetManager.h"
#include "Optional.h"
#include "XRAssetMgr.generated.h"
#define __out__ 

USTRUCT(BlueprintType)
struct FResourceTableRow : public FTableRowBase
{
	GENERATED_BODY()
public:

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Resource")
		FString ResourcePath;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Resource")
		FString ResourceName;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Resource")
		FString ResourceLocalTransForm;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Resource")
		float	CapsuleHeight;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Resource")
		float	CapsuleRad;

};



struct FResourceLocalSize
{
	FTransform LocalTransform;
	float CapsuleHeight;
	float CapsuleRad;
};
/**
 * 
 */
UCLASS()
class XRPROJECT_API UXRAssetMgr : public UObject
{
	GENERATED_BODY()

public:
	UXRAssetMgr();
	//static UXRAssetMgr* GetInstnace();

	UFUNCTION(BlueprintCallable)
	bool ReadAssetDirectory(FString DirName, UClass* baseClass);


	void ASyncLoadAssetFromPath(FSoftObjectPath SoftObj, FStreamableDelegate CompleteDelegate);

	FSoftObjectPath FindResourceFromDataTable(int32 ResousrceID);

	FResourceLocalSize FindResourceSizeFromTable(int32 ResousrceID);

	void ASyncLoadAssetComplete(FString LoadAssetName);
	class UDataTable* GetResourceDataTable() { return ResourceDataTable; }

private:

	TMap<uint32,TArray<struct FAssetData>> AssetDatas;
	TMap<int32,struct FSoftObjectPath> AssetSoftPathList;

	UPROPERTY()
	class UDataTable* ResourceDataTable;
};





