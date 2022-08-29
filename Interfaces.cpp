#include "Interfaces.h"

IBaseClientDLL*			pClient;
IInputSystem*			pInputSystem;

IInputInternal*			pInputInternal;
IEngineClient*			pEngine;
IClientEntityList*		pEntityList;
CClientState*			pClientState;
NetworkChannel*			net_channel;
IClientMode*			pClientMode;
ISurface*				pSurface;
IVDebugOverlay*			pDebugOverlay;
IVPanel*				pPanel;
IEngineTrace*			pTrace;
ILocalize*				pLocalize;
IMaterialSystem*		pMaterial;
IVModelRender*			pModelRender;
IPhysicsSurfaceProps*	pPhysics;
CEffects*				pEffects;
IVModelInfo*			pModelInfo;
ICvar*					pCvar;
CGlobalVars*			pGlobalVars;
IGameEventManager2*		pGameEvent;
CSGameRulesProxy**		pGameRules;
C_CSPlayerResource** csPlayerResource;
IDirect3DDevice9*		pD3device;
CGameUI*				pGameui;
IPrediction*			pPrediction;
IGameMovement*			pGameMovement;
//ICommandLine*           pCommandline;
IMoveHelper*			pMoveHelper;
CInput*					pInput;
IEngineSound*			pSound;
CViewRender*			pViewRender;
CGlowObjectManager*			pGlowManager;
ISteamGameCoordinator* pSteamGameCoordinator;

CBaseHudChat * pChat;
//ISteamUser* g_pSteamUser;

MemAlloc* pMemAlloc;


ISteamClient * pSteamClient;
ISteamMatchmaking * pSteamMatchmaking;
ISteamUser * pSteamUser;
ISteamFriends * pSteamFriends;


LPDIRECT3DTEXTURE9 BackgroundTexture = NULL;
LPDIRECT3DTEXTURE9 UsericoTexture = NULL;
LPDIRECT3DTEXTURE9 LogoicoTexture = NULL;
LPDIRECT3DTEXTURE9 PasswordicoTexture = NULL;

LPDIRECT3DTEXTURE9 wheelTexture = NULL;
LPDIRECT3DTEXTURE9 speedoTexture = NULL;
LPDIRECT3DTEXTURE9 needleTexture = NULL;
LPDIRECT3DTEXTURE9 gasTexture = NULL;
LPDIRECT3DTEXTURE9 oilTexture = NULL;
LPDIRECT3DTEXTURE9 rearmirrorTexture = NULL;


ImFont * Fonts = nullptr; // 
DWORD_PTR** dwPresent;
DWORD_PTR* dwAddress;
DWORD dwTemp;
int* pPredSeed;

std::unique_ptr<VMTHook>D3D9Hook;
std::unique_ptr<VMTHook>ClientModeHook;
std::unique_ptr<VMTHook>PanelHook;
std::unique_ptr<VMTHook>ClientHook;
std::unique_ptr<VMTHook>FireEventHook;
std::unique_ptr<VMTHook>InputInternalHook;
std::unique_ptr<VMTHook>PredictionHook;
std::unique_ptr<VMTHook>ModelRenderHook;
std::unique_ptr<VMTHook>MaterialHook;
std::unique_ptr<VMTHook>SoundHook;
std::unique_ptr<VMTHook>SurfaceHook;
std::unique_ptr<VMTHook>RenderViewHook;
//std::unique_ptr<VMTHook>NetChannelHook;
std::unique_ptr<VMTHook>SteamHook;
std::unique_ptr<VMTHook>SteamGameCoordinator;
std::unique_ptr<VMTHook>NetDataGram;

//std::unique_ptr<VMTHook>gameCoordinator;

//CSX::Hook::VTable SteamGameCoordinatorTable;


HFont esp_font;

bool SendPacket;
bool bSendPackett;
Vector lastTickViewAngles;

Present_t Present;

DWORD KeyValues_KeyValues;
DWORD KeyValues_LoadFromBuffer;



RecvVarProxyFn fnSequenceProxyFn;


bool* s_bOverridePostProcessingDisable = nullptr;

//CViewSetup mirror;
class MemAlloc;

void* WriteUsercmdDeltaToBufferReturn;
uintptr_t WriteUserCmds;


