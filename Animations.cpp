//#include "Features.h"
//#include "..\Math.h"
//
//
//void CAnimations::UpdateServerAnimations(CUserCmd* cmd, bool* bsendpacket)
//{
//	C_BasePlayer* m_pLocalPlayer = (C_BasePlayer*)pEntityList->GetClientEntity(pEngine->GetLocalPlayer());
//	if (!m_pLocalPlayer)
//		return;
//
//	bool allocate = (m_serverAnimState == nullptr);
//	bool change = (!allocate) && (m_pLocalPlayer->GetRefEHandle != m_ulEntHandle);
//	bool reset = (!allocate && !change) && (m_pLocalPlayer->GetSpawnTime() != m_flSpawnTime);
//
//	if (change)
//	{
//		g_pMemAlloc->Free(m_serverAnimState);
//	}
//
//	if (reset)
//	{
//		ResetAnimationState(m_serverAnimState);
//		m_flSpawnTime = m_pLocalPlayer->GetSpawnTime();
//	}
//
//	if (allocate || change)
//	{
//		auto state = reinterpret_cast< CCSGOPlayerAnimState* > (g_pMemAlloc->Alloc(0x344));
//
//		if (state != nullptr)
//			CreateAnimationState(state, m_pLocalPlayer);
//
//		m_ulEntHandle() = m_pLocalPlayer->GetRefEHandle();
//		m_flSpawnTime = m_pLocalPlayer->GetSpawnTime();
//
//		m_serverAnimState = state;
//	}
//
//	else if (!pClientState->chokedcommands)
//	{
//		m_angRealAngle = cmd->viewangles.Clamp();
//
//		UpdateAnimationState(m_serverAnimState, m_angRealAngle);
//
//		float delta = std::abs(m_angRealAngle.y - localplayer->GetLowerBodyYawTarget);
//
//		if (m_serverAnimState->m_flSpeed > 0.1f)
//		{
//			m_flNextBodyUpdate = pGlobalVars->curtime + 0.22f;
//			m_flFirstBodyUpdate = m_flNextBodyUpdate;
//			F::AntiAim->bUpdateLby = true;
//
//		}
//		else if (pGlobalVars->curtime > m_flNextBodyUpdate)
//		{
//			if (m_flFirstBodyUpdate != m_flNextBodyUpdate)
//				F::AntiAim->bUpdateLby = true;
//			m_flNextBodyUpdate = pGlobalVars->curtime + 1.1f;
//		}
//	}
//}
//
//void CAnimations::CreateAnimationState(CCSGOPlayerAnimState* state, C_BasePlayer* player)
//{
//	using CreateAnimState_t = void(__thiscall*)(CCSGOPlayerAnimState*, C_BasePlayer*);
//
//	static auto CreateAnimState = (CreateAnimState_t)U::FindSig("client.dll", "55 8B EC 56 8B F1 B9 ? ? ? ? C7 46");
//	if (!CreateAnimState)
//		return;
//
//	CreateAnimState(state, player);
//}
//
//void CAnimations::UpdateAnimationState(CCSGOPlayerAnimState* state, QAngle ang)
//{
//	static auto UpdateAnimState = U::FindSig("client.dll", "55 8B EC 83 E4 F8 83 EC 18 56 57 8B F9 F3 0F 11 54 24");
//	if (!UpdateAnimState)
//		return;
//
//	__asm {
//		mov ecx, state
//
//		movss xmm1, dword ptr[ang + 4]
//		movss xmm2, dword ptr[ang]
//
//		call UpdateAnimState
//	}
//}
//
//void CAnimations::ResetAnimationState(CCSGOPlayerAnimState* state)
//{
//	using ResetAnimState_t = void(__thiscall*)(CCSGOPlayerAnimState*);
//
//	static auto ResetAnimState = (ResetAnimState_t)U::FindSig("client.dll", "56 6A 01 68 ? ? ? ? 8B F1");
//
//	if (!ResetAnimState)
//		return;
//
//	ResetAnimState(state);
//} //crashes here?