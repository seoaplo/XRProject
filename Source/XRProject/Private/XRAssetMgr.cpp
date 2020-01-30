// Fill out your copyright notice in the Description page of Project Settings.


#include "XRAssetMgr.h"
#include "Engine/ObjectLibrary.h"
#define OUT 



UXRAssetMgr::UXRAssetMgr()
{
	static ConstructorHelpers::FObjectFinder<UDataTable> DataTable(TEXT("DataTable'/Game/Resources/DataTable/ResourceTable.ResourceTable'"));
	if(DataTable.Succeeded())
	{
		XRLOG(Warning, TEXT("FindedResourceTable"));
		ResourceDataTable = DataTable.Object;
	}
	else
	{
		XRLOG(Error, TEXT("Can't Find ResourceTable"));

	}
}

bool UXRAssetMgr::ReadAssetDirectory(FString DirName, UClass* baseClass)
{
	/*XRLOG(Warning, TEXT("ReadAssetDir :%s"),*DirName);
	UObjectLibrary* ObjLib;
	ObjLib = UObjectLibrary::CreateLibrary(baseClass, false, GIsEditor);
	ObjLib->AddToRoot();
	ObjLib->LoadAssetDataFromPath(DirName);
	ObjLib->LoadAssetsFromAssetData();
	ObjLib->GetAssetDataList(AssetDatas[0]);
	XRLOG(Warning, TEXT("ReadAssetDirComplete :%s"), *DirName);
	return true;*/
	return true;
}



void UXRAssetMgr::ASyncLoadAssetFromPath(FSoftObjectPath  SoftObj, FStreamableDelegate CompleteDelegate)
{
	if (SoftObj != nullptr)
	{
		XRLOG(Warning, TEXT("ASyncLoadAssetStart : %s"), *SoftObj.ToString());
		UAssetManager::GetStreamableManager().RequestAsyncLoad(SoftObj,
			CompleteDelegate);
	}
	else
	{
		XRLOG(Warning, TEXT("Asset Not Exist!! "));
	}
}



void UXRAssetMgr::ASyncLoadAssetComplete(FString LoadAssetName)
{
	XRLOG(Warning, TEXT("ASyncLoadAssetCompleted"));
}

FSoftObjectPath UXRAssetMgr::FindResourceFromDataTable(int32 ResousrceID)
{
	if (ResourceDataTable != nullptr)
	{
		FResourceTableRow* ResourceTableRow =
			ResourceDataTable->FindRow<FResourceTableRow>
			(FName(*(FString::FromInt(ResousrceID))), FString(""));
		if (ResourceTableRow)
		{
			XRLOG(Warning, TEXT("Finded Resource ID : %d  Path : %s  Name : %s "), ResousrceID, *ResourceTableRow->ResourcePath, *ResourceTableRow->ResourceName);
			if (!AssetSoftPathList.Contains(ResourceTableRow->ResourceName))
			{
				AssetSoftPathList.Add(ResourceTableRow->ResourceName, FSoftObjectPath(ResourceTableRow->ResourcePath));
			}
			return AssetSoftPathList[ResourceTableRow->ResourceName];
		}
	}
	XRLOG(Warning, TEXT("ResourceID %d Not Exist "), ResousrceID);
	return nullptr;
}


