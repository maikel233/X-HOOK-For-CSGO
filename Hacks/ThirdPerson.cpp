#include "../Features.h"

bool Settings::ThirdPerson::enabled = false;
ThirdPersonMode Settings::ThirdPerson::mode = ThirdPersonMode::FAKE;
float Settings::ThirdPerson::distance = 30.0f;
ButtonCode_t Settings::ThirdPerson::key = ButtonCode_t::KEY_DELETE;
float Settings::ThirdPerson::transparency = 0.3f;


IMaterial* transparentPlayerMaterial = nullptr;
long millisSinceLastPress = 0;

// Credit to LWSS for making the initial non-bytepatching TP in Fuzion
// I just fixed it so it can be disabled and that it shows fake angles
void ThirdPerson::OverrideView(ViewSetup* setup) {

	if (!Settings::ThirdPerson::enabled)
		return;

	long millis = Util::GetEpochTime();

	if (millis - millisSinceLastPress >= 1000) {
		if (pInputSystem->IsButtonDown(Settings::ThirdPerson::key)) {
			Settings::ThirdPerson::enabled = !Settings::ThirdPerson::enabled;
			millisSinceLastPress = Util::GetEpochTime();
		}
	}

	if (!Settings::ThirdPerson::enabled) {
		pInput->m_fCameraInThirdPerson = false;
		pInput->m_vecCameraOffset.z = 150.0f;
		return;
	}

	C_BasePlayer* pLocal = (C_BasePlayer*)pEntityList->GetClientEntity(pEngine->GetLocalPlayer());
	if (!pLocal || !pLocal->GetAlive())
		return;

	Vector desiredOffset = Vector(cos(DEG2RAD(setup->y)) * Settings::ThirdPerson::distance,
		sin(DEG2RAD(setup->y)) * Settings::ThirdPerson::distance,
		sin(DEG2RAD(-setup->x)) * Settings::ThirdPerson::distance);

	Ray_t ray;
	ray.Init(pLocal->GetEyePosition(), (pLocal->GetEyePosition() - desiredOffset));

	CTraceFilter filter;
	filter.pSkip = pLocal;

	trace_t tr;
	pTrace->TraceRay(ray, MASK_SHOT, &filter, &tr);

	Vector difference = pLocal->GetEyePosition() - tr.endpos;
	float distance2D = sqrt(abs(difference.x * difference.x) + abs(difference.y * difference.y)); // Pythagorean
	bool horizontalOk = distance2D > (Settings::ThirdPerson::distance - 2.0f);
	bool verticalOk = (abs(difference.z) - abs(desiredOffset.z) < 3.0f);

	float cameraDistance;

	if (horizontalOk && verticalOk) {
		cameraDistance = Settings::ThirdPerson::distance;
	}
	else if (verticalOk) {
		cameraDistance = distance2D * 0.95f;
	}
	else {
		cameraDistance = abs(difference.z) * 0.95f;
	}

	Vector origin; // Origin angles which we're seeing
	pEngine->GetViewAngles(origin);

	pInput->m_fCameraInThirdPerson = true;
	pInput->m_vecCameraOffset.x = origin.x;
	pInput->m_vecCameraOffset.y = origin.y;
	pInput->m_vecCameraOffset.z = cameraDistance;
}

void ThirdPerson::FrameStageNotify(ClientFrameStage_t stage) {

	if (!Settings::ThirdPerson::enabled)
		return;

	if (!pEngine->IsInGame())
		return;

	if (stage != ClientFrameStage_t::FRAME_RENDER_START)
		return;

	C_BasePlayer* pLocal = (C_BasePlayer*)pEntityList->GetClientEntity(pEngine->GetLocalPlayer());
	if (!pLocal || !pLocal->GetAlive())
		return;

	Vector view;
	pEngine->GetViewAngles(view);

	switch (Settings::ThirdPerson::mode) {
	case ThirdPersonMode::FAKE: {
		view = Vector(pLocal->GetEyeAngles()->x, AntiAim::lastFakeYaw, 0.0f);
		break;
	}
	case ThirdPersonMode::REAL: {
		if ((AntiAim::IsAirborne() && Settings::AntiAim::Airborne::Yaw::enabled) ||
			(AntiAim::IsMoving() && Settings::AntiAim::Moving::Yaw::enabled) ||
			(AntiAim::IsStanding() && Settings::AntiAim::Standing::Yaw::enabled)) {
			if (AntiAim::fakeTp) {
				view = Vector(pLocal->GetEyeAngles()->x, pLocal->GetEyeAngles()->y, 0.0f);
			}
			else {
				view = Vector(pLocal->GetEyeAngles()->x, AntiAim::lastRealYaw, 0.0f);
			}
		}
		else {
			view = Vector(pLocal->GetEyeAngles()->x, pLocal->GetEyeAngles()->y, 0.0f);
		}
		break;
	}
	case ThirdPersonMode::LBY: {
		float* lby = pLocal->GetLowerBodyYawTarget();

		view = Vector(pLocal->GetEyeAngles()->x, *(float*)&lby, 0.0f);
		break;
	}

	}

	*pLocal->GetVAngles() = view;
}



void ThirdPerson::DrawModelExecute(IMatRenderContext* matctx, const DrawModelState_t& state, const ModelRenderInfo_t& pInfo, matrix3x4_t* pCustomBoneToWorld)
{
	if (!Settings::ESP::enabled && Settings::ESP::Chams::enabled && Settings::ThirdPerson::enabled)
		return;

	if (!pEngine->IsInGame())
		return;

	if (!pInfo.pModel)
		return;

	C_BasePlayer* pLocal = (C_BasePlayer*)pEntityList->GetClientEntity(pEngine->GetLocalPlayer());
	if (!pLocal)
		return;

	C_BasePlayer* entity = (C_BasePlayer*)pEntityList->GetClientEntity(pInfo.entity_index);
	if (!entity || entity->GetDormant() || !entity->GetAlive())
		return;

	std::string modelName = pModelInfo->GetModelName(pInfo.pModel);
	if ((!Settings::ESP::Chams::enabled || !Settings::ESP::Filters::localplayer) && pLocal->IsScoped()) {
		if (modelName.find(XorStr("models/player")) != std::string::npos) {
			if (transparentPlayerMaterial == nullptr) {
				IMaterial* studioSkins[32];
				pModelInfo->GetModelMaterials(pLocal->GetModel(),
					pModelInfo->GetStudioModel(pLocal->GetModel())->numtextures,
					studioSkins);
				transparentPlayerMaterial = *studioSkins;
			}

			transparentPlayerMaterial->AlphaModulate(Settings::ThirdPerson::transparency);


			pModelRender->ForcedMaterialOverride(transparentPlayerMaterial);
			ModelRenderHook->GetOriginalFunction<DrawModelExecuteFn>(21)(pModelRender, matctx, state, pInfo, pCustomBoneToWorld);
		}
	}
	else {
		if (transparentPlayerMaterial != nullptr) {
			transparentPlayerMaterial->AlphaModulate(1.0f);
		}
	}
}

