#include "Features.h"

std::map<int, std::deque<CTickRecord>> Backtracking::lagRecords;
float previousInterp = -1.0f;
int previousInterpolate = -1;
int previousLagCompensation = -1;
bool Backtracking::backtrackingLby = false;
std::deque< CIncomingSequence > sequences;
int m_last_incoming_sequence_number;
float lastSimTime[64];


template<class T, class U>
inline T clamp(T in, U low, U high)
{
	if (in <= low)
		return low;
	else if (in >= high)
		return high;
	else
		return in;
}

void Backtracking::RestorePosition(int playerIndex, int tickDiff) {
	return;
}

/* Used for a Queue size-limit. Too lazy to write a wrapper class fam */
static inline void PushLagRecord(int index, C_BasePlayer* player = NULL) {
	if (player) {
		Backtracking::lagRecords[index].push_back(CTickRecord(player));
	}
	else {
		Backtracking::lagRecords[index].push_back(CTickRecord());
	}

	if (Backtracking::lagRecords[index].size() > Backtracking::MAX_QUEUE_SIZE) {
		Backtracking::lagRecords[index].pop_front();
	}
}

/* Keep track of players */
void Backtracking::FrameStageNotify(ClientFrameStage_t stage) {

	if (!Settings::Aimbot::backtrack)
		return;

	if (!pEngine->IsInGame()) {
		Backtracking::lagRecords.clear(); // empty records.
		return;
	}

	if (stage == ClientFrameStage_t::FRAME_NET_UPDATE_POSTDATAUPDATE_START) {
		C_BasePlayer* me = (C_BasePlayer*)pEntityList->GetClientEntity(pEngine->GetLocalPlayer());
		if (!me || !me->GetAlive())
			return;
		for (int i = 1; i < pEngine->GetMaxClients(); ++i) {
			C_BasePlayer* target = (C_BasePlayer*)pEntityList->GetClientEntity(i);
			if (!target) {
				Backtracking::lagRecords.erase(i);
				continue;
			}
			if (target == me || (target->GetTeam() == me->GetTeam()))
				continue;
			if (target->GetDormant() || !target->GetAlive() || target->GetImmune()) {
				PushLagRecord(i, NULL);
				//push record with blank coords
			}
			if (Settings::Resolver::LagComp && Settings::Resolver::enabled) {
				INetChannelInfo *nci = pEngine->GetNetChannelInfo();

				float curTime = pGlobalVars->curtime;

				float outgoingPing = nci->GetLatency(FLOW_OUTGOING);

				static float lastUpdate = 0.f;

				if (Resolver::lbyUpdated) {
					PushLagRecord(i, target);
					lastUpdate = curTime + 0.2f + outgoingPing;
					Backtracking::backtrackingLby = true;
				}
				else if (curTime > lastUpdate) {
					Backtracking::backtrackingLby = false;
				}
			}
			else {
				PushLagRecord(i, target);
			}
		}
	}
}

void Backtracking::ToggleRequiredCVars(bool activate) {
	ConVar* interp = pCvar->FindVar(XorStr("cl_interp"));
	ConVar* interpolate = pCvar->FindVar(XorStr("cl_interpolate"));
	ConVar* lagCompensation = pCvar->FindVar(XorStr("cl_lagcompensation"));

	if (activate) {
		if (interp->GetFloat() != 0.0f) {
			previousInterp = interp->GetFloat();
			interp->SetValue(0.0f);
		}

		if (interpolate->GetInt() != 0) {
			previousInterpolate = interpolate->GetInt();
			interpolate->SetValue(0);
		}

		if (lagCompensation->GetInt() != 1) {
			previousLagCompensation = lagCompensation->GetInt();
			lagCompensation->SetValue(1);
		}
	}
	else {
		if (previousInterp != -1.0f) {
			interp->SetValue(previousInterp);
		}

		if (previousInterpolate != -1) {
			interpolate->SetValue(previousInterpolate);
		}

		if (previousLagCompensation != -1) {
			lagCompensation->SetValue(previousLagCompensation);
		}
	}
}



//void Backtracking::add_latency(INetChannelInfo * net_channel) {
//	for (auto& seq : sequences) {
//		if (pGlobalVars->realtime - seq.m_current_time >= Settings::Aimbot::FakeLatencyMode / 1000.f) {
//			net_channel->m_nInReliableState = seq.m_in_reliable_state;
//			net_channel->m_nInSequenceNr = seq.m_sequence_nr;
//
//			break;
//		}
//	}
//}


//Pasta time

bool Backtracking::IsTickValid(int tick)
{
	// better use polak's version than our old one, getting more accurate results

	INetChannelInfo *nci = pEngine->GetNetChannelInfo();

	if (!nci)
		return false;

	float sv_maxunlag = pCvar->FindVar("sv_maxunlag")->GetFloat();

	float correct = clamp(nci->GetLatency(FLOW_OUTGOING) + GetLerpTime(), 0.f, sv_maxunlag/*1.f*//*sv_maxunlag*/);

	float deltaTime = correct - (pGlobalVars->curtime - TICKS_TO_TIME(tick));

	return fabsf(deltaTime) < 0.2f;
}
//std::map<int, std::deque<CTickRecord>>

// calcs dmg for all records
void FindBest(C_BasePlayer* target, std::deque<CTickRecord>::iterator record, int i) {
	//std::deque<CTickRecord>
	float bestDmg = 0.f;
	Vector bestAimSpot = Vector(0, 0, 0);

	C_BasePlayer* Localplayer = (C_BasePlayer*)pEntityList->GetClientEntity(pEngine->GetLocalPlayer());
	
	if (!Localplayer->GetAlive())
		return;

	Vector viewAngles;
	pEngine->GetViewAngles(viewAngles);
	Vector pVecTarget = Localplayer->GetEyePosition();
	float tempFov = Settings::Aimbot::AutoAim::fov;
	float cbFov = Math::GetFov(viewAngles, Math::CalcAngle(pVecTarget,
		Backtracking::lagRecords[target->GetIndex()][i].headPos));

	pCvar->ConsoleColorPrintf(ColorRGBA(255, 255, 255), XorStr("Reached end of our BT state."));

	
	if (record->calcPos != Localplayer->GetEyePosition()) {
	target->GetCollideable()->OBBMins() = record->mins;
	target->GetCollideable()->OBBMaxs() = record->maxs;
	target->setAbsAngle(Vector(0, record->m_angEyeAngles.y, 0)); 
	target->setAbsOriginal(record->absOrigin);
	target->GetFlags2() = record->flags;
	}

	if (!record->matrixBuilt)
	{
	if (!target->SetupBones(record->matrix, 128, 256, record->m_flSimulationTime))
	return;
	record->matrixBuilt = true;
	}



	if ( cbFov < tempFov ) {
	pCvar->ConsoleColorPrintf(ColorRGBA(255, 0, 0), XorStr("Target is in our FOV...\n"));
	record->calcDmg = bestDmg;
	record->calcBestPos = target->GetBonePositionBacktrack(int(Bone::BONE_HEAD), record->matrix);
	record->calcPos = Localplayer->GetEyePosition();
	}

	


}

void Backtracking::Createmove(CUserCmd* cmd)
{

	float bestDmg = 0.f;
	Vector bestAimSpot = Vector(0, 0, 0);
	if (!pEngine->IsInGame() || !Settings::Aimbot::backtrack) {
		Backtracking::lagRecords.clear();
		return;
	}

	C_BasePlayer* Localplayer = (C_BasePlayer*)pEntityList->GetClientEntity(pEngine->GetLocalPlayer());
	IEngineClient::player_info_t info;
	if (!Localplayer->GetAlive())
		return;

	for (int i = 1; i < pEngine->GetMaxClients(); ++i) {
		C_BasePlayer* target = (C_BasePlayer*)pEntityList->GetClientEntity(i);

		if (!target
			|| target == Localplayer
			|| target->GetDormant()
			|| !target->GetAlive()
			|| target->GetImmune()
			|| lastSimTime[target->GetIndex()] == target->GetSimulationTime()
			|| target->GetTeam() == Localplayer->GetTeam())
			continue;



		lastSimTime[target->GetIndex()] = target->GetSimulationTime(); // dont wanna record fakelag records/position


		//if (Backtracking::lagRecords[target->GetIndex()][i].calcPos != Localplayer->GetEyePosition()) {
		//	target->GetCollideable()->OBBMins() = Backtracking::lagRecords[target->GetIndex()][i].mins;
		//	target->GetCollideable()->OBBMaxs() = Backtracking::lagRecords[target->GetIndex()][i].maxs;
		//	target->setAbsAngle(Vector(0, Backtracking::lagRecords[target->GetIndex()][i].m_angEyeAngles.y, 0));
		//	target->setAbsOriginal(Backtracking::lagRecords[target->GetIndex()][i].absOrigin);
		//	target->GetFlags2() = Backtracking::lagRecords[target->GetIndex()][i].flags;
		//}

		//if (!Backtracking::lagRecords[target->GetIndex()][i].matrixBuilt)
		//{
		//	if (!target->SetupBones(Backtracking::lagRecords[target->GetIndex()][i].matrix, 128, 256, Backtracking::lagRecords[target->GetIndex()][i].m_flSimulationTime))
		//		return;
		//	Backtracking::lagRecords[target->GetIndex()][i].matrixBuilt = true;
		//}

		//if (!Backtracking::lagRecords[target->GetIndex()][i].matrixBuilt)
		//{
		//	if (!target->SetupBones(Backtracking::lagRecords[target->GetIndex()][i].matrix, 128, 256, Backtracking::lagRecords[target->GetIndex()][i].m_flSimulationTime))
		//		return;
		//	Backtracking::lagRecords[target->GetIndex()][i].matrixBuilt = true;
		//}

		float bestTargetSimTime;
		Vector viewAngles;
		pEngine->GetViewAngles(viewAngles);
		// FOV Style Closest
		float tempFov = Settings::Aimbot::AutoAim::fov;
		Vector bestSpot = { 0, 0, 0 };
		Vector pVecTarget = Localplayer->GetEyePosition();
		for (unsigned int i = 1; i < Backtracking::lagRecords[target->GetIndex()].size(); i++) {
			float cbFov = Math::GetFov(viewAngles, Math::CalcAngle(pVecTarget,
				Backtracking::lagRecords[target->GetIndex()][i].headPos));
			if (cbFov < tempFov) {
				tempFov = cbFov;
				bestSpot = Backtracking::lagRecords[target->GetIndex()][i].headPos;


		//		Backtracking::lagRecords[target->GetIndex()][i].calcDmg = bestDmg;
		//		Backtracking::lagRecords[target->GetIndex()][i].calcBestPos = target->GetBonePositionBacktrack(int(Bone::BONE_HEAD), Backtracking::lagRecords[target->GetIndex()][i].matrix);
		//		Backtracking::lagRecords[target->GetIndex()][i].calcPos = Localplayer->GetEyePosition();
				bestTargetSimTime = Backtracking::lagRecords[target->GetIndex()][i].m_flSimulationTime;


			}

			if (cmd->buttons & IN_ATTACK)
			{
				cmd->tick_count = TIME_TO_TICKS(bestTargetSimTime);
			}

		}
	}
}

