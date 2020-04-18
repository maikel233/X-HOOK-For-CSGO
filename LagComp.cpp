#include "LagComp.h"
#include "Features.h"
 
//using namespace SDK;
 
bool Settings::Aimbot::LegitBackTrack = false;
//
//template<class T, class U>
//inline T clamp(T in, U low, U high)
//{
//	if (in <= low)
//		return low;
//	else if (in >= high)
//		return high;
//	else
//		return in;
//}
//
//
//void BackTracks::Update(int tick_count)
//{
//	latest_tick = tick_count;
//	for (int i = 0; i < 64; i++)
//	{
//		UpdateRecord(i);
//	}
//}
//
//ConVar* minupdate;
//ConVar* maxupdate;
//ConVar * updaterate;
//ConVar * interprate;
//ConVar* cmin;
//ConVar* cmax;
//ConVar* interp;
//
//float BackTracks::GetLerpTime()
//{
//	if (!minupdate)
//		minupdate = pCvar->FindVar(XorStr("sv_minupdaterate"));
//	if (!maxupdate)
//		maxupdate = pCvar->FindVar(XorStr("sv_maxupdaterate"));
//	if (!updaterate)
//		updaterate = pCvar->FindVar(XorStr("cl_updaterate"));
//	if (!interprate)
//		interprate = pCvar->FindVar(XorStr("cl_interp_ratio"));
//	if (!cmin)
//		cmin = pCvar->FindVar(XorStr("sv_client_min_interp_ratio"));
//	if (!cmax)
//		cmax = pCvar->FindVar(XorStr("sv_client_max_interp_ratio"));
//	if (!interp)
//		interp = pCvar->FindVar(XorStr("cl_interp"));
//
//	float UpdateRate = updaterate->GetFloat();
//	float LerpRatio = interprate->GetFloat();
//
//	return max(LerpRatio / UpdateRate, interp->GetFloat());
//}
// 
//
//int GetLatencyTicks()
//{
//	double v0;
//	INetChannelInfo* v1;
//	INetChannelInfo* v2;
//	float v3;
//	float v4;
//
//	v1 = (INetChannelInfo*)pEngine->GetNetChannelInfo();
//	v2 = (INetChannelInfo*)pEngine->GetNetChannelInfo();
//
//	v3 = v1->GetAvgLatency(1); // localplayer
//	v4 = v2->GetAvgLatency(0); // Generic prop
//
//	float interval_per_tick = 1.0f / pGlobalVars->interval_per_tick;
//
//	return floorf(((v3 + v4) * interval_per_tick) + 0.5f);
//}
//
//
//bool BackTracks::IsTickValid(int tick)
//{
//	/*int delta = latest_tick - tick;
//	float deltaTime = delta * pGlobalVars->interval_per_tick;
//	return (fabs(deltaTime) <= 0.2f);*/
//
//
//	// better use polak's version than our old one, getting more accurate results
//
//	INetChannelInfo *nci = pEngine->GetNetChannelInfo();
//
//	if (!nci)
//		return false;
//
//	float correct = clamp(nci->GetLatency(FLOW_OUTGOING) + GetLerpTime(), 0.f, 1.f/*sv_maxunlag*/);
//
//	float deltaTime = correct - (pGlobalVars->curtime - TICKS_TO_TIME(tick));
//
//	return fabsf(deltaTime) < 0.2f;
//}
// 
//void BackTracks::UpdateRecord(int i)
//{
//	C_BasePlayer* pEntity = (C_BasePlayer*)pEntityList->GetClientEntity(i);
//	if (pEntity && pEntity->GetAlive() && !pEntity->GetDormant())
//	{
//		float lby = *pEntity->GetLowerBodyYawTarget();
//		if (lby != records[i].lby)
//		{
//			records[i].tick_count = latest_tick;
//			records[i].lby = lby;
//			records[i].headPosition = pEntity->GetBonePosition((int)Bone::BONE_HEAD);
//		}
//	}
//	else
//	{
//		records[i].tick_count = 0;
//	}
//}
// 
//bool BackTracks::RunLBYBackTrack(int i, CUserCmd* cmd, Vector& aimPoint)
//{
//	if (IsTickValid(records[i].tick_count))
//	{
//		aimPoint = records[i].headPosition;
//		cmd->tick_count = records[i].tick_count;
//		return true;
//	}
//	return false;
//}
//
//
//
// 
//void BackTracks::legitBackTrack(CUserCmd* cmd)
//{
//
//	if (!Settings::Aimbot::LegitBackTrack)
//		return;
//
//	int bestTargetIndex = -1;
//	float bestFov = FLT_MAX;
//	C_BasePlayer* pLocal = (C_BasePlayer*)pEntityList->GetClientEntity(pEngine->GetLocalPlayer());
//	IEngineClient::player_info_t info;
//	if (!pLocal->GetAlive())
//		return;
//
//	for (int i = 1; i < pEngine->GetMaxClients(); i++)
//	{
//		auto entity = (C_BasePlayer*)pEntityList->GetClientEntity(i);
//
//		if (!entity || !pLocal)
//			continue;
//
//		if (entity == pLocal)
//			continue;
//
//		if (!pEngine->GetPlayerInfo(i, &info))
//			continue;
//
//		if (entity->GetDormant())
//			continue;
//
//		if (entity->GetTeam() == pLocal->GetTeam())
//			continue;
//
//		if (entity->GetAlive())
//		{
//
//			/*			float simtime = entity->GetSimulationTime();
//						Vector hitboxPos = entity->GetBonePosition((int)Bone::BONE_HEAD);
//
//						headPositions[i][cmd->command_number % 12] = backtrackData{ simtime, hitboxPos };
//						Vector ViewDir = angle_vector(cmd->viewangles + (*pLocal->GetAimPunchAngle() * 2.f));
//						float FOVDistance = distance_point_to_line(hitboxPos, pLocal->GetEyePosition(), ViewDir);
//
//						if (bestFov > FOVDistance)
//						{
//							bestFov = FOVDistance;
//							bestTargetIndex = i;
//						}
//					}
//				}
//
//				float bestTargetSimTime;
//				if (bestTargetIndex != -1)
//				{
//					float tempFloat = FLT_MAX;
//					Vector ViewDir = angle_vector(cmd->viewangles + (*pLocal->GetAimPunchAngle() * 2.f));
//					Vector local_position = pLocal->GetVecOrigin() + pLocal->GetVecViewOffset();
//
//					for (int t = 0; t < 13; ++t)
//					{
//						float tempFOVDistance = distance_point_to_line(headPositions[bestTargetIndex][t].hitboxPos, pLocal->GetEyePosition(), ViewDir);
//						if (tempFloat > tempFOVDistance && headPositions[bestTargetIndex][t].simtime > pLocal->GetSimulationTime() - 1)
//						{
//							tempFloat = tempFOVDistance;
//							bestTargetSimTime = headPositions[bestTargetIndex][t].simtime;
//						}
//					}*/
//
//			Vector viewAngles;
//			pEngine->GetViewAngles(viewAngles);
//			// FOV Style Closest
//			float tempFov = Settings::Aimbot::AutoAim::fov;
//			Vector bestSpot = { 0, 0, 0 };
//			Vector pVecTarget = pLocal->GetEyePosition();
//			for (unsigned int i = 1; i < Backtracking::lagRecords[entity->GetIndex()].size(); i++) {
//				float cbFov = Math::GetFov(viewAngles, Math::CalcAngle(pVecTarget,
//					Backtracking::lagRecords[entity->GetIndex()][i].headPos));
//				if (cbFov < tempFov) {
//					tempFov = cbFov;
//					bestSpot = Backtracking::lagRecords[entity->GetIndex()][i].headPos;
//				}
//			}
//
//
//			if (cmd->buttons & IN_ATTACK)
//			{
//				cmd->tick_count = TIME_TO_TICKS(Backtracking::lagRecords[entity->GetIndex()][i].m_flSimulationTime);
//			}
//			//	}
//		}
//	}
//}
//
//static Vector GetClosestBacktrack(CUserCmd* cmd, C_BasePlayer* localPlayer, C_BasePlayer* enemy, int* tickDiff) {
//	Vector viewAngles;
//	pEngine->GetViewAngles(viewAngles);
//	// FOV Style Closest
//	float tempFov = Settings::Aimbot::AutoAim::fov;
//	Vector bestSpot = { 0, 0, 0 };
//	Vector pVecTarget = localPlayer->GetEyePosition();
//	for (unsigned int i = 1; i < Backtracking::lagRecords[enemy->GetIndex()].size(); i++) {
//		float cbFov = Math::GetFov(viewAngles, Math::CalcAngle(pVecTarget,
//			Backtracking::lagRecords[enemy->GetIndex()][i].headPos));
//		if (cbFov < tempFov) {
//			tempFov = cbFov;
//			bestSpot = Backtracking::lagRecords[enemy->GetIndex()][i].headPos;
//			*tickDiff = i;
//		}
//	}
//	return bestSpot;
//}
// 
//BackTracks* backtracking = new BackTracks();
//backtrackData headPositions[64][12];