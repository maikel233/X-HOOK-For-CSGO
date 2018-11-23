#include "Features.h"

bool Settings::ShowSpectators::enabled = false;

std::list<int> ShowSpectators::GetObservervators(int playerId)
{
	std::list<int> list;

	if (!pEngine->IsInGame())
		return list;

	C_BasePlayer* player = (C_BasePlayer*)pEntityList->GetClientEntity(playerId);
	if (!player)
		return list;

	if (!player->GetAlive())
	{
		C_BasePlayer* observerTarget = (C_BasePlayer*)pEntityList->GetClientEntityFromHandle(player->GetObserverTarget());
		if (!observerTarget)
			return list;

		player = observerTarget;
	}

	for (int i = 1; i < pEngine->GetMaxClients(); i++)
	{
		C_BasePlayer* pPlayer = (C_BasePlayer*)pEntityList->GetClientEntity(i);
		if (!pPlayer)
			continue;

		if (pPlayer->GetDormant() || pPlayer->GetAlive())
			continue;

		C_BasePlayer* target = (C_BasePlayer*)pEntityList->GetClientEntityFromHandle(pPlayer->GetObserverTarget());
		if (player != target)
			continue;

		list.push_back(i);
	}

	return list;
}

static void SquareConstraint(ImGuiSizeConstraintCallbackData *data)
{
	data->DesiredSize = ImVec2(max(data->DesiredSize.x, data->DesiredSize.y), max(data->DesiredSize.x, data->DesiredSize.y));
}

void ShowSpectators::RenderWindow()
{
	if (!Settings::ShowSpectators::enabled)
		return;

	if (!pEngine->IsInGame())
		return;

	ImGui::SetNextWindowSize(ImVec2(256, 256), ImGuiSetCond_FirstUseEver);
	ImGui::SetNextWindowSizeConstraints(ImVec2(0, 0), ImVec2(FLT_MAX, FLT_MAX), SquareConstraint);

	if (ImGui::Begin(XorStr("Spectators"), &Settings::ShowSpectators::enabled, ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_ShowBorders))
	{
		ImGui::Columns(2);
		ImGui::Separator();

		ImGui::Text(XorStr("Name"));
		ImGui::NextColumn();

		ImGui::Text(XorStr("Mode"));
		ImGui::NextColumn();

		for (int playerId : ShowSpectators::GetObservervators(pEngine->GetLocalPlayer()))
		{
			if (playerId == pEngine->GetLocalPlayer())
				continue;

			C_BasePlayer* player = (C_BasePlayer*)pEntityList->GetClientEntity(playerId);

			IEngineClient::player_info_t entityInformation;
			pEngine->GetPlayerInfo(playerId, &entityInformation);

			/*if (entityInformation.fakeplayer)
				continue;*/

			ImGui::Separator();

			ImGui::Text("%s", entityInformation.name);
			ImGui::NextColumn();

			switch (*player->GetObserverMode())
			{
			case ObserverMode_t::OBS_MODE_IN_EYE:
				ImGui::Text(XorStr("Perspective"));
				break;
			case ObserverMode_t::OBS_MODE_CHASE:
				ImGui::Text(XorStr("3rd person"));
				break;
			case ObserverMode_t::OBS_MODE_ROAMING:
				ImGui::Text(XorStr("Free look"));
				break;
			case ObserverMode_t::OBS_MODE_DEATHCAM:
				ImGui::Text(XorStr("Deathcam"));
				break;
			case ObserverMode_t::OBS_MODE_FREEZECAM:
				ImGui::Text(XorStr("Freezecam"));
				break;
			case ObserverMode_t::OBS_MODE_FIXED:
				ImGui::Text(XorStr("Fixed"));
				break;
			default:
				break;
			}
			ImGui::NextColumn();
		}

		ImGui::Columns(1);
		ImGui::Separator();

		ImGui::End();
	}
}
