#include "CGlobalVars.h"
#include "../Interfaces.h"

float CGlobalVars::serverTime(CUserCmd* cmd) const noexcept
{
   static int tick;
    static CUserCmd* lastCmd;

    if (cmd) {
        C_BasePlayer* localplayer = (C_BasePlayer*)pEntityList->GetClientEntity(pEngine->GetLocalPlayer());
        if (localplayer && (!lastCmd || lastCmd->hasbeenpredicted))
            tick = localplayer->GetTickBase();
        else
            tick++;
        lastCmd = cmd;
    }
    return tick * interval_per_tick;
}