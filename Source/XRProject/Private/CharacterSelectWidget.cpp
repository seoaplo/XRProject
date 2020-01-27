#include "CharacterSelectWidget.h"
#include "NetworkOpcode.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetSystemLibrary.h"
#include "XRGameInstance.h"
#include "OutputStream.h"

void UCharacterSelectWidget::AddCharacter(AUserCharacter* Character)
{
	CharacterList.push_back(Character);
	CharacterCount = CharacterList.size();
	UpdateList();
}

void UCharacterSelectWidget::DeleteCharacter(int Num)
{
	for (int i = Num; i < CharacterCount - 1; i++)
	{
		CharacterList[i] = CharacterList[i + 1];
	}
	CharacterList.pop_back();
	CharacterCount = CharacterList.size();
	UpdateList();
}

AUserCharacter* UCharacterSelectWidget::GetCharacter(int Num)
{
	if (Num < CharacterList.size() || Num >= 0)
	{
		return CharacterList[Num];
	}
	return nullptr;
}

void UCharacterSelectWidget::CharacterSelectionRequest(int32 Numder)
{
	OutputStream out;
	out.WriteOpcode(ENetworkCSOpcode::kCharacterSelectionRequest);
	out << Numder;
	out.CompletePacketBuild();

	GetNetMgr().SendPacket(out);
	BlockButton();
}

void UCharacterSelectWidget::CharacterCreateRequest(FText Name, int32 Sex)
{
	if (Name.IsEmpty())
	{
		return;
	}
	std::wstring w_id(*Name.ToString());
	std::string c_id(w_id.begin(), w_id.end());

	OutputStream out;
	out.WriteOpcode(ENetworkCSOpcode::kCharacterCreateRequest);
	out.WriteCString(c_id.c_str());
	out << Sex;
	out.CompletePacketBuild();

	GetNetMgr().SendPacket(out);
	BlockButton();
}

void UCharacterSelectWidget::CharacterDeleteRequest(int32 Numder)
{
	OutputStream out;
	out.WriteOpcode(ENetworkCSOpcode::kCharacterDeleteRequest);
	out << Numder;
	out.CompletePacketBuild();

	GetNetMgr().SendPacket(out);
	BlockButton();
}

void UCharacterSelectWidget::ReturnLoginServer()
{
	OutputStream out;
	// 로그인 서버로 회귀한다는 패킷 필요
	//out.WriteOpcode(ENetworkCSOpcode::kReturnLoginRequest);
	out.CompletePacketBuild();

	GetNetMgr().SendPacket(out);
	BlockButton();
}