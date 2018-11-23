#include "Features.h"

std::map<int, std::deque<CTickRecord>> Backtracking::lagRecords;
float previousInterp = -1.0f;
int previousInterpolate = -1;
int previousLagCompensation = -1;
bool Backtracking::backtrackingLby = false;
std::deque< CIncomingSequence > sequences;
int m_last_incoming_sequence_number;

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