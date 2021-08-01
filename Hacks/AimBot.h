#pragma once

namespace Aimbot
{
	extern bool aimStepInProgress;
	extern std::vector<int64_t> friends;
	extern std::vector<int64_t> baim;
	extern int targetAimbot;
	extern bool useAbTarget;

	extern bool shootingRevolver;

	bool HitChance(const Vector& point, bool teamCheck, C_BasePlayer* localplayer);

	void CreateMove(CUserCmd* cmd);
	void FireGameEvent(IGameEvent* event);
	void UpdateValues();
}

