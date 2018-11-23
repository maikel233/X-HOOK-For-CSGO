#include "Features.h"

bool ResolverInfo::showWindow = false;

void ResolverInfo::RenderWindow() {

	if (!Settings::ESP::enabled)
		return;


	if (!Settings::Resolver::enabled)
		return;


	if (!Settings::Resolver::info)
		return;

	if (ImGui::Begin(XorStr("Resolver Info"), NULL, ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoScrollbar /*| ImGuiWindowFlags_ShowBorders*/)) {
		
		ImVec2 temp = ImGui::GetWindowSize();
	
		temp = ImGui::GetWindowPos();
	
		if (!pEngine->IsInGame())
			ImGui::Text(XorStr("Not in-game\n"));
		else if (!Settings::Resolver::enabled)
			ImGui::Text(XorStr("Resolver not enabled\n"));
		else {
			int playerId = Resolver::resolvingId;

			IEngineClient::player_info_t playerInfo;
			pEngine->GetPlayerInfo(playerId, &playerInfo);

			char* name = playerInfo.name;
			char* guid = playerInfo.guid;

			float lastHit = Math::ResNormalizeYaw(Resolver::lastHitAng[playerId]);
			float angForce = Math::ResNormalizeYaw(Resolver::angForce[playerId]);
			const char* angForceTxt = Resolver::angForceTxt[playerId];
			int shotsMissed = Resolver::shotsMiss[playerId];
			float lby = Math::ResNormalizeYaw(Resolver::lby[playerId]);

			ImGui::Text(XorStr("%s [%s]\n"), name, guid);
			ImGui::Text(XorStr("Trying Angle : %s [%.1f]\n"), angForceTxt, angForce);
			ImGui::Text(XorStr("Last Hit : %.1f\n"), lastHit);
			ImGui::Text(XorStr("Shots missed : %d\n"), shotsMissed);
			ImGui::Text(XorStr("LBY : %.1f\n"), lby);
		}

		ImGui::End();
	}
}