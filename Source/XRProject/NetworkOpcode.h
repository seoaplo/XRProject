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
	kSendChatRequest = 1011,

	kRequestCharacterWait = 1020,
	kRequestCharacterSprint = 1021,

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
	kSpawnCharacter = 1002,
	kSetMonsterController = 1003,

	kNotifyCharacterAttack = 1004,
	kNotifyCharacterRolling = 1005,
	kActorDamaged = 1006,

	kExitCharacter = 1007,
	kUpdateCharacterPosition = 1008,
	kUpdateMonsterAction = 1009,

	kSpawnMonster = 1010,

	kInventoryUpdate = 1011,
	kQuickSlotUpdate = 1012,

	kNotifyMatchResult = 1013,
	kNotifyMatchCanceled = 1014,
	kNotifyChat = 1015,

	kNotifyCharacterWait = 1020,
	kNotifyCharacterSprint = 1021,
};