// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "SerializeInterface.h"
#include "InputStream.h"
#include "OutputStream.h"
#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "LoginWidget.generated.h"

UCLASS()
class XRPROJECT_API ULoginWidget : public UUserWidget
{
	GENERATED_BODY()
public:
	UPROPERTY(EditDefaultsOnly, BlueprintReadwrite, Category = "C_Info")
		FString LoginID;
	UPROPERTY(EditDefaultsOnly, BlueprintReadwrite, Category = "C_Info")
		FString LoginPassword;
public:
	UFUNCTION(BlueprintCallable)
		void LoginRequest(FText Id, FText Password);
	UFUNCTION(BlueprintCallable)
		void SignUp(FText Id, FText Password);
	UFUNCTION(BlueprintCallable)
		void Exit();
	UFUNCTION(BlueprintImplementableEvent)
		void LoginFail();
	UFUNCTION(BlueprintImplementableEvent)
		void SignUpResult(bool bResult);

};