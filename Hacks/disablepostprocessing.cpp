#include "../Features.h"


bool Settings::DisablePostProcessing::enabled = false;


void DisablePostProcessing::BeginFrame()
{   

	if (!Settings::ESP::enabled && Settings::DisablePostProcessing::enabled)
		return;

	static bool ToggleOnce = false;
	if (!ToggleOnce)
	{

		auto postprocessing = pCvar->FindVar("mat_postprocess_enable");
		auto postprocessingspoof = new SpoofedConvar(postprocessing);
		postprocessingspoof->SetInt(0);

		ToggleOnce = true;
	}

}
