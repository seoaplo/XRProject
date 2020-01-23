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
}

bool UXRAssetMgr::ReadAssetDirectory(FString DirName)
{
	XRLOG(Warning, TEXT("ReadAssetDir"));
	UObjectLibrary* ObjLib;
	ObjLib = UObjectLibrary::CreateLibrary(USkeletalMesh::StaticClass(), false, GIsEditor);
	ObjLib->AddToRoot();
	ObjLib->LoadAssetDataFromPath(TEXT("/Game/Resource"));
	ObjLib->LoadAssetsFromAssetData();
	ObjLib->GetAssetDataList(AssetDatas[1]);
	XRLOG(Warning, TEXT("ReadAssetDirComplete"));
	return true;
}



void UXRAssetMgr::ASyncLoadAssetFromPath(FString ResourcePath,FSoftObjectPath __out__ DestSoftObj, FStreamableDelegate CompleteDelegate)
{

	XRLOG(Warning, TEXT("ASyncLoadAssetStart : %s"), *ResourcePath);
	UAssetManager::GetStreamableManager().RequestAsyncLoad(DestSoftObj,
		CompleteDelegate);
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
			//(FName(*(FString::FormatAsNumber(ResousrceID))), FString(""));

		XRLOG(Warning, TEXT("Finded Resource ID : %d  Path : %s  Name : %s "), ResousrceID, *ResourceTableRow->ResourcePath, *ResourceTableRow->ResourceName);
		AssetSoftPathList.Add(ResourceTableRow->ResourcePath, FSoftObjectPath(ResourceTableRow->ResourcePath));
		
		return AssetSoftPathList[ResourceTableRow->ResourcePath];
	}

	return nullptr;

}
