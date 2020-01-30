#include "CharacterSelectWidget.h"
#include "NetworkOpcode.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetSystemLibrary.h"
#include "XRGameInstance.h"
#include "OutputStream.h"
#include "CharacterSelectSceneGameMode.h"

UCharacterSelectWidget::UCharacterSelectWidget(const FObjectInitializer& ObjectInitializer) : UUserWidget(ObjectInitializer)
{
	CharacterList.resize(kMaxCharacterSlot);
	for (int i = 0; i < kMaxCharacterSlot; i++)
	{
		CharacterList[i] = nullptr;
	}
}

void UCharacterSelectWidget::AddCharacter(int Num, APlayerCharacter* Character)
{
	CharacterList[Num] = Character;
	UpdateList();
}

void UCharacterSelectWidget::DeleteCharacter(int Num)
{
	CharacterList[Num] = nullptr;
	UpdateList();
}

APlayerCharacter* UCharacterSelectWidget::GetCharacter(int Num)
{
	if (Num < kMaxCharacterSlot || Num >= 0)
	{
		return CharacterList[Num];
	}
	return nullptr;
}

void UCharacterSelectWidget::CharacterSelectionRequest(int Numder)
{
	OutputStream out;
	out.WriteOpcode(ENetworkCSOpcode::kCharacterSelectionRequest);
	out << (int32_t)Numder;
	out.CompletePacketBuild();

	GetNetMgr().SendPacket(out);
	BlockButton();
}

void UCharacterSelectWidget::CharacterCreateRequest(FText Name, int FaceID, int HairID, int Gender)
{
	int SlotNum = 0;
	while (true)
	{
		if (SlotNum >= kMaxCharacterSlot) return;
		if (CharacterList[SlotNum] == nullptr)
		{
			break;
		}
		else
		{
			SlotNum++;
		}
	}

	if (Name.IsEmpty())
	{
		return;
	}

	std::wstring w_Name(*Name.ToString());
	std::string c_Name = wcs_to_mbs(w_Name, std::locale("kor"));

	OutputStream out;
	out.WriteOpcode(ENetworkCSOpcode::kCharacterCreateRequest);
	int32_t Zero = 0;
	out << (int32_t)SlotNum;
	out.WriteCString(c_Name.c_str());
	out << Zero; // 레벨
	out << Zero; // 힘
	out << Zero; // 민
	out << Zero; // 지
	out << Zero; // 잡
	out << (int32_t)FaceID; // 얼굴
	out << (int32_t)HairID; // 머리
	out << Zero; // 돈
	out << Zero; // 존
	out << Zero; // x
	out << Zero; // y
	out << Zero; // z
	out << Zero; // 아머id
	out << Zero; // 핸드id
	out << Zero; // 슈즈id
	out << Zero; // 웨폰id
	out << Zero; // 서브웨폰id
	out << (int32_t)Gender; // 성별
	out.CompletePacketBuild();

	GetNetMgr().SendPacket(out);
}

void UCharacterSelectWidget::CharacterDeleteRequest()
{
	AGameModeBase* GameModeBase = UGameplayStatics::GetGameMode(GetWorld());
	ACharacterSelectSceneGameMode* CSSGM = Cast<ACharacterSelectSceneGameMode>(GameModeBase);
	if (CSSGM != nullptr)
	{
		int Number = CSSGM->BeforeSlotNumber;
		OutputStream out;
		out.WriteOpcode(ENetworkCSOpcode::kCharacterDeleteRequest);
		out << (int32_t)Number;
		out.CompletePacketBuild();

		GetNetMgr().SendPacket(out);
		BlockButton();
	}
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
