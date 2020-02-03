#pragma once

#include <list>
#include <string>
#include <locale>
#include "CustomSingleton.h"

const int kMaxChatListSize = 100;

enum class TextType
{
	SystemMessage = 0,
	NormalMessage,
};

struct ChatStruct
{
	TextType Type;
	std::wstring Data;
	ChatStruct(TextType fType, std::wstring fData)
	{
		Type = fType;
		Data = fData;
	};
};

class ChatingManager : public CustomSingleton<ChatingManager>
{
public:
	friend class CustomSingleton<ChatingManager>;
private:
	ChatingManager();
public:
	~ChatingManager();
public:
	std::list<ChatStruct> ChatList;
public:
	int GetMaxChatSize();
	void ReceiveChat(int Type, std::string& Chat);
	void SendChat(std::wstring& Chat);

};