#include "../Features.h"

bool Settings::NameStealer::enabled = false;
int Settings::NameStealer::team = 1;

int NameStealer::entityId = -1;

void NameStealer::BeginFrame(float frameTime)
{
	if (!Settings::NameStealer::enabled)
		return;

	if (!pEngine->IsInGame())
		return;

	long currentTime_ms = Util::GetEpochTime();
	static long timeStamp = currentTime_ms;

	if (currentTime_ms - timeStamp < 350)
		return;

	C_BasePlayer* localplayer = (C_BasePlayer*)pEntityList->GetClientEntity(pEngine->GetLocalPlayer());
	if (!localplayer)
		return;

	while (entityId < pEngine->GetMaxClients())
	{
		entityId++;

		if (entityId >= pEngine->GetMaxClients())
			entityId = 1;

		if (entityId == 0)
		{
			NameChanger::SetName("\n\xAD\xAD\xAD");

			timeStamp = currentTime_ms;

			break;
		}
		
		if ((*csPlayerResource) && (*csPlayerResource)->GetConnected(entityId))
		{
			if (Settings::NameStealer::team == 0 && (*csPlayerResource)->GetTeam(entityId) != localplayer->GetTeam())
				break;

			if (Settings::NameStealer::team == 1 && (*csPlayerResource)->GetTeam(entityId) == localplayer->GetTeam())
				break;

			IEngineClient::player_info_t entityInformation;
			pEngine->GetPlayerInfo(entityId, &entityInformation);

			if (entityInformation.ishltv)
				break;

			NameChanger::SetName(entityInformation.name);

			timeStamp = currentTime_ms;
		}
		
		break;
	}
}

void NameStealer::FireGameEvent(IGameEvent* event)
{
	if (!event)
		return;

	if (!Settings::NameStealer::enabled)
		return;

	if (!pEngine->IsInGame())
		return;

	if (strcmp(event->GetName(), XorStr("player_connect_full")) != 0 &&
		strcmp(event->GetName(), XorStr("cs_game_disconnected")) != 0)
		return;

	if (event->GetInt(XorStr("userid")) &&
		pEngine->GetPlayerForUserID(event->GetInt(XorStr("userid"))) != pEngine->GetLocalPlayer())
		return;


	entityId = -1;
}