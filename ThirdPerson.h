#pragma once

namespace ThirdPerson
{
	//void BeginFrame();
	void OverrideView(ViewSetup* setup);

	void FrameStageNotify(ClientFrameStage_t stage);

	void DrawModelExecute(IMatRenderContext* matctx, const DrawModelState_t& state, const ModelRenderInfo_t& pInfo, matrix3x4_t* pCustomBoneToWorld);
}