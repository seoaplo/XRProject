#include "LoginWidget.h"
#include "NetworkOpcode.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetSystemLibrary.h"
#include "XRGameInstance.h"

void ULoginWidget::LoginRequest(FText Id, FText Password)
{
	if (Id.IsEmpty() || Password.IsEmpty()) {
		return;
	}

	std::wstring w_id(*Id.ToString());
	std::wstring w_pw(*Password.ToString());
	std::string c_id(w_id.begin(), w_id.end());
	std::string c_pw(w_pw.begin(), w_pw.end());

	OutputStream out;
	out.WriteOpcode(ENetworkCSOpcode::kLoginRequest);
	out.WriteCString(c_id.c_str());
	out.WriteCString(c_pw.c_str());
	out.CompletePacketBuild();

	AccountManager::GetInstance().SetAccountID(c_id);

	GetNetMgr().SendPacket(out);
	BlockButton();
}

void ULoginWidget::SignUp(FText Id, FText Password)
{
	if (Id.IsEmpty() || Password.IsEmpty()) {
		return;
	}
	std::wstring w_id(*Id.ToString());
	std::wstring w_pw(*Password.ToString());
	std::string c_id(w_id.begin(), w_id.end());
	std::string c_pw(w_pw.begin(), w_pw.end());

	OutputStream out;
	out.WriteOpcode(ENetworkCSOpcode::kCreateAccountRequest);
	out.WriteCString(c_id.c_str());
	out.WriteCString(c_pw.c_str());
	out.CompletePacketBuild();

	GetNetMgr().SendPacket(out);
	BlockButton();
}

void ULoginWidget::Exit()
{
	UKismetSystemLibrary::QuitGame(
		GetWorld(),
		GetWorld()->GetPlayerControllerIterator()->Get(),
		EQuitPreference::Quit,
		true);
}