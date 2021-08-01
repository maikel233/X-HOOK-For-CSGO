#pragma once
#include <Windows.h>
#include "SDK/SDK.h"
#include "Interfaces.h"
#include "Features.h"
#include "Renderer.h"
#include "ImGui\imgui.h"
#include "GUI/Menu.h"
#include "GUI/Windows/FPSWindow.h"
#include "Utils/Draw.h"
#include "Utils/recvproxyhook.h"
#include "Utils/Singleton.hpp"


typedef bool(__thiscall* SendLobbyChatMessage_t)(ISteamMatchmaking*, CSteamID, const void*, int);

typedef long(__stdcall* EndSceneFn)(IDirect3DDevice9* device);
typedef long(__stdcall* ResetFn)(IDirect3DDevice9* device, D3DPRESENT_PARAMETERS* pp);
typedef long(__stdcall* PresentFn)(IDirect3DDevice9* device, const RECT* pSourceRect, const RECT* pDestRect, HWND hDestWindowOverride, const RGNDATA* pDirtyRegion);
typedef bool(__thiscall* CreateMoveFn)(IClientMode*, float, CUserCmd*);
typedef void(__thiscall* RunCommand_t)(IPrediction*, IClientEntity*, CUserCmd*, void*);
typedef void(__thiscall* PaintTraverseFn)(void*, unsigned int, bool, bool);
typedef void(__thiscall* DrawModelExecuteFn)(void*, void*, const DrawModelState_t&, const ModelRenderInfo_t&, matrix3x4_t*);
typedef void(__thiscall* FrameStageNotifyFn)(void*, ClientFrameStage_t);

typedef bool(__thiscall* FireEventClientSideFn)(void*, IGameEvent*);

typedef bool(__thiscall* FireEventFn)(void*, IGameEvent*, bool);
/* Internal Input */
typedef void(__thiscall* SetKeyCodeStateFn)(void*, ButtonCode_t, bool);
typedef void(__thiscall* SetMouseCodeStateFn)(void*, ButtonCode_t, MouseCodeState_t);

typedef void(__thiscall* SetSteamFn)(void*, uint32_t* punMsgType, void* pubDest, uint32_t cubDest, uint32_t* pcubMsgSize); //steamhook
typedef void(__thiscall* SetSteamFn2)(void*, uint32_t unMsgType, const void* pubData, uint32_t cubData); //steamhook

/* Material */
typedef void(__thiscall* BeginFrameFn) (void*, float);
typedef bool(__thiscall* OverrideConfigFn)(void*, MaterialSystem_Config_t*, bool);
/* Surface */
typedef void(*OnScreenSizeChangedFn)(void*, int, int);
typedef void(__thiscall* PlaySoundFn)(ISurface*, const char*);

typedef void(__thiscall* LockCursor)(void*);

typedef int(__thiscall* SendDatagramFn)(void*, void*);




/* ViewRender */

//typedef void(__thiscall* RenderViewFn)(void*, CViewSetup&, CViewSetup&, int, int);
typedef void(__thiscall* RenderViewFn)(void*, CViewSetup&, CViewSetup&, int, int);
typedef void(__thiscall* NoSmokeFn)(void*);
typedef void(__thiscall* OverreideViewFn)(void*, ViewSetup*);
typedef float(__thiscall* GetViewModelFOVFn)();


typedef void(__thiscall* EmitSound1Fn)(void*, IRecipientFilter&, int, int, const char*, unsigned int, const char*, float, int,
	float, int, int, const Vector*, const Vector*, void*, bool, float, int, int);

typedef void(__thiscall* EmitSound2Fn)(void*, void*, int, int, const char*, unsigned int, const char*, float, int,
	int, int, int, const Vector*, const Vector*, void*, bool, float, int, void*);

typedef bool(*ShouldDrawFogFn)(void*);
namespace Hooks
{

	bool HookRecvProp(const char* className, const char* propertyName, std::unique_ptr<RecvPropHook>& recvPropHook);
	bool ShouldDrawFog(void* thisptr);
	HRESULT WINAPI hPresent(IDirect3DDevice9* pDevice, RECT* pSourceRect, RECT* pDestRect, HWND hDestWindowOverride, RGNDATA* pDirtyRegion);
	HRESULT WINAPI hReset(LPDIRECT3DDEVICE9 pDevice, D3DPRESENT_PARAMETERS* pPresentationParameters);
	HRESULT __stdcall hkEndScene(IDirect3DDevice9* device);


	bool __stdcall hCreateMove(float frametime, CUserCmd* cmd);
	void __stdcall hPaintTraverse(unsigned int VGUIPanel, bool forcerepaint, bool allowforce);
	void __stdcall hDrawModelExecute(IMatRenderContext* matctx, const DrawModelState_t& state, const ModelRenderInfo_t& pInfo, matrix3x4_t* pCustomBoneToWorld);
	bool __stdcall Hooked_SendLobbyChatMessage(CSteamID steamIdLobby, const void* pvMsgBody, int cubMsgBody);
	void __fastcall hFrameStageNotify(void* ecx, void* edx, ClientFrameStage_t Stage);
	//bool __fastcall hWriteUsercmdDeltaToBuffer(void* ecx, void* edx, int slot, void* buffer, int from, int to, bool isnewcommand) noexcept;
	bool __fastcall hFireEventClientSide(void* ecx, void* edx, IGameEvent* pEvent);

	/* Internal Input */
	void __fastcall SetKeyCodeState(void* thisptr, void* edx, ButtonCode_t code, bool bPressed);
	void __fastcall SetMouseCodeState(void* thisptr, void* edx, ButtonCode_t code, MouseCodeState_t state);
	/*                */

	void __stdcall hBeginFrame(float frameTime);
	bool __fastcall hkOverrideConfig(IMaterialSystem* this0, int edx, MaterialSystem_Config_t* config, bool forceUpdate);

	void __fastcall hkEmitSound(IEngineSound* thisptr, int edx, IRecipientFilter& filter, int iEntIndex, int iChannel, const char* pSoundEntry, unsigned int nSoundEntryHash, const char* pSample,
		float flVolume, float flAttenuation, int nSeed, int iFlags, int iPitch,
		const Vector* pOrigin, const Vector* pDirection, CUtlVector< Vector >* pUtlVecOrigins, bool bUpdatePositions, float soundtime, int speakerentity, int Unknown);

	void __fastcall EmitSound1(void* thisptr, void* edx, IRecipientFilter& filter, int iEntIndex, int iChannel, const char* pSoundEntry,
		unsigned int nSoundEntryHash, const char* pSample, float flVolume, int nSeed, float flAttenuation,
		int iFlags, int iPitch, const Vector* pOrigin, const Vector* pDirection, void* pUtlVecOrigins,
		bool bUpdatePositions, float soundtime, int speakerentity, int Unknown);

	void __fastcall EmitSound2(void* thisptr, void* edx, void* filter, int iEntIndex, int iChannel, const char* pSoundEntry,
		unsigned int nSoundEntryHash, const char* pSample, float flVolume, int nSeed,
		int iSoundLevel, int iFlags, int iPitch, const Vector* pOrigin, const Vector* pDirection,
		void* pUtlVecOrigins, bool bUpdatePositions, float soundtime, int speakerentity, void* SoundParam);


	EGCResult __fastcall hkGCRetrieveMessage(void* ecx, void*, uint32_t* punMsgType, void* pubDest, uint32_t cubDest, uint32_t* pcubMsgSize);
	EGCResult __fastcall hkGCSendMessage(void* ecx, void*, uint32_t unMsgType, const void* pubData, uint32_t cubData);

	void OnScreenSizeChanged(void* thisptr, int oldwidth, int oldheight);
	void __stdcall hPlaySounds(const char* fileName);
	void __stdcall hLockCursor();
	void __fastcall hRenderView(void* ecx, void* edx, CViewSetup& setup, CViewSetup& hudViewSetup, int nClearFlags, int whatToDraw);
	void __fastcall  RenderSmokePostViewmodel(void* ecx, void* edx);
	void __fastcall hOverrideView(void* _this, void* _edx, ViewSetup* setup);
	float __stdcall hGetViewModelFOV();
}

namespace SetKeyCodeState {
	extern bool shouldListen;
	extern ButtonCode_t* keyOutput;
}
