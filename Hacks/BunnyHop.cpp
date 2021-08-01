#include "../Features.h"

//bhop
bool Settings::BHop::enabled = false;
bool Settings::BHop::Chance::enabled = false;
int Settings::BHop::Chance::value = 70;
bool Settings::BHop::Hops::enabledMax = false;
int Settings::BHop::Hops::Max = 7;
bool Settings::BHop::Hops::enabledMin = false;
int Settings::BHop::Hops::Min = 3;
//circle strafe
bool Settings::CircleStrafe::enabled = false;
ButtonCode_t Settings::CircleStrafe::key = ButtonCode_t::KEY_F;
Vector AutoStrafeView;

void BHop::CreateMove(CUserCmd* cmd)
{
	if (!Settings::BHop::enabled)
		return;

	static bool bLastJumped = false;
	static bool bShouldFake = false;
	static int bActualHop = 0;

	C_BasePlayer* localplayer = (C_BasePlayer*)pEntityList->GetClientEntity(pEngine->GetLocalPlayer());

	if (!localplayer)
		return;

	if (localplayer->GetMoveType() == MOVETYPE_LADDER || localplayer->GetMoveType() == MOVETYPE_NOCLIP)
		return;


	if (!bLastJumped && bShouldFake)
	{
		bShouldFake = false;
		cmd->buttons |= IN_JUMP;
	}
	else if (cmd->buttons & IN_JUMP)
	{
		if (localplayer->GetFlags() & FL_ONGROUND)
		{
			bActualHop++;
			bLastJumped = true;
			bShouldFake = true;
		}
		else
		{
			if (Settings::BHop::Chance::enabled &&
				Settings::BHop::Hops::enabledMin &&
				(bActualHop > Settings::BHop::Hops::Min) &&
				(rand() % 100>Settings::BHop::Chance::value))
				return;

			if (Settings::BHop::Chance::enabled &&
				!Settings::BHop::Hops::enabledMin &&
				(rand() % 100 > Settings::BHop::Chance::value))
				return;

			if (Settings::BHop::Hops::enabledMin &&
				!Settings::BHop::Chance::enabled &&
				(bActualHop > Settings::BHop::Hops::Min))
				return;

			if (Settings::BHop::Hops::enabledMax &&
				(bActualHop>Settings::BHop::Hops::Max))
				return;

			cmd->buttons &= ~IN_JUMP;
			bLastJumped = false;
		}
	}
	else
	{
		bActualHop = 0;
		bLastJumped = false;
		bShouldFake = false;
	}
}

void BHop::CreateMoveCircle(CUserCmd* cmd)
{
	if (!Settings::CircleStrafe::enabled)
		return;

	if (!pInputSystem->IsButtonDown(Settings::CircleStrafe::key))
		return;

	pEngine->GetViewAngles(AutoStrafeView);

	doCircularStrafe(cmd, AutoStrafeView);
}

void BHop::RotateMovement(CUserCmd* pCmd, float rotation)
{
	rotation = DEG2RAD(rotation);

	float cos_rot = cos(rotation);
	float sin_rot = sin(rotation);

	float new_forwardmove = (cos_rot * pCmd->forwardmove) - (sin_rot * pCmd->sidemove);
	float new_sidemove = (sin_rot * pCmd->forwardmove) + (cos_rot * pCmd->sidemove);

	pCmd->forwardmove = new_forwardmove;
	pCmd->sidemove = -new_sidemove;

}
int CircleFactor = 0;
bool BHop::doCircularStrafe(CUserCmd* pCmd, Vector& OriginalView) {

	C_BasePlayer* localplayer = (C_BasePlayer*)pEntityList->GetClientEntity(pEngine->GetLocalPlayer());

	if (!localplayer)
		return false;

	if (!localplayer->GetAlive())
		return false;

	CircleFactor++;
	if (CircleFactor > 361)
		CircleFactor = 0;

	int GetItDoubled = 3.0 * CircleFactor - pGlobalVars->interval_per_tick;//Interfaces::Globals->interval_per_tick;

	Vector StoredViewAngles = pCmd->viewangles;

	if ((pCmd->buttons & IN_JUMP) || !(localplayer->GetFlags() & FL_ONGROUND))
	{
		pCmd->viewangles = OriginalView;
		pCmd->forwardmove = 450.f;
		RotateMovement(pCmd, GetItDoubled);
	}

	return true;
}