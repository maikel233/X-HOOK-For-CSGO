#include "../Features.h"

bool Settings::NoDuckCooldown::enabled = false;

void NoDuckCooldown::CreateMove(CUserCmd* cmd)
{
	if (!Settings::NoDuckCooldown::enabled)
		return;

	C_BasePlayer* localplayer = (C_BasePlayer*)pEntityList->GetClientEntity(pEngine->GetLocalPlayer());

	if (!localplayer || !localplayer->GetAlive())
		return;

	cmd->buttons |= IN_BULLRUSH;
}