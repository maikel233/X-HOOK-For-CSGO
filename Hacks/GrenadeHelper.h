#pragma once

GrenadeType getGrenadeType(C_BaseCombatWeapon* wpn);

namespace GrenadeHelper
{
//	void DrawGrenadeInfo(Vector pos, GrenadeType gType, std::string name);
//	/void DrawAimHelp(QAngle angle, Vector pos);
//	void AimAssist(CUserCmd* cmd);
//	ImColor getColor(GrenadeType type);
//	void CheckForUpdate();

	void CreateMove(CUserCmd* cmd);//Activly Help to throw grenade
	void Paint();//Draw the Grenade Helper Esp
}