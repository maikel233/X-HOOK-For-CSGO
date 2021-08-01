#pragma once

#include "../sdk/SDK.h"

namespace Walkbot
{
		void CreateMove(CUserCmd* cmd);
		void update();
		void Refresh(std::vector<Vector> spots, bool save);
		void CheckForUpdate();
		void Clear();
}		

