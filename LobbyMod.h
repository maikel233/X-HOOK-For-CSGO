#pragma once


#if !defined(SPDLOG_FMT_EXTERNAL)
#ifndef FMT_HEADER_ONLY
#define FMT_HEADER_ONLY
#endif
#ifndef FMT_USE_WINDOWS_H
#define FMT_USE_WINDOWS_H 0
#endif
#include "extra/fmt/bundled/core.h"
#include "extra/fmt/bundled/format.h"
#else // external fmtlib
#include <extra/fmt/bundled/core.h>
#include <extra/fmt/bundled/format.h>
#endif

class LobbyMod
{
public:
	// returns true if message has been overwritten
	bool InterpretLobbyMessage(CSteamID steamIdLobby, const void* pvMsgBody, int cubMsgBody);

	bool DoesOwnCurrentLobby() const { return IsLobbyOwner; }

	//enum LobbyChatColours {
	//	Color_Standard = 0,
	//	Color_Custom,
	//	Color_Error,
	//	Color_Green,
	//	Color_Yellow
	//};

private:
	// a few easy things to get around the file fast AF
	const char* FindStringEnd(const char* Message);
	const char* FindString(const char* in, const char* pat, unsigned int mxdst = 1000);

	bool ModifyStandardChatMessage(CSteamID Lobby, const char* pMessage, const size_t MessageSize);
	bool OnEnterMatchmakingQueue(CSteamID Lobby, const char* pMessage, const size_t MessageSize);
	bool OnChatInviteMessage(CSteamID Lobby, const char* pMessage, const size_t MessageSize);
	bool OnSetPlayerRanking(CSteamID Lobby, const char* pMessage, const size_t MessageSize);
	bool test(CSteamID Lobby, const char* pMessage, const size_t MessageSize);
	bool OnPlayerUpdated(CSteamID Lobby, const char* pMessage, const size_t MessageSize);

	bool CallOriginalSendLobbyChatMessage(CSteamID steamIdLobby, const void* pvMsgBody, int cubMsgBody);

	bool IsLobbyOwner = false;


private:
	Singletons(LobbyMod);
};