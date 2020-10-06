#include "resolver.h"
bool Settings::Resolver::enabled = false;
bool Settings::Resolver::info = false;
bool Settings::Resolver::pitch = false;
float Settings::Resolver::ticks = 2;
float Settings::Resolver::modulo = 2;
bool Settings::Resolver::LagComp = false;
bool Settings::Resolver::lbyOnly = false;
bool Settings::Resolver::ResolverOverride = false;
int Settings::Resolver::baimAfter = -1;
int Resolver::resolvingId = -1;
std::vector<int64_t> Resolver::playerAngleLogs = {};
std::array<CResolveInfo, 32> Resolver::m_arrInfos;


ButtonCode_t Settings::Resolver::angleFlip = ButtonCode_t::KEY_F;
ButtonCode_t Settings::Resolver::ResolverOverrideKey = ButtonCode_t::KEY_T;
bool Settings::Resolver::angleFlipEnabled = false;

int Shotsmissed = 0;
bool shotATT;
std::vector<std::pair<C_BasePlayer*, Vector>> player_data;
std::random_device rd;
// static float rSimTime[65]; Unused

bool didDmg = false;

bool Resolver::lbyUpdated = false;

bool Resolver::shouldBaim = false;

AnimationLayer curBalanceLayer, prevBalanceLayer;

std::map<int, int> Resolver::shotsMiss = {
	{ -1, 0 }
};

std::map<int, float> Resolver::angForce = {
	{ -1, 0 }
};

std::map<int, float> Resolver::lastHitAng = {
	{ -1, 0 }
};

std::map<int, const char*> Resolver::angForceTxt = {
	{ -1, "None" }
};

std::map<int, float> Resolver::lby = {
	{ -1, 0 }
};

static void StartLagComp(C_BasePlayer* player, CUserCmd* cmd) {
	if (!Settings::Aimbot::backtrack)
		Settings::Aimbot::backtrack = true;
}

void Resolver::Hug(C_BasePlayer* player) {


	auto cur = m_arrInfos.at(player->GetIndex()).m_sRecords;

	Resolver::resolvingId = player->GetIndex();

	Vector angle = *player->GetEyeAngles();

	float lby = *player->GetLowerBodyYawTarget();
	float curTime = pGlobalVars->curtime;
	float velocity = fabsf(player->GetVelocity().Length2D());
	bool onGround = player->GetFlags() & FL_ONGROUND;
	bool isMoving = (onGround && player->GetVelocity().Length2D() > 0.1f);
	bool maybeFakeWalking = (isMoving && velocity < 35.0f);
	float lbyUpdateTime = isMoving ? 0.22f : 1.1f;




	int index = player->GetIndex();
//	CTickRecord &player_recs = arr_infoSecond[i];
//	SResolveInfo &player_recs = arr_infoSecond[i];

	std::map<int, float> lbyDeltaMove = {
		{ player->GetIndex(), 0.f }
	};
	std::map<int, float> lastUpdate = {
		{ player->GetIndex(), 0.f }
	};
	std::map<int, float> playerAngle1 = {
		{ player->GetIndex(), lby }
	};
	std::map<int, float> playerAngle2 = {
		{ player->GetIndex(), lby }
	};
	std::map<int, int> playerCounter = {
		{ player->GetIndex(), 0 }
	};
	std::map<int, bool> staticReal = {
		{ player->GetIndex(), false }
	};
	std::map<int, int> shotsMissSave = {
		{ player->GetIndex(), Resolver::shotsMiss[player->GetIndex()] }
	};
	std::map<int, float> shotsMissSaveLTime = {
		{ player->GetIndex(), curTime }
	};
	std::map<int, bool> hasFakeWalk = {
		{ player->GetIndex(), false }
	};

	Resolver::lby[player->GetIndex()] = lby;

	float shotsMissSaveTime = 2.0f;

	if (Resolver::shotsMiss[player->GetIndex()] > shotsMissSave[player->GetIndex()]) {
		shotsMissSave[player->GetIndex()] = Resolver::shotsMiss[player->GetIndex()];
		shotsMissSaveLTime[player->GetIndex()] = curTime;
	}
	else if (shotsMissSaveLTime[player->GetIndex()] > curTime + shotsMissSaveTime)
		shotsMissSave[player->GetIndex()] = Resolver::shotsMiss[player->GetIndex()];


	
	if (Settings::Resolver::enabled) {
		if (IsEntityMoving(player)) {
			lbyDeltaMove[player->GetIndex()] = fabsf(lby - player->GetEyeAngles()->y);
			Resolver::lbyUpdated = true;
			lastUpdate[player->GetIndex()] = curTime;
			angle.y = lby;
		}
		else if (curTime == lastUpdate[player->GetIndex()] + lbyUpdateTime) {
			Resolver::lbyUpdated = true;
			lastUpdate[player->GetIndex()] = curTime;
		}
		else
			Resolver::lbyUpdated = false;

		if (pInputSystem->IsButtonDown(Settings::Resolver::angleFlip)) {
			angle.y = lby + 180.0f;
			Resolver::angForce[player->GetIndex()] = lby + 180.f;
			Resolver::angForceTxt[player->GetIndex()] = "LBY + 180";
		}
		
		//if (Settings::Resolver::ResolverOverride && pInputSystem->IsButtonDown(Settings::Resolver::ResolverOverrideKey)) {
		//	Override(); //needs an improvement sometimes fucked up xD
		//	Resolver::angForceTxt[player->GetIndex()] = "Overriding";
		//}

		else {
			if (Settings::Resolver::lbyOnly) {
				if (Resolver::lbyUpdated || (Backtracking::backtrackingLby && Settings::Resolver::LagComp))
					Resolver::shouldBaim = false;
				else
					Resolver::shouldBaim = true;

				angle.y = lby;
				Resolver::angForce[player->GetIndex()] = lby;
				Resolver::angForceTxt[player->GetIndex()] = "LBY";
			}
			else {
				if (Resolver::lbyUpdated || (Backtracking::backtrackingLby && Settings::Resolver::LagComp)) {
					angle.y = lby;
					Resolver::angForceTxt[player->GetIndex()] = "LBY";
					if (maybeFakeWalking && hasFakeWalk[player->GetIndex()] && shotsMissSave[player->GetIndex()] > 1) {
						angle.y = lby + 180.0f;
						Resolver::angForce[player->GetIndex()] = lby + 180.f;
						Resolver::angForceTxt[player->GetIndex()] = "LBY + 180";
					}
					else if (shotsMissSave[player->GetIndex()] > 2) {
						switch (shotsMissSave[player->GetIndex()] % 3) {
						case 0: angle.y = lby + 180.f; Resolver::angForce[player->GetIndex()] = lby + 180.f; Resolver::angForceTxt[player->GetIndex()] = "LBY + 180"; break;
						case 1: angle.y = lby + 90.f; Resolver::angForce[player->GetIndex()] = lby + 90.f; Resolver::angForceTxt[player->GetIndex()] = "LBY + 90"; break;
						case 2: angle.y = lby - 90.f; Resolver::angForce[player->GetIndex()] = lby - 90.f; Resolver::angForceTxt[player->GetIndex()] = "LBY - 90"; break;
						}
						hasFakeWalk[player->GetIndex()] = true;
					}
				}
				else {
					if (staticReal[player->GetIndex()] && playerAngle1[player->GetIndex()] != lby) {
						if (shotsMissSave[player->GetIndex()] > 1) {
							switch (shotsMissSave[player->GetIndex()] % 5) {
							case 0: angle.y = lby + 90.f; Resolver::angForce[player->GetIndex()] = lby + 90.f; Resolver::angForceTxt[player->GetIndex()] = "LBY + 90"; break;
							case 1: angle.y = lby + 180.f; Resolver::angForce[player->GetIndex()] = lby + 180.f; Resolver::angForceTxt[player->GetIndex()] = "LBY + 180"; break;
							case 2: angle.y = lby + lbyDeltaMove[player->GetIndex()]; Resolver::angForce[player->GetIndex()] = lby + lbyDeltaMove[player->GetIndex()]; Resolver::angForceTxt[player->GetIndex()] = "LBY + LBY Delta Move"; break;
							case 3: angle.y = lby - lbyDeltaMove[player->GetIndex()]; Resolver::angForce[player->GetIndex()] = lby - lbyDeltaMove[player->GetIndex()];  Resolver::angForceTxt[player->GetIndex()] = "LBY - LBY Delta Move"; break;
							case 4: angle.y = lby - 90.f; Resolver::angForce[player->GetIndex()] = lby - 90.f; Resolver::angForceTxt[player->GetIndex()] = "LBY - 90"; break;
							}
						}
						else {
							angle.y = playerAngle1[player->GetIndex()];
							Resolver::angForce[player->GetIndex()] = playerAngle1[player->GetIndex()];
							Resolver::angForceTxt[player->GetIndex()] = "Saved angle";
						}
					}
					else {
						switch (shotsMissSave[player->GetIndex()] % 7) {
						case 0: angle.y = lby; Resolver::angForce[player->GetIndex()] = lby; Resolver::angForceTxt[player->GetIndex()] = "LBY"; break;
						case 1: angle.y = lby + 180.f; Resolver::angForce[player->GetIndex()] = lby + 180.f; Resolver::angForceTxt[player->GetIndex()] = "LBY + 180"; break;
						case 2: angle.y = lby + lbyDeltaMove[player->GetIndex()]; Resolver::angForce[player->GetIndex()] = lby + lbyDeltaMove[player->GetIndex()]; Resolver::angForceTxt[player->GetIndex()] = "LBY + LBY Delta Move"; break;
						case 3: angle.y = lby - lbyDeltaMove[player->GetIndex()]; Resolver::angForce[player->GetIndex()] = lby - lbyDeltaMove[player->GetIndex()]; Resolver::angForceTxt[player->GetIndex()] = "LBY - LBY Delta Move"; break;
						case 5: angle.y = lby + 90.f; Resolver::angForce[player->GetIndex()] = lby + 90.f; Resolver::angForceTxt[player->GetIndex()] = "LBY + 90"; break;
						case 6: angle.y = lby - 90.f; Resolver::angForce[player->GetIndex()] = lby - 90.f; Resolver::angForceTxt[player->GetIndex()] = "LBY - 90"; break;
						}
					}

					if (Settings::Resolver::baimAfter == -1)
						Resolver::shouldBaim = false;
					else if (shotsMissSave[player->GetIndex()] > Settings::Resolver::baimAfter)
						Resolver::shouldBaim = true;
					else
						Resolver::shouldBaim = false;
				}

				if (didDmg) {
					if (angle.y != lby) {
						switch (playerCounter[player->GetIndex()]) {
						case 0: playerAngle1[player->GetIndex()] = angle.y; playerCounter[player->GetIndex()]++; break;
						case 1: playerAngle2[player->GetIndex()] = angle.y; playerCounter[player->GetIndex()] = 0; break;
						}

						float angle1 = playerAngle1[player->GetIndex()];
						float angle2 = playerAngle2[player->GetIndex()];

						float angDiff = fabsf(angle1 - angle2);

						float tolerance = 15.f;

						if (angDiff < tolerance)
							staticReal[player->GetIndex()] = true;
						else
							staticReal[player->GetIndex()] = false;
					}
					Resolver::lastHitAng[player->GetIndex()] = angle.y;
				}

			}
		}

		if (Settings::Resolver::pitch) {
			if (angle.x < -179.f) angle.x += 360.f;
			else if (angle.x > 90.0 || angle.x < -90.0)
				angle.x = 89.f;
			else if (angle.x > 89.0 && angle.x < 91.0)
				angle.x -= 90.f;
			else if (angle.x > 179.0 && angle.x < 181.0)
				angle.x -= 180;
			else if (angle.x > -179.0 && angle.x < -181.0)
				angle.x += 180;
			else if (fabs(angle.x) == 0)
				angle.x = std::copysign(89.0f, angle.x);
		}

		player->GetEyeAngles()->y = Math::ResNormalizeYaw(angle.y);
		player->GetEyeAngles()->x = angle.x;
	}
}

bool IsPlayerValid(C_BasePlayer *player)
{
	if (!player)
		return false;

	if (!player->GetAlive())
		return false;

	if (player == G::LocalPlayer)
		return false;

	if (player->IsDormant())
		return false;


	if (player->GetTeam() == G::LocalPlayer->GetTeam())
		return false;

	if (player->GetImmune())
		return false;

	return true;
}

void Resolver::Override()
{

	if (!Settings::Resolver::ResolverOverride)
		return;

	if (!pInputSystem->IsButtonDown(Settings::Resolver::ResolverOverrideKey))
		return;


	int w, h, cx, cy;

	pEngine->GetScreenSize(w, h);

	cx = w / 2;
	cy = h / 2;

	Vector crosshair = Vector(cx, cy, 0);

	C_BasePlayer * nearest_player = nullptr;
	float bestFoV = 0;
	Vector bestHead2D;

	for (int i = 1; i <= pGlobalVars->maxClients; i++) //0 is always the world entity
	{
		C_BasePlayer *player = (C_BasePlayer*)pEntityList->GetClientEntity(i);

		if (!IsPlayerValid(player))
			continue;

		Vector headPos3D = player->GetBonePosition((int)Bone::BONE_HEAD), headPos2D;

		if (!Draw::WorldToScreen(headPos3D, headPos2D))
			continue;

		float FoV2D = crosshair.DistTo(headPos2D);

		if (!nearest_player || FoV2D < bestFoV)
		{
			nearest_player = player;
			bestFoV = FoV2D;
			bestHead2D = headPos2D;
		}
	}
	
	if (nearest_player) //use pointers and avoid double calling of GetClientEntity
	{
		int minX = cx - (w / 10), maxX = cx + (w / 10);

		if (bestHead2D.x < minX || bestHead2D.x > maxX)
			return;

		int totalWidth = maxX - minX;

		int playerX = bestHead2D.x - minX;

		int yawCorrection = -(((playerX * 360) / totalWidth) - 180);

		float_t new_yaw = yawCorrection;

		Math::ClampYaw(new_yaw);

		nearest_player->GetEyeAngles()->y += new_yaw;
	}
}

void Resolver::FrameStageNotify(ClientFrameStage_t stage) {
	if (!Settings::Resolver::enabled || !pEngine->IsInGame())
		return;

	C_BasePlayer* me = (C_BasePlayer*)pEntityList->GetClientEntity(pEngine->GetLocalPlayer());
	if (!me || !me->GetAlive())
		return;

	if (stage == ClientFrameStage_t::FRAME_NET_UPDATE_POSTDATAUPDATE_START) {
		for (int i = 1; i < pEngine->GetMaxClients(); ++i) {
			C_BasePlayer* target = (C_BasePlayer*)pEntityList->GetClientEntity(i);

			if (!target
				|| target == me
				|| target->GetDormant()
				|| !target->GetAlive()
				|| target->GetImmune()
				|| target->GetTeam() == me->GetTeam())
				continue;

			IEngineClient::player_info_t entityInformation;
			pEngine->GetPlayerInfo(i, &entityInformation);

			if (std::find(Resolver::playerAngleLogs.begin(), Resolver::playerAngleLogs.end(),
				entityInformation.xuid) == Resolver::playerAngleLogs.end()) {
				player_data.push_back(std::pair<C_BasePlayer*, Vector>(target, *target->GetEyeAngles()));
			}

			if (shotATT) {
				Shotsmissed++;
				shotATT = false;
			}

			C_BasePlayer* aimbotTarget = (C_BasePlayer*)pEntityList->GetClientEntity(Aimbot::targetAimbot);

			if (Aimbot::useAbTarget)
				Hug(aimbotTarget);
			else
				Hug(target);
		}
	}
	else if (stage == ClientFrameStage_t::FRAME_RENDER_END) {
		for (unsigned long i = 0; i < player_data.size(); i++) {
			std::pair<C_BasePlayer*, Vector> player_aa_data = player_data[i];
			*player_aa_data.first->GetEyeAngles() = player_aa_data.second;
		}

		player_data.clear();
	}
}

void Resolver::PostFrameStageNotify(ClientFrameStage_t stage) {
	if (!Settings::Resolver::enabled)
		return;
}

CTickRecord Resolver::GetShotRecord(C_BasePlayer* player) {
	for (auto cur : m_arrInfos[player->GetIndex()].m_sRecords) {
		if (cur.validtick)
			return CTickRecord(cur);
	}
	return CTickRecord();
}

bool& Resolver::LowerBodyYawChanged(C_BasePlayer* player) {
	return m_arrInfos.at(player->GetIndex()).m_bLowerBodyYawChanged;
}

void Resolver::StoreVars(C_BasePlayer* player) {
	if (m_arrInfos.at(player->GetIndex()).m_sRecords.size() >= Settings::Resolver::ticks) {
		m_arrInfos.at(player->GetIndex()).m_sRecords.pop_back();
	}
	m_arrInfos.at(player->GetIndex()).m_sRecords.push_front(CTickRecord(player));
}

void Resolver::StoreVars(C_BasePlayer* player, Vector ang, float lby, float simtime, float tick) {
	if (m_arrInfos.at(player->GetIndex()).m_sRecords.size() >= Settings::Resolver::ticks)
		m_arrInfos.at(player->GetIndex()).m_sRecords.pop_back();
	m_arrInfos.at(player->GetIndex()).m_sRecords.push_front(CTickRecord(player));
}


bool& Resolver::BacktrackThisTick(C_BasePlayer* player) {
	return m_arrInfos.at(player->GetIndex()).m_bBacktrackThisTick;
}

bool Resolver::HasStaticRealAngle(const std::deque<CTickRecord>& l, float tolerance) {
	auto minmax = std::minmax_element(std::begin(l), std::end(l),
		[](const CTickRecord& t1, const CTickRecord& t2) {
		return t1.m_flLowerBodyYawTarget < t2.m_flLowerBodyYawTarget;
	});
	return (fabs(minmax.first->m_flLowerBodyYawTarget - minmax.second->m_flLowerBodyYawTarget) <= tolerance);
}

bool Resolver::HasStaticRealAngle(int index, float tolerance) {
	return HasStaticRealAngle(m_arrInfos[index].m_sRecords, tolerance);
}

bool Resolver::HasStaticYawDifference(const std::deque<CTickRecord>& l, float tolerance) {
	for (auto i = l.begin(); i < l.end() - 1; ) {
		if (GetDelta(LBYDelta(*i), LBYDelta(*++i)) > tolerance)
			return false;
	}
	return true;
}

bool Resolver::HasSteadyDifference(const std::deque<CTickRecord>& l, float tolerance) {
	size_t misses = 0;
	for (size_t i = 0; i < l.size() - 1; i++) {
		float tickdif = static_cast<float> (l.at(i).m_flSimulationTime - l.at(i + 1).tickcount);
		float lbydif = GetDelta(l.at(i).m_flLowerBodyYawTarget, l.at(i + 1).m_flLowerBodyYawTarget);
		float ntickdif = static_cast<float> (pGlobalVars->tickcount - l.at(i).tickcount);
		if (((lbydif / tickdif) * ntickdif) > tolerance) misses++;
	}
	return (misses <= (l.size() / 3));
}

int Resolver::GetDifferentDeltas(const std::deque<CTickRecord>& l, float tolerance) {
	std::vector<float> vec;
	for (auto var : l) {
		float curdelta = LBYDelta(var);
		bool add = true;
		for (auto fl : vec) {
			if (!IsDifferent(curdelta, fl, tolerance))
				add = false;
		}
		if (add)
			vec.push_back(curdelta);
	}
	return vec.size();
}

int Resolver::GetDifferentLBYs(const std::deque<CTickRecord>& l, float tolerance) {
	std::vector<float> vec;
	for (auto var : l) {
		float curyaw = var.m_flLowerBodyYawTarget;
		bool add = true;
		for (auto fl : vec) {
			if (!IsDifferent(curyaw, fl, tolerance))
				add = false;
		}
		if (add)
			vec.push_back(curyaw);
	}
	return vec.size();
}

bool Resolver::DeltaKeepsChanging(const std::deque<CTickRecord>& cur, float tolerance) {
	return (GetDifferentDeltas(cur) > (int)cur.size() / 2);
}

bool Resolver::LBYKeepsChanging(const std::deque<CTickRecord>& cur, float tolerance) {
	return (GetDifferentLBYs(cur, tolerance) > (int)cur.size() / 2);
}

float Resolver::GetLBYByComparingTicks(const std::deque<CTickRecord>& l) {
	int modulo = Settings::Resolver::modulo;
	pCvar->ConsoleColorPrintf(ColorRGBA(255, 255, 255), "\nmodulo " + modulo);
	int difangles = GetDifferentLBYs(l);
	int inc = modulo * difangles;
	for (auto var : l) {
		for (int lasttick = var.tickcount; lasttick < pGlobalVars->tickcount; lasttick += inc) {
			if (lasttick == pGlobalVars->tickcount)
				return var.m_flLowerBodyYawTarget;
		}
	}
	return 0.f;
}

float Resolver::GetDeltaByComparingTicks(const std::deque<CTickRecord>& l) {
	int modulo = Settings::Resolver::modulo;
	int difangles = GetDifferentDeltas(l);
	int inc = modulo * difangles;
	for (auto var : l) {
		for (int lasttick = var.tickcount; lasttick < pGlobalVars->tickcount; lasttick += inc) {
			if (lasttick == pGlobalVars->tickcount)
				return LBYDelta(var);
		}
	}
	return 0.f;
}

void Resolver::FireGameEvent(IGameEvent* event) {
	if (!event)
		return;

	C_BasePlayer* localplayer = (C_BasePlayer*)pEntityList->GetClientEntity(pEngine->GetLocalPlayer());

	if (!localplayer || !localplayer->GetAlive())
		return;

	if (strcmp(event->GetName(), XorStr("player_hurt")) == 0) {
		int hurt_player_id = event->GetInt(XorStr("userid"));
		int attacker_id = event->GetInt(XorStr("attacker"));

		if (pEngine->GetPlayerForUserID(hurt_player_id) == pEngine->GetLocalPlayer())
			return;

		if (pEngine->GetPlayerForUserID(attacker_id) != pEngine->GetLocalPlayer())
			return;

		C_BasePlayer* hurt_player = (C_BasePlayer*)pEntityList->GetClientEntity(
			pEngine->GetPlayerForUserID(hurt_player_id));
		if (!hurt_player)
			return;

		shotATT = false;

		IEngineClient::player_info_t localPlayerInfo;
		pEngine->GetPlayerInfo(localplayer->GetIndex(), &localPlayerInfo);

		IEngineClient::player_info_t hurtPlayerInfo;
		pEngine->GetPlayerInfo(hurt_player->GetIndex(), &hurtPlayerInfo);

		didDmg = true;

		Shotsmissed = 0;

		Resolver::shotsMiss[Aimbot::targetAimbot] = 0;
	}
	else {
		C_BaseCombatWeapon* activeWeapon = (C_BaseCombatWeapon*)pEntityList->GetClientEntityFromHandle(
			localplayer->GetActiveWeapon());

		if (!activeWeapon || activeWeapon->GetAmmo() == 0)
			return;

		int ammo = activeWeapon->GetAmmo();
		static int ammoSave = ammo;

		if (ammoSave != ammo) {
			Resolver::shotsMiss[Aimbot::targetAimbot]++;
			ammoSave = ammo;
		}
	}

	if (strcmp(event->GetName(), "player_connect_full") != 0 &&
		strcmp(event->GetName(), "cs_game_disconnected") != 0)
		return;

	if (event->GetInt("userid") &&
		pEngine->GetPlayerForUserID(event->GetInt("userid")) != pEngine->GetLocalPlayer())
		return;

	Resolver::playerAngleLogs.clear();
}

bool Resolver::IsEntityMoving(C_BasePlayer *player) {
	return (player->GetVelocity().Length2D() > 0.1f && player->GetFlags() & FL_ONGROUND);
}

int GetSequenceActivity(int GetSequence)
{
	auto hdr = pModelInfo->GetStudioModel(G::LocalPlayer->GetModel());

	if (!hdr)
		return -1;

	// sig for stuidohdr_t version: 53 56 8B F1 8B DA 85 F6 74 55
	// sig for C_BaseAnimating version: 55 8B EC 83 7D 08 FF 56 8B F1 74 3D
	// c_csplayer vfunc 242, follow calls to find the function.

	static auto get_sequence_activity = reinterpret_cast<int(__fastcall*)(studiohdr_t*, int)>(offsets.CBaseAnimating.m_nSequence);


	return get_sequence_activity(hdr, G::LocalPlayer->GetSequence());
}


bool Resolver::IsAdjustingBalance(C_BasePlayer *player, CTickRecord &record, AnimationLayer *layer)
{
	for (int i = 0; i < record.m_iLayerCount; i++)
	{
		const int activity = GetSequenceActivity(record.animationLayer[i].m_nSequence);
		if (activity == 979)
		{
			*layer = record.animationLayer[i];
			return true;
		}
	}
	return false;
}

void Resolver::CreateMove(CUserCmd* cmd) {

	if (!Settings::Resolver::enabled)
		return;

	for (int x = 1; x < pEngine->GetMaxClients(); ++x) {
		C_BasePlayer* target = (C_BasePlayer*)pEntityList->GetClientEntity(x);

		//if (!target
		//	|| target == pEntityList->GetClientEntity(pEngine->GetLocalPlayer())
		//	|| target->GetDormant()
		//	|| !target->GetAlive()
		//	|| target->GetImmune()
		////	|| target->GetTeam() == pEntityList->GetClientEntity(pEngine->GetLocalPlayer())->GetTeam())
		//	continue;

		Resolver::StoreVars(target);

		if (Settings::Resolver::LagComp)
			StartLagComp(target, cmd);

	}
}