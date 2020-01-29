#pragma once
#include <cstdint>

enum class ENetworkCSOpcode : uint16_t
{
    // LoginServer
    kCreateAccountRequest = 0,
    kLoginRequest,

    // LobbyServer
    kLobbyConfirmRequest = 100,
    kCharacterListRequest,
    kCharacterCreateRequest,
    kCharacterDeleteRequest,
    kCharacterSelectionRequest,

    // ZoneServer
    kZoneConrifmRequest = 1000,
    kZoneInitializeRequest,
    kNotifyCurrentChrPosition,
};


enum class ENetworkSCOpcode : uint16_t
{
    // LoginServer
    kCreateAccountResult = 0,
    kLoginResult,

    // LobbyServer
    kConfirmFailNotify = 100,
    kCharacterCreateNotify,
    kCharacterDeleteNotify,
    kCharacterListNotify,
    kMigrateZoneNotify,

    // ZoneServer
    kUserConfirmResult = 1000,
    kMapInitialize,
    kSpawnLocalPlayer,
    kSpawnRemotePlayer,
    kExitRemotePlayer,

    kSpawnMonster,
    kSpawnPlayer,


    kBroadCastChrPosition,
    kInventoryUpdate,
    kQuickSlotUpdate,

};