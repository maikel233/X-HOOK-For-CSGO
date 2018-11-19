#include "Features.h"

bool Settings::Aimbot::NoSpread = false;

void AimbotNoSpread::NoSpread(Vector& angles, CUserCmd* cmd)
{

	if ((*pGameRules) && (*pGameRules)->IsValveDS())
		return;

	if (!Settings::Aimbot::NoSpread)
		return;

	C_BasePlayer* localplayer = (C_BasePlayer*)pEntityList->GetClientEntity(pEngine->GetLocalPlayer());
	if (!localplayer)
		return;

	C_BaseCombatWeapon* activeWeapon = (C_BaseCombatWeapon*)pEntityList->GetClientEntityFromHandle(localplayer->GetActiveWeapon());
	if (!activeWeapon)
		return;

	
	if (activeWeapon)
	{
		if (cmd->random_seed > 0)
			
		//No pasta for you (:
	}
}