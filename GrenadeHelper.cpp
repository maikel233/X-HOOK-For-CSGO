#include "Features.h"
#include "Draw.h"
#include "SDK\Definitions.h"

std::vector<GrenadeInfo> Settings::GrenadeHelper::grenadeInfos = {};
bool Settings::GrenadeHelper::enabled = false;
bool Settings::GrenadeHelper::onlyMatchingInfos = true;
bool Settings::GrenadeHelper::aimAssist = false;
float Settings::GrenadeHelper::aimDistance = 75;
float Settings::GrenadeHelper::aimFov = 45;
float Settings::GrenadeHelper::aimStep = 5;

ColorVar Settings::GrenadeHelper::aimDot = ImColor(10, 10, 200, 255);
ColorVar Settings::GrenadeHelper::aimLine = ImColor(200, 200, 200, 255);
ColorVar Settings::GrenadeHelper::infoHE = ImColor(7, 183, 7, 255);
ColorVar Settings::GrenadeHelper::infoMolotov = ImColor(236, 0, 0, 255);
ColorVar Settings::GrenadeHelper::infoSmoke = ImColor(172, 172, 172, 255);
ColorVar Settings::GrenadeHelper::infoFlash = ImColor(255, 255, 0, 255);

bool shotLastTick = false;
std::string Settings::GrenadeHelper::actMapName;

GrenadeType getGrenadeType(C_BaseCombatWeapon* wpn)
{
	if (!strcmp(wpn->GetCSWpnData()->szClassName, XorStr("weapon_hegrenade")))
		return GrenadeType::HEGRENADE;
	if (!strcmp(wpn->GetCSWpnData()->szClassName, XorStr("weapon_smokegrenade")))
		return GrenadeType::SMOKE;
	if (!strcmp(wpn->GetCSWpnData()->szClassName, XorStr("weapon_flashbang")) || !strcmp(wpn->GetCSWpnData()->szClassName, XorStr("weapon_decoy")))
		return GrenadeType::FLASH;
	return GrenadeType::MOLOTOV;// "weapon_molotov", "weapon_incgrenade"
}

ImColor GrenadeHelper::getColor(GrenadeType type)
{
	switch (type)
	{
	case GrenadeType::HEGRENADE:
		return Settings::GrenadeHelper::infoHE.Color();
	case GrenadeType::SMOKE:
		return Settings::GrenadeHelper::infoSmoke.Color();
	case GrenadeType::FLASH:
		return Settings::GrenadeHelper::infoFlash.Color();
	case GrenadeType::MOLOTOV:
		return Settings::GrenadeHelper::infoMolotov.Color();
	default:
		return ImColor(255, 255, 255);
	}
}

void GrenadeHelper::DrawGrenadeInfo(Vector pos, GrenadeType gType, std::string name)
{
	Vector pos2d;
	if (pDebugOverlay->ScreenPosition(Vector(pos.x, pos.y, pos.z), pos2d))
		return;

	Color clr = Color::FromImColor(getColor(gType));
	float radius = 20;
	Draw::Circle(Vector2D(pos2d.x, pos2d.y), 15, radius, clr);
	Draw::Text(pos2d.x + radius, pos2d.y, name.c_str(), esp_font, clr);
}

void GrenadeHelper::DrawAimHelp(Vector angle, Vector pos)
{
	Vector infoVec;
	Math::AngleVectors(angle, infoVec);
	infoVec *= 150 / infoVec.Length();
	infoVec += pos;

	Vector posVec;
	if (pDebugOverlay->ScreenPosition(infoVec, posVec))
		return;

	int w, h;
	pEngine->GetScreenSize(w, h);

	Vector2D pos2d(posVec.x, posVec.y);

	// Draw Point to Throw to
	Draw::FilledCircle(pos2d, 20, 5, Color::FromImColor(Settings::GrenadeHelper::aimDot.Color()));

	// Draw Help line
	Draw::Line(Vector2D(w / 2, h / 2), pos2d, Color::FromImColor(Settings::GrenadeHelper::aimLine.Color()));
}

void GrenadeHelper::Paint()
{
	if (!Settings::ESP::enabled || !pEngine->IsInGame() || !Settings::GrenadeHelper::enabled)
		return;

	C_BasePlayer* localPlayer = (C_BasePlayer*)pEntityList->GetClientEntity(pEngine->GetLocalPlayer());
	if (!localPlayer)
		return;

	C_BaseCombatWeapon* activeWeapon = (C_BaseCombatWeapon*)pEntityList->GetClientEntityFromHandle(localPlayer->GetActiveWeapon());
	if (!activeWeapon || activeWeapon->GetCSWpnData()->GetWeaponType() != CSWeaponType::WEAPONTYPE_GRENADE)
		return;

	if (Settings::GrenadeHelper::grenadeInfos.empty())
		return;

	for (auto grenadeInfo = Settings::GrenadeHelper::grenadeInfos.begin(); grenadeInfo != Settings::GrenadeHelper::grenadeInfos.end(); grenadeInfo++)
	{
		if (Settings::GrenadeHelper::onlyMatchingInfos && getGrenadeType(activeWeapon) != grenadeInfo->gType)
			continue;

		float dist = grenadeInfo->pos.DistTo(localPlayer->GetEyePosition());
		if (dist > 1000)
			continue;

		GrenadeHelper::DrawGrenadeInfo(grenadeInfo->pos, grenadeInfo->gType,grenadeInfo->name);

		if (dist < Settings::GrenadeHelper::aimDistance)
			GrenadeHelper::DrawAimHelp(grenadeInfo->angle, grenadeInfo->pos);
	}
}

void GrenadeHelper::AimAssist(CUserCmd* cmd)
{
	if (!Settings::GrenadeHelper::aimAssist || !pEngine->IsInGame() || !Settings::GrenadeHelper::enabled)
		return;

	C_BasePlayer* localPlayer = (C_BasePlayer*)pEntityList->GetClientEntity(pEngine->GetLocalPlayer());
	if (!localPlayer || !localPlayer->GetAlive())
		return;

	C_BaseCombatWeapon* activeWeapon = (C_BaseCombatWeapon*)pEntityList->GetClientEntityFromHandle(localPlayer->GetActiveWeapon());

	if (!activeWeapon || activeWeapon->GetCSWpnData()->GetWeaponType() != CSWeaponType::WEAPONTYPE_GRENADE)
		return;

	bool shootThisTick = cmd->buttons & IN_ATTACK;
	if (!shootThisTick && !shotLastTick)
		return;

	if (Settings::GrenadeHelper::grenadeInfos.empty())
		return;
	//for storing the best result
	GrenadeInfo* grenadeInfo = nullptr;
	float distOnScreen;//Viewangle difference
	float dist;//3D distance

	for (auto act = Settings::GrenadeHelper::grenadeInfos.begin(); act != Settings::GrenadeHelper::grenadeInfos.end(); act++)
	{
		if (Settings::GrenadeHelper::onlyMatchingInfos && getGrenadeType(activeWeapon) != act->gType)
			continue;

		float dist3D = localPlayer->GetEyePosition().DistTo(act->pos);
		if (dist3D > Settings::GrenadeHelper::aimDistance)
			continue;

		float actDistOnScreen = Math::GetFov(act->angle, *localPlayer->GetVAngles());

		if ((grenadeInfo &&  distOnScreen < actDistOnScreen) || actDistOnScreen > Settings::GrenadeHelper::aimFov)
			continue;

		//grenadeInfo = act.base();
		distOnScreen = actDistOnScreen;
		dist = dist3D;
	}
	if (!grenadeInfo)
		return;

	if (!shootThisTick && shotLastTick && dist < 5)
	{
		//throw the grenade
		if (grenadeInfo->tType == ThrowType::JUMP)
			cmd->buttons |= IN_JUMP;
		if (grenadeInfo->tType == ThrowType::WALK)
			cmd->buttons |= IN_WALK;
		pEngine->SetViewAngles(grenadeInfo->angle);
	}
	else
	{
		if (dist > 0.5f)
		{
			//Move towards the aimpos
			Vector movement = Math::CalcAngle(localPlayer->GetEyePosition(), grenadeInfo->pos);
			if (cmd->forwardmove < dist) cmd->forwardmove = dist * 2;
			cmd->sidemove = 0;
			cmd->buttons |= IN_WALK;
			Math::CorrectMovement(movement, cmd, cmd->forwardmove, cmd->sidemove);
		}
		if (cmd->viewangles != grenadeInfo->angle)
		{
			//Aim towards the aimspot
			float maxStep = Settings::GrenadeHelper::aimStep;
			Vector old = cmd->viewangles;
			float diffX = max(-maxStep, min(grenadeInfo->angle.x - old.x, maxStep));
			float p = Math::GetFov(old + Vector(diffX, 0.1, 0), grenadeInfo->angle);
			float q = Math::GetFov(old + Vector(diffX, -0.1, 0), grenadeInfo->angle);
			float diffY = p < q ? min(p, maxStep) : -min(q, maxStep);
			cmd->viewangles += Vector(diffX, diffY, 0);
			Math::ClampAngles(cmd->viewangles);
			pEngine->SetViewAngles(cmd->viewangles);
			Math::CorrectMovement(old, cmd, cmd->forwardmove, cmd->sidemove);
		}
	}
}

struct stat Maininfos;
void GrenadeHelper::CheckForUpdate()
{
	if (!pEngine->IsInGame())
		return;

	//std::string s = pClientState->m_szLevelNameShort; // <== Our current map 

	const char *test1 = pClientState->m_szLevelNameShort;
	std::string s =  std::string(test1, 0, 10);

	/*pCvar->ConsoleColorPrintf(ColorRGBA(255, 255, 255),
		XorStr("\n=====\nLoading Config1:\n %s\n=====\n"), s);*/

	unsigned long p = s.find_last_of("/");
	if (p != std::string::npos)
		s.erase(0, p + 1);

	if (!Settings::GrenadeHelper::actMapName.compare(s))
		return;

	Settings::GrenadeHelper::actMapName = s;

	const char *GetGhConfigDirectory = "C://xhook//configs//GrenadeHelper//";
	std::ostringstream path;
	path << GetGhConfigDirectory << Settings::GrenadeHelper::actMapName;
	path << XorStr("/config.json");

	// If Dir exist...
	const char * GetPathInconst = s.c_str();
	if (stat(GetPathInconst, &Maininfos) != 0)
	{
		pCvar->ConsoleColorPrintf(ColorRGBA(255, 255, 255),
			XorStr("\n=====\n Error cant verify that directory exist! trying to load config.json \n %s\n=====\n"), path.str());

		Settings::LoadGrenadeInfo(path.str());
	}
	else if (Maininfos.st_mode & S_IFDIR)  // S_ISDIR() doesn't exist on my windows 
	{
		//printf("%s Dir Exist!\n", path);

		pCvar->ConsoleColorPrintf(ColorRGBA(255, 255, 255),
			XorStr("\n=====\nConfig already exist loaded! \n %s\n=====\n"), path.str());

		Settings::LoadGrenadeInfo(path.str());
	}
	else
	{
		pCvar->ConsoleColorPrintf(ColorRGBA(255, 255, 255),
			XorStr("\n=====\nConfig doesn't exist! \n %s\n=====\n"), path.str());

		Settings::GrenadeHelper::grenadeInfos = {};
	}
}

void GrenadeHelper::CreateMove(CUserCmd* cmd)
{
	if (!Settings::GrenadeHelper::enabled)
		return;

	GrenadeHelper::CheckForUpdate();
	GrenadeHelper::AimAssist(cmd);
	shotLastTick = cmd->buttons & IN_ATTACK;
}
