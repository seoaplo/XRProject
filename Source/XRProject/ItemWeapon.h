// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Item.h"
#include "ItemWeapon.generated.h"

USTRUCT(BlueprintType)
struct FWeaponDefaultInfo
{
	GENERATED_USTRUCT_BODY()
public:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "C_Info")
		int32 ID;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "C_Info")
		FString Name;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "C_Info")
		UTexture2D* Icon;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "C_Info")
		int32 MeshResourceID;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "C_Info")
		int32 ReqLEV;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "C_Info")
		int32 MinATK;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "C_Info")
		int32 MaxATK;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "C_Info")
		int32 STR;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "C_Info")
		int32 DEX;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "C_Info")
		int32 INT;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "C_Info")
		int32 ATKLangth;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "C_Info")
		int32 ReqSTR;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "C_Info")
		int32 ReqDEX;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "C_Info")
		int32 ReqINT;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "C_Info")
		FString ToolTip;
};

struct WeaponInfo // 서버에서 보내주는 데이터
{
	int32_t ID;
	int32_t AddATK;
	int32_t AddSTR;
	int32_t AddDEX;
	int32_t AddINT;
};


UCLASS()
class XRPROJECT_API UItemWeapon : public UItem
{
	GENERATED_BODY()
public:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "C_Info")
		FWeaponDefaultInfo DefaultInfo; // 아이템 기본 속성
private:
	WeaponInfo AddInfo;  // 아이템 추가 능력치 및 서버 전송용
public:
	virtual bool Use() override;
public:
	UFUNCTION(BlueprintCallable, Category = "C_Function")
		int32 GetAddATK() { return AddInfo.AddATK; };
	UFUNCTION(BlueprintCallable, Category = "C_Function")
		int32 GetAddSTR() { return AddInfo.AddSTR; };
	UFUNCTION(BlueprintCallable, Category = "C_Function")
		int32 GetAddDEX() { return AddInfo.AddDEX; };
	UFUNCTION(BlueprintCallable, Category = "C_Function")
		int32 GetAddINT() { return AddInfo.AddINT; };
};
