// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "XRProject.h"
#include "UObject/NoExportTypes.h"
#include "Engine/DataTable.h"
#include "Engine/AssetManager.h"

#include "XRAssetMgr.generated.h"
#define __out__ 
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
	bool ReadAssetDirectory(FString DirName);
	void ASyncLoadAssetFromPath(FString ResourcePath, FSoftObjectPath __out__ DestSoftObj, FStreamableDelegate CompleteDelegate);

	TMap<uint32,TArray<struct FAssetData>> AssetDatas;
	TMap<FString,struct FSoftObjectPath> AssetSoftPathList;
	 
	class UDataTable* ResourceDataTable;
	void ASyncLoadAssetComplete(FString LoadAssetName);
	FSoftObjectPath FindResourceFromDataTable(int32 ResousrceID);


};

USTRUCT(BlueprintType)
struct FResourceTableRow : public FTableRowBase
{
	GENERATED_BODY()
public:

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Resource")
		FString ResourcePath;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Resource")
		FString ResourceName;
};

