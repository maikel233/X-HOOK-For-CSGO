#include "../Features.h"
#include "../Hooks.h"
#include "../Utils/FindPattern.h"

typedef void(__thiscall* SetHost_t)(void*, void*);
float m_flOldCurtime;
float m_flOldFrametime;
int * m_random_seed = 0;

MoveData data;

void PredictionSystem::StartPrediction(CUserCmd* cmd) {


	if (!Settings::Aimbot::Prediction::enabled)
		return;

	C_BasePlayer* localplayer = (C_BasePlayer*)pEntityList->GetClientEntity(pEngine->GetLocalPlayer());
	
	if (m_random_seed == 0)
	{
		m_random_seed = *reinterpret_cast<int **>(FindPattern("client.dll", "A3 ? ? ? ? 66 0F 6E 86") + 1);
	}

	*m_random_seed = 0;

	m_flOldCurtime = pGlobalVars->curtime;
	m_flOldFrametime = pGlobalVars->frametime;

	pGlobalVars->curtime = pGlobalVars->serverTime();
	pGlobalVars->frametime = pGlobalVars->interval_per_tick;

	pGameMovement->StartTrackPredictionErrors(localplayer);
	pMoveHelper->SetHost(localplayer);
	memset(&data, 0, sizeof(MoveData));
	pPrediction->SetupMove(localplayer, cmd, pMoveHelper, &data);
	pGameMovement->ProcessMovement(localplayer, &data);
	pPrediction->FinishMove(localplayer, cmd, &data);

	*m_random_seed = 1;

	pGlobalVars->curtime = m_flOldCurtime;
	pGlobalVars->frametime = m_flOldFrametime;
}

void PredictionSystem::EndPrediction() {

	if (!Settings::Aimbot::Prediction::enabled)
		return;

	C_BasePlayer* localplayer = (C_BasePlayer*)pEntityList->GetClientEntity(pEngine->GetLocalPlayer());

	pGameMovement->FinishTrackPredictionErrors(localplayer);
	pMoveHelper->SetHost(0);

	*m_random_seed = -1;

	pGlobalVars->curtime = m_flOldCurtime;
	pGlobalVars->frametime = m_flOldFrametime;
}
