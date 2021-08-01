#include "../Settings.h"
//#include "../Memory.h"
#include "../Config.h"
#include "../ProfileChanger/MemAlloc.h"
#include "../SDK/SteamAPI.h"
#include "../GUI/Tabs/LobbyChanger.h" // ProfileChanger
#include "Protobuffs.h"
#include "../ProfileChanger/Messages.h"
#include "ProtoWriter.h"
#include "../ProfileChanger/valve_parser.h"

#include "../Interfaces.h"

#include <array>
//
//#include "../Protobuf/base_gcmessages.pb.h"
//#include "../Protobuf/cstrike15_gcmessages.pb.h"
//#include "../Protobuf/econ_gcmessages.pb.h"
//#include "../Protobuf/engine_gcmessages.pb.h"
//#include "../Protobuf/gcsystemmsgs.pb.h"
//#include "../Protobuf/gcsdk_gcmessages.pb.h"
//#include "../Protobuf/netmessages.pb.h"
//#include "../Protobuf/steammessages.pb.h"


#include <base_gcmessages.pb.h>
#include <cstrike15_gcmessages.pb.h>
#include <econ_gcmessages.pb.h>
#include <engine_gcmessages.pb.h>
#include <gcsystemmsgs.pb.h>
#include <gcsdk_gcmessages.pb.h>
#include <netmessages.pb.h>
#include <steammessages.pb.h>

#include "../protobuf/ProtoField.h"
//
//#pragma comment(lib, "libprotobuf.lib")
//#pragma comment(lib, "libprotoc.lib")


#include <../ProfileChanger/Messages.h>

#define CAST(cast, address, add) reinterpret_cast<cast>((uint32_t)address + (uint32_t)add)

#define _gc2ch MatchmakingGC2ClientHellos
#define _pci PlayerCommendationInfos
#define _pri PlayerRankingInfos
static std::string ProfileChangerr(void* pubDest, uint32_t* pcubMsgSize)
{
	ProtoWriter msg((void*)((DWORD)pubDest + 8), *pcubMsgSize - 8, 19);
	auto _commendation = msg.has(_gc2ch::commendation) ? msg.get(_gc2ch::commendation).String() : std::string("");
	ProtoWriter commendation(_commendation, 4);
	commendation.replace(Field(_pci::cmd_friendly, TYPE_UINT32, (int64_t)Settings::Misc::misc_Friendly));
	commendation.replace(Field(_pci::cmd_teaching, TYPE_UINT32, (int64_t)Settings::Misc::misc_Teacher));
	commendation.replace(Field(_pci::cmd_leader, TYPE_UINT32, (int64_t)Settings::Misc::misc_Leader));
	msg.replace(Field(_gc2ch::commendation, TYPE_STRING, commendation.serialize()));
	auto _ranking = msg.has(_gc2ch::ranking) ? msg.get(_gc2ch::ranking).String() : std::string("");
	ProtoWriter ranking(_ranking, 6);
	ranking.replace(Field(_pri::rank_id, TYPE_UINT32, (int64_t)Settings::Misc::misc_Rank));
	ranking.replace(Field(_pri::wins, TYPE_UINT32, (int64_t)Settings::Misc::misc_Wins));
	msg.replace(Field(_gc2ch::ranking, TYPE_STRING, ranking.serialize()));
	msg.replace(Field(_gc2ch::player_level, TYPE_INT32, (int64_t)Settings::Misc::misc_Level));
	msg.replace(Field(_gc2ch::player_cur_xp, TYPE_INT32, (int64_t)Settings::Misc::misc_XP));

	if (Settings::Misc::misc_Bantype != 0 && Settings::Misc::misc_Bantime != 0)
	{
		msg.replace(Field(_gc2ch::penalty_reason, TYPE_INT32, (int64_t)Settings::Misc::misc_Bantype));
		msg.replace(Field(_gc2ch::penalty_seconds, TYPE_INT32, (int64_t)Settings::Misc::misc_Bantime));
	}
	return msg.serialize();
}



void Protobuffs::WritePacket(std::string packet, void* thisPtr, void* oldEBP, void* pubDest, uint32_t cubDest, uint32_t* pcubMsgSize)
{
	auto g_MemAlloc = pMemAlloc;
	if ((uint32_t)packet.size() <= cubDest - 8)
	{
		memcpy((void*)((DWORD)pubDest + 8), (void*)packet.data(), packet.size());
		*pcubMsgSize = packet.size() + 8;
	}
	else if (g_MemAlloc)
	{
		auto memPtr = *CAST(void**, thisPtr, 0x14);
		auto memPtrSize = *CAST(uint32_t*, thisPtr, 0x18);
		auto newSize = (memPtrSize - cubDest) + packet.size() + 8;

		auto memory = g_MemAlloc->Realloc(memPtr, newSize + 4);

		*CAST(void**, thisPtr, 0x14) = memory;
		*CAST(uint32_t*, thisPtr, 0x18) = newSize;
		*CAST(void**, oldEBP, -0x14) = memory;

		memcpy(CAST(void*, memory, 24), (void*)packet.data(), packet.size());

		*pcubMsgSize = packet.size() + 8;
	}
}

void Protobuffs::ReceiveMessage(void* thisPtr, void* oldEBP, uint32_t messageType, void* pubDest, uint32_t cubDest, uint32_t* pcubMsgSize)
{
	if (messageType == k_EMsgGCCStrike15_v2_Party_Search && Settings::Lobbychat::LobbyInvite_InviteAll)
	{
		CMsgGCCStrike15_v2_Party_SearchResults message;
		message.ParsePartialFromArray(reinterpret_cast<void*>(reinterpret_cast<uintptr_t>(pubDest) + 8), *pcubMsgSize - 8);

		int invite_count = 0;
		for (auto entry : message.entries())
		{
			CSteamID player;

			player.InstancedSet(entry.id(), 1, k_EUniversePublic, k_EAccountTypeIndividual);
			if (pSteamMatchmaking->InviteUserToLobby(Settings::steamIdLobby, player))
				++invite_count;
			
			pCvar->ConsoleDPrintf("Invited %i players\n", invite_count);
		}
		
		message.SerializeToArray(reinterpret_cast<void*>(reinterpret_cast<uintptr_t>(pubDest) + 8), message.ByteSize());
		*pcubMsgSize = message.ByteSize() + 8;
	}

	else if (messageType == k_EMsgGCCStrike15_v2_MatchmakingGC2ClientHello && Settings::Misc::enabled)
	{
		auto packet = ProfileChangerr(pubDest, pcubMsgSize);
		WritePacket(packet, thisPtr, oldEBP, pubDest, cubDest, pcubMsgSize);
	}
	else if (messageType == k_EMsgGCClientWelcome && Settings::InventoryChanger::medalsEnabled)
	{
		//	CMsgClientWelcome Message;
	}


}

bool Protobuffs::PreSendMessage(uint32_t& unMsgType, void* pubData, uint32_t& cubData)
{
	pProfileChanger->Presend(unMsgType, pubData, cubData);

	if (unMsgType == k_EMsgGCAdjustItemEquippedState)
	{
		return pProfileChanger->Presend(unMsgType, pubData, cubData);
	}

	return true;
}


bool Protobuffs::SendInviteAllHello()
{
	CMsgGCCStrike15_v2_Party_Search message;
	message.set_game_type(8);
	message.set_ver(pEngine->GetEngineBuildNumber());
	message.set_apr(1);
	message.set_ark(160);

	void* ptr = malloc(message.ByteSize() + 8);
	if (!ptr)
		return false;

	uint32_t msgtype = k_EMsgGCCStrike15_v2_Party_Search | (1 << 31);
	std::memcpy(ptr, &msgtype, sizeof(uint32_t));
	reinterpret_cast<uint32_t*>(ptr)[1] = 0;

	message.SerializeToArray(reinterpret_cast<void*>(reinterpret_cast<uintptr_t>(ptr) + 8), message.ByteSize());

	bool result = pSteamGameCoordinator->GCSendMessage(msgtype, ptr, message.ByteSize() + 8) == EGCResult::k_EGCResultOK;

	free(ptr);

	return result;
}

bool Protobuffs::SendClientHello()
{
	ProtoWriter msg(7);
	msg.add(Field(3, TYPE_UINT32, (int64_t)1));
	auto packet = msg.serialize();

	void* ptr = malloc(packet.size() + 8);

	if (!ptr)
		return false;

	((uint32_t*)ptr)[0] = k_EMsgGCClientHello | ((DWORD)1 << 31);
	((uint32_t*)ptr)[1] = 0;

	memcpy((void*)((DWORD)ptr + 8), (void*)packet.data(), packet.size());
	bool result = pSteamGameCoordinator->GCSendMessage(k_EMsgGCClientHello | ((DWORD)1 << 31), ptr, packet.size() + 8) == EGCResult::k_EGCResultOK;
	free(ptr);

	return result;
}


bool Protobuffs::SendMatchmakingClient2GCHello()
{
	ProtoWriter msg(0);
	auto packet = msg.serialize();
	void* ptr = malloc(packet.size() + 8);

	if (!ptr)
		return false;

	((uint32_t*)ptr)[0] = k_EMsgGCCStrike15_v2_MatchmakingClient2GCHello | ((DWORD)1 << 31);
	((uint32_t*)ptr)[1] = 0;

	memcpy((void*)((DWORD)ptr + 8), (void*)packet.data(), packet.size());
	bool result = pSteamGameCoordinator->GCSendMessage(k_EMsgGCCStrike15_v2_MatchmakingClient2GCHello | ((DWORD)1 << 31), ptr, packet.size() + 8) == EGCResult::k_EGCResultOK;
	free(ptr);

	return result;
}