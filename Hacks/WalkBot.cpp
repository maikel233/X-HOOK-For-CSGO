#include "../Features.h"

std::string Settings::WalkBot::actMapName;

bool Settings::WalkBot::enabled = false;
bool Settings::WalkBot::plantbomb = false;
bool Settings::WalkBot::forceReset = false; // if a Reset is queued up by the User
bool Settings::WalkBot::autobuy = false; // autobuy during FreezeTime
int Settings::WalkBot::autobuyAt = 0;   // autobuy >= this amount of cash
int Settings::WalkBot::marker = 0;
bool button_down = false;
bool button_clicked = false;
std::vector<Vector> spots;


Color colordefault = Color(0, 0, 255, 255);
Color colorClose = Color(0, 255, 0, 255);
Color colorSelected= Color(255, 0, 0, 255);
ButtonCode_t Settings::WalkBot::Key = ButtonCode_t::MOUSE_MIDDLE;

void  Walkbot::update() {

	if (!Settings::WalkBot::enabled)
		return;

	if (!pEngine->IsInGame())
		return;;

	C_BasePlayer* localplayer = (C_BasePlayer*)pEntityList->GetClientEntity(pEngine->GetLocalPlayer());
	if (!localplayer || !localplayer->GetAlive())
		return;

	int w, h, cx, cy;

	pEngine->GetScreenSize(w, h);
	cx = w / 2;
	cy = h / 2;
	bool spotErased = false, spotFound = false;

	for (int i = 0; i < spots.size(); i++) {
		Vector spot = spots.at(i);
		Color c = colordefault;
		float distance = localplayer->GetVecOrigin().DistTo(spot);

		if (distance <= 1) c = colorClose;
		Vector pos2D;
		if (Draw::WorldToScreen(spot, pos2D)) {
			float dist2d = pos2D.DistTo(Vector(cx, cy, 0));
			if (dist2d <= 15 && !spotFound)
			{
				c = colorSelected;
				spotFound = true;
				if (pInputSystem->IsButtonDown(Settings::WalkBot::Key) && !spotErased)
				{
					spots.erase(spots.begin() + i);
					spotErased = true;
					//update (save)

				}
			}
		}
		Ray_t ray; trace_t tr; CTraceFilter filter;
		ray.Init(localplayer->GetEyePosition(), spot);
		filter.pSkip = localplayer;
		pTrace->TraceRay(ray, 0x46004003, &filter, &tr);
		bool isvisible = tr.IsVisible();

		if (isvisible)
		//	Draw::Text(Vector2D(spot.x, spot.y), "Spot", esp_font, Color(255, 0, 255, 255)); // hot pink
		Draw::Circle3D(spot, 20, 20, Color(c.r, c.g, c.b));
		else {
			char dist_to[32];
			sprintf_s(dist_to, "%.0fm", distance);
			pSurface->DrawSetColor(c);
			pSurface->DrawFilledRect(pos2D.x - 3, pos2D.y - 3, pos2D.x + 3, pos2D.y + 3);
			//Draw::Text(Vector2D(bone2D.x, bone2D.y), buffer, boneMapFont, Color(255, 0, 255, 255));
			Draw::Text(Vector2D(pos2D.x, pos2D.y - 7), dist_to, esp_font, Color(255, 255, 255));
		}
	}

	if (pInputSystem->IsButtonDown(Settings::WalkBot::Key) && !spotErased) {

		Vector traceStart, traceEnd;
		Vector viewAngles;
		pEngine->GetViewAngles(viewAngles);
		Math::AngleVectors(viewAngles, traceEnd);
		traceStart = localplayer->GetEyePosition();
		traceEnd = traceStart + (traceEnd * 8192.0f);


		Ray_t ray; trace_t tr; CTraceFilter filter;
		ray.Init(traceStart, traceEnd);
		filter.pSkip = localplayer;
		pTrace->TraceRay(ray, 0x46004003, &filter, &tr);
		Vector spot = tr.endpos;
		spots.push_back(spot);
		Refresh(spots, true);
	}
}


void Walkbot::Clear()
{
	for (int i = 0; i < spots.size(); i++) {
		Vector spot = spots.at(i);

		spots.erase(spots.begin() + i);
	}
}


void Walkbot::CheckForUpdate()
{
	if (!pEngine->IsInGame())
		return;

	//std::string s = pClientState->m_szLevelNameShort; // <== Our current map 

	const char *test1 = pClientState->m_szLevelNameShort;
	std::string s = std::string(test1, 0, 10);

	/*pCvar->ConsoleColorPrintf(ColorRGBA(255, 255, 255),
	XorStr("\n=====\nLoading Config1:\n %s\n=====\n"), s);*/

	unsigned long p = s.find_last_of("/");
	if (p != std::string::npos)
		s.erase(0, p + 1);

	if (!Settings::WalkBot::actMapName.compare(s))
		return;

	Settings::WalkBot::actMapName = s;

}


void  Walkbot::Refresh(std::vector<Vector> spots, bool save)
{
	/*if (!Settings::WalkBot::enabled)
		return;
*/


	static char path[MAX_PATH];
	std::string folder, file;

	folder = std::string(path) + "C:\\XHOOK\\Configs\\WalkBot\\" + std::string(Settings::WalkBot::actMapName) + "\\";
	file = std::string(folder) + "Config" + ".json";

	CreateDirectoryA(folder.c_str(), NULL);
	pCvar->ConsoleColorPrintf(ColorRGBA(255, 255, 255),
		XorStr("\n=====\n Creating test directory at \n %s\n=====\n"), folder.c_str());
	if (save)
	{
		pCvar->ConsoleColorPrintf(ColorRGBA(255, 255, 255),
			XorStr("\n=====\n Saving config at dir \n %s\n=====\n"), folder.c_str());
		DeleteFileA(file.c_str());
		for (int i = 0; i < spots.size(); i++) {
			Vector spot = spots.at(i);
			WritePrivateProfileStringA(std::to_string(i).c_str(), "x", std::to_string(spot.x).c_str(), file.c_str());
			WritePrivateProfileStringA(std::to_string(i).c_str(), "y", std::to_string(spot.y).c_str(), file.c_str());
			WritePrivateProfileStringA(std::to_string(i).c_str(), "z", std::to_string(spot.z).c_str(), file.c_str());
		}
	}
	else {
		char value[32] = { ' \0 ' };
		spots.clear();
		int i = 0;
		bool limitReached = false;
		while (!limitReached) {
			GetPrivateProfileStringA(std::to_string(i).c_str(), "x", "", value, 32, file.c_str());
			float x = atof(value);
			GetPrivateProfileStringA(std::to_string(i).c_str(), "y", "", value, 32, file.c_str());
			float y = atof(value);
			GetPrivateProfileStringA(std::to_string(i).c_str(), "z", "", value, 32, file.c_str());
			float z = atof(value);
			if (x == 0 && y == 0 && z == 0) {
				limitReached = true;
				continue;
			}

			Vector spot = Vector(x, y, z);
			spots.push_back(spot);
			i++;

		}
	}
}

static unsigned int rally;
static bool reachedEnd;
static bool hasAutobought;
static bool hasDied;
bool slowLastTick = false;

//
//const std::vector<Vector> tDust2Points = { Vector(-503.56f, -739.92f, 114.16f),
//Vector(-1913.50f,-735.72f, 121.20f),Vector(-1676.94f, 726.70f, 31.00f),
//Vector(-1655.66f,1053.87f,31.57f), Vector(-1989.64f,1330.22f,28.93f),Vector(-1926.35f,2197.58f,-3.22f), Vector(-1685.82,2231.82,-2.47),
//Vector(-1331.98f,2231.99f,2.05f), Vector(-1286.65f,2164.06f,4.25f),Vector(-606.69f,2370.25f,-105.67f),
//Vector(-429.61f, 2196.21f, -123.86),Vector(279.11f,2175.06f,-128.18f),Vector(1380.f, 2131.6f, -5.48f),
//Vector(1398.04f, 928.93f, -8.78f), Vector(841.17f, 950.18f, 0.14f),Vector(581.58f, 674.48f, 1.50f) ,
//Vector(690.59f, 345.20f, 1.54f), Vector(446.82f, 70.25f, -3.26f), Vector(-5.52f, -829.16f, -3.58f)
//};
//
//const std::vector<Vector> BPlantPoints = { Vector(-1366.03f,2565.97f,4.65f), Vector(-1734.52f,1951.20f,-3.66f),Vector(-1680.61f,1796.50f,2.22f), Vector(-1676.78f, 1620.03f, 2.55f)
//};
//
//const std::vector<Vector> ctDust2Points = { Vector(279.11f,2175.06f,-128.18f),
//Vector(1380.f, 2131.6f, -5.48f), Vector(1398.04f, 928.93f, -8.78f), Vector(841.17f, 950.18f, 0.14f),
//Vector(581.58f, 674.48f, 1.50f) , Vector(690.59f, 345.20f, 1.54f), Vector(446.82f, 70.25f, -3.26f),
//Vector(-5.52f, -829.16f, -3.58f), Vector(-503.56f, -739.92f, 114.16f), Vector(-1913.50f,-735.72f, 121.20f),
//Vector(-1676.94f, 726.70f, 31.00f), Vector(-1655.66f,1053.87f,31.57f), Vector(-1989.64f,1330.22f,28.93f),
//Vector(-1926.35f,2197.58f,-3.22f), Vector(-1331.98f,2231.99f,2.05f), Vector(-1286.65f,2164.06f,4.25f),
//Vector(-606.69f,2370.25f,-105.67f),Vector(-429.61f, 2196.21f, -123.86) };
//
////const std::vector<Vector> tRallyPoints = { Vector(-417.1f, -707.38f, 174.8f), Vector(234.3f, -664.2f, 66.0f), Vector(-419.8f, -446.0f, 66.0f), Vector(-375.1f, 701.1f, 67.4f) };
////const std::vector<Vector> ctRallyPoints = { Vector(262.8f, 2157.6f, -63.2f), Vector(-537.6f, 2110.4f, -58.4f), Vector(-383.1f, 1489.1f, -60.0f), Vector(-375.1f, 701.1f, 67.4f) };
//
//C_BasePlayer* localPlayer;
C_BasePlayer* localPlayer;
void inline Reset()
{
	if (reachedEnd || Settings::WalkBot::forceReset || hasDied) {
		pCvar->ConsoleColorPrintf(ColorRGBA(225, 225, 10), "--Reset Rally--\n");
		rally = 0;
		reachedEnd = false;
		Settings::WalkBot::forceReset = false;
		hasDied = false;
	}
}

bool DoRally(const std::vector<Vector> points, CUserCmd *cmd) // return true if rally is completed.
{
	if (reachedEnd)
		return true;
	if ((std::abs(localPlayer->GetEyePosition().x - points[rally].x) < 0.6f) &&
		(std::abs(localPlayer->GetEyePosition().y - points[rally].y) < 0.6f)) {
		if (rally == points.size() - 1)
		{
			if (!reachedEnd)
			{
				pCvar->ConsoleColorPrintf(ColorRGBA(225, 225, 10), "--Reached Rally--\n");
				pCvar->ConsoleColorPrintf(ColorRGBA(50, 200, 100), "Finished Rally Points!\n");

				reachedEnd = true;
			}
			return true; // Finished Walking
		}
		pCvar->ConsoleColorPrintf(ColorRGBA(225, 225, 10), "--Reached Rally--\n");
		rally++;
		pCvar->ConsoleColorPrintf(ColorRGBA(225, 225, 10), "Moving To Rally\n");
	}

	Vector move = Math::CalcAngle(localPlayer->GetEyePosition(), points[rally]);
	cmd->forwardmove = 450.0f;
	cmd->sidemove = 0;
//	cmd->buttons |= IN_WALK;
	Math::CorrectMovement(move, cmd, cmd->forwardmove, cmd->sidemove);
	return false;
}

void Walkbot::CreateMove(CUserCmd *cmd)
{
	if (!Settings::WalkBot::enabled)
		return;

	Walkbot::CheckForUpdate(); // update map info

	if (spots.size() < 2)
		return;

	localPlayer = (C_BasePlayer*)pEntityList->GetClientEntity(pEngine->GetLocalPlayer());
	if (!localPlayer || !pEngine->IsInGame() || Settings::WalkBot::forceReset) {
		Reset();
		return;
	}
	//if (!Settings::WalkBot::enabled) {
	//	return;
	//}
	if ((*pGameRules)->IsFreezeTime())
	{
		Reset();
		if (Settings::WalkBot::autobuy && !hasAutobought && (localPlayer->GetMoney() >= Settings::WalkBot::autobuyAt)) 
		{
			pEngine->ExecuteClientCmd(XorStr("autobuy"));
			hasAutobought = true;
		}
	}
	if (Settings::Aimbot::AutoSlow::goingToSlow) 
	{
		slowLastTick = true;
		return;
	}
	else if (slowLastTick) {
		slowLastTick = false;
		return;
	}

	TeamID ourTeam = localPlayer->GetTeam();
	if (!localPlayer->GetAlive())
	{
		if (ourTeam == TeamID::TEAM_UNASSIGNED)
		{
			pEngine->ExecuteClientCmd(XorStr("teammenu")); // this will trigger the auto select timer
			return;
		}
		else
		{
			hasDied = true;
			hasAutobought = false;
			return;
		}
	}

//	bool dropped = false;

	//EClassIds classId = localPlayer->GetClientClass()->m_ClassID;
	//
	//if ((classId == EClassIds::CC4) && Settings::WalkBot::plantbomb == 0 && !dropped) // If we prefer to drop the bomb and not plant, lets drop.
	//{
	//	// Small delay so we don't drop our slot1 too.
	//	droptick++;
	//	if (droptick > 64)
	//	{
	//		pEngine->ClientCmd("drop");
	//		dropped = true;
	//	}
	//}

	if (ourTeam == TeamID::TEAM_TERRORIST)
	{
		DoRally(spots, cmd);
	}
	else if (ourTeam == TeamID::TEAM_COUNTER_TERRORIST)
	{
		DoRally(spots, cmd);
	}
}




