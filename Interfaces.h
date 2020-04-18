#pragma once
#include <Windows.h>
#include "SDK\SDK.h"
#include "SDK\VMTHook.h"
extern IBaseClientDLL*			pClient;
extern IInputSystem*			pInputSystem;
extern IInputInternal*			pInputInternal;
extern IEngineClient*			pEngine;
extern IClientEntityList*		pEntityList;
extern CClientState*			pClientState;
extern INetChannelInfo*			net_channel;
//extern CLatancy				pLatancy;
extern IClientMode*			pClientMode;
extern ISurface*				pSurface;
extern IVDebugOverlay*			pDebugOverlay;
extern IVPanel*				pPanel;
extern IEngineTrace*			pTrace;
extern ILocalize*				pLocalize;
extern IMaterialSystem*		pMaterial;
extern IVModelRender*			pModelRender;
extern IPhysicsSurfaceProps*	pPhysics;
extern CEffects*				pEffects;
extern IVModelInfo*			pModelInfo;
extern ICvar*					pCvar;
extern CGlobalVars*			pGlobalVars;
extern IGameEventManager2*		pGameEvent;
extern CSGameRulesProxy**		pGameRules;
extern C_CSPlayerResource** csPlayerResource;
extern IDirect3DDevice9*		pD3device;
extern CGameUI*				pGameui;
extern IPrediction*			pPrediction;
extern IGameMovement*			pGameMovement;
//extern ICommandLine*		pCommandline;
extern IMoveHelper*			pMoveHelper;
extern CInput*				pInput;
extern IEngineSound*		pSound;
extern CViewRender*			pViewRender;
extern CBaseHudChat * pChat;
extern CGlowObjectManager*			pGlowManager;
extern ISteamClient * pSteamClient;
extern ISteamMatchmaking * pSteamMatchmaking;
extern ISteamUser * pSteamUser;
extern ISteamFriends * pSteamFriends;



//extern CMoveData* g_MoveData;
//extern ICommandLine* commandline;
extern LPDIRECT3DTEXTURE9 BackgroundTexture;
extern LPDIRECT3DTEXTURE9 UsericoTexture;
extern LPDIRECT3DTEXTURE9 LogoicoTexture;
extern LPDIRECT3DTEXTURE9 PasswordicoTexture;

extern LPDIRECT3DTEXTURE9 wheelTexture;
extern LPDIRECT3DTEXTURE9 speedoTexture;
extern LPDIRECT3DTEXTURE9 needleTexture;
extern LPDIRECT3DTEXTURE9 gasTexture;
extern LPDIRECT3DTEXTURE9 oilTexture;
extern LPDIRECT3DTEXTURE9 rearmirrorTexture;

extern ImFont* Fonts;
extern DWORD_PTR** dwPresent;
extern DWORD_PTR* dwAddress;
extern DWORD dwTemp;
extern int* pPredSeed;

extern std::unique_ptr<VMTHook>D3D9Hook;
extern std::unique_ptr<VMTHook>ClientModeHook;
extern std::unique_ptr<VMTHook>PanelHook;
extern std::unique_ptr<VMTHook>ClientHook;
extern std::unique_ptr<VMTHook>FireEventHook;
extern std::unique_ptr<VMTHook>InputInternalHook;
extern std::unique_ptr<VMTHook>PredictionHook;
extern std::unique_ptr<VMTHook>ModelRenderHook;
extern std::unique_ptr<VMTHook>MaterialHook;
extern std::unique_ptr<VMTHook>SoundHook; 
extern std::unique_ptr<VMTHook>SurfaceHook;
extern std::unique_ptr<VMTHook>RenderViewHook;
extern std::unique_ptr<VMTHook>NetChannelHook;
extern std::unique_ptr<VMTHook>SteamHook;
extern std::unique_ptr<VMTHook>test;
//extern std::unique_ptr<VMTHook>SteamGameCoordinatorTable;
//extern CSX::Hook::VTable SteamGameCoordinatorTable;
//extern ISteamGameCoordinator* g_pSteamGameCoordinator;
//extern ISteamUser* g_pSteamUser;



extern HFont esp_font;


extern bool SendPacket;
extern bool bSendPackett;
extern Vector lastTickViewAngles;
extern Present_t Present;
extern DWORD KeyValues_KeyValues;
extern DWORD KeyValues_LoadFromBuffer;
extern RecvVarProxyFn fnSequenceProxyFn;
//extern CViewSetup mirror;

extern bool* s_bOverridePostProcessingDisable;