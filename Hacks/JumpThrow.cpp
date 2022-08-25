#include "../Features.h"

bool inAttackState = false;
bool Settings::JumpThrow::enabled = false;
ButtonCode_t Settings::JumpThrow::key = ButtonCode_t::KEY_T;

void JumpThrow::CreateMove(CUserCmd* cmd)
{
	if (!Settings::JumpThrow::enabled)
		return;

	C_BasePlayer* localplayer = (C_BasePlayer*)pEntityList->GetClientEntity(pEngine->GetLocalPlayer());
	if (!localplayer)
		return;

	C_BaseCombatWeapon* activeWeapon = (C_BaseCombatWeapon*)pEntityList->GetClientEntityFromHandle(localplayer->GetActiveWeapon());
	if (!activeWeapon)
		return;

	if (!activeWeapon->IsGrenade())
		return;

	if (localplayer->GetMoveType() == MOVETYPE_LADDER || localplayer->GetMoveType() == MOVETYPE_NOCLIP)
		return;

	if (!pInputSystem->IsButtonDown(Settings::JumpThrow::key) && !inAttackState)
		return;

	if (!pInputSystem->IsButtonDown(Settings::JumpThrow::key) && inAttackState)
	{
		cmd->buttons |= IN_JUMP;
		inAttackState = false;
		return;
	}

	cmd->buttons |= IN_ATTACK;
	inAttackState = true;
}