#include "../Features.h"

std::string NameChanger::origName = "";
int NameChanger::changes = -1;
NC_Type NameChanger::type = NC_Type::NC_NORMAL;
Colors NameChanger::color = Colors::LIGHT_RED;


int Settings::Misc::banColor = 6;
std::string Settings::Misc::banText = { "Cheater has been permanently banned from official CS:GO servers." };

std::vector<std::pair<Colors, const char*>> NameChanger::colors = {
	{ Colors::WHITE, "White" },
	{Colors::DARK_RED, "Dark Red" },
	{Colors::LIGHT_PURPLE, "Light Purple" },
	{ Colors::DARK_GREEN, "Dark Green" },
	{ Colors::LIGHT_GREEN, "Light Green" },
	{ Colors::GREEN, "Green" },
	{ Colors::LIGHT_RED, "Light Red" },
	{ Colors::GRAY, "Gray" },
	{ Colors::YELLOW, "Yellow" },
	{ Colors::LIGHT_BLUE, "Light Blue" },
	{ Colors::BLUE, "Blue" },
	{ Colors::DARK_BLUE, "Dark Blue" },
	{ Colors::DARK_GRAY, "Dark Gray" },
	{ Colors::PURPLE, "Purple" },
	{ Colors::RED, "Red" },
	{ Colors::ORANGE, "Orange" },
};

std::string NameChanger::GetName()
{
	IEngineClient::player_info_t playerInfo;
	pEngine->GetPlayerInfo(pEngine->GetLocalPlayer(), &playerInfo);
	return std::string(playerInfo.name);
}

std::string Rainbowify(const std::string& name)
{
	//


	std::string base = " \x01\x0B";
	std::vector<char> rainbow = {
		(char)(Colors::RED),
		(char)(Colors::ORANGE),
		(char)(Colors::YELLOW),
		(char)(Colors::GREEN),
		(char)(Colors::BLUE),
		(char)(Colors::PURPLE),
	};

	size_t color = 0;
	for (char c : name)
	{
		if (color > rainbow.size() - 1)
			color = 0;
		base.push_back(rainbow[color]);
		base.push_back(c);
		color++;
	}

	//base.append("\230");
	return base;
}

std::string Colorize(const std::string& name, Colors color = Colors::LIGHT_RED)
{

	std::string res = { "\x1\xB" };
	res.append(std::string{ static_cast<char>(color) }).append(name).append("\x1").c_str();

	/*std::string res = " \x01\x0B";
	res += (char)(color);
	res.append(name);*/
	return res;
}

void NameChanger::BeginFrame(float frameTime)
{

	if (!pEngine->IsInGame())
		return;

	if (changes == -1)
		return;

	long currentTime_ms = Util::GetEpochTime();
	static long timeStamp = currentTime_ms;

	if (currentTime_ms - timeStamp < 150)
		return;

	timeStamp = currentTime_ms;
	changes++;

	if (changes >= 5)
	{
		switch (NameChanger::type)
		{
		case NC_Type::NC_NORMAL:
			SetName("\XHOOK.XYZ");
			break;
		case NC_Type::NC_TROLL:
			SetName("\XHOOK.XYZ");
			break;
		case NC_Type::NC_RAINBOW:
			SetName(Rainbowify(origName).c_str());
			break;//.append("\x1").c_str()
		case NC_Type::NC_SOLID:
			SetName(Colorize(origName, NameChanger::color).c_str());
			break;
		}

		changes = -1;

		return;
	}

	SetName(Util::PadStringRight("XHOOK.XYZ", strlen("XHOOK.XYZ") + changes));
}

void NameChanger::SetName(const char* name)
{
	ConVar* cvar_name = pCvar->FindVar("name");
	cvar_name->onChangeCallbacks.size = 0;
	cvar_name->SetValue(name);
}

void NameChanger::InitColorChange(NC_Type type, Colors color )
{
	NameChanger::changes = 0;
	NameChanger::origName = NameChanger::GetName();
	NameChanger::type = type;
	NameChanger::color = color;
}

bool NameChanger::changeName(bool reconnect, const char* newName, float delay) noexcept
{
	static auto exploitInitialized{ false };

	static auto name{ pCvar->FindVar("name") };

	if (reconnect) {
		exploitInitialized = false;
		return false;
	}

	if (!exploitInitialized && pEngine->IsInGame()) {	
		C_BasePlayer* LocalPlayer = (C_BasePlayer*)pEntityList->GetClientEntity(pEngine->GetLocalPlayer());
		if (IEngineClient::player_info_t playerInfo; LocalPlayer && pEngine->GetPlayerInfo(pEngine->GetLocalPlayer(), &playerInfo) && (!strcmp(playerInfo.name, "?empty") || !strcmp(playerInfo.name, "\n\xAD\xAD\xAD"))) {
			exploitInitialized = true;
		}
		else {
			name->onChangeCallbacks.size = 0;
			name->SetValue("\n\xAD\xAD\xAD");
			return false;
		}
	}

	if (static auto nextChangeTime = 0.0f; nextChangeTime <= pGlobalVars->realtime)
	{
		name->SetValue(newName);
		nextChangeTime = pGlobalVars->realtime + delay;
		return true;
	}
	return false;
}


int banColor{ 6 };
void NameChanger::fakeBan(bool set) noexcept
{
	static bool shouldSet = false;

	if (set)
		shouldSet = set;

	if (shouldSet && pEngine->IsInGame() && changeName(false, std::string{ "\x1\xB" }.append(std::string{ static_cast<char>(Settings::Misc::banColor + 1) }).append(Settings::Misc::banText).append("\x1").c_str(), 5.0f))
		shouldSet = false;
}