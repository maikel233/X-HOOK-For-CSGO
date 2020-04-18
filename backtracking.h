#pragma once

//#include <deque>
//#include <map>
//#include "interfaces.h"
//#include "settings.h"
//#include "CTickRecord.h"

namespace Backtracking {

	const int MAX_QUEUE_SIZE = 16;

	extern std::map<int, std::deque<CTickRecord>> lagRecords; // PlayerID, TickRecord
	

	extern bool backtrackingLby;

	void RestorePosition(int playerIndex, int tickDiff);

	void ToggleRequiredCVars(bool activate = true);

	void FrameStageNotify(ClientFrameStage_t stage);
	void Createmove(CUserCmd* cmd);

	bool IsTickValid(int tick);
	float GetLerpTime();


}

//

