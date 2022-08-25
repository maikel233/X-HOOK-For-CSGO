#include "../Features.h"
#include "autowall.h"

bool Settings::Triggerbot::enabled = false;
bool Settings::Triggerbot::Filters::enemies = true;
bool Settings::Triggerbot::Filters::allies = false;
bool Settings::Triggerbot::Filters::walls = false;
bool Settings::Triggerbot::Filters::smokeCheck = false;
bool Settings::Triggerbot::Filters::flashCheck = false;
bool Settings::Triggerbot::Filters::head = true;
bool Settings::Triggerbot::Filters::chest = true;
bool Settings::Triggerbot::Filters::stomach = true;
bool Settings::Triggerbot::Filters::arms = true;
bool Settings::Triggerbot::Filters::legs = true;
bool Settings::Triggerbot::RandomDelay::enabled = true;
int Settings::Triggerbot::RandomDelay::lowBound = 20;
int Settings::Triggerbot::RandomDelay::highBound = 35;
int Settings::Triggerbot::RandomDelay::lastRoll = 0;
ButtonCode_t Settings::Triggerbot::key = ButtonCode_t::KEY_LALT;

void Triggerbot::CreateMove(CUserCmd *cmd)
{
	if (!Settings::Triggerbot::enabled)
		return;

	if (!pInputSystem->IsButtonDown(Settings::Triggerbot::key))
		return;

	C_BasePlayer* localplayer = (C_BasePlayer*)pEntityList->GetClientEntity(pEngine->GetLocalPlayer());
	if (!localplayer || !localplayer->GetAlive())
		return;

	if (Settings::Triggerbot::Filters::flashCheck && localplayer->GetFlashBangTime() - pGlobalVars->curtime > 2.0f)
		return;

	long currentTime_ms = Util::GetEpochTime();
	static long timeStamp = currentTime_ms;
	long oldTimeStamp;

	static int localMin = Settings::Triggerbot::RandomDelay::lowBound;
	static int localMax = Settings::Triggerbot::RandomDelay::highBound;
	static int randomDelay = localMin + rand() % (localMax - localMin);

	if (localMin != Settings::Triggerbot::RandomDelay::lowBound || localMax != Settings::Triggerbot::RandomDelay::highBound) // Done in case Low/high bounds change before the next triggerbot shot.
	{
		localMin = Settings::Triggerbot::RandomDelay::lowBound;
		localMax = Settings::Triggerbot::RandomDelay::highBound;
		randomDelay = localMin + rand() % (localMax - localMin);
	}

	Vector traceStart, traceEnd;
	trace_t tr;

	Vector viewAngles;
	pEngine->GetViewAngles(viewAngles);
	Vector viewAngles_rcs = viewAngles + *localplayer->GetAimPunchAngle() * 2.0f;

	Math::AngleVectors(viewAngles_rcs, traceEnd);

	traceStart = localplayer->GetEyePosition();
	traceEnd = traceStart + (traceEnd * 8192.0f);

	if (Settings::Triggerbot::Filters::walls)
	{
		Autowall::FireBulletData data;
		if (Autowall::GetDamage(traceEnd, !Settings::Triggerbot::Filters::allies, data) == 0.0f)
			return;

		tr = data.enter_trace;
	}
	else
	{
		Ray_t ray;
		ray.Init(traceStart, traceEnd);
		CTraceFilter traceFilter;
		traceFilter.pSkip = localplayer;
		pTrace->TraceRay(ray, 0x46004003, &traceFilter, &tr);
	}

	oldTimeStamp = timeStamp;
	timeStamp = currentTime_ms;

	C_BasePlayer* player = (C_BasePlayer*)tr.m_pEnt;
	if (!player)
		return;

	if (player->GetClientClass()->m_ClassID != EClassIds::ccsplayer)
		return;

	if (player == localplayer
		|| player->GetDormant()
		|| !player->GetAlive()
		|| player->GetImmune())
		return;

	if (player->GetTeam() != localplayer->GetTeam() && !Settings::Triggerbot::Filters::enemies)
		return;

	if (player->GetTeam() == localplayer->GetTeam() && !Settings::Triggerbot::Filters::allies)
		return;

	bool filter;

	switch (tr.hitgroup)
	{
	case HitGroups::HITGROUP_HEAD:
		filter = Settings::Triggerbot::Filters::head;
		break;
	case HitGroups::HITGROUP_CHEST:
		filter = Settings::Triggerbot::Filters::chest;
		break;
	case HitGroups::HITGROUP_STOMACH:
		filter = Settings::Triggerbot::Filters::stomach;
		break;
	case HitGroups::HITGROUP_LEFTARM:
	case HitGroups::HITGROUP_RIGHTARM:
		filter = Settings::Triggerbot::Filters::arms;
		break;
	case HitGroups::HITGROUP_LEFTLEG:
	case HitGroups::HITGROUP_RIGHTLEG:
		filter = Settings::Triggerbot::Filters::legs;
		break;
	default:
		filter = false;
	}

	if (!filter)
		return;

	if (Settings::Triggerbot::Filters::smokeCheck && Entity::LineGoesThroughSmoke(tr.startpos, tr.endpos, true))
		return;

	C_BaseCombatWeapon* activeWeapon = (C_BaseCombatWeapon*)pEntityList->GetClientEntityFromHandle(localplayer->GetActiveWeapon());
	if (!activeWeapon || activeWeapon->GetAmmo() == 0)
		return;

	ItemDefinitionIndex itemDefinitionIndex = *activeWeapon->GetItemDefinitionIndex();
	if (itemDefinitionIndex == ItemDefinitionIndex::WEAPON_KNIFE || itemDefinitionIndex >= ItemDefinitionIndex::WEAPON_BAYONET)
		return;

	//CSWeaponType weaponType = activeWeapon->GetCSWpnData()->GetWeaponType();
	if (!activeWeapon->IsGun())
		return;

	bool can_shoot = true;
	float server_time = localplayer->GetTickBase() * pGlobalVars->interval_per_tick;
	float next_shot = activeWeapon->GetNextPrimaryAttack() - server_time;
	if (next_shot > 0)
		can_shoot = false;
	if (!can_shoot)
	{
		if (*activeWeapon->GetItemDefinitionIndex() == ItemDefinitionIndex::WEAPON_REVOLVER)
			cmd->buttons &= ~IN_ATTACK2;
		else
			cmd->buttons &= ~IN_ATTACK;
	}
	else
	{
		if (Settings::Triggerbot::RandomDelay::enabled && currentTime_ms - oldTimeStamp < randomDelay)
		{
			timeStamp = oldTimeStamp;
			return;
		}

		if (*activeWeapon->GetItemDefinitionIndex() == ItemDefinitionIndex::WEAPON_REVOLVER)
			cmd->buttons |= IN_ATTACK2;
		else
			cmd->buttons |= IN_ATTACK;
	}

	timeStamp = currentTime_ms;
}