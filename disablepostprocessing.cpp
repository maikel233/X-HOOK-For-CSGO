#include "Features.h"


bool Settings::DisablePostProcessing::enabled = false;


void DisablePostProcessing::BeginFrame()
{   

	if (!Settings::DisablePostProcessing::enabled)
		return;

		auto postprocessing = pCvar->FindVar("mat_postprocess_enable");
		auto postprocessingspoof = new SpoofedConvar(postprocessing);
		postprocessingspoof->SetInt(0);

}
