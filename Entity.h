#pragma once
#include "../interfaces.h"

namespace Entity
{
	bool LineGoesThroughSmoke(Vector, Vector, int16_t);
	bool IsVisible(C_BasePlayer* player, Bone bone, float fov = 180.f, bool smoke_check = false);
	bool IsSpotVisible(C_BasePlayer* player, Vector spot, float fov = 180.f, bool smoke_check = false);
	bool IsVisibleThroughEnemies(C_BasePlayer *player, int bone, float fov = 180.f, bool smoke_check = false);
	bool IsSpotVisibleThroughEnemies(C_BasePlayer *player, Vector spot, float fov = 180.f, bool smoke_check = false);
	bool IsPlanting(C_BasePlayer* player);
	Bone GetBoneByName(C_BasePlayer* player, const char* boneName);
	
}