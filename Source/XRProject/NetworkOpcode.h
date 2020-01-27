#pragma once
#include <cstdint>

enum class ENetworkCSOpcode : uint16_t
{
    // LoginServer
    kCreateAccountRequest,
    kLoginRequest,

    // LobbyServer
    kLobbyConfirmRequest,
    kCharacterListRequest,
    kCharacterCreateRequest,
    kCharacterDeleteRequest,
    kCharacterSelectionRequest,

    // ZoneServer
    kZoneConrifmRequest,
    kZoneInitializeRequest,
    kNotifyCurrentChrPosition,
};


enum class ENetworkSCOpcode : uint16_t
{
    // LoginServer
    kCreateAccountResult,
    kLoginResult,

    // LobbyServer
    kConfirmFailNotify,
    kCharacterListNotify,
    kMigrateZoneNotify,

    // ZoneServer
    kUserConfirmResult,
    kUserEnterZone,
    kBroadCastChrPosition,
};