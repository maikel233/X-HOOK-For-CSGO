#pragma once

namespace Chams {

	extern IMaterial* chamsMaterial;
	extern IMaterial* ignoredChamsMaterial;

	void DrawModelExecute(IMatRenderContext* matctx, const DrawModelState_t& state, const ModelRenderInfo_t& pInfo, matrix3x4_t* pCustomBoneToWorld);
}
