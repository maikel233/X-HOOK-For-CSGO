#include "Tickbase.h"
#include "../Settings.h"
//
bool Settings::Aimbot::TickBaseManipulation = false;
int Settings::Aimbot::TickBaseDoubleTap = 0;
//bool canShift(int ticks, bool shiftAnyways = false)
//{
//
//    C_BasePlayer* LocalPlayer = (C_BasePlayer*)pEntityList->GetClientEntity(pEngine->GetLocalPlayer());
//    if (!LocalPlayer || !LocalPlayer->GetAlive() || !Settings::Aimbot::TickBaseManipulation || ticks <= 0)
//        return false;
//
//    if (shiftAnyways)
//        return true;
//
//    if ((Tickbase::tick->ticksAllowedForProcessing - ticks) < 0)
//        return false;
//
//    if (LocalPlayer->nextAttack() > pGlobalVars->serverTime())
//        return false;
//
//    float nextAttack = (LocalPlayer->nextAttack() + (ticks * pGlobalVars->interval_per_tick));
//    if (nextAttack >= pGlobalVars->serverTime())
//        return false;
//
//    C_BaseCombatWeapon* activeWeapon = (C_BaseCombatWeapon*)pEntityList->GetClientEntityFromHandle(LocalPlayer->GetActiveWeapon());
//    if (!activeWeapon)
//        return false;
//
//    CSWeaponType CurrentWeapon = activeWeapon->GetCSWpnData()->GetWeaponType();
//
//    if (!activeWeapon || !activeWeapon->GetAmmo() || CurrentWeapon == CSWeaponType::WEAPONTYPE_GRENADE)
//        return false;
//
//    if (CurrentWeapon == CSWeaponType::WEAPONTYPE_KNIFE || CurrentWeapon == CSWeaponType::WEAPONTYPE_GRENADE || CurrentWeapon == CSWeaponType::WEAPONTYPE_SHOTGUN
//        || *activeWeapon->GetItemDefinitionIndex() == ItemDefinitionIndex::WEAPON_REVOLVER
//        || *activeWeapon->GetItemDefinitionIndex() == ItemDefinitionIndex::WEAPON_AWP
//        || *activeWeapon->GetItemDefinitionIndex() == ItemDefinitionIndex::WEAPON_SSG08
//        || *activeWeapon->GetItemDefinitionIndex() == ItemDefinitionIndex::WEAPON_TASER
//        || *activeWeapon->GetItemDefinitionIndex() == ItemDefinitionIndex::WEAPON_REVOLVER)
//        return false;
//
//    float shiftTime = (LocalPlayer->GetTickBase() - ticks) * pGlobalVars->interval_per_tick;
//
//    if (shiftTime < activeWeapon->GetNextPrimaryAttack())
//        return false;
//
//    return true;
//}
//
//void recalculateTicks() noexcept
//{
//    Tickbase::tick->chokedPackets = std::clamp(Tickbase::tick->chokedPackets, 0, Tickbase::tick->maxUsercmdProcessticks);
//    Tickbase::tick->ticksAllowedForProcessing = Tickbase::tick->maxUsercmdProcessticks - Tickbase::tick->chokedPackets;
//    Tickbase::tick->ticksAllowedForProcessing = std::clamp(Tickbase::tick->ticksAllowedForProcessing, 0, Tickbase::tick->maxUsercmdProcessticks);
//}
//
//void Tickbase::shiftTicks(int ticks, CUserCmd* cmd, bool shiftAnyways) noexcept //useful, for other funcs
//{
//
//    C_BasePlayer* LocalPlayer = (C_BasePlayer*)pEntityList->GetClientEntity(pEngine->GetLocalPlayer());
//    if (!LocalPlayer || !LocalPlayer->GetAlive() || !Settings::Aimbot::TickBaseManipulation)
//        return;
//    if (!canShift(ticks, shiftAnyways))
//        return;
//    tick->commandNumber = cmd->command_number;
//    tick->tickbase = LocalPlayer->GetTickBase();
//    tick->tickshift = ticks;
//    //Teleport kinda buggy
//    //tick->chokedPackets += ticks;
//    //recalculateTicks();
//}
//
//void Tickbase::run(CUserCmd* cmd) noexcept
//{
//
//    static void* oldNetwork = nullptr;
//    if (auto network = pEngine->GetNetChannelInfo(); network && oldNetwork != network)
//    {
//        oldNetwork = network;
//        tick->ticksAllowedForProcessing = tick->maxUsercmdProcessticks;
//        tick->chokedPackets = 0;
//    }
//    if (auto network = pEngine->GetNetChannelInfo(); network && network->chokedPackets > tick->chokedPackets)
//        tick->chokedPackets = network->chokedPackets;
//
//    recalculateTicks();
//
//    tick->ticks = cmd->tick_count;
//    C_BasePlayer* LocalPlayer = (C_BasePlayer*)pEntityList->GetClientEntity(pEngine->GetLocalPlayer());
//    if (!LocalPlayer || !LocalPlayer->GetAlive() || !Settings::Aimbot::TickBaseManipulation)
//        return;
//
//    auto ticks = 0;
//
//    switch (Settings::Aimbot::TickBaseDoubleTap) {
//    case 0: //Instant
//        ticks = 16;
//        break;
//    case 1: //Fast
//        ticks = 14;
//        break;
//    case 2: //Accurate
//        ticks = 12;
//        break;
//    }
//
//    if (Settings::Aimbot::TickBaseDoubleTap && (cmd->buttons & IN_ATTACK))
//        shiftTicks(ticks, cmd);
//
//    if (tick->tickshift <= 0 && tick->ticksAllowedForProcessing < (tick->maxUsercmdProcessticks - tick->fakeLag)) /*&& !config->antiAim.fakeDucking && ((config->antiAim.fakeLag && config->antiAim.fakeLagAmount <= (tick->maxUsercmdProcessticks - ticks)) || !config->antiAim.fakeLag))*/
//    {
//        cmd->tick_count = INT_MAX; //recharge
//        tick->chokedPackets--;
//    }
//
//    recalculateTicks();
//}