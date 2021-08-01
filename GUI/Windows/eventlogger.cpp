#include "../../Features.h"

bool Settings::EventLogger::enabled = false;
bool Settings::EventLogger::OnFreezeTime = false;
const char* team[]{ "", "", " (T)", " (CT)" };
const char* defKit[]{ "without a defuse kit.", "with a defuse kit." };

std::map<int, const char*> bombsites = {
	{ 275, "Bombsite A" }, // Dust 2 v2
	{ 276, "Bombsite B" }, // Dust 2 v2
	{ 369, "Bombsite A" }, // Dust 2
	{ 366, "Bombsite B" }, // Dust 2
	{ 451, "Bombsite A" }, // Mirage
	{ 452, "Bombsite B" }, // Mirage
	{ 315, "Bombsite A" }, // Cache
	{ 316, "Bombsite B" }, // Cache
	{ 334, "Bombsite A" }, // Inferno
	{ 423, "Bombsite B" }, // Inferno
	{ 260, "Bombsite A" }, // Cobblestone
	{ 95,  "Bombsite B" }, // Cobblestone
	{ 79,  "Bombsite A" }, // Overpass
	{ 507, "Bombsite B" }, // Overpass
	{ 149, "Bombsite A" }, // Nuke
	{ 441, "Bombsite B" }, // Nuke
	{ 93,  "Bombsite A" }, // Train
	{ 538, "Bombsite B" }, // Train
	{ 222, "Bombsite A" }, // Canals
	{ 221, "Bombsite B" }, // Canals
	{ 239, "Bombsite A" }, // Aztec
	{ 238, "Bombsite B" }, // Aztec
	{ 81,  "Bombsite A" }, // Vertigo
	{ 82,  "Bombsite B" }, // Vertigo
	{ 526, "Bombsite A" }, // Dust 1
	{ 525, "Bombsite B" }, // Dust 1
};

void Capitalize(std::string& s);

std::string GetBombsiteByID(int id);


void EventLogger::PaintImGui() {

	if (!Settings::EventLogger::enabled || !pEngine->IsInGame())
		return;

	ImGui::SetNextWindowSize(ImVec2(400, 400), ImGuiSetCond_FirstUseEver);

	ImGui::Begin("EventLogger", &Settings::EventLogger::enabled,  ImVec2(400, 250), 0.2f, ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoScrollbar /*| ImGuiWindowFlags_ShowBorders*/ | ImGuiWindowFlags_NoTitleBar);

	for (size_t i = 0; i < events.size(); i++) 
	{
		float time = pGlobalVars->curtime - events[i].time;

		if (time > 6.0f)
			events[i].color.Value.w *= 0.98f;

		ImGui::TextColored(ImVec4(events[i].color), events[i].text.c_str());
	}
	ImGui::End();

}

void EventLogger::FireGameEvent(IGameEvent* event) {

	if (!event)
		return;

	if (!Settings::EventLogger::enabled)
		return;

	if (strstr(event->GetName(), XorStr("player_hurt"))) {
		C_BasePlayer* hurt = (C_BasePlayer*)pEntityList->GetClientEntity(
			pEngine->GetPlayerForUserID(event->GetInt(XorStr("userid")))
		);
		if (!hurt)
			return;

		C_BasePlayer* attacker = (C_BasePlayer*)pEntityList->GetClientEntity(
			pEngine->GetPlayerForUserID(event->GetInt(XorStr("attacker")))
		);
		if (event->GetInt(XorStr("attacker")) != 0) {
			if (!attacker)
				return;
		}

		int damage = event->GetInt(XorStr("dmg_health"));
		if (!damage)
			return;

		IEngineClient::player_info_t hurtPlayerInfo;
		pEngine->GetPlayerInfo(hurt->GetIndex(), &hurtPlayerInfo);

		IEngineClient::player_info_t attackerInfo;
		pEngine->GetPlayerInfo(attacker->GetIndex(), &attackerInfo);

		std::string attackerName = attackerInfo.name;
		if (event->GetInt(XorStr("attacker")) == 0) {
			attackerName = XorStr("World");
		}

		std::stringstream text;
		text << attackerName << XorStr(" hit ") << hurtPlayerInfo.name << XorStr(" for ") << damage
			<< XorStr(" HP");

		if (hurt->GetAlive()) {
			if (!hurt->GetDormant()) {
				int health = hurt->GetHealth() - damage;

				if (health > 0) {
					text << XorStr(" (") << health << XorStr(" HP remaining)");
				}
				else {
					text << XorStr(" (Dead)");
				}
			}
		}
		else {
			text << XorStr(" (Dead)");
		}

		EventLogger::AddEvent(text.str());
	}

	if (strstr(event->GetName(), XorStr("item_purchase"))) {
		C_BasePlayer* user = (C_BasePlayer*)pEntityList->GetClientEntity(
			pEngine->GetPlayerForUserID(event->GetInt(XorStr("userid")))
		);
		std::string weapon = std::string(event->GetString(XorStr("weapon")));
		int teamNum = event->GetInt(XorStr("team"));

		if (!user)
			return;

		if (weapon.empty())
			return;

		if (!teamNum)
			return;

		IEngineClient::player_info_t entityInfo;
		pEngine->GetPlayerInfo(user->GetIndex(), &entityInfo);

		std::string prefix(XorStr("weapon_"));
		if (strncmp(weapon.c_str(), prefix.c_str(), prefix.size()) == 0) {
			weapon = weapon.substr(prefix.size());
		}
		std::replace(weapon.begin(), weapon.end(), '_', ' ');
		Capitalize(weapon);

		std::stringstream text;
		text << entityInfo.name << team[teamNum] << XorStr(" purchased ") << weapon;
		EventLogger::AddEvent(text.str());
	}

	if (strstr(event->GetName(), XorStr("player_given_c4"))) {
		C_BasePlayer* user = (C_BasePlayer*)pEntityList->GetClientEntity(
			pEngine->GetPlayerForUserID(event->GetInt(XorStr("userid")))
		);

		if (!user)
			return;

		IEngineClient::player_info_t entityInfo;
		pEngine->GetPlayerInfo(user->GetIndex(), &entityInfo);

		std::stringstream text;
		text << entityInfo.name << XorStr(" is now the bomb carrier");
		EventLogger::AddEvent(text.str());
	}

	if (strstr(event->GetName(), XorStr("bomb_beginplant"))) {
		C_BasePlayer* user = (C_BasePlayer*)pEntityList->GetClientEntity(
			pEngine->GetPlayerForUserID(event->GetInt(XorStr("userid")))
		);
		int site = event->GetInt(XorStr("site"));

		if (!user)
			return;

		IEngineClient::player_info_t entityInfo;
		pEngine->GetPlayerInfo(user->GetIndex(), &entityInfo);

		std::stringstream text;
		text << entityInfo.name << XorStr(" started planting at ") << GetBombsiteByID(site);
		EventLogger::AddEvent(text.str(), ImColor(26, 104, 173));
	}

	if (strstr(event->GetName(), XorStr("bomb_begindefuse"))) {
		C_BasePlayer* user = (C_BasePlayer*)pEntityList->GetClientEntity(
			pEngine->GetPlayerForUserID(event->GetInt(XorStr("userid")))
		);
		bool defuser = event->GetBool(XorStr("haskit"));

		if (!user)
			return;

		IEngineClient::player_info_t entityInfo;
		pEngine->GetPlayerInfo(user->GetIndex(), &entityInfo);

		std::stringstream text;
		text << entityInfo.name << XorStr(" started defusing the bomb ") << defKit[defuser];
		EventLogger::AddEvent(text.str(), ImColor(26, 104, 173));
	}

	if (strstr(event->GetName(), XorStr("hostage_follows"))) {
		C_BasePlayer* user = (C_BasePlayer*)pEntityList->GetClientEntity(
			pEngine->GetPlayerForUserID(event->GetInt(XorStr("userid")))
		);

		if (!user)
			return;

		IEngineClient::player_info_t entityInfo;
		pEngine->GetPlayerInfo(user->GetIndex(), &entityInfo);

		std::stringstream text;
		text << entityInfo.name << XorStr(" has picked up a hostage");
		EventLogger::AddEvent(text.str(), ImColor(26, 104, 173));
	}


	if (strstr(event->GetName(), XorStr("hostage_rescued"))) {
		C_BasePlayer* user = (C_BasePlayer*)pEntityList->GetClientEntity(
			pEngine->GetPlayerForUserID(event->GetInt(XorStr("userid")))
		);

		if (!user)
			return;

		IEngineClient::player_info_t entityInfo;
		pEngine->GetPlayerInfo(user->GetIndex(), &entityInfo);

		std::stringstream text;
		text << entityInfo.name << XorStr(" has rescued a hostage");
		EventLogger::AddEvent(text.str(), ImColor(26, 104, 173));
	}

	if (strstr(event->GetName(), XorStr("bomb_planted"))) {
		int site = event->GetInt(XorStr("site"));

		std::stringstream text(XorStr(""));
		text << XorStr("The bomb has been planted at ") << GetBombsiteByID(site);
		EventLogger::AddEvent(text.str(), ImColor(26, 104, 173));
	}

	if (strstr(event->GetName(), XorStr("enter_bombzone"))) {
		C_BasePlayer* user = (C_BasePlayer*)pEntityList->GetClientEntity(
			pEngine->GetPlayerForUserID(event->GetInt(XorStr("userid")))
		);
		bool bomb = event->GetBool(XorStr("hasbomb"));

		if (!user)
			return;

		IEngineClient::player_info_t entityInfo;
		pEngine->GetPlayerInfo(user->GetIndex(), &entityInfo);

		if (!event->GetBool(XorStr("isplanted")) && bomb) {
			std::stringstream text(XorStr("Bomb carrier "));
			text << entityInfo.name << " has entered a bombsite";
			EventLogger::AddEvent(text.str());
		}
	}

	if (events.size() > 12) {
		events.pop_back();
	}
}

void EventLogger::AddEvent(std::string text, ImColor color) {
	events.push_front(loginfo(text, color, pGlobalVars->curtime));

	if (events.size() > 12) {
		events.pop_back();
	}
}

std::string GetBombsiteByID(int id) {
	if (bombsites.find(id) == bombsites.end()) {
		pCvar->ConsoleColorPrintf(ColorRGBA(255, 255, 255), XorStr("(EventLogger::GetBombsiteByID): Warning: "
			"Could not find localized string for ID \"%i\".\n"),
			id);

		return XorStr("bombsite");
	}

	return bombsites.at(id);
}

void Capitalize(std::string& s) {
	bool cap = true;

	for (unsigned int i = 0; i <= s.length(); i++) {
		if (isalpha(s[i]) && cap) {
			s[i] = toupper(s[i]);
			cap = false;
		}
		else if (isspace(s[i])) {
			cap = true;
		}
	}
}
