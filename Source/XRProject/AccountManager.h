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
	FString AccountID;
	string LobbyIP;
	int16 LobbyPort;
public:
	FString GetAccountID() { return AccountID; };
	string GetLobbyIP() { return LobbyIP; };
	int16 GetLobbyPort() { return LobbyPort; };
	void SetAccountID(FString NewAccountID) { AccountID = NewAccountID; };
	void SetLobbyIP(string NewLobbyIP) { LobbyIP = NewLobbyIP; };
	void SetLobbyPort(int16 NewLobbyPort) { LobbyPort = NewLobbyPort; };
};