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
    kNotifyCurrentChrPosition = 1001,
    kNotifyMonsterAction = 1002,

    kInventoryUpdateRequest = 1003,
    kQuickSlotUseRequest = 1004,
    kEquipmentMountRequest = 1005,

    kMatchingRequest = 1006,
    kMatchingConfirmRequest = 1007, // true / false

    kCharacterAttack = 1008,
    kCharcterHitSuccess = 1009,

    kMonsterHitCharacter = 1010,
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
    kUserEnterTheMap = 1000,
    kSpawnCharacter,
    kSetMonsterController,

    kNotifyCharacterAttack,
    kNotifyCharacterRolling,
    kActorDamaged,

    kExitCharacter,
    kUpdateCharacterPosition,
    kUpdateMonsterAction,

    kSpawnMonster,

    kInventoryUpdate,
    kQuickSlotUpdate,

    kNotifyMatchResult,
    kNotifyMatchCanceled,

};