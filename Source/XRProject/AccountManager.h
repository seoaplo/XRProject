#include "CoreMinimal.h"
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
public:
	std::string GetAccountID() { return AccountID; };
	std::string GetLobbyIP() { return LobbyIP; };
	int16 GetLobbyPort() { return LobbyPort; };
	void SetAccountID(std::string NewAccountID) { AccountID = NewAccountID; };
	void SetLobbyIP(std::string NewLobbyIP) { LobbyIP = NewLobbyIP; };
	void SetLobbyPort(int16 NewLobbyPort) { LobbyPort = NewLobbyPort; };
};