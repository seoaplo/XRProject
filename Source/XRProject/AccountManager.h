#pragma once
#include "CoreMinimal.h"
#include "PlayerCharacter.h"
#include <string>
#include "CustomSingleton.h"

class AccountManager : public CustomSingleton<AccountManager>
{
public:
	friend class CustomSingleton<AccountManager>;
private:
	AccountManager() {};
public:
	~AccountManager() {};
private:
	std::string AccountID;
	std::string LobbyIP;
	int16 LobbyPort;
	std::string InGameIP;
	int16 InGamePort;
	APlayerCharacter* CurrentPlayerCharacter;

public:
	std::string GetAccountID() { return AccountID; };
	std::string GetLobbyIP() { return LobbyIP; };
	int16 GetLobbyPort() { return LobbyPort; };
	void SetAccountID(std::string NewAccountID) { AccountID = NewAccountID; };
	void SetLobbyIP(std::string NewLobbyIP) { LobbyIP = NewLobbyIP; };
	void SetLobbyPort(int16 NewLobbyPort) { LobbyPort = NewLobbyPort; };
	std::string GetInGameIP() { return InGameIP; };
	int16 GetInGamePort() { return InGamePort; };
	void SetInGameIP(std::string NewInGameIP) { InGameIP = NewInGameIP; };
	void SetInGamePort(int16 NewInGamePort) { InGamePort = NewInGamePort; };
	bool SetCurrentPlayerCharacter(APlayerCharacter* Character)
	{
		CurrentPlayerCharacter = Character;
		if (CurrentPlayerCharacter == nullptr)
			return false;
		else
			return true;
	};
	APlayerCharacter* GetCurrentPlayerCharacter() { return CurrentPlayerCharacter; };
};