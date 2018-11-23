//#include "antiaiminfos.h"
//
//bool Settings::AntiAimInfos::enabled = false;
//
//Vector fakeAngle = { 0, 0, 0 };
//Vector realAngle = { 0, 0 ,0 };
//float lbyAngle = 0;
//
//void AntiAimInfos::Paint()
//{
//	if (!Settings::AntiAimInfos::enabled || !pEngine->IsInGame())
//		return;
//
//}
//
//void AntiAimInfos::CreateMove(CUserCmd* cmd)
//{
//	if (!Settings::AntiAimInfos::enabled || !pEngine->IsInGame())
//		return;
//
//	C_BasePlayer* localPlayer = (C_BasePlayer*)pEntityList->GetClientEntity(pEngine->GetLocalPlayer());
//	if (!localPlayer || !localPlayer->GetAlive())
//		return;
//
//	if (!bSendPackett)
//		fakeAngle = cmd->viewangles;
//	else
//		realAngle = cmd->viewangles;
//
//	lbyAngle = *localPlayer->GetLowerBodyYawTarget();
//
//}