#include "Features.h"

void NadePrediction()
{
	if (!pEngine->IsInGame())
		return;

	C_BasePlayer* pLocal = (C_BasePlayer*)pEntityList->GetClientEntity(pEngine->GetLocalPlayer());
	if (!pLocal)
		return;

	C_BaseCombatWeapon* activeWeapon = (C_BaseCombatWeapon*)pEntityList->GetClientEntityFromHandle(pLocal->GetActiveWeapon());
	if (!activeWeapon)
		return;

	CSWeaponType weaponType = activeWeapon->GetCSWpnData()->GetWeaponType();
	if (weaponType != CSWeaponType::WEAPONTYPE_GRENADE)
		return;
	
	const float TIMEALIVE = 5.f;
	const float GRENADE_COEFFICIENT_OF_RESTITUTION = 0.4f;

	float fStep = 0.1f;
	float fGravity = 800.0f / 8.f;

	Vector vPos;
	Vector vStart;
	Vector vThrow;
	Vector vThrow2;
	int iCollisions = 0;

	Vector vViewAngles;
	pEngine->GetViewAngles(vViewAngles);

	vThrow[0] = vViewAngles[0];
	vThrow[1] = vViewAngles[1];
	vThrow[2] = vViewAngles[2];

	if (vThrow[0] < 0)
		vThrow[0] = -10 + vThrow[0] * ((90 - 10) / 90.0);
	else
		vThrow[0] = -10 + vThrow[0] * ((90 + 10) / 90.0);

	float fVel = (90 - vThrow[0]) * 4;
	if (fVel > 500)
		fVel = 500;

	Math::AngleVectors(vThrow, vThrow2);

	Vector vEye = pLocal->GetEyePosition();
	vStart[0] = vEye[0] + vThrow2[0] * 16;
	vStart[1] = vEye[1] + vThrow2[1] * 16;
	vStart[2] = vEye[2] + vThrow2[2] * 16;

	vThrow2[0] = (vThrow2[0] * fVel) + pLocal->GetVelocity()[0];
	vThrow2[1] = (vThrow2[1] * fVel) + pLocal->GetVelocity()[1];
	vThrow2[2] = (vThrow2[2] * fVel) + pLocal->GetVelocity()[2]; 

	for (float fTime = 0.0f; fTime < TIMEALIVE; fTime += fStep)
	{
		vPos = vStart + vThrow2 * fStep;

		Ray_t ray;
		trace_t tr;

		CTraceFilter loc;
		loc.pSkip = pLocal;

		ray.Init(vStart, vPos);
		pTrace->TraceRay(ray, MASK_SOLID, &loc, &tr);

		if (tr.DidHit()) 
		{
			/*vThrow2 = tr.plane.normal * -2.0f *Math::DotProduct2(vThrow2, tr.plane.normal) + vThrow2;
			vThrow2 *= GRENADE_COEFFICIENT_OF_RESTITUTION;

			iCollisions++;
			if (iCollisions > 2)
				break;

			vPos = vStart + vThrow2*tr.fraction*fStep;

			fTime += (fStep * (1 - tr.fraction));*/
		}
		
		Vector vOutStart, vOutEnd;

		if (!pDebugOverlay->ScreenPosition(vStart, vOutStart), !pDebugOverlay->ScreenPosition(vPos, vOutEnd))
		{
			pSurface->DrawSetColor(Color(0, 150, 255, 255));
			pSurface->DrawLine(vOutStart.x, vOutStart.y, vOutEnd.x, vOutEnd.y);
		}

		vStart = vPos;
		vThrow2.z -= fGravity*tr.fraction*fStep;
	}
}