#include "ChatingManager.h"
#include "ChatingWidget.h"
#include "NetworkManager.h"
#include "XRGameInstance.h"
#include "IngameGameMode.h"
#include "XRProject.h"

ChatingManager::ChatingManager()
{
}

ChatingManager::~ChatingManager()
{
}

void ChatingManager::ReceiveChat(int Type, std::string& Chat)
{
	std::wstring RecivedChat = mbs_to_wcs(Chat, std::locale("kor"));
	ChatList.push_back(ChatStruct((TextType)Type, RecivedChat));
	if (ChatList.size() > kMaxChatListSize)
	{
		ChatList.pop_front();
		UChatingWidget::ChatingWidgetInstance->ReciveChat((TextType)Type, RecivedChat, true);
		return;
	}
	UChatingWidget::ChatingWidgetInstance->ReciveChat((TextType)Type, RecivedChat, false);
}

void ChatingManager::SendChat(std::wstring& Chat)
{
	std::string TargetChat = wcs_to_mbs(Chat, std::locale("kor"));
	OutputStream Out;
	Out.WriteOpcode(ENetworkCSOpcode::kSendChatRequest);
	Out.WriteCString(TargetChat.c_str());
	Out.CompletePacketBuild();
	UNetworkManager::GetInstance().SendPacket(Out);
}

int ChatingManager::GetMaxChatSize()
{
	return kMaxChatListSize;
}
