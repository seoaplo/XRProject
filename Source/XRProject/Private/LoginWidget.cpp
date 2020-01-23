// Fill out your copyright notice in the Description page of Project Settings.


#include "LoginWidget.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetSystemLibrary.h"
#include "NetworkGameInstance.h"
#include "NetworkModule/Public/OutputStream.h"

void ULoginWidget::LoginRequest(FText id, FText password)
{
	if (id.IsEmpty() || password.IsEmpty()) {
		return;
	}

	std::wstring w_id(*id.ToString());
	std::wstring w_pw(*password.ToString());
	std::string c_id(w_id.begin(), w_id.end());
	std::string c_pw(w_pw.begin(), w_pw.end());

	OutputStream out;
	out.WriteOpcode(ENetworkCSOpcode::LoginRequest);
	out.WriteCString(c_id.c_str());
	out.WriteCString(c_pw.c_str());
	out.CompletePacketBuild();

	GetNetworkManager().SendPacket(out);
}

void ULoginWidget::SignUp(FText id, FText password)
{
	if (id.IsEmpty() || password.IsEmpty()) {
		return;
	}
	std::wstring w_id(*id.ToString());
	std::wstring w_pw(*password.ToString());
	std::string c_id(w_id.begin(), w_id.end());
	std::string c_pw(w_pw.begin(), w_pw.end());

	OutputStream out;
	out.WriteOpcode(ENetworkCSOpcode::AccountCreateRequest);
	out.WriteCString(c_id.c_str());
	out.WriteCString(c_pw.c_str());
	out.CompletePacketBuild();

	GetNetworkManager().SendPacket(out);
}

void ULoginWidget::Exit()
{
	UKismetSystemLibrary::QuitGame(
		GetWorld(),
		GetWorld()->GetPlayerControllerIterator()->Get(),
		EQuitPreference::Quit,
		true);
}
