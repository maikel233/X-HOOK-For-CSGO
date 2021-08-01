#include "LagComp.h"
#include "../Settings.h"
#include "../Utils/Entity.h"
#include <iostream>
#include <string>
#include "Vector.h"
#include <algorithm>
#include <array>
#include "../Utils/XorStr.hpp"
std::deque<Backtrack::IncomingSequence>Backtrack::sequences;
std::array<std::deque<Backtrack::Record>, 513> records;

bool Settings::Aimbot::fakeLat = false;
bool Settings::Aimbot::backtron = false;
bool Settings::Aimbot::ignoreSmoke = false;
bool Settings::Aimbot::recoilBasedFov = false;
int Settings::Aimbot::timeLimit = 0;
bool Settings::Aimbot::drawAllChams = false;


//std::deque<Backtrack::Record> Backtrack::records[65];
// static Backtrack::Cvars cvars; // From Master, but this breaks code in hooks.cpp line 132 max() function
//Backtrack::Cvars Backtrack::cvars;

struct Cvars {
    ConVar* updateRate;
    ConVar* maxUpdateRate;
    ConVar* interp;
    ConVar* interpRatio;
    ConVar* minInterpRatio;
    ConVar* maxInterpRatio;
    ConVar* maxUnlag;
};

static Cvars cvars;

static auto timeToTicks(float time) noexcept
{
    return static_cast<int>(0.5f + time / pGlobalVars->interval_per_tick);
}

void Backtrack::update(ClientFrameStage_t stage) noexcept
{
    if (!pEngine->IsInGame())
        return;

    if (Settings::Aimbot::timeLimit == 0)
        return;

    //if (stage == FrameStage::RENDER_START) {
    int timeLimit = Settings::Aimbot::timeLimit;
    if (timeLimit <= 0 || timeLimit >= 201) { Settings::Aimbot::fakeLat = true; }
    else { Settings::Aimbot::fakeLat = false; }
    if (timeLimit <= 0 || timeLimit > 400) { Settings::Aimbot::backtron = false; }
    else { Settings::Aimbot::backtron = true; }

    if (stage == ClientFrameStage_t::FRAME_RENDER_START)
    {
        C_BasePlayer* LocalPlayer = (C_BasePlayer*)pEntityList->GetClientEntity(pEngine->GetLocalPlayer());
        if (!Settings::Aimbot::backtron || !LocalPlayer || !LocalPlayer->GetAlive()) {
            for (auto& record : records)
                for (auto& record : records)
                    record.clear();
            return;
        }

            for (int i = 1; i < pEngine->GetMaxClients(); ++i) {
                C_BasePlayer* player = (C_BasePlayer*)pEntityList->GetClientEntity(i);

                if (!player
                    || player == LocalPlayer
                    || player->GetDormant()
                    || !player->GetAlive()
                    || player->GetImmune()
                    || player->GetTeam() == pEntityList->GetClientEntity(pEngine->GetLocalPlayer())->GetTeam()) {
                    records[i].clear();
                    continue;
            }
            
            if (!records[i].empty() && (records[i].front().simulationTime == player->GetSimulationTime()))
                continue;


            Record record{ };
            record.headPos = player->GetBonePosition((int)Bone::BONE_HEAD);
            record.origin = player->getAbsOrigin();
            record.simulationTime = player->GetSimulationTime();
            record.mins = player->GetCollideable()->OBBMins();
            record.max = player->GetCollideable()->OBBMaxs();
            
            player->SetupBones(record.matrix, 256, 0x7FF00, pGlobalVars->curtime);
            records[i].push_front(record);

            while (records[i].size() > 3 && records[i].size() > static_cast<size_t>(timeToTicks(static_cast<float>(Settings::Aimbot::timeLimit) / 1000.f + getExtraTicks())))
                records[i].pop_back();

            if (auto invalid = std::find_if(std::cbegin(records[i]), std::cend(records[i]), [](const Record& rec) { return !valid(rec.simulationTime); }); invalid != std::cend(records[i]))
                records[i].erase(invalid, std::cend(records[i]));
                
        }
    }
}

static float getLerp() noexcept
{
    auto ratio = std::clamp(cvars.interpRatio->GetFloat(), cvars.minInterpRatio->GetFloat(), cvars.maxInterpRatio->GetFloat());
    return (std::max)(cvars.interp->GetFloat(), (ratio / ((cvars.maxUpdateRate) ? cvars.maxUpdateRate->GetFloat() : cvars.updateRate->GetFloat())));
}

//constexpr auto degreesToRadians = [](float degrees) constexpr noexcept { return degrees * static_cast<float>(M_PI) / 180.0f; };
constexpr auto radiansToDegrees = [](float radians) constexpr noexcept { return radians * 180.0f / static_cast<float>(M_PI); };
Vector calculateRelativeAngle(const Vector& source, const Vector& destination, const Vector& viewAngles) noexcept
{
    Vector delta = destination - source;
    Vector angles{ radiansToDegrees(atan2f(-delta.z, std::hypotf(delta.x, delta.y))) - viewAngles.x,
                   radiansToDegrees(atan2f(delta.y, delta.x)) - viewAngles.y, 0.f };
    angles.normalize2();
    return angles;
}
void Backtrack::run(CUserCmd* cmd) noexcept
{
    if (!Settings::Aimbot::backtron)
        return;

    C_BasePlayer* LocalPlayer = (C_BasePlayer*)pEntityList->GetClientEntity(pEngine->GetLocalPlayer());
    if (!LocalPlayer)
        return;

    if (!(cmd->buttons & IN_ATTACK))
        return;

    auto localPlayerEyePosition = LocalPlayer->GetEyePosition();

    auto bestFov{ 255.f };
    C_BasePlayer* bestTarget{ };
    int bestTargetIndex{ };
    Vector bestTargetOrigin{ };
    int bestRecord{ };

    const auto aimPunch = LocalPlayer->GetAimPunchAngle();

    for (int i = 1; i < pEngine->GetMaxClients(); ++i) {
        C_BasePlayer* player = (C_BasePlayer*)pEntityList->GetClientEntity(i);

        if (!player
            || player == LocalPlayer
            || player->GetDormant()
            || !player->GetAlive()
            || player->GetImmune()
            || player->GetTeam() == pEntityList->GetClientEntity(pEngine->GetLocalPlayer())->GetTeam())
            continue;


        const auto& Head = player->GetBonePosition((int)Bone::BONE_HEAD);

        auto angle = calculateRelativeAngle(localPlayerEyePosition, Head, cmd->viewangles + *aimPunch);
        auto fov = std::hypotf(angle.x, angle.y);

        if (fov < bestFov) {
            bestFov = fov;
            bestTarget = player;
            bestTargetIndex = i;
            bestTargetOrigin = Head;
        }
    }

    if (bestTarget) {

        if (records[bestTargetIndex].size() <= 3 || (!Settings::Aimbot::SmokeCheck::enabled && Entity::LineGoesThroughSmoke(LocalPlayer->GetEyePosition(), bestTargetOrigin, true)))
            return;

        bestFov = 255.f;

        for (size_t i = 0; i < records[bestTargetIndex].size(); i++) {
            const auto& record = records[bestTargetIndex][i];
            if (!valid(record.simulationTime))
                continue;

            auto angle = calculateRelativeAngle(localPlayerEyePosition, record.origin, cmd->viewangles + *aimPunch);
            auto fov = std::hypotf(angle.x, angle.y);
            if (fov < bestFov) {
                bestFov = fov;
                bestRecord = i;
            }
        }
    }

    if (bestRecord) {
        const auto& record = records[bestTargetIndex][bestRecord];
       //  Backtrack::setAbsOrigin(bestTarget, record.origin);
        cmd->tick_count = timeToTicks(record.simulationTime + getLerp());
    }
}

std::deque<Backtrack::Record>* Backtrack::getRecords(std::size_t index) noexcept
{
    if (!Settings::Aimbot::backtron)
        return nullptr;
    return &records[index];
}

bool Backtrack::valid(float simtime) noexcept
{
    const auto network = pEngine->GetNetChannelInfo();
    if (!network)
        return false;

    auto delta = std::clamp(network->getLatency(0) + network->getLatency(1) + getLerp(), 0.f, cvars.maxUnlag->GetFloat()) - (pGlobalVars->serverTime() - simtime);
    return std::abs(delta) <= 0.2f;
}

float Backtrack::getExtraTicks() noexcept
{
    auto network = pEngine->GetNetChannelInfo();
    if (!network)
        return 0.f;

    return std::clamp(network->getLatency(1) - network->getLatency(0), 0.f, cvars.maxUnlag->GetFloat());
}

void Backtrack::AddLatencyToNetwork(NetworkChannel* network, float latency) noexcept
{
    for (auto& sequence : sequences)
    {
        if (pGlobalVars->serverTime() - sequence.servertime >= latency)
        {
            network->InReliableState = sequence.inreliablestate;
            network->InSequenceNr = sequence.sequencenr;
            break;
        }
    }
}

void Backtrack::UpdateIncomingSequences(bool reset) noexcept
{
    static float lastIncomingSequenceNumber = 0.f;

    if (!Settings::Aimbot::backtron)
        return;

    if (Settings::Aimbot::timeLimit == 0)
        return;

    auto network = pEngine->GetNetChannelInfo();
    if (!network)
        return;

    if (network->InSequenceNr != lastIncomingSequenceNumber)
    {
        lastIncomingSequenceNumber = static_cast<float>(network->InSequenceNr);

        IncomingSequence sequence{ };
        sequence.inreliablestate = network->InReliableState;
        sequence.sequencenr = network->InSequenceNr;
        sequence.servertime = pGlobalVars->serverTime();
        sequences.push_front(sequence);
    }

    while (sequences.size() > 2048)
        sequences.pop_back();
}

void Backtrack::init() noexcept
{
    cvars.updateRate = pCvar->FindVar("cl_updaterate");
    cvars.maxUpdateRate = pCvar->FindVar("sv_maxupdaterate");
    cvars.interp = pCvar->FindVar("cl_interp");
    cvars.interpRatio = pCvar->FindVar("cl_interp_ratio");
    cvars.minInterpRatio = pCvar->FindVar("sv_client_min_interp_ratio");
    cvars.maxInterpRatio = pCvar->FindVar("sv_client_max_interp_ratio");
    cvars.maxUnlag = pCvar->FindVar("sv_maxunlag");
}