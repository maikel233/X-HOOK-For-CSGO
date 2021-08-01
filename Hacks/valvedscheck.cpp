#include "../Features.h"

bool ValveDSCheck::forceUT = false;

void ValveDSCheck::FireGameEvent(IGameEvent* event) {
	if (!event)
		return;

	if (!pEngine->IsInGame())
		return;

	if (strcmp(event->GetName(), XorStr("player_connect_full")) != 0 &&
		strcmp(event->GetName(), XorStr("cs_game_disconnected")) != 0)
		return;

	if (event->GetInt(XorStr("userid")) &&
		pEngine->GetPlayerForUserID(event->GetInt(XorStr("userid"))) != pEngine->GetLocalPlayer())
		return;

	ValveDSCheck::forceUT = false;
}
