#include "../../Features.h"

bool Settings::InventoryChanger::enabled = false;
bool Settings::InventoryChanger::medalsEnabled = false;

bool Settings::Misc::enabled = false;
int Settings::Misc::misc_Rank = 18;
int Settings::Misc::misc_Level = 11;
int Settings::Misc::misc_XP = 1;
int Settings::Misc::misc_Wins = 1337;
int Settings::Misc::misc_Friendly = 1337;
int Settings::Misc::misc_Leader = 1337;
int Settings::Misc::misc_Teacher = 1337;
int Settings::Misc::misc_Bantype = 0;
int Settings::Misc::misc_Bantime = 0;

bool Settings::Lobbychat::LobbyInvite_InviteAll = false;

std::vector<uint32_t> Settings::InventoryChanger::medals = { };
bool Settings::InventoryChanger::equipped_medal_override;
uint32_t Settings::InventoryChanger::equipped_medal;

const char* ChatColours[] = { "Off", "Custom", "Red", "Green", "Yellow", "Unknow", "Unknow2", "Unknow3", "Unknow4" };
const char* ChatColoursNonOwner[] = { "Off", "Custom", "Red" };

#define IM_ARRAYSIZE(_ARR)          ((int)(sizeof(_ARR) / sizeof(*(_ARR)))) 

#include <base_gcmessages.pb.h>
#include <cstrike15_gcmessages.pb.h>
#include <econ_gcmessages.pb.h>
#include <engine_gcmessages.pb.h>
#include <gcsystemmsgs.pb.h>
#include <gcsdk_gcmessages.pb.h>
#include <netmessages.pb.h>
#include <steammessages.pb.h>

#include "../protobuf/ProtoField.h"

bool ProfileChanger::Presend(uint32_t& unMsgType, void* pubData, uint32_t& cubData)
{
	//uint32_t MessageType = unMsgType & 0x7FFFFFFF;

	//if (MessageType == k_EMsgGCAdjustItemEquippedState) {

	//	ProtoWriter msg((void*)((DWORD)pubData + 8), cubData - 8, 19);

	//	if (!msg.has(CMsgAdjustItemEquippedState::item_id)
	//		|| !msg.has(CMsgAdjustItemEquippedState::new_class)
	//		|| !msg.has(CMsgAdjustItemEquippedState::new_slot))
	//		return true;

	//	uint32_t item_id = msg.get(CMsgAdjustItemEquippedState::item_id).UInt32();
	//	uint32_t new_class = msg.get(CMsgAdjustItemEquippedState::new_class).UInt32();

	//	pClientState->ForceFullUpdate();

	//	//return false;
	//}
	return true;
}

void ProfileChanger::RenderWindow(bool contentOnly) noexcept
{
    bool profileWindowOpen = true;
    if (!contentOnly) {
        if (!profileWindowOpen)
            return;
        ImGui::SetNextWindowSize({ 290.0f, 0.0f });
        ImGui::Begin("Rank Changer", &profileWindowOpen, ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoScrollWithMouse);
    }

    static const char* bans_gui[] =
    {
        "Off",
        "You were kicked from the last match (competitive cooldown)",
        "You killed too many teammates (competitive cooldown)",
        "You killed a teammate at round start (competitive cooldown)",
        "You failed to reconnect to the last match (competitive cooldown)",
        "You abandoned the last match (competitive cooldown)",
        "You did too much damage to your teammates (competitive cooldown)",
        "You did too much damage to your teammates at round start (competitive cooldown)",
        "This account is permanently untrusted (global cooldown)",
        "You were kicked from too many recent matches (competitive cooldown)",
        "Convicted by overwatch - majorly disruptive (global cooldown)",
        "Convicted by overwatch - minorly disruptive (global cooldown)",
        "Resolving matchmaking state for your account (temporary cooldown)",
        "Resolving matchmaking state after the last match (temporary cooldown)",
        "This account is permanently untrusted (global cooldown)",
        "(global cooldown)",
        "You failed to connect by match start. (competitive cooldown)",
        "You have kicked too many teammates in recent matches (competitive cooldown)",
        "Congratulations on your recent competitive wins! before you play competitive matches further please wait for matchmaking servers to calibrate your skill group placement based on your lastest performance. (temporary cooldown)",
        "A server using your game server login token has been banned. your account is now permanently banned from operating game servers, and you have a cooldown from connecting to game servers. (global cooldown)"
    };
    const char* ranks_gui[] = {
        "Off",
        "Silver 1",
        "Silver 2",
        "Silver 3",
        "Silver 4",
        "Silver Elite",
        "Silver Elite Master",
        "Gold Nova 1",
        "Gold Nova 2",
        "Gold Nova 3",
        "Gold Nova Master",
        "Master Guardian 1",
        "Master Guardian 2",
        "Master Guardian elite",
        "Distinguished Master Guardian",
        "Legendary Eagle",
        "Legendary Eagle master",
        "Supreme Master First Class",
        "The Global Elite"
    };
    ImGui::Columns(2, nullptr, false);
    ImGui::Checkbox("Enabled (May make MM times longer)##profile", &Settings::Misc::enabled);
    ImGui::Text("Rank");
    ImGui::Combo("##Rank", &Settings::Misc::misc_Rank, ranks_gui, ARRAYSIZE(ranks_gui));
    ImGui::Text("Level");
    ImGui::SliderInt("##Level", &Settings::Misc::misc_Level, 0, 40);
    ImGui::Text("XP");
    ImGui::InputInt("##Xp##level", &Settings::Misc::misc_XP);
    ImGui::Text("Wins");
    ImGui::InputInt("##Wins", &Settings::Misc::misc_Wins);
    if (ImGui::Button("Apply", ImVec2(190, 30)))
    {
        write.SendClientHello();
        write.SendMatchmakingClient2GCHello();
    }


    if (ImGui::Checkbox("Invite All", &Settings::Lobbychat::LobbyInvite_InviteAll))
    {
       write.SendInviteAllHello();
    }
    ImGui::NextColumn();
    ImGui::Text("Friend");
    ImGui::InputInt("##Friend", &Settings::Misc::misc_Friendly);
    ImGui::Text("Teach");
    ImGui::InputInt("##Teach", &Settings::Misc::misc_Teacher);
    ImGui::Text("Leader");
    ImGui::InputInt("##Leader", &Settings::Misc::misc_Leader);
    ImGui::Text("Fake ban type");
    ImGui::Combo("##fake-ban", &Settings::Misc::misc_Bantype, bans_gui, IM_ARRAYSIZE(bans_gui));
    ImGui::Text("Fake ban time");
    ImGui::SliderInt("##fake-ban-time", &Settings::Misc::misc_Bantime, 0, 1000, "Seconds: %0.f");
    ImGui::SetCursorPosX(ImGui::GetCursorPosX() + (250 / 2) - (190 / 2) - 20.f);
    ImGui::Text("More");
    if (LobbyMod::Get()->DoesOwnCurrentLobby())
    ImGui::Combo("Chat Colour", (int*)&Settings::Lobbychat::preset, ChatColours, ARRAYSIZE(ChatColours));
    else
    ImGui::Combo("Chat Colour", (int*)& Settings::Lobbychat::preset, ChatColoursNonOwner, ARRAYSIZE(ChatColoursNonOwner));

    if (Settings::Lobbychat::preset != chatType::Color_Standard)
    {
        if (Settings::Lobbychat::preset == chatType::Color_Custom)
            ImGui::InputText("Chat Format", Settings::Lobbychat::LobbyChat_Format, 256);
        else
            ImGui::Checkbox("Prepend Name", &Settings::Lobbychat::LobbyChat_PrependName);

        ImGui::Checkbox("Endline Spam", &Settings::Lobbychat::LobbyChat_EndlineSpam);
        ImGui::SliderFloat("Repeat Exponent", &Settings::Lobbychat::LobbyChat_RepeatExponent, 0.f, 3.f, "%.2f");
    }

    ImGui::Separator();

    ImGui::Checkbox("Hide Invite Names", &Settings::Lobbychat::LobbyInvite_HideInviteNames);

    if (LobbyMod::Get()->DoesOwnCurrentLobby())
    {
        ImGui::Separator();

        //Queue Bomb reporting.

        ImGui::Checkbox("Queue Bomb", &Settings::Lobbychat::LobbyNuke_Enable);
        ImGui::SliderFloat("Exponent", &Settings::Lobbychat::LobbyNuke_Exponent, 0.f, 3.f, "%.2f");
    }
    ImGui::Separator();

    ImGui::Checkbox("Modify Player Profiles", &Settings::Lobbychat::LobbyRank_ModifyProfiles);

    ImGui::SliderInt("Player Rank", &Settings::Lobbychat::LobbyRank_PlayerRank, 0, 18);
    ImGui::SliderInt("Player Level", &Settings::Lobbychat::LobbyRank_PlayerLevel, 0, 50);
    ImGui::Checkbox("Prime", &Settings::Lobbychat::LobbyRank_Prime);

    //if (LobbyMod::Get()->DoesOwnCurrentLobby())
    //{
        ImGui::Separator();

        ImGui::SliderInt("Others Player Rank", &Settings::Lobbychat::LobbyRank_OtherPlayersRank, 0, 18);
        ImGui::SliderInt("Others Player Level", &Settings::Lobbychat::LobbyRank_OtherPlayersLevel, 0, 50);
        ImGui::Checkbox("Others Prime Status", &Settings::Lobbychat::LobbyRank_OtherPlayersPrime);
    //}

    if (!contentOnly)
        ImGui::End();
}


