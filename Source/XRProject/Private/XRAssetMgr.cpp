// Fill out your copyright notice in the Description page of Project Settings.


#include "XRAssetMgr.h"
#include "Engine/ObjectLibrary.h"


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

void UXRAssetMgr::ASyncLoadAssetFromPath(FString ResourcePath, FStreamableDelegate CompleteDelegate)
{
	FSoftObjectPath SoftObj(ResourcePath);
	XRLOG(Warning, TEXT("ASyncLoadAssetStart : %s"), *ResourcePath);
	UAssetManager::GetStreamableManager().RequestAsyncLoad(SoftObj,
		CompleteDelegate);
}

void UXRAssetMgr::ASyncLoadAssetComplete(FString LoadAssetName)
{
	XRLOG(Warning, TEXT("ASyncLoadAssetCompleted"));
}

FString UXRAssetMgr::FindResourceFromDataTable(int32 ResousrceID)
{
	if (ResourceDataTable != nullptr)
	{
		FResourceTableRow* ResourceTableRow =
			ResourceDataTable->FindRow<FResourceTableRow>
			(FName(*FString::FormatAsNumber(ResousrceID)), FString(""));

		XRLOG(Warning,TEXT("Finded Resource ID : %d  Path : %s  Name : %s "), ResousrceID, *ResourceTableRow->ResourcePath,*ResourceTableRow->ResourceName)
		return ResourceTableRow->ResourcePath;
	}

	return FString();
}
