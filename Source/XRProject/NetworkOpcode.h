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

	kCharacterAction = 1008,
	kCharcterHitSuccess = 1009,

	kMonsterHitCharacter = 1010,
	kSendChatRequest = 1011,
	
	kNotifiyLoadComplete = 1012,

	kRequestCharacterWait = 1020,
	kRequestCharacterSprint = 1021,
	kRequestChangeZone = 1022,
	kRequestCharacterRolling = 1023,

	kNoticeMonsterSkill = 1024,
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
	kSpawnCharacter = 1001,
	kSetMonsterController = 1002,

	kNotifyCharacterAction = 1003,
	kNotifyCharacterRolling = 1004,
	kActorDamaged = 1005,

	kExitCharacter = 1006,
	kUpdateCharacterPosition = 1007,
	kUpdateMonsterAction = 1008,

	kSpawnMonster = 1009,

	kInventoryUpdate = 1010,
	kQuickSlotUpdate = 1011,

	kNotifyMatchResult = 1012,
	kNotifyMatchCanceled = 1013,
	kNotifyChat = 1014,
	kNotifiyStartLevel = 1015,

	kNotifyCharacterWait = 1020,
	kNotifyCharacterSprint = 1021,

	kNotifySpawnPotal = 1022,
	kNotifyCharacterDead = 1023,

	kNotifyStatChange = 1024,
	kNotifyDungeonReward = 1025,
	
	kNotifyMultiPlayerEquipChange = 1026,
};