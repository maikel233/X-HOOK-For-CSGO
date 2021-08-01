#pragma once

namespace SpeedIndicator {
	//Hooks
	void Paint();
	void RenderView(void* thisptr, CViewSetup& setup, CViewSetup& hudViewSetup, unsigned int nClearFlags, int whatToDraw);
}