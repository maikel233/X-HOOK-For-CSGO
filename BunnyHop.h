#pragma once

namespace BHop
{
	void CreateMove(CUserCmd* cmd);
	void CreateMoveCircle(CUserCmd* cmd);
	bool doCircularStrafe(CUserCmd * pCmd, Vector & OriginalView);
	void RotateMovement(CUserCmd * pCmd, float rotation);
}