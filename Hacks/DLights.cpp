#include "../Features.h"

bool Settings::Dlights::enabled = false;
float Settings::Dlights::radius = 500.0f;

void Dlights::Paint()
{
	if (!Settings::ESP::enabled)
		return;

	if (!Settings::Dlights::enabled)
		return;

	//if ((!Settings::ESP::Filters::enemies) && (!Settings::ESP::Filters::allies))
	//	return;

	if (!pEngine->IsInGame())
		return;

	C_BasePlayer* localplayer = (C_BasePlayer*)pEntityList->GetClientEntity(pEngine->GetLocalPlayer() );
	if (!localplayer)
		return;

		for (int i = 1; i < pEntityList->GetHighestEntityIndex(); i++) {
			C_BasePlayer* player = (C_BasePlayer*)pEntityList->GetClientEntity(i);
			if (!player)
				continue;

		if (player == localplayer)
			continue;

		if (player->GetTeam() != localplayer->GetTeam() && !Settings::ESP::Filters::enemies)
			continue;

		if (player->GetTeam() == localplayer->GetTeam() && !Settings::ESP::Filters::allies)
			continue;

		bool bIsVisible = false;
		if (Settings::ESP::Filters::visibilityCheck || Settings::ESP::Filters::legit)
		{
			bIsVisible = Entity::IsVisible(player, Bone::BONE_HEAD, 180.f, Settings::ESP::Filters::smokeCheck);
			if (!bIsVisible && Settings::ESP::Filters::legit)
				continue;
		}

		Color color = Color::FromImColor(ESP::GetESPPlayerColor(player, bIsVisible));

		dlight_t* dLight = pEffects->CL_AllocDlight(i);
		dLight->key = i;
		dLight->color.r = (unsigned char)color.r;
		dLight->color.g = (unsigned char)color.g;
		dLight->color.b = (unsigned char)color.b;
		dLight->color.exponent = true;
		dLight->flags = DLIGHT_NO_MODEL_ILLUMINATION;
		dLight->m_Direction = player->GetVecOrigin();
		dLight->origin = player->GetVecOrigin();
		dLight->radius = Settings::Dlights::radius;
		dLight->die = pGlobalVars->curtime + 0.1f;
		dLight->decay = 20.0f;
	}
}