#include "../Features.h"

#include <algorithm>
bool Settings::Reportbot::Enabled = false;
bool Settings::Reportbot::textAbuse = false;
bool Settings::Reportbot::griefing = false;
bool Settings::Reportbot::wallhack = true;
bool Settings::Reportbot::aimbot = true;
bool Settings::Reportbot::other = true;
int Settings::Reportbot::target = 0;
int Settings::Reportbot::delay = 1;
int Settings::Reportbot::rounds = 1;

static std::vector<std::uint64_t> reportedPlayers;
static int reportbotRound;


bool ReportBot::submitReport(const char* xuid, const char* report)
{
    return reinterpret_cast<bool(__stdcall*)(const char*, const char*)>(offsets.sigs.SubmitReport)(xuid, report);
}


void ReportBot::run() noexcept
{
    if (!Settings::Reportbot::Enabled)
        return;

    C_BasePlayer* LocalPlayer = (C_BasePlayer*)pEntityList->GetClientEntity(pEngine->GetLocalPlayer());
    if (!LocalPlayer)
        return;

    static auto lastReportTime = 0.0f;

    if (lastReportTime + Settings::Reportbot::delay > pGlobalVars->realtime)
        return;

    if (reportbotRound >= Settings::Reportbot::rounds)
        return;

    for (int i = 1; i < pEngine->GetMaxClients(); ++i) {
        C_BasePlayer* player = (C_BasePlayer*)pEntityList->GetClientEntity(i);

        if (!player
            || player == LocalPlayer || player->GetTeam() == LocalPlayer->GetTeam())
            continue;


        if (Settings::Reportbot::target != 2 ? Settings::Reportbot::target != 0 : Settings::Reportbot::target != 1)
            continue;

        C_BasePlayer* LocalPlayer = (C_BasePlayer*)pEntityList->GetClientEntity(pEngine->GetLocalPlayer());
        IEngineClient::player_info_t playerInfo;

        if (!pEngine->GetPlayerInfo(i, &playerInfo))
            continue;

        if (playerInfo.fakeplayer || std::find(reportedPlayers.begin(), reportedPlayers.end(),
            playerInfo.xuid) != reportedPlayers.end())
            continue;

        std::string report;

        if (Settings::Reportbot::textAbuse)
            report += "textabuse,";
        if (Settings::Reportbot::griefing)
            report += "grief,";
        if (Settings::Reportbot::wallhack)
            report += "wallhack,";
        if (Settings::Reportbot::aimbot)
            report += "aimbot,";
        if (Settings::Reportbot::other)
            report += "speedhack,";

        if (!report.empty()) {
            submitReport(std::to_string(playerInfo.xuid).c_str(), report.c_str());
            lastReportTime = pGlobalVars->realtime;
            reportedPlayers.push_back(playerInfo.xuid);
        }
        return;
    }

    reportedPlayers.clear();
    ++reportbotRound;
}

void ReportBot::resetReportbot() noexcept
{
    reportbotRound = 0;
    reportedPlayers.clear();
}