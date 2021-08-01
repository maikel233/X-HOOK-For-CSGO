#include "../Features.h"

bool Settings::MoonWalk::enabled = false;

void MoonWalk::CreateMove(CUserCmd* cmd)
{

	if (!Settings::MoonWalk::enabled)
		return;

	C_BasePlayer* localplayer = (C_BasePlayer*)pEntityList->GetClientEntity(pEngine->GetLocalPlayer());
	if (!localplayer)
		return;

	if (cmd->forwardmove > 0)
	{
		cmd->buttons |= IN_BACK;
		cmd->buttons &= ~IN_FORWARD;
	}

	if (cmd->forwardmove < 0)
	{
		cmd->buttons |= IN_FORWARD;
		cmd->buttons &= ~IN_BACK;
	}

	if (cmd->sidemove < 0)
	{
		cmd->buttons |= IN_MOVERIGHT;
		cmd->buttons &= ~IN_MOVELEFT;
	}

	if (cmd->sidemove > 0)
	{
		cmd->buttons |= IN_MOVELEFT;
		cmd->buttons &= ~IN_MOVERIGHT;
	}
}


