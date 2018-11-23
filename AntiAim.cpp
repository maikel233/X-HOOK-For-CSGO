#include "Features.h"
//#include "lua.hpp"
#include "antiaim.h"

bool Settings::AntiAim::Misc::LegitAA = false;
#ifndef REGION_ANTIAIM_STANDING_SETTINGS
bool Settings::AntiAim::Standing::Yaw::enabled = false;
AntiAimType_Y Settings::AntiAim::Standing::Yaw::type = AntiAimType_Y::NOAA;
AntiAimType_Y Settings::AntiAim::Standing::Yaw::typeFake = AntiAimType_Y::NOAA;
float Settings::AntiAim::Standing::Yaw::typeAdd = 0.0f;
float Settings::AntiAim::Standing::Yaw::typeFakeAdd = 0.0f;

bool Settings::AntiAim::Standing::Pitch::enabled = false;
AntiAimType_X Settings::AntiAim::Standing::Pitch::type = AntiAimType_X::STATIC_DOWN;
float Settings::AntiAim::Standing::Pitch::custom = 0.0f;

bool Settings::AntiAim::Standing::Roll::enabled = false;
AntiAimType_Z Settings::AntiAim::Standing::Roll::type = AntiAimType_Z::TEST;

bool Settings::AntiAim::Standing::SwitchAA::enabled = false;
ButtonCode_t Settings::AntiAim::Standing::SwitchAA::key = ButtonCode_t::KEY_COMMA;

bool Settings::AntiAim::Standing::LBY::enabled = false;
AntiAimType_LBY Settings::AntiAim::Standing::LBY::type = AntiAimType_LBY::ONE;

bool Settings::AntiAim::Standing::HeadEdge::enabled = false;
float Settings::AntiAim::Standing::HeadEdge::distance = 25.0f;
float Settings::AntiAim::Standing::HeadEdge::fakeAdd = 0.f;
float Settings::AntiAim::Standing::HeadEdge::realAdd = 0.f;

bool Settings::AntiAim::Standing::antiResolver = false;
bool Settings::AntiAim::Standing::dynamicAA = false;
bool Settings::AntiAim::Standing::untrustedAngles = false;
#endif
#ifndef REGION_ANTIAIM_MOVING_SETTINGS
bool Settings::AntiAim::Moving::Yaw::enabled = false;
AntiAimType_Y Settings::AntiAim::Moving::Yaw::type = AntiAimType_Y::NOAA;
AntiAimType_Y Settings::AntiAim::Moving::Yaw::typeFake = AntiAimType_Y::NOAA;
float Settings::AntiAim::Moving::Yaw::typeAdd = 0.0f;
float Settings::AntiAim::Moving::Yaw::typeFakeAdd = 0.0f;

bool Settings::AntiAim::Moving::Pitch::enabled = false;
AntiAimType_X Settings::AntiAim::Moving::Pitch::type = AntiAimType_X::STATIC_DOWN;
float Settings::AntiAim::Moving::Pitch::custom = 0.0f;

bool Settings::AntiAim::Moving::Roll::enabled = false;
AntiAimType_Z Settings::AntiAim::Moving::Roll::type = AntiAimType_Z::TEST;

bool Settings::AntiAim::Moving::SwitchAA::enabled = false;
ButtonCode_t Settings::AntiAim::Moving::SwitchAA::key = ButtonCode_t::KEY_COMMA;

bool Settings::AntiAim::Moving::LBY::enabled = false;
AntiAimType_LBY Settings::AntiAim::Moving::LBY::type = AntiAimType_LBY::ONE;

bool Settings::AntiAim::Moving::HeadEdge::enabled = false;
float Settings::AntiAim::Moving::HeadEdge::distance = 25.0f;
float Settings::AntiAim::Moving::HeadEdge::fakeAdd = 0.f;
float Settings::AntiAim::Moving::HeadEdge::realAdd = 0.f;

bool Settings::AntiAim::Moving::antiResolver = false;
bool Settings::AntiAim::Moving::dynamicAA = false;
bool Settings::AntiAim::Moving::untrustedAngles = false;
#endif
#ifndef REGION_ANTIAIM_AIRBORNE_SETTINGS
bool Settings::AntiAim::Airborne::Yaw::enabled = false;
AntiAimType_Y Settings::AntiAim::Airborne::Yaw::type = AntiAimType_Y::NOAA;
AntiAimType_Y Settings::AntiAim::Airborne::Yaw::typeFake = AntiAimType_Y::NOAA;
float Settings::AntiAim::Airborne::Yaw::typeAdd = 0.0f;
float Settings::AntiAim::Airborne::Yaw::typeFakeAdd = 0.0f;

bool Settings::AntiAim::Airborne::Pitch::enabled = false;
AntiAimType_X Settings::AntiAim::Airborne::Pitch::type = AntiAimType_X::STATIC_DOWN;
float Settings::AntiAim::Airborne::Pitch::custom = 0.0f;

bool Settings::AntiAim::Airborne::Roll::enabled = false;
AntiAimType_Z Settings::AntiAim::Airborne::Roll::type = AntiAimType_Z::TEST;

bool Settings::AntiAim::Airborne::SwitchAA::enabled = false;
ButtonCode_t Settings::AntiAim::Airborne::SwitchAA::key = ButtonCode_t::KEY_COMMA;

bool Settings::AntiAim::Airborne::LBY::enabled = false;
AntiAimType_LBY Settings::AntiAim::Airborne::LBY::type = AntiAimType_LBY::ONE;

bool Settings::AntiAim::Airborne::HeadEdge::enabled = false;
float Settings::AntiAim::Airborne::HeadEdge::distance = 25.0f;
float Settings::AntiAim::Airborne::HeadEdge::fakeAdd = 0.f;
float Settings::AntiAim::Airborne::HeadEdge::realAdd = 0.f;

bool Settings::AntiAim::Airborne::antiResolver = false;
bool Settings::AntiAim::Airborne::dynamicAA = false;
bool Settings::AntiAim::Airborne::untrustedAngles = false;
#endif
#ifndef REGION_ANTIAIM_MISC_SETTINGS
bool Settings::AntiAim::Misc::AutoDisable::noEnemy = false;
bool Settings::AntiAim::Misc::AutoDisable::knifeHeld = true;
bool Settings::AntiAim::Misc::AutoDisable::bombHeld = false;
bool Settings::AntiAim::Misc::AutoDisable::freezeTime = true;
#endif
float AntiAim::lastRealYaw = 0.0f;
float AntiAim::lastFakeYaw = 0.0f;
bool AntiAim::isAntiAiming = false;
long lastPress = 0;

bool AntiAim::canEdge = false;
bool AntiAim::fakeTp = false;

static float Distance(Vector a, Vector b) {
	return (sqrt(pow(a.x - b.x, 2) + pow(a.y - b.y, 2) + pow(a.z - b.z, 2)));
}

// static bool uff = false; // NICE VARIABLE NAMING, also Unused
static bool uff2 = false;
static bool uff3 = false;
static int uff4 = 0;
static bool besteap = false;
static bool firstBreak;
static float oldCurtime;
static float offset = 160;



bool AntiAim::CanEdge() {
	C_BasePlayer* localplayer = (C_BasePlayer*)pEntityList->GetClientEntity(pEngine->GetLocalPlayer());

	Vector position = localplayer->GetVecOrigin() + localplayer->GetVecViewOffset();

	float closest_distance = 50.0f;

	float radius = 0.0f;

	if (AntiAim::IsStanding())
		radius = Settings::AntiAim::Standing::HeadEdge::distance + 0.1f;
	else if (AntiAim::IsMoving())
		radius = Settings::AntiAim::Moving::HeadEdge::distance + 0.1f;
	else if (AntiAim::IsAirborne())
		radius = Settings::AntiAim::Airborne::HeadEdge::distance + 0.1;

	float step = M_PI * 2.0 / 8;

	for (float a = 0; a < (M_PI * 2.0); a += step) {
		Vector location(radius * cos(a) + position.x, radius * sin(a) + position.y, position.z);

		Ray_t ray;
		trace_t tr;
		ray.Init(position, location);
		CTraceFilter pTraceFilter;
		pTraceFilter.pSkip = localplayer;
		pTrace->TraceRay(ray, 0x4600400B, &pTraceFilter, &tr);

		float distance = Distance(position, tr.endpos);

		if (distance < closest_distance) {
			closest_distance = distance;
		}
	}

	return closest_distance < (radius - 0.1f);
}

float GetBestHeadEdgeAngle() {
	C_BasePlayer* localplayer = (C_BasePlayer*)pEntityList->GetClientEntity(pEngine->GetLocalPlayer());

	Vector position = localplayer->GetVecOrigin() + localplayer->GetVecViewOffset();

	float closest_distance = 100.0f;

	float radius = 0.0f;

	if (AntiAim::IsStanding())
		radius = Settings::AntiAim::Standing::HeadEdge::distance + 0.1f;
	else if (AntiAim::IsMoving())
		radius = Settings::AntiAim::Moving::HeadEdge::distance + 0.1f;
	else if (AntiAim::IsAirborne())
		radius = Settings::AntiAim::Airborne::HeadEdge::distance + 0.1;

	float step = M_PI * 2.0 / 8;

	static float returnAngle = 0.f;

	for (float a = 0; a < (M_PI * 2.0); a += step) {
		Vector location(radius * cos(a) + position.x, radius * sin(a) + position.y, position.z);

		// static bool bFlip = false; Unused

		// static float fakeAdd = 0.f; Unused
		// static float realAdd = 0.f; Unused

		Ray_t ray;
		trace_t tr;
		ray.Init(position, location);
		CTraceFilter pTraceFilter;
		pTraceFilter.pSkip = localplayer;
		pTrace->TraceRay(ray, 0x4600400B, &pTraceFilter, &tr);

		float distance = Distance(position, tr.endpos);

		if (distance < closest_distance) {
			closest_distance = distance;

			returnAngle = Math::ClampYaw(RAD2DEG(a));
		}
	}

	return returnAngle;
}

static float HeadEdgeAng() {
	C_BasePlayer* localplayer = (C_BasePlayer*)pEntityList->GetClientEntity(pEngine->GetLocalPlayer());

	Vector position = localplayer->GetVecOrigin() + localplayer->GetVecViewOffset();

	float closest_distance = 100.0f;

	float radius = 0.0f;

	if (AntiAim::IsStanding())
		radius = Settings::AntiAim::Standing::HeadEdge::distance + 0.1f;
	else if (AntiAim::IsMoving())
		radius = Settings::AntiAim::Moving::HeadEdge::distance + 0.1f;
	else if (AntiAim::IsAirborne())
		radius = Settings::AntiAim::Airborne::HeadEdge::distance + 0.1;

	float step = M_PI * 2.0 / 8;

	float finalAngleY = 0.f;

	for (float a = 0; a < (M_PI * 2.0); a += step) {
		Vector location(radius * cos(a) + position.x, radius * sin(a) + position.y, position.z);

		Ray_t ray;
		trace_t tr;
		ray.Init(position, location);
		CTraceFilter pTraceFilter;
		pTraceFilter.pSkip = localplayer;
		pTrace->TraceRay(ray, 0x4600400B, &pTraceFilter, &tr);

		float distance = Distance(position, tr.endpos);

		if (distance < closest_distance) {
			closest_distance = distance;
			AntiAim::canEdge = true;
			//angle.y = RAD2DEG( a );
			finalAngleY = RAD2DEG(a);
		}
		else
			AntiAim::canEdge = false;
	}

	//return closest_distance < ( radius - 0.1f );
	return finalAngleY;
}

static bool HasViableEnemy() {
	C_BasePlayer* localplayer = (C_BasePlayer*)pEntityList->GetClientEntity(pEngine->GetLocalPlayer());

	for (int i = 1; i < pEngine->GetMaxClients(); ++i) {
		C_BasePlayer* entity = (C_BasePlayer*)pEntityList->GetClientEntity(i);

		if (!entity
			|| entity == localplayer
			|| entity->GetDormant()
			|| !entity->GetAlive()
			|| entity->GetImmune())
			continue;

		if (!Aimbot::friends.empty()) // check for friends, if any
		{
			IEngineClient::player_info_t entityInformation;
			pEngine->GetPlayerInfo(i, &entityInformation);

			if (std::find(Aimbot::friends.begin(), Aimbot::friends.end(), entityInformation.xuid) !=
				Aimbot::friends.end())
				continue;
		}

		if (Settings::Aimbot::friendly || entity->GetTeam() != localplayer->GetTeam())
			return true;
	}

	return false;
}
/* Unused Function
static float DoAAatTarget() {
static C_BasePlayer* pLocal = ( C_BasePlayer* ) pEntityList->GetClientEntity( pEngine->GetLocalPlayer() );
static float Angle = 0.0f;

if ( ( AntiAim::IsStanding() && Settings::AntiAim::Standing::Yaw::enabled ) ||
( AntiAim::IsMoving() && Settings::AntiAim::Moving::Yaw::enabled ) ||
( AntiAim::IsAirborne() && Settings::AntiAim::Airborne::dynamicAA ) ) {
if ( ( AntiAim::IsStanding() && Settings::AntiAim::Standing::dynamicAA ) ||
( AntiAim::IsMoving() && Settings::AntiAim::Moving::dynamicAA ) ||
( AntiAim::IsAirborne() && Settings::AntiAim::Airborne::dynamicAA ) ) {
float bestDist = 999999999.f; // easy cuz im retarded
for ( int i = 1; i < pEngine->GetMaxClients(); ++i ) {
C_BasePlayer* target = ( C_BasePlayer* ) pEntityList->GetClientEntity( i );

if ( !target
|| target == pLocal
|| target->GetDormant()
|| !target->GetAlive()
|| target->GetImmune()
|| target->GetTeam() == pLocal->GetTeam() )
continue;

Vector eye_pos = pLocal->GetEyePosition();
Vector target_pos = target->GetEyePosition();

float tempDist = eye_pos.DistTo( target_pos );

if ( bestDist > tempDist ) {
bestDist = tempDist;
Angle = Math::CalcAngle( eye_pos, target_pos ).y;
return Angle;
}
}
}

}
}
*/

void AntiAim::SwitchAA(int state) {
	switch (state) {
	case ANTIAIM_AIRBORNE: {
		// Airborne
		AntiAimType_Y fakeYaw = Settings::AntiAim::Airborne::Yaw::typeFake;
		AntiAimType_Y realYaw = Settings::AntiAim::Airborne::Yaw::type;
		Settings::AntiAim::Airborne::Yaw::typeFake = realYaw;
		Settings::AntiAim::Airborne::Yaw::type = fakeYaw;

		float fakeYawAdd = Settings::AntiAim::Airborne::Yaw::typeFakeAdd;
		float realYawAdd = Settings::AntiAim::Airborne::Yaw::typeAdd;
		Settings::AntiAim::Airborne::Yaw::typeFakeAdd = realYawAdd;
		Settings::AntiAim::Airborne::Yaw::typeAdd = fakeYawAdd;

		pCvar->ConsoleColorPrintf(ColorRGBA(255, 255, 255), XorStr("Switched AntiAims for Airborne!"));
		pCvar->ConsoleColorPrintf(ColorRGBA(255, 255, 255),
		XorStr("New Fake: %s - New Real: %s - New Fake Add: %f - New Real Add: %f"),
		realYaw, fakeYaw, realYawAdd, fakeYawAdd);
		break;
	}
	case ANTIAIM_MOVING: {
		// Moving
		AntiAimType_Y fakeYaw = Settings::AntiAim::Moving::Yaw::typeFake;
		AntiAimType_Y realYaw = Settings::AntiAim::Moving::Yaw::type;
		Settings::AntiAim::Moving::Yaw::typeFake = realYaw;
		Settings::AntiAim::Moving::Yaw::type = fakeYaw;

		float fakeYawAdd = Settings::AntiAim::Moving::Yaw::typeFakeAdd;
		float realYawAdd = Settings::AntiAim::Moving::Yaw::typeAdd;
		Settings::AntiAim::Moving::Yaw::typeFakeAdd = realYawAdd;
		Settings::AntiAim::Moving::Yaw::typeAdd = fakeYawAdd;

		pCvar->ConsoleColorPrintf(ColorRGBA(255, 255, 255), XorStr("Switched AntiAims for Moving!"));
		pCvar->ConsoleColorPrintf(ColorRGBA(255, 255, 255),
		XorStr("New Fake: %s - New Real: %s - New Fake Add: %f - New Real Add: %f"),
		realYaw, fakeYaw, realYawAdd, fakeYawAdd);
		break;
	}
	case ANTIAIM_STANDING: {
		// Standing
		AntiAimType_Y fakeYaw = Settings::AntiAim::Standing::Yaw::typeFake;
		AntiAimType_Y realYaw = Settings::AntiAim::Standing::Yaw::type;
		Settings::AntiAim::Standing::Yaw::typeFake = realYaw;
		Settings::AntiAim::Standing::Yaw::type = fakeYaw;

		float fakeYawAdd = Settings::AntiAim::Standing::Yaw::typeFakeAdd;
		float realYawAdd = Settings::AntiAim::Standing::Yaw::typeAdd;
		Settings::AntiAim::Standing::Yaw::typeFakeAdd = realYawAdd;
		Settings::AntiAim::Standing::Yaw::typeAdd = fakeYawAdd;

		pCvar->ConsoleColorPrintf(ColorRGBA(255, 255, 255), XorStr("Switched AntiAims for Standing!"));
		pCvar->ConsoleColorPrintf(ColorRGBA(255, 255, 255),
		XorStr("New Fake: %s - New Real: %s - New Fake Add: %f - New Real Add: %f"),
			realYaw, fakeYaw, realYawAdd, fakeYawAdd);
		break;
	}
	}
}

float GetLatency() {
	INetChannelInfo* nci = pEngine->GetNetChannelInfo();
	if (nci) {
		float Latency = nci->GetAvgLatency(FLOW_OUTGOING) + nci->GetAvgLatency(FLOW_INCOMING);
		return Latency;
	}
	else {
		return 0.0f;
	}
}

float GetOutgoingLatency() {
	INetChannelInfo* nci = pEngine->GetNetChannelInfo();
	if (nci) {
		float OutgoingLatency = nci->GetAvgLatency(FLOW_OUTGOING);
		return OutgoingLatency;
	}
	else {
		return 0.0f;
	}
}

float GetIncomingLatency() {
	INetChannelInfo* nci = pEngine->GetNetChannelInfo();
	if (nci) {
		float IncomingLatency = nci->GetAvgLatency(FLOW_INCOMING);
		return IncomingLatency;
	}
	else {
		return 0.0f;
	}
}

float OldLBY;
float LBYBreakerTimer;
float LastLBYUpdateTime;
bool bSwitch;

float CurrentVelocity(C_BasePlayer* LocalPlayer) {
	float vel = LocalPlayer->GetVelocity().Length2D();
	return vel;
}

bool NextLBYUpdate()
{
	C_BasePlayer* LocalPlayer = (C_BasePlayer*)pEntityList->GetClientEntity(pEngine->GetLocalPlayer());
	//bool bSwitch = false;
	float flServerTime = (float)(LocalPlayer->GetTickBase() * pGlobalVars->interval_per_tick);
	//float flServerTime = (tick * Interfaces.Globals->interval_per_tick);
	if (((LastLBYUpdateTime + 1.1 - GetOutgoingLatency()) <= flServerTime) && (LocalPlayer->GetFlags() & FL_ONGROUND))
	{
		LastLBYUpdateTime += 1.1;
		bSwitch = !bSwitch;
		return true;
	}
	return false;
}

static void DoAntiAimY(Vector& angle, int command_number, bool bFlip, bool& clamp) {
	AntiAimType_Y antiAim = bFlip ? Settings::AntiAim::Standing::Yaw::typeFake : Settings::AntiAim::Standing::Yaw::type;
	if (AntiAim::IsMoving()) {
		antiAim = bFlip ? Settings::AntiAim::Moving::Yaw::typeFake : Settings::AntiAim::Moving::Yaw::type;
	}
	if (AntiAim::IsAirborne()) {
		antiAim = bFlip ? Settings::AntiAim::Airborne::Yaw::typeFake : Settings::AntiAim::Airborne::Yaw::type;
	}

	static bool yFlip;
	double factor;
	static bool flip1 = false;
	// static bool back = false; Unused
	static float prevLBY1 = *((C_BasePlayer*)pEntityList->GetClientEntity(
		pEngine->GetLocalPlayer()))->GetLowerBodyYawTarget();
	static bool lolgay;
	int random;
	int ticks = 0;
	int jitterticks = 0;
	static C_BasePlayer* pLocal = (C_BasePlayer*)pEntityList->GetClientEntity(pEngine->GetLocalPlayer());


	yFlip = bFlip != yFlip;

	switch (antiAim) {
	case AntiAimType_Y::LEGITTROLLING:
		if (AntiAim::IsStanding()) {
			Settings::AntiAim::Standing::Yaw::typeFake = AntiAimType_Y::NOAA;
			Settings::AntiAim::Standing::Yaw::typeFakeAdd = 0.0f;
			Settings::AntiAim::Standing::Yaw::typeAdd = 0.0f;
		}
		if (AntiAim::IsMoving()) {
			Settings::AntiAim::Moving::Yaw::typeFake = AntiAimType_Y::NOAA;
			Settings::AntiAim::Moving::Yaw::typeFakeAdd = 0.0f;
			Settings::AntiAim::Moving::Yaw::typeAdd = 0.0f;
		}
		if (AntiAim::IsAirborne()) {
			Settings::AntiAim::Airborne::Yaw::typeFake = AntiAimType_Y::NOAA;
			Settings::AntiAim::Airborne::Yaw::typeFakeAdd = 0.0f;
			Settings::AntiAim::Airborne::Yaw::typeAdd = 0.0f;
		}

		angle.y -= 180.f;
		break;
	case AntiAimType_Y::LEGITTROLLING2:
		if (AntiAim::IsStanding()) {
			Settings::AntiAim::Standing::Yaw::typeFake = AntiAimType_Y::NOAA;
			Settings::AntiAim::Standing::Yaw::typeFakeAdd = 0.0f;
			Settings::AntiAim::Standing::Yaw::typeAdd = 0.0f;
		}
		if (AntiAim::IsMoving()) {
			Settings::AntiAim::Moving::Yaw::typeFake = AntiAimType_Y::NOAA;
			Settings::AntiAim::Moving::Yaw::typeFakeAdd = 0.0f;
			Settings::AntiAim::Moving::Yaw::typeAdd = 0.0f;
		}
		if (AntiAim::IsAirborne()) {
			Settings::AntiAim::Airborne::Yaw::typeFake = AntiAimType_Y::NOAA;
			Settings::AntiAim::Airborne::Yaw::typeFakeAdd = 0.0f;
			Settings::AntiAim::Airborne::Yaw::typeAdd = 0.0f;
		}

		angle.y -= 90.0f;
		break;
	case AntiAimType_Y::SPIN:
		factor = 360.0 / M_PHI;
		factor *= Settings::spinFactor::value;
		angle.y = fmodf(pGlobalVars->curtime * factor, 360.0);
		break;
	case AntiAimType_Y::CUSTOM:
		Math::ClampY(Settings::customYaw::value);
		Math::ClampY(Settings::customYaw::value);

		if (Settings::customYaw::lby) {
			if (Settings::customYaw::value > 0)
				angle.y = *((C_BasePlayer*)pEntityList->GetClientEntity(
					pEngine->GetLocalPlayer()))->GetLowerBodyYawTarget() + (Settings::customYaw::value);
			else
				angle.y = *((C_BasePlayer*)pEntityList->GetClientEntity(
					pEngine->GetLocalPlayer()))->GetLowerBodyYawTarget() - (Settings::customYaw::value);

		}
		else {
			angle.y += Settings::customYaw::value;
		}
		break;
	case AntiAimType_Y::CUSTOM2:
		Math::ClampY(Settings::customYaw2::value);
		Math::ClampY(Settings::customYaw2::value);

		if (Settings::customYaw2::lby) {
			if (Settings::customYaw2::value > 0)
				angle.y += *((C_BasePlayer*)pEntityList->GetClientEntity(
					pEngine->GetLocalPlayer()))->GetLowerBodyYawTarget() + (Settings::customYaw2::value);
			else
				angle.y -= *((C_BasePlayer*)pEntityList->GetClientEntity(
					pEngine->GetLocalPlayer()))->GetLowerBodyYawTarget() - (Settings::customYaw2::value);
		}
		else {
			angle.y += Settings::customYaw2::value;
		}
		break;

	case AntiAimType_Y::TANK1:
		if (SendPacket) {
			random = rand() % 4;
			switch (random) {
			case 1: angle.y = *((C_BasePlayer*)pEntityList->GetClientEntity(pEngine->GetLocalPlayer()))->GetLowerBodyYawTarget() + rand() % 35 + 165;
				break;
			case 2: yFlip ? angle.y -= 160 : angle.y += 160;
				break;
			case 3: factor = 360.0 / M_PHI; factor *= rand() % 25; float x = fmodf(pGlobalVars->curtime * factor, 360.0); factor *= 5; float y = fmodf(pGlobalVars->curtime * factor, 360.0f);
				if (y >= 100.0f)
				{
					y -= 170.0f;
				} if (y <= 200.0f)
				{
					y += 305.00f;
				}
				angle.y = ((x / y) + 60.2f)*M_PI;
			}
		}
		else
		{
			random = rand() % 4;
			switch (random) {
			case 1: angle.y -= 180.0f;
				break;
			case 2: yFlip ? angle.y += 90.f : angle.y -= 90.0f;
				break;
			case 3: factor = 360.0 / M_PHI; angle.y = fmodf(pGlobalVars->curtime * factor, 360.0);

			}
		}
		if (angle.y == *((C_BasePlayer*)pEntityList->GetClientEntity(pEngine->GetLocalPlayer()))->GetLowerBodyYawTarget()) {
			angle.y = *((C_BasePlayer*)pEntityList->GetClientEntity(pEngine->GetLocalPlayer()))->GetLowerBodyYawTarget() + 90;

		}

		break;
	case AntiAimType_Y::TANK2:
		if (SendPacket) {
			random = rand() % 4;
			switch (random) {
			case 1:
				angle.y = *((C_BasePlayer*)pEntityList->GetClientEntity(
					pEngine->GetLocalPlayer()))->GetLowerBodyYawTarget() + rand() % 35 + 165;
				break;
			case 2:
				yFlip ? angle.y -= 160 : angle.y += 160;
				break;
			case 3:
				factor = 360.0 / M_PHI;
				factor *= rand() % 25;
				float x = fmodf(pGlobalVars->curtime * factor, 360.0);
				factor *= 5;
				float y = fmodf(pGlobalVars->curtime * factor, 360.0f);
				if (y >= 100.0f) {
					y -= 170.0f;
				}
				if (y <= 200.0f) {
					y += 305.00f;
				}
				angle.y = ((x / y) + 60.2f) * M_PI;
			}
		}
		else {
			random = rand() % 4;
			switch (random) {
			case 1:
				angle.y -= 180.0f;
				break;
			case 2:
				yFlip ? angle.y += 90.f : angle.y -= 90.0f;
				break;
			case 3:
				factor = 360.0 / M_PHI;
				angle.y = fmodf(pGlobalVars->curtime * factor, 360.0);

			}
		}
		if (angle.y ==
			*((C_BasePlayer*)pEntityList->GetClientEntity(
				pEngine->GetLocalPlayer()))->GetLowerBodyYawTarget()) {
			angle.y = *((C_BasePlayer*)pEntityList->GetClientEntity(
				pEngine->GetLocalPlayer()))->GetLowerBodyYawTarget() + 90;

		}

		break;
	case AntiAimType_Y::TANK3:
		if (pLocal->GetVelocity().x < 0.1f && pLocal->GetVelocity().x > -0.1f) {
			if (SendPacket) {
				if (pLocal->GetFlags() & FL_ONGROUND)
					angle.y = *((C_BasePlayer*)pEntityList->GetClientEntity(
						pEngine->GetLocalPlayer()))->GetLowerBodyYawTarget() + rand() % 90 + 99;
				else {
					random = rand() % 4;
					switch (random) {
					case 1:
						yFlip ? angle.y += 90.f : angle.y -= 90.0f;
						break;
					case 2:
						angle.y -= 120.0f;
						break;
					case 3:
						factor = 360.0 / M_PHI;
						factor *= 25;
						angle.y = fmodf(pGlobalVars->curtime * factor, 360.0);
						break;
					default:
						angle.y -= 180.0f;
					}
				}
				SendPacket = false;
			}
			else {
				{
					angle.y = *((C_BasePlayer*)pEntityList->GetClientEntity(
						pEngine->GetLocalPlayer()))->GetLowerBodyYawTarget() + 97.0f;

					if (uff2) {

						if (uff4 > 0) {

							uff4 = 1000;
							angle.y += 1888 + uff4 / rand();
							uff4 = 88000;
						}
						else {
							angle.y = *((C_BasePlayer*)pEntityList->GetClientEntity(
								pEngine->GetLocalPlayer()))->GetLowerBodyYawTarget() + uff4;
							uff4 = rand();
						}

						uff2 = false;
					}
					else {

						angle.y = rand() * rand();
						uff2 = true;

					}


					if (uff3) {
						angle.y = -97.f;
						uff3 = false;
					}
					else {
						angle.y = *((C_BasePlayer*)pEntityList->GetClientEntity(
							pEngine->GetLocalPlayer()))->GetLowerBodyYawTarget() + 97.f;
						uff3 = true;


					}
				}

				SendPacket = true;

			}
		}
		else {

			yFlip ? angle.y -= 170 : angle.y += 170;

		}
		break;
	case AntiAimType_Y::RASP:
		static float realAngle = AntiAim::lastRealYaw;
		static float fakeAngle = AntiAim::lastFakeYaw;
		realAngle = AntiAim::lastRealYaw;
		realAngle = AntiAim::lastRealYaw;
		if (Settings::AntiAim::Standing::Yaw::typeFake == AntiAimType_Y::RASP || Settings::AntiAim::Moving::Yaw::typeFake == AntiAimType_Y::RASP || Settings::AntiAim::Airborne::Yaw::typeFake == AntiAimType_Y::RASP) {

			if (AntiAim::lastRealYaw == realAngle) {
				angle.y = realAngle + rand() % 110 + 70;
				realAngle = AntiAim::lastRealYaw;
			}
			else {
				angle.y = realAngle + rand() % 110 + 70;
				realAngle = AntiAim::lastRealYaw;
			}
		}
		else {
			if (AntiAim::lastFakeYaw == fakeAngle) {
				angle.y = fakeAngle + rand() % 110 + 70;
				fakeAngle = AntiAim::lastFakeYaw;
			}
			else {
				angle.y = fakeAngle + rand() % 110 + 70;
				fakeAngle = AntiAim::lastFakeYaw;
			}
		}
		break;
	case AntiAimType_Y::LBYBREAK:
		//Placeholder
		break;
	case AntiAimType_Y::FAKELBY:
		//clamp = false;
		if (!(pLocal->GetVelocity().x < 0.1f && pLocal->GetVelocity().x > -0.1f)) {
			angle.y -= 180.f;
		}
		else {
			if (lolgay) {
				yFlip ? angle.y -= *((C_BasePlayer*)pEntityList->GetClientEntity(
					pEngine->GetLocalPlayer()))->GetLowerBodyYawTarget() - 97 : angle.y -=
					*((C_BasePlayer*)pEntityList->GetClientEntity(
						pEngine->GetLocalPlayer()))->GetLowerBodyYawTarget() +
					97;
			}
			else {
				if (prevLBY1 != *((C_BasePlayer*)pEntityList->GetClientEntity(
					pEngine->GetLocalPlayer()))->GetLowerBodyYawTarget())
					flip1 = false;
				if (prevLBY1 != *((C_BasePlayer*)pEntityList->GetClientEntity(
					pEngine->GetLocalPlayer()))->GetLowerBodyYawTarget())
					flip1 = true;
				if (flip1)
					angle.y += 120.f;
				else
					angle.y -= 120.f;

				prevLBY1 = *((C_BasePlayer*)pEntityList->GetClientEntity(
					pEngine->GetLocalPlayer()))->GetLowerBodyYawTarget();
			}
			break;
	case AntiAimType_Y::LBYSPIN:
		factor = 360.0 / M_PHI;
		factor *= Settings::spinFactor::value;
		angle.y = *((C_BasePlayer*)pEntityList->GetClientEntity(
			pEngine->GetLocalPlayer()))->GetLowerBodyYawTarget() +
			fmodf(pGlobalVars->curtime * factor, 360.0);
		break;
	case AntiAimType_Y::LBYJITTER:
		yFlip ? angle.y = *((C_BasePlayer*)pEntityList->GetClientEntity(
			pEngine->GetLocalPlayer()))->GetLowerBodyYawTarget() + 35 : angle.y = *((C_BasePlayer*)pEntityList->GetClientEntity(
				pEngine->GetLocalPlayer()))->GetLowerBodyYawTarget() - 35;
		break;
	case AntiAimType_Y::NOAA:
		break;
	case AntiAimType_Y::BACKJITTER:
		static int random = rand() % 16;
		yFlip ? angle.y -= 180.f + random : angle.y += 180.f + random;
		break;
	case AntiAimType_Y::FAKESIDEWAYS:

		if (SendPacket) {
			angle.y -= 90.f;
			SendPacket = false;
		}
		else {
			angle.y = 120.0f;
			SendPacket = true;
		}
		break;
	case AntiAimType_Y::BACKJITTER2:
		yFlip ? angle.y += 146 : angle.y -= 146;
		break;
	case AntiAimType_Y::BACKWARDS:
		angle.y -= 180.0f;

		break;

	case AntiAimType_Y::FORWARDS:
		angle.y -= 0.0f;
		break;
	case AntiAimType_Y::LOWERBODY:
		angle.y = *((C_BasePlayer*)pEntityList->GetClientEntity(
			pEngine->GetLocalPlayer()))->GetLowerBodyYawTarget();
		break;
	case AntiAimType_Y::FJITTER: {
		if (SendPacket) {
			if (jitterticks > 0)
				jitterticks = -1;
			jitterticks++;
		}
		int add = 0;
		if (jitterticks == 0)
			add += 130;
		if (jitterticks == 1)
			add -= 145;
		if (ticks > 0 || !SendPacket) {
			add = -add;
			SendPacket = false;
		}
		angle.y += add;
	}
								 break;
	case AntiAimType_Y::richieap:

		if (fabsf(pLocal->GetVelocity().x) < 0.1) {
			besteap = !besteap;
			SendPacket = besteap;
			angle.y -= besteap ? 180 : *((C_BasePlayer*)pEntityList->GetClientEntity(
				pEngine->GetLocalPlayer()))->GetLowerBodyYawTarget() + 90.f;

		}
		else {
			besteap = !besteap;
			SendPacket = besteap;
			angle.y -= besteap ? 0 : 180;
		}
		break;
	case AntiAimType_Y::MYRRIB:
		if (SendPacket) {
			if (fabsf(pLocal->GetVelocity().x) != 0) {
				angle.y -= 180.f;
				SendPacket = false;
			}
			else {
				angle.y -= *((C_BasePlayer*)pEntityList->GetClientEntity(
					pEngine->GetLocalPlayer()))->GetLowerBodyYawTarget() + 45.f;
				SendPacket = false;
			}
		}
		else {
			if (fabsf(pLocal->GetVelocity().x) != 0) {
				angle.y -= 180; //pMemez here plz fix me
				SendPacket = true;
			}
			else {
				angle.y -= *((C_BasePlayer*)pEntityList->GetClientEntity(
					pEngine->GetLocalPlayer()))->GetLowerBodyYawTarget() - 180.f;
				SendPacket = true;
			}
		}
		break;
	case AntiAimType_Y::LBYONGROUND:
		if (pLocal->GetFlags() & FL_ONGROUND)
			angle.y = *((C_BasePlayer*)pEntityList->GetClientEntity(
				pEngine->GetLocalPlayer()))->GetLowerBodyYawTarget() + rand() % 35 + 165;
		else {
			*((C_BasePlayer*)pEntityList->GetClientEntity(
				pEngine->GetLocalPlayer()))->GetLowerBodyYawTarget();
			static int aimType = rand() % 4;
			switch (aimType) {
			case 1:
				yFlip ? angle.y += 90.f : angle.y -= 90.0f;
				break;
			case 2:
				yFlip ? angle.y -= 120.0f : angle.y -= 210.0f;
				break;
			case 3:
				factor = 360.0 / M_PHI;
				factor *= 25;
				angle.y = fmodf(pGlobalVars->curtime * factor, 360.0);
				break;
			default:
				angle.y -= 180.0f;
			}
		}
		break;
	case AntiAimType_Y::RASP2:  // get your own dank names :feelsmocked:
		if (!(pLocal->GetFlags() & FL_ONGROUND))
		{
			firstBreak = false;
		}
		else if (pLocal->GetVelocity().Length2D() > 1)
		{
			oldCurtime = pGlobalVars->curtime;
			firstBreak = false;
			if (std::fabs(*pLocal->GetLowerBodyYawTarget() - AntiAim::lastRealYaw) <= 35)
				angle.y = *pLocal->GetLowerBodyYawTarget() + offset;
		}
		if (pLocal->GetVelocity().Length2D() < 1 && pGlobalVars->curtime - oldCurtime > 0.22 && !firstBreak)
		{
			firstBreak = true;
			angle.y = AntiAim::lastRealYaw + offset;
			oldCurtime = pGlobalVars->curtime;
			angle.y = AntiAim::lastRealYaw + offset;
		}
		else if (pLocal->GetVelocity().Length2D() < 1 && pGlobalVars->curtime - oldCurtime > 1.1)
		{
			angle.y = AntiAim::lastRealYaw + offset;
			oldCurtime = pGlobalVars->curtime;
			angle.y = AntiAim::lastRealYaw + offset;
		}
		break;
	case AntiAimType_Y::FEETWIGGLE: {
		float Diff;
		bool MarcisAWeeb = false;
		float oldyaw = *((C_BasePlayer*)pEntityList->GetClientEntity(
			pEngine->GetLocalPlayer()))->GetLowerBodyYawTarget();

		if (MarcisAWeeb) {
			angle.y -= 145.0f;

			*((C_BasePlayer*)pEntityList->GetClientEntity(
				pEngine->GetLocalPlayer()))->GetLowerBodyYawTarget() += 45.0f;

			Diff = *((C_BasePlayer*)pEntityList->GetClientEntity(
				pEngine->GetLocalPlayer()))->GetLowerBodyYawTarget() - angle.y;

			MarcisAWeeb = false;
		}
		else {
			angle.y += 145.0f;

			*((C_BasePlayer*)pEntityList->GetClientEntity(
				pEngine->GetLocalPlayer()))->GetLowerBodyYawTarget() -= 45.0f;

			Diff = *((C_BasePlayer*)pEntityList->GetClientEntity(
				pEngine->GetLocalPlayer()))->GetLowerBodyYawTarget() - angle.y;

			MarcisAWeeb = true;
		}


		if (oldyaw == *((C_BasePlayer*)pEntityList->GetClientEntity(
			pEngine->GetLocalPlayer()))->GetLowerBodyYawTarget()) {

			angle.y += oldyaw / 2 + 83;
			*((C_BasePlayer*)pEntityList->GetClientEntity(
				pEngine->GetLocalPlayer()))->GetLowerBodyYawTarget() -= oldyaw / 3 + 93;
		}

		if (std::abs(Diff) <= 35.0f) {
			angle.y += 55.0f;
			*((C_BasePlayer*)pEntityList->GetClientEntity(
				pEngine->GetLocalPlayer()))->GetLowerBodyYawTarget() -= 75.0f;

		}
	}
									break;
	case AntiAimType_Y::MASTERLOOSER: {
		int value = rand() % 361;
		float clamped = Math::ClampYaw(*pLocal->GetLowerBodyYawTarget() + value);
		if (fabsf(*pLocal->GetLowerBodyYawTarget() - clamped) < 45)
			value += 50;
		else if (fabsf(*pLocal->GetLowerBodyYawTarget() - clamped) > 310)
			value -= 50;

		angle.y = Math::ClampYaw(*pLocal->GetLowerBodyYawTarget() + value) - 180;
	}
									  break;
	case AntiAimType_Y::PAAFAKE: {
		bool isDucked = pLocal->GetFlags() & FL_DUCKING;
		// int shotsFired = pLocal->GetShotsFired(); Unused
		bool hasHelmet = pLocal->HasHelmet();
		// float lby = *pLocal->GetLowerBodyYawTarget(); Unused

		ButtonCode_t LeftArrow = KEY_LEFT;
		ButtonCode_t RightArrow = KEY_RIGHT;

		static bool switch1 = false;

		if (pInputSystem->IsButtonDown(LeftArrow)) {
			angle.y += 90;
		}
		else if (pInputSystem->IsButtonDown(RightArrow)) {
			angle.y -= 90;
		}
		else {
			float headAngEdge = HeadEdgeAng();
			if (AntiAim::canEdge) {
				angle.y = headAngEdge + 180;
			}
			else {
				if (!hasHelmet) {
					int x = rand() % 41 - 20;
					switch1 = !switch1;
					if (switch1)
						angle.y += 90 + x;
					else
						angle.y -= 90 + x;
				}
				else {
					if (isDucked) {
						angle.y -= 90;
					}
					else {
						int spin = -180;
						spin++;
						if (spin > 180)
							spin = -180;

						angle.y = spin;
					}
				}
			}
		}
	}
								 break;
	case AntiAimType_Y::PAAREAL: {
		bool isDucked = pLocal->GetFlags() & FL_DUCKING;
		// int shotsFired = pLocal->GetShotsFired(); Unused
		bool hasHelmet = pLocal->HasHelmet();
		float lby = *pLocal->GetLowerBodyYawTarget();

		ButtonCode_t LeftArrow = KEY_LEFT;
		ButtonCode_t RightArrow = KEY_RIGHT;

		if (pInputSystem->IsButtonDown(LeftArrow)) {
			angle.y -= 90;
		}
		else if (pInputSystem->IsButtonDown(RightArrow)) {
			angle.y += 90;
		}
		else {
			float headAngEdge = HeadEdgeAng();
			if (AntiAim::canEdge) {
				angle.y = headAngEdge;
			}
			else {
				if (!hasHelmet) {
					int x = rand() % 21 - 10;
					angle.y -= 180 + x;
				}
				else {
					if (isDucked) {
						int y = rand() % 181;
						angle.y += y;
					}
					else {
						int z = 90 + rand() % 91;
						angle.y = lby + z;
					}
				}
			}
		}
	}
								 break;
	case AntiAimType_Y::KIDUA: {
		static bool bFlip = false;
		float flCurTime = pGlobalVars->curtime;
		static float flTimeUpdate = 1.09f;
		static float flNextTimeUpdate = flCurTime + flTimeUpdate;
		static int state = 0;
		if (flCurTime >= flNextTimeUpdate) {
			bFlip = !bFlip;
			state = 0;
		}

		if (bFlip) {
			angle.y = 90.f + state * 34.f;
			if (state > 1)
				angle.y = 90.f + 34.8f;
		}
		else {
			angle.y = -90.f - state * 34.f;
			if (state > 1)
				angle.y = -90.f - 34.8f;
		}
		state++;
	}
							   break;
		}
	case AntiAimType_Y::NUMBER_OF_TYPES:break;
	}

	if (AntiAim::IsAirborne()) {
		angle.y += bFlip ? Settings::AntiAim::Airborne::Yaw::typeFakeAdd : Settings::AntiAim::Airborne::Yaw::typeAdd;
	}
	else if (AntiAim::IsMoving()) {
		angle.y += bFlip ? Settings::AntiAim::Moving::Yaw::typeFakeAdd : Settings::AntiAim::Moving::Yaw::typeAdd;
	}
	else {
		angle.y += bFlip ? Settings::AntiAim::Standing::Yaw::typeFakeAdd : Settings::AntiAim::Standing::Yaw::typeAdd;
	}
}

static void DoAntiAimX(Vector& angle, bool bFlip, bool& clamp) {
	static C_BasePlayer* pLocal = (C_BasePlayer*)pEntityList->GetClientEntity(pEngine->GetLocalPlayer());

	AntiAimType_X antiAim = Settings::AntiAim::Standing::Pitch::type;
	if (AntiAim::IsMoving()) {
		antiAim = Settings::AntiAim::Moving::Pitch::type;
	}
	if (AntiAim::IsAirborne()) {
		antiAim = Settings::AntiAim::Airborne::Pitch::type;
	}

	static bool fakezeroS = false;
	static bool fakeupS = false;

	switch (antiAim) {
	case AntiAimType_X::STATIC_UP:
		angle.x = -89.0f;
		break;
	case AntiAimType_X::FLIP:
		if (fabsf(pLocal->GetVelocity().x) != 0) {
			angle.x = bFlip ? -55.0f : 40.0f;
		}
		else {
			angle.x = 50.0f;
		}
		break;
	case AntiAimType_X::STATIC_DOWN:
		angle.x = 89.0f;
		break;
	case AntiAimType_X::FAKEZERO:
		fakezeroS = !fakezeroS;
		SendPacket = fakezeroS;
		angle.x = fakezeroS ? 0 : 89;
		break;
	case AntiAimType_X::FAKEUP:
		fakeupS = !fakeupS;
		SendPacket = fakeupS;
		angle.x = fakeupS ? -89 : 89;
		break;
	case AntiAimType_X::NUMBER_OF_TYPES:break;
	}

	if (AntiAim::IsAirborne()) {
		angle.x += Settings::AntiAim::Airborne::Pitch::custom;
	}
	else if (AntiAim::IsMoving()) {
		angle.x += Settings::AntiAim::Moving::Pitch::custom;
	}
	else {
		angle.x += Settings::AntiAim::Standing::Pitch::custom;
	}
}

static void DoAntiAimZ(Vector& angle, int command_number, bool& clamp) {
	AntiAimType_Z antiAim = Settings::AntiAim::Standing::Roll::type;
	if (AntiAim::IsMoving()) {
		antiAim = Settings::AntiAim::Moving::Roll::type;
	}
	if (AntiAim::IsAirborne()) {
		antiAim = Settings::AntiAim::Airborne::Roll::type;
	}

	switch (antiAim) {
	case AntiAimType_Z::TEST:
		angle.z = 180.0f;
		break;
	case AntiAimType_Z::NUMBER_OF_TYPES:break;
	}
}

static void DoAntiAimLBY(Vector& angle, int command_number, bool bFlip, bool& clamp) {
	// static float pDance = 0.0f; Unused
	static bool xFlip;

	AntiAimType_LBY antiAim = Settings::AntiAim::Standing::LBY::type;
	if (AntiAim::IsMoving()) {
		antiAim = Settings::AntiAim::Moving::LBY::type;
	}
	if (AntiAim::IsAirborne()) {
		antiAim = Settings::AntiAim::Airborne::LBY::type;
	}

	static C_BasePlayer* pLocal = (C_BasePlayer*)pEntityList->GetClientEntity(pEngine->GetLocalPlayer());
	xFlip = bFlip != xFlip;

	switch (antiAim) {
	case AntiAimType_LBY::ONE: {
		static bool flip1 = false;
		static float prevLBY1 = *((C_BasePlayer*)pEntityList->GetClientEntity(
			pEngine->GetLocalPlayer()))->GetLowerBodyYawTarget();

		if (pLocal->GetVelocity().x < 0.1f && pLocal->GetVelocity().x > -0.1f) {

			if (prevLBY1 !=
				*((C_BasePlayer*)pEntityList->GetClientEntity(
					pEngine->GetLocalPlayer()))->GetLowerBodyYawTarget())
				flip1 = false;
			else
				flip1 = true;
			if (flip1)
				angle.y += 108.f;
			else
				angle.y -= 109.f;

			prevLBY1 = *((C_BasePlayer*)pEntityList->GetClientEntity(
				pEngine->GetLocalPlayer()))->GetLowerBodyYawTarget();
		}
		else {
			angle.y -= 180;
		}
	}
							   break;
	case AntiAimType_LBY::TWO: {
		static bool flip2 = false;
		static float prevLBY2 = *((C_BasePlayer*)pEntityList->GetClientEntity(
			pEngine->GetLocalPlayer()))->GetLowerBodyYawTarget();

		if (pLocal->GetVelocity().x < 0.1f && pLocal->GetVelocity().x > -0.1f) {

			if (prevLBY2 !=
				*((C_BasePlayer*)pEntityList->GetClientEntity(
					pEngine->GetLocalPlayer()))->GetLowerBodyYawTarget())
				flip2 = !flip2;

			if (flip2)
				angle.y += 90.f;
			else
				angle.y -= 90.f;

			prevLBY2 = *((C_BasePlayer*)pEntityList->GetClientEntity(
				pEngine->GetLocalPlayer()))->GetLowerBodyYawTarget();
		}
		else
			angle.y -= 0.0f;
	}
							   break;
	case AntiAimType_LBY::THREE: {
		static bool LBYflip = true;
		static float prevLBY1;
		static int cuck = 0;
		static float nextUpdate = pGlobalVars->curtime + 1.1f;
		cuck = prevLBY1 + 108;
		LBYflip = !LBYflip;
		if (LBYflip) {
			if (fabsf(pLocal->GetVelocity().x) != 0) {
				angle.y += 180.f;
				SendPacket = false;
			}
			else {
				if (pGlobalVars->curtime > nextUpdate) {

					angle.y += cuck;
					SendPacket = false;
					nextUpdate = pGlobalVars->curtime + 1.1f;
				}
			}
		}
		else {
			angle.y += *((C_BasePlayer*)pEntityList->GetClientEntity(
				pEngine->GetLocalPlayer()))->GetLowerBodyYawTarget() + 90.f;
			SendPacket = true;
		}
	}
								 break;
	case AntiAimType_LBY::MYRRIB: {
		static float LBYsave = 0;
		float LBY = *((C_BasePlayer*)pEntityList->GetClientEntity(
			pEngine->GetLocalPlayer()))->GetLowerBodyYawTarget();

		if (LBY != LBYsave) {
			LBYsave = LBY;
		}

		static bool LBYflipM = false;
		LBYflipM = !LBYflipM;
		if (LBYflipM) {
			angle.y = LBYsave;
			SendPacket = true;
		}
		else {
			angle.y = LBYsave + 180.f;
			SendPacket = false;
		}
	}
								  break;
	case AntiAimType_LBY::FOUR:
		// TODO
		break;
	case AntiAimType_LBY::MYRRIB2: {


		bool onGround = (pLocal->GetFlags() & FL_ONGROUND);
		bool moving = (fabsf(pLocal->GetVelocity().x) != 0);
		bool onGroundMoving = (onGround && moving);

		float lby = *((C_BasePlayer*)pEntityList->GetClientEntity(
			pEngine->GetLocalPlayer()))->GetLowerBodyYawTarget();
		float realAngle = AntiAim::lastRealYaw;
		float fakeAngle = AntiAim::lastFakeYaw;

		static bool switch1 = false;
		static bool switch2 = false;
		static bool switch3 = false;

		if (onGroundMoving) {
			angle.y -= 180;
		}
		else {
			if (realAngle == lby) {
				switch1 = !switch1;
				if (switch1) {
					angle.y -= 180;
					SendPacket = false;
				}
				else {
					angle.y += 90;
					SendPacket = true;
				}
			}
			else if (realAngle == fakeAngle) {
				switch2 = !switch2;
				if (switch2) {
					angle.y -= 180;
					SendPacket = false;
				}
				else {
					angle.y += 45;
					SendPacket = true;
				}
			}
			else {
				switch3 = !switch3;
				if (switch3) {
					int rng = rand() % 3 + 1;
					if (rng == 1) {
						angle.y = lby + 90;
						SendPacket = false;
					}
					else if (rng == 2) {
						angle.y = lby + 180;
						SendPacket = false;
					}
					else {
						angle.y = lby - 90;
						SendPacket = false;
					}
				}
				else {
					angle.y = lby;
					SendPacket = true;
				}
			}
		}

	}
								   break;
	case AntiAimType_LBY::SUICIDE: {
		// static bool willUpdate; Value not used
		static float lastLbyUpdateTime = 0;
		float serverTime = pLocal->GetTickBase() * pGlobalVars->interval_per_tick;
		if (serverTime >= lastLbyUpdateTime)
		{
			lastLbyUpdateTime = serverTime + 1.125f;
			// willUpdate = true;
			angle.y -= *pLocal->GetLowerBodyYawTarget() + rand() % 32 + 30;
		}
		else
		{
			// willUpdate = false;
			angle.y += *pLocal->GetLowerBodyYawTarget() - rand() % 181 + 180;
		}
	}
								   break;
	case AntiAimType_LBY::NONE:
		//Settings::AntiAim::Lby::enabled = false;
		break;
	case AntiAimType_LBY::NUMBER_OF_TYPES:break;
	}
}


void AntiAim::CreateMove(CUserCmd* cmd) {
	isAntiAiming = false;

	if ((AntiAim::IsStanding() && !Settings::AntiAim::Standing::Yaw::enabled &&
		!Settings::AntiAim::Standing::Pitch::enabled) ||
		(AntiAim::IsMoving() && !Settings::AntiAim::Moving::Yaw::enabled &&
			!Settings::AntiAim::Moving::Pitch::enabled) ||
			(AntiAim::IsAirborne() && !Settings::AntiAim::Airborne::Yaw::enabled &&
				!Settings::AntiAim::Airborne::Pitch::enabled)) {
		if (Settings::FakeLag::type != FakeLagType::OFF) {
			FakeLag::bFlipping = true;
		}
		return;
	}

	if (Settings::Aimbot::AimStep::enabled && Aimbot::aimStepInProgress)
		return;

	Vector oldAngle = cmd->viewangles;
	float oldForward = cmd->forwardmove;
	float oldSideMove = cmd->sidemove;

	Vector angle = cmd->viewangles;

	C_BasePlayer* pLocal = (C_BasePlayer*)pEntityList->GetClientEntity(pEngine->GetLocalPlayer());
	if (!pLocal) {
		AntiAim::fakeTp = true;
		return;
	}

	C_BaseCombatWeapon* activeWeapon = (C_BaseCombatWeapon*)pEntityList->GetClientEntityFromHandle(
		pLocal->GetActiveWeapon());
	if (!activeWeapon) {
		AntiAim::fakeTp = true;
		return;
	}

	if (activeWeapon->GetCSWpnData()->GetWeaponType() == CSWeaponType::WEAPONTYPE_GRENADE) {
		C_BaseCSGrenade* csGrenade = (C_BaseCSGrenade*)activeWeapon;

		if (csGrenade->GetThrowTime() > 0.0f) {
			AntiAim::fakeTp = true;
			return;
		}
	}

	if (cmd->buttons & IN_USE) {
		AntiAim::fakeTp = true;
		return;
	}

	if (*activeWeapon->GetItemDefinitionIndex() == ItemDefinitionIndex::WEAPON_REVOLVER) {
		if (Settings::Aimbot::AutoCockRevolver::enabled) {
			if (Aimbot::shootingRevolver) {
				AntiAim::fakeTp = true;
				return;
			}
		}
		else {
			if (cmd->buttons & IN_ATTACK2 || cmd->buttons & IN_ATTACK) {
				AntiAim::fakeTp = true;
				return;
			}
		}
	}
	else {
		if (cmd->buttons & IN_ATTACK) {
			AntiAim::fakeTp = true;
			return;
		}
	}
	// ^ This requires a rework for auto cock ^ working now?

	if (pLocal->GetMoveType() == MOVETYPE_LADDER || pLocal->GetMoveType() == MOVETYPE_NOCLIP) {
		AntiAim::fakeTp = true;
		return;
	}

	// AutoDisable checks

	// Knife
	if (Settings::AntiAim::Misc::AutoDisable::knifeHeld && pLocal->GetAlive() &&
		activeWeapon->GetCSWpnData()->GetWeaponType() == CSWeaponType::WEAPONTYPE_KNIFE) {
		AntiAim::fakeTp = true;
		return;
	}

	// Bomb
	if (Settings::AntiAim::Misc::AutoDisable::bombHeld && pLocal->GetAlive() &&
		activeWeapon->GetCSWpnData()->GetWeaponType() == CSWeaponType::WEAPONTYPE_C4) {
		AntiAim::fakeTp = true;
		return;
	}

	// No Enemy
	if (Settings::AntiAim::Misc::AutoDisable::noEnemy && pLocal->GetAlive() && !HasViableEnemy()) {
		AntiAim::fakeTp = true;
		return;
	}

	// Freezetime
	if (Settings::AntiAim::Misc::AutoDisable::freezeTime &&
		((*pGameRules) && (*pGameRules)->IsFreezeTime())) {
		AntiAim::fakeTp = true;
		return;
	}

	AntiAim::fakeTp = false;

	isAntiAiming = true;


	/* Unused shieet
	* Vector edge_angle = angle;
	* bool shouldEdge = AntiAim::IsAirborne() ? Settings::AntiAim::Airborne::HeadEdge::enabled :
	AntiAim::IsMoving() ? Settings::AntiAim::Moving::HeadEdge::enabled :
	Settings::AntiAim::Standing::HeadEdge::enabled;
	shouldEdge = shouldEdge && GetBestHeadAngle( edge_angle );*/

	static bool bFlip;

	bFlip = !bFlip;
	FakeLag::bFlipping = bFlip;

	bool shouldClamp = (IsStanding() && !Settings::AntiAim::Standing::untrustedAngles) ||
		(IsMoving() && !Settings::AntiAim::Moving::untrustedAngles) ||
		(IsAirborne() && !Settings::AntiAim::Airborne::untrustedAngles);

	if (!Settings::AntiAim::Standing::untrustedAngles) {
		if (Settings::AntiAim::Standing::Pitch::type > AntiAimType_X::STATIC_DOWN) {
			Settings::AntiAim::Standing::Pitch::type = AntiAimType_X::STATIC_DOWN;
		}
		if (Settings::AntiAim::Standing::Roll::enabled) {
			Settings::AntiAim::Standing::Roll::enabled = false;
		}
	}

	if (!Settings::AntiAim::Moving::untrustedAngles) {
		if (Settings::AntiAim::Moving::Pitch::type > AntiAimType_X::STATIC_DOWN) {
			Settings::AntiAim::Moving::Pitch::type = AntiAimType_X::STATIC_DOWN;
		}
		if (Settings::AntiAim::Moving::Roll::enabled) {
			Settings::AntiAim::Moving::Roll::enabled = false;
		}
	}

	if (!Settings::AntiAim::Airborne::untrustedAngles) {
		if (Settings::AntiAim::Airborne::Pitch::type > AntiAimType_X::STATIC_DOWN) {
			Settings::AntiAim::Airborne::Pitch::type = AntiAimType_X::STATIC_DOWN;
		}
		if (Settings::AntiAim::Airborne::Roll::enabled) {
			Settings::AntiAim::Airborne::Roll::enabled = false;
		}
	}

	long millis = Util::GetEpochTime();
	if (millis - lastPress >= 1000) {
		if (Settings::AntiAim::Airborne::SwitchAA::enabled &&
			pInputSystem->IsButtonDown(Settings::AntiAim::Airborne::SwitchAA::key)) {
			if (AntiAim::IsAirborne()) {
				SwitchAA(ANTIAIM_AIRBORNE);
				lastPress = Util::GetEpochTime();
			}
		}
		else if (Settings::AntiAim::Moving::SwitchAA::enabled &&
			pInputSystem->IsButtonDown(Settings::AntiAim::Moving::SwitchAA::key)) {
			if (AntiAim::IsMoving()) {
				SwitchAA(ANTIAIM_MOVING);
				lastPress = Util::GetEpochTime();
			}
		}
		else if (Settings::AntiAim::Standing::SwitchAA::enabled &&
			pInputSystem->IsButtonDown(Settings::AntiAim::Standing::SwitchAA::key)) {
			if (AntiAim::IsStanding()) {
				SwitchAA(ANTIAIM_STANDING);
				lastPress = Util::GetEpochTime();
			}
		}
	}

	if (IsAirborne() ? Settings::AntiAim::Airborne::Yaw::enabled :
		IsMoving() ? Settings::AntiAim::Moving::Yaw::enabled :
		Settings::AntiAim::Standing::Yaw::enabled) {

		if ((IsAirborne() ? Settings::AntiAim::Airborne::LBY::enabled :
			IsMoving() ? Settings::AntiAim::Moving::LBY::enabled :
			Settings::AntiAim::Standing::LBY::enabled) && !bFlip) {
			DoAntiAimLBY(angle, cmd->command_number, bFlip, shouldClamp);
			Math::NormalizeAngles(angle);

		}
		else {
			DoAntiAimY(angle, cmd->command_number, bFlip, shouldClamp);
			Math::NormalizeAngles(angle);

			float angleYEdge = GetBestHeadEdgeAngle();
			if (AntiAim::CanEdge()) {
				//pCvar->ConsoleColorPrintf(ColorRGBA(255, 255, 255), XorStr("Started edge!"));
				


				static float fakeAdd = 0.f;
				static float realAdd = 0.f;

				if (AntiAim::IsAirborne()) {
					fakeAdd = Settings::AntiAim::Airborne::HeadEdge::fakeAdd;
					realAdd = Settings::AntiAim::Airborne::HeadEdge::realAdd;
				}
				else if (AntiAim::IsMoving()) {
					fakeAdd = Settings::AntiAim::Moving::HeadEdge::fakeAdd;
					realAdd = Settings::AntiAim::Moving::HeadEdge::realAdd;
				}
				else if (AntiAim::IsStanding()) {
					fakeAdd = Settings::AntiAim::Standing::HeadEdge::fakeAdd;
					realAdd = Settings::AntiAim::Standing::HeadEdge::realAdd;
				}


				if (!bFlip) {
					SendPacket = false;
					angle.y = Math::ClampYaw(angleYEdge + realAdd);
				}
				else {
					SendPacket = true;
					angle.y = Math::ClampYaw(angleYEdge + fakeAdd);
				}
			}
		
		}
		if (Settings::FakeLag::type == FakeLagType::OFF)
			SendPacket = bFlip;
		/*if ( ( IsAirborne() ? Settings::AntiAim::Airborne::HeadEdge::enabled :
		IsMoving() ? Settings::AntiAim::Moving::HeadEdge::enabled :
		Settings::AntiAim::Standing::HeadEdge::enabled ) && shouldEdge && !bFlip )
		angle.y = edge_angle.y;*/
	}
	/*if (Settings::AntiAim::Yaw::dynamicAA) {

	DoAAatTarget(angle, cmd->command_number, bFlip, should_clamp);
	Math::NormalizeAngles(angle);
	if (!Settings::FakeLag::enabled)
	SendPacket = bFlip;
	if (Settings::AntiAim::HeadEdge::enabled && edging_head && !bFlip)
	angle.y = edge_angle.y;
	}*/

	if ((IsAirborne() ? Settings::AntiAim::Airborne::SwitchAA::enabled && Settings::AntiAim::Airborne::untrustedAngles
		:
		IsMoving() ? Settings::AntiAim::Moving::SwitchAA::enabled && Settings::AntiAim::Moving::untrustedAngles :
		Settings::AntiAim::Standing::SwitchAA::enabled && Settings::AntiAim::Standing::untrustedAngles)) {
		DoAntiAimZ(angle, cmd->command_number, shouldClamp);
	}
	if ((IsAirborne() ? Settings::AntiAim::Airborne::Pitch::enabled :
		IsMoving() ? Settings::AntiAim::Moving::Pitch::enabled :
		Settings::AntiAim::Standing::Pitch::enabled)) {
		DoAntiAimX(angle, bFlip, shouldClamp);
	}

	/*if ((IsAirborne() ? Settings::AntiAim::Airborne::HeadEdge::enabled :
		IsMoving() ? Settings::AntiAim::Moving::HeadEdge::enabled :
		Settings::AntiAim::Standing::HeadEdge::enabled)) {
		if (CanEdge()) {
			float angleYEdge = GetBestHeadEdgeAngle();


			static float fakeAdd = 0.f;
			static float realAdd = 0.f;

			if (AntiAim::IsAirborne()) {
				fakeAdd = Settings::AntiAim::Airborne::HeadEdge::fakeAdd;
				realAdd = Settings::AntiAim::Airborne::HeadEdge::realAdd;
			}
			else if (AntiAim::IsMoving()) {
				fakeAdd = Settings::AntiAim::Moving::HeadEdge::fakeAdd;
				realAdd = Settings::AntiAim::Moving::HeadEdge::realAdd;
			}
			else if (AntiAim::IsStanding()) {
				fakeAdd = Settings::AntiAim::Standing::HeadEdge::fakeAdd;
				realAdd = Settings::AntiAim::Standing::HeadEdge::realAdd;
			}


			if (!bFlip) {
				SendPacket = false;
				angle.y = Math::ClampYaw(angleYEdge + realAdd);
			}
			else {
				SendPacket = true;
				angle.y = Math::ClampYaw(angleYEdge + fakeAdd);
			}
		}
	}*/

	Math::NormalizeAngles(angle);

	if (shouldClamp) {
		Math::ClampAngles(angle);
	}

	cmd->viewangles = angle;

	if ((IsAirborne() ? Settings::AntiAim::Airborne::antiResolver :
		IsMoving() ? Settings::AntiAim::Moving::antiResolver :
		Settings::AntiAim::Standing::antiResolver)) {
		static bool antiResolverFlip = false;

		if (cmd->viewangles.y == *((C_BasePlayer*)pEntityList->GetClientEntity(
			pEngine->GetLocalPlayer()))->GetLowerBodyYawTarget()) {
			if (antiResolverFlip) {
				cmd->viewangles.y += +90;
			}
			else {
				cmd->viewangles.y -= +90;
			}

			antiResolverFlip = !antiResolverFlip;

			Math::NormalizeAngles(cmd->viewangles);

			if (shouldClamp) {
				Math::ClampAngles(cmd->viewangles);
			}
		}
	}

	if (bFlip) {
		AntiAim::lastFakeYaw = cmd->viewangles.y;
	}
	else {
		AntiAim::lastRealYaw = cmd->viewangles.y;
	}

	Math::CorrectMovement(oldAngle, cmd, oldForward, oldSideMove);
}

bool AntiAim::IsStanding() {
	return !IsMoving() && !IsAirborne();
}

bool AntiAim::IsMoving() {
	C_BasePlayer* pLocal = (C_BasePlayer*)pEntityList->GetClientEntity(pEngine->GetLocalPlayer());
	if (!pLocal || !pLocal->GetAlive())
		return false;

	return pLocal->GetVelocity().Length2D() > 0.2f;
}

bool AntiAim::IsAirborne() {
	C_BasePlayer* pLocal = (C_BasePlayer*)pEntityList->GetClientEntity(pEngine->GetLocalPlayer());
	if (!pLocal || !pLocal->GetAlive() || !pLocal->GetFlags())
		return false;

	return !(pLocal->GetFlags() & FL_ONGROUND);
}
