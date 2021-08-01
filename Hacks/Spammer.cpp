#include "../Features.h"
#include "../Utils/pstring.h"

Radio Settings::Spammer::RadioSpammer::radio = Radio::NONE;
SpammerType Settings::Spammer::type = SpammerType::SPAMMER_NONE;
bool Settings::Spammer::say_team = false;
bool Settings::Spammer::KillSpammer::enabled = false;
bool Settings::Spammer::KillSpammer::sayTeam = false;
bool Settings::Spammer::KillSpammer::enabledSync = false;
bool Settings::Spammer::KillSpammer::enableCustomSounds = false;
bool Settings::Spammer::KillSpammer::enabledHurt = false;
//bool Settings::Spammer::KillSpammer::enabledBombTimerCountdown = false;
std::vector<std::string> Settings::Spammer::KillSpammer::messages = {
	"Killing Spree",
	"BOOM HEADSHOT sort of...",
	"Killing Frenzy!",
	"Running Riot!",
	"Rampage!",
	"Untouchable!",
	"Invincible!"
};

std::vector<std::string> Settings::Spammer::NormalSpammer::messages = {
		"XHOOK.XYZ",
		"XHOOK.XYZ | Free & Premium cheats",
		"XHOOK.XYZ - Legit | Rage"
};


int Settings::Spammer::PositionSpammer::team = 1;
bool Settings::Spammer::PositionSpammer::showName = true;
bool Settings::Spammer::PositionSpammer::showWeapon = true;
bool Settings::Spammer::PositionSpammer::showRank = true;
bool Settings::Spammer::PositionSpammer::showWins = true;
bool Settings::Spammer::PositionSpammer::showHealth = true;
bool Settings::Spammer::PositionSpammer::showMoney = true;
bool Settings::Spammer::PositionSpammer::showLastplace = true;

std::vector<int> killedPlayerQueue;
#define ARRAYSIZE(_ARR)      ((int)(sizeof(_ARR)/sizeof(*_ARR)))
void Spammer::BeginFrame(float frameTime)
{
	if (!pEngine->IsInGame())
		return;

	// Grab the current time in milliseconds
	long currentTime_ms = Util::GetEpochTime();
	static long timeStamp = currentTime_ms;

	if (currentTime_ms - timeStamp < 850)
		return;

	// Kill spammer
	if (Settings::Spammer::KillSpammer::enabled && killedPlayerQueue.size() > 0)
	{

		if (Settings::Spammer::KillSpammer::messages.empty())
			return;

		IEngineClient::player_info_t playerInfo;
		pEngine->GetPlayerInfo(killedPlayerQueue[0], &playerInfo);

		// Prepare dead player's nickname without ';' & '"' characters
		// as they might cause user to execute a command.
		std::string dead_player_name = std::string(playerInfo.name);
		dead_player_name.erase(std::remove(dead_player_name.begin(), dead_player_name.end(), ';'), dead_player_name.end());
		dead_player_name.erase(std::remove(dead_player_name.begin(), dead_player_name.end(), '"'), dead_player_name.end());

		// Remove end line character
		dead_player_name.erase(std::remove(dead_player_name.begin(), dead_player_name.end(), '\n'), dead_player_name.end());

		// Construct a command with our message
		pstring str;
		str << (Settings::Spammer::KillSpammer::sayTeam ? XorStr("say_team") : XorStr("say"));
		std::string message = Settings::Spammer::KillSpammer::messages[std::rand() % Settings::Spammer::KillSpammer::messages.size()];
		str << " \"" << Util::ReplaceString(message, XorStr("$nick"), dead_player_name) << "\"";

		// Execute our constructed command
		pEngine->ExecuteClientCmd(str.c_str());

		// Remove the first element from the vector
		killedPlayerQueue.erase(killedPlayerQueue.begin(), killedPlayerQueue.begin() + 1);

		return;
	}

	if (Settings::Spammer::RadioSpammer::radio == Radio::Normal)
	{

		C_BasePlayer* localplayer = (C_BasePlayer*)pEntityList->GetClientEntity(pEngine->GetLocalPlayer());
		if (!localplayer->GetAlive())
			return;

		const char* radioCommands[] = {
			"coverme",
			"takepoint",
			"holdpos",
			"regroup",
			"followme",
			"takingfire",
			"go",
			"fallback",
			"sticktog",
			"report",
			"roger",
			"enemyspot",
			"needbackup",
			"sectorclear",
			"inposition",
			"reportingin",
			"getout",
			"negative",
			"enemydown",
		};
		pEngine->ClientCmd_Unrestricted(radioCommands[std::rand() % ARRAYSIZE(radioCommands)]);
	}

	else if (Settings::Spammer::RadioSpammer::radio == Radio::Bombsounds)
	{
		{
			C_BasePlayer* localplayer = (C_BasePlayer*)pEntityList->GetClientEntity(pEngine->GetLocalPlayer());
			if (!localplayer->GetAlive())
				return;

			const char* radioCommands[] = {
				"getout",
			};

			pEngine->ClientCmd_Unrestricted(radioCommands[std::rand() % ARRAYSIZE(radioCommands)]);
		}
	}
		if (Settings::Spammer::type == SpammerType::SPAMMER_NORMAL)
		{
			if (Settings::Spammer::NormalSpammer::messages.empty())
				return;

			// Grab a random message string
			std::string message = Settings::Spammer::NormalSpammer::messages[std::rand() % Settings::Spammer::NormalSpammer::messages.size()];

			// Construct a command with our message
			pstring str;
			str << (Settings::Spammer::say_team ? XorStr("say_team") : XorStr("say")) << " ";
			str << message;

			// Execute our constructed command
			pEngine->ExecuteClientCmd(str.c_str());
		}


		else if (Settings::Spammer::type == SpammerType::SPAMMER_POSITIONS)
		{
			C_BasePlayer* localplayer = (C_BasePlayer*)pEntityList->GetClientEntity(pEngine->GetLocalPlayer());
			static int lastId = 1;

			for (int i = lastId; i < pEngine->GetMaxClients(); i++)
			{
				C_BasePlayer* player = (C_BasePlayer*)pEntityList->GetClientEntity(i);

				lastId++;
				if (lastId == pEngine->GetMaxClients())
					lastId = 1;

				if (!player
					|| player->GetDormant()
					|| !player->GetAlive())
					continue;

				if (Settings::Spammer::PositionSpammer::team == 0 && player->GetTeam() != localplayer->GetTeam())
					continue;

				if (Settings::Spammer::PositionSpammer::team == 1 && player->GetTeam() == localplayer->GetTeam())
					continue;

				IEngineClient::player_info_t entityInformation;
				pEngine->GetPlayerInfo(i, &entityInformation);

				C_BaseCombatWeapon* activeWeapon = (C_BaseCombatWeapon*)pEntityList->GetClientEntityFromHandle(player->GetActiveWeapon());

				// Prepare player's nickname without ';' & '"' characters
				// as they might cause user to execute a command.
				std::string playerName = std::string(entityInformation.name);
				playerName.erase(std::remove(playerName.begin(), playerName.end(), ';'), playerName.end());
				playerName.erase(std::remove(playerName.begin(), playerName.end(), '"'), playerName.end());

				// Remove end line character
				playerName.erase(std::remove(playerName.begin(), playerName.end(), '\n'), playerName.end());

				// Construct a command with our message
				pstring str;
				str << (Settings::Spammer::say_team ? XorStr("say_team") : XorStr("say")) << " \"";

				if (Settings::Spammer::PositionSpammer::showName)
					str << playerName << " | ";

				if (Settings::Spammer::PositionSpammer::showWeapon)
					str << Util::Items::GetItemDisplayName(*activeWeapon->GetItemDefinitionIndex()) << " | ";

				if (Settings::Spammer::PositionSpammer::showRank)
					str << ESP::ranks[*(*csPlayerResource)->GetCompetitiveRanking(i)] << " | ";

				if (Settings::Spammer::PositionSpammer::showWins)
					str << *(*csPlayerResource)->GetCompetitiveWins(i) << XorStr(" wins | ");

				if (Settings::Spammer::PositionSpammer::showHealth)
					str << player->GetHealth() << XorStr("HP | ");

				if (Settings::Spammer::PositionSpammer::showMoney)
					str << "$" << player->GetMoney() << XorStr(" | ");

				if (Settings::Spammer::PositionSpammer::showLastplace)
					str << player->GetLastPlaceName();

				str << "\"";

				// Execute our constructed command
				pEngine->ExecuteClientCmd(str.c_str());

				break;
			}
		}
	

		// Update the time stamp
		timeStamp = currentTime_ms;
}

void Spammer::FireGameEvent(IGameEvent* event)
{
	if (!Settings::Spammer::KillSpammer::enabled)
		return;

	if (!pEngine->IsInGame())
		return;

	if (strcmp(event->GetName(), "player_death") != 0)
		return;

	int attacker_id = pEngine->GetPlayerForUserID(event->GetInt("attacker"));
	int deadPlayer_id = pEngine->GetPlayerForUserID(event->GetInt("userid"));

	// Make sure it's not a suicide.x
	if (attacker_id == deadPlayer_id)
		return;

	// Make sure we're the one who killed someone...
	if (attacker_id != pEngine->GetLocalPlayer())
		return;

	killedPlayerQueue.push_back(deadPlayer_id);
}