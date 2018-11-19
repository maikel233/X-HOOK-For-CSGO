#include "Features.h"
//
bool Settings::NoFog::enabled = false;
//
//bool NoFog::ShouldDrawFog(void* thisptr) {
//	if (!Settings::NoFog::enabled)
//		return ClientModeHook->GetOriginalFunction<ShouldDrawFogFn>(17)(thisptr); //clientModeVMT->GetOriginalMethod<ShouldDrawFogFn>(18)(thisptr);
//
//	/* Skybox Fog is separate */
//	IMatRenderContext *renderCtx = pMaterial->GetRenderContext();
//	renderCtx->FogMode(MaterialFogMode_t::MATERIAL_FOG_NONE);
//	renderCtx->Release();
//	/* Return false for normal fog */
//	return false; // uhhh, no Sweetie, don't draw that fog.
//}