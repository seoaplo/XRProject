// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Item.h"
#include "ItemEquipment.generated.h"

USTRUCT(BlueprintType)
struct FEquipmentDefaultInfo
{
	GENERATED_USTRUCT_BODY()
public:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "C_Info")
		int32 ID;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "C_Info")
		FString Name;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "C_Info")
		int32 Icon;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "C_Info")
		int32 MaleMeshResourceID;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "C_Info")
		int32 FemaleMeshResourceID;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "C_Info")
		int32 Type;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "C_Info")
		int32 SubType;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "C_Info")
		int32 ReqLEV;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "C_Info")
		int32 ATK;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "C_Info")
		int32 DEF;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "C_Info")
		int32 STR;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "C_Info")
		int32 DEX;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "C_Info")
		int32 INT;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "C_Info")
		int32 ReqSTR;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "C_Info")
		int32 ReqDEX;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "C_Info")
		int32 ReqINT;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "C_Info")
		FString ToolTip;
};

struct EquipmentInfo // 서버에서 보내주는 데이터
{
	int32_t ID;
	int32_t AddATK;
	int32_t AddDEF;
	int32_t AddSTR;
	int32_t AddDEX;
	int32_t AddINT;
};

UCLASS()
class XRPROJECT_API UItemEquipment : public UItem
{
	GENERATED_BODY()
public:
	UItemEquipment();
public:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "C_Info")
		FEquipmentDefaultInfo DefaultInfo; // 아이템 기본 속성
private:
	EquipmentInfo AddInfo; // 아이템 추가 능력치 및 서버 전송용
public:
	virtual bool Use() override { return false; };
	virtual int GetID() override;
public:
	UFUNCTION(BlueprintCallable, Category = "C_Function")
		int32 GetAddATK() { return AddInfo.AddATK; };
	UFUNCTION(BlueprintCallable, Category = "C_Function")
		int32 GetAddDEF() { return AddInfo.AddDEF; };
	UFUNCTION(BlueprintCallable, Category = "C_Function")
		int32 GetAddSTR() { return AddInfo.AddSTR; };
	UFUNCTION(BlueprintCallable, Category = "C_Function")
		int32 GetAddDEX() { return AddInfo.AddDEX; };
	UFUNCTION(BlueprintCallable, Category = "C_Function")
		int32 GetAddINT() { return AddInfo.AddINT; };
};
