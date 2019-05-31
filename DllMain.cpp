#include "DllMain.h"
#include "Renderer.h"
#include <Windows.h>
#include <tchar.h>
#include "Settings.h"
#include "HWID.h"
#include "Util.h"
#include <wininet.h>
#include "Features.h"
#include "Config.h"
#include "IRC.h"
#define XHOOKCURRGAME "Counter-Strike: Global Offensive"
#define XHOOKCURRGAMEX "X-HOOK for Counter-Strike: Global Offensive"
#define XHOOKCURRVERSION "V1.32"

HMODULE g_hDll;


static bool bVisible;




DWORD WINAPI Stage1(LPVOID hInstance)
{
	pGlowManager = *(CGlowObjectManager **)(FindPattern("client_panorama.dll", "\x0F\x11\x05\x00\x00\x00\x00\x83\xC8\x01\xC7\x05\x00\x00\x00\x00\x00\x00\x00\x00", "xxx????xxxxx????xxxx") + 3);

	MsgFunc_ServerRankRevealAll = reinterpret_cast<MsgFunc_ServerRankRevealAllFn>(FindPatternV2("client_panorama.dll", "55 8B EC 8B 0D ? ? ? ? 85 C9 75 ? A1 ? ? ? ? 68 ? ? ? ? 8B 08 8B 01 FF 50 ? 85 C0 74 ? 8B C8 E8 ? ? ? ? 8B C8 EB ? 33 C9 89 0D ? ? ? ? 8B 45 ? FF 70 ? E8 ? ? ? ? B0 ? 5D"));
	//MsgFunc_ServerRankRevealAll = reinterpret_cast<MsgFunc_ServerRankRevealAllFn>(FindPattern("client_panorama.dll", "\x55\x8B\xEC\x83\xEC\x0C\xC7\x45\x00\x00\x00\x00\x00\8D", "xxxxxxxx?????x"));
	IsReadyCallback = reinterpret_cast < IsReady > ((DWORD)FindPatternV2("client_panorama.dll", "55 8B EC 83 E4 F8 8B 4D 08 BA ? ? ? ? E8 ? ? ? ? 85 C0 75 12"));          //FindPattern("client_panorama.dll", "\x55\x8B\xEC\x83\xE4\xF8\x83\xEC\x08\x56\x8B\x35\x00\x00\x00\x00\x57\x83\xBE", "xxxxxxxxxxxx????xxx"));
				
/*	pViewRender = **reinterpret_cast<CViewRender***>(FindPattern("client_panorama.dll", "\xFF\x50\x14\xE8\x00\x00\x00\x00\x5D", "xxxx????x") - 7);			*/																																						   
	pViewRender = **reinterpret_cast<CViewRender***>(FindPatternV2("client_panorama.dll", "FF 50 14 E8 ? ? ? ? 5D") - 7);

//	KeyValues_KeyValues = FindPattern("client_panorama.dll", "\x68\x00\x00\x00\x00\x8B\xC8\xE8\x00\x00\x00\x00\x89\x45\xFC\xEB\x07\xC7\x45\x00\x00\x00\x00\x00\x8B\x03\x56", "x????xxx????xxxxxxx?????xxx");
	KeyValues_KeyValues = FindPatternV2("client_panorama.dll", "55 8B EC 51 33 C0 C7 45");
	KeyValues_LoadFromBuffer = FindPatternV2("client_panorama.dll", "55 8B EC 83 E4 F8 83 EC 34 53 8B 5D 0C 89 4C 24 04");
	pPredSeed = *(int**)(FindPattern("client_panorama.dll", "\x8B\x0D\x00\x00\x00\x00\xBA\x00\x00\x00\x00\xE8\x00\x00\x00\x00\x83\xC4\x04", "xx????x????x????xxx") + 2);



	dwPresent = (DWORD_PTR**)(FindPattern("gameoverlayrenderer.dll", "\xA3\x00\x00\x00\x00\x68\x00\x00\x00\x00\xFF\x76\x54\xE8\x00\x00\x00\x00\x83\xC4\x08\x84\xC0\x75\x17", "x????x????xxxx????xxxxxxx") + 1);
	pClientState = *(CClientState**)(FindPattern("engine.dll", "\x8B\x3D\x00\x00\x00\x00\x8A\xF9", "xx????xx") + 2); pClientState = *(CClientState**)(pClientState);

	pGameRules = *(CSGameRulesProxy***)(FindPattern("client_panorama.dll", "\xA1\x00\x00\x00\x00\x85\xC0\x0F\x84\x00\x00\x00\x00\x80\xB8\x00\x00\x00\x00\x00\x0F\x84\x00\x00\x00\x00\x0F\x10\x05", "x????xxxx????xx?????xx????xxx") + 0x1);
	csPlayerResource = *(C_CSPlayerResource***)(FindPattern("client_panorama.dll", "\xA1\x00\x00\x00\x00\x57\x85\xC0\x74\x08", "x????xxxxx") + 1);
	pD3device = **reinterpret_cast<IDirect3DDevice9***>(FindPattern("shaderapidx9.dll", "\xA1\x00\x00\x00\x00\x50\x8B\x08\xFF\x51\x0C", "x????xxxxxx") + 1);

	pMoveHelper = **(IMoveHelper***)(FindPattern("client_panorama.dll", "\x8B\x0D\x00\x00\x00\x00\x8B\x45\x00\x51\x8B\xD4\x89\x02\x8B\x01", "xx????xx?xxxxxxx") + 2);
	pGameMovement = (IGameMovement*)(CreateInterfaceFn(GetProcAddress(GetModuleHandleA("client_panorama.dll"), "CreateInterface"))	("GameMovement001", nullptr));
	pPrediction = (IPrediction*)(CreateInterfaceFn(GetProcAddress(GetModuleHandleA("client_panorama.dll"), "CreateInterface"))	("VClientPrediction001", nullptr));
	pInputSystem = (IInputSystem*)(CreateInterfaceFn(GetProcAddress(GetModuleHandleA("inputsystem.dll"), "CreateInterface"))	("InputSystemVersion001", nullptr));
	pSurface = (ISurface*)(CreateInterfaceFn(GetProcAddress(GetModuleHandleA("vguimatsurface.dll"), "CreateInterface"))		("VGUI_Surface031", nullptr));
	pInputInternal = (IInputInternal*)(CreateInterfaceFn(GetProcAddress(GetModuleHandleA("vgui2.dll"), "CreateInterface")) ("VGUI_InputInternal001", nullptr)); //Vgui_inputinte db 'VGUI_InputInternal001',0
	pPanel = (IVPanel*)(CreateInterfaceFn(GetProcAddress(GetModuleHandleA("vgui2.dll"), "CreateInterface"))		("VGUI_Panel009", nullptr));
	pPhysics = (IPhysicsSurfaceProps*)(CreateInterfaceFn(GetProcAddress(GetModuleHandleA("vphysics.dll"), "CreateInterface"))	("VPhysicsSurfaceProps001", nullptr));
	pMaterial = (IMaterialSystem*)(CreateInterfaceFn(GetProcAddress(GetModuleHandleA("materialsystem.dll"), "CreateInterface"))		("VMaterialSystem080", nullptr));
	pCvar = (ICvar*)(CreateInterfaceFn(GetProcAddress(GetModuleHandleA("materialsystem.dll"), "CreateInterface"))		("VEngineCvar007", nullptr));
	pGameui = (CGameUI*)(CreateInterfaceFn(GetProcAddress(GetModuleHandleA("client_panorama.dll"), "CreateInterface"))		("GameUI011", nullptr));
	pClient = (IBaseClientDLL*)(CreateInterfaceFn(GetProcAddress(GetModuleHandleA("client_panorama.dll"), "CreateInterface"))		("VClient018", nullptr));
	pEntityList = (IClientEntityList*)(CreateInterfaceFn(GetProcAddress(GetModuleHandleA("client_panorama.dll"), "CreateInterface"))		("VClientEntityList003", nullptr));
	pTrace = (IEngineTrace*)(CreateInterfaceFn(GetProcAddress(GetModuleHandleA("engine.dll"), "CreateInterface"))		("EngineTraceClient004", nullptr));
	pModelInfo = (IVModelInfo*)(CreateInterfaceFn(GetProcAddress(GetModuleHandleA("engine.dll"), "CreateInterface"))		("VModelInfoClient004", nullptr));
	pModelRender = (IVModelRender*)(CreateInterfaceFn(GetProcAddress(GetModuleHandleA("engine.dll"), "CreateInterface"))		("VEngineModel016", nullptr));
	pEngine = (IEngineClient*)(CreateInterfaceFn(GetProcAddress(GetModuleHandleA("engine.dll"), "CreateInterface"))		("VEngineClient014", nullptr));
	pDebugOverlay = (IVDebugOverlay*)(CreateInterfaceFn(GetProcAddress(GetModuleHandleA("engine.dll"), "CreateInterface"))		("VDebugOverlay004", nullptr));
	pGameEvent = (IGameEventManager2*)(CreateInterfaceFn(GetProcAddress(GetModuleHandleA("engine.dll"), "CreateInterface"))		("GAMEEVENTSMANAGER002", nullptr));
	pLocalize = (ILocalize*)(CreateInterfaceFn(GetProcAddress(GetModuleHandleA("localize.dll"), "CreateInterface"))		("Localize_001", nullptr));
	pEffects = (CEffects*)(CreateInterfaceFn(GetProcAddress(GetModuleHandleA("engine.dll"), "CreateInterface"))		("VEngineEffects001", nullptr));
	pSound = (IEngineSound*)(CreateInterfaceFn(GetProcAddress(GetModuleHandleA("engine.dll"), "CreateInterface"))		("IEngineSoundClient003", nullptr));
	offsets.sigs.MaterialGameEnded = FindPatternV2("materialsystem.dll", "80 B9 ? ? ? ? ? 74 0F" + 2);
	offsets.sigs.LoadSky = FindPatternV2("engine.dll", "55 8B EC 81 EC ? ? ? ? 56 57 8B F9 C7 45");
	offsets.sigs.LineGoesThroughSmoke = FindPatternV2("client_panorama.dll", "55 8B EC 83 EC 08 8B 15 ? ? ? ? 0F 57 C0");
	offsets.sigs.PaintkitSig = FindPatternMask("client_panorama.dll", (BYTE*)"\xE8\x00\x00\x00\x00\xFF\x76\x0C\x8D\x48\x04\xE8", "x????xxxxxxx");
	offsets.sigs.StickerSig = FindPatternMask("client_panorama.dll", (BYTE*)"\x53\x8D\x48\x04\xE8\x00\x00\x00\x00\x8B\x4D\x10", "xxxxx????xxx") + 4;
	offsets.sigs.LevelInit = FindPatternV2("client_panorama.dll", "E8 ? ? ? ? 8B 4F ? 85 C9 74 06 51") + 7;
	//	
	pClientMode = **(IClientMode***)((*(DWORD**)pClient)[10] + 0x5);
	pGlobalVars = **(CGlobalVars***)((*(DWORD**)pClient)[0] + 0x1B);

	////DWORD ClientState = (DWORD)*(DWORD*)pClientState;
	//if (pClientState)
	//{
	//	INetChannel *netchan = (INetChannel*)*(DWORD*)(pClientState + 0x9C);
	//	net_channel = netchan;
	//}

	/*auto clientState = *reinterpret_cast<uintptr_t*>(uintptr_t(GetModuleHandle("engine.dll")) + 0x588B34);

	auto intnetchan = *reinterpret_cast<uintptr_t*>(clientState + 0x9C);
	net_channel = reinterpret_cast<INetChannel*>(intnetchan);*/


//	net_channel = *reinterpret_cast< INetChannel** >(reinterpret_cast< std::uintptr_t >(pClientState) + 0x9C);


	pInput = *(CInput**)(FindPattern("client_panorama.dll", "\xB9\x00\x00\x00\x00\xF3\x0F\x11\x04\x24\xFF\x50\x10", "x????xxxxxxxx") + 1);
	NetVarManager::DumpNetvars();
	Offsets::GetOffsets();

	Settings::LoadSettings();

	esp_font = pSurface->CreateFontA();
	pSurface->SetFontGlyphSet(esp_font, "Helvetica", 13, 500, 0, 0, (int)FontFlags::FONTFLAG_ANTIALIAS);

	renderer.Initialize(FindWindowA(XorStr("Valve001"), NULL), pD3device);

	D3D9Hook = std::make_unique<VMTHook>(pD3device);
	D3D9Hook->HookFunction(Hooks::hReset, 16);
	D3D9Hook->HookFunction(Hooks::hkEndScene, 42);
	
	//
	dwAddress = *dwPresent;
	//Present = (Present_t)(*dwAddress);
	//*dwAddress = (DWORD_PTR)(Hooks::hPresent);
	
	PanelHook = std::make_unique<VMTHook>(pPanel);
	PanelHook->HookFunction(Hooks::hPaintTraverse, 41);

	//
	SurfaceHook = std::make_unique<VMTHook>(pSurface);
	SurfaceHook->HookFunction(Hooks::hLockCursor, 67);
	//	SurfaceHook->HookFunction(Hooks::hPlaySounds, 82);
	//	//SurfaceHook->HookFunction(Hooks::OnScreenSizeChanged, 116);
	//
	MaterialHook = std::make_unique<VMTHook>(pMaterial);
	MaterialHook->HookFunction(Hooks::hkOverrideConfig, 21); //21
	MaterialHook->HookFunction(Hooks::hBeginFrame, 42);
	
	SoundHook = std::make_unique<VMTHook>(pSound);
	SoundHook->HookFunction(Hooks::EmitSound1, 5);
	SoundHook->HookFunction(Hooks::EmitSound2, 5);
//
	RenderViewHook = std::make_unique<VMTHook>(pViewRender);
	RenderViewHook->HookFunction(Hooks::hRenderView, 6);
	RenderViewHook->HookFunction(Hooks::RenderSmokePostViewmodel, 41);

	ModelRenderHook = std::make_unique<VMTHook>(pModelRender);
	ModelRenderHook->HookFunction(Hooks::hDrawModelExecute, 21);


	ClientModeHook = std::make_unique<VMTHook>(pClientMode);
	ClientModeHook->HookFunction((void*)Hooks::ShouldDrawFog, 17);
	ClientModeHook->HookFunction(Hooks::hOverrideView, 18);
	ClientModeHook->HookFunction(Hooks::hCreateMove, 24);
	ClientModeHook->HookFunction(Hooks::hGetViewModelFOV, 35);
//
//	//NetChannelHook = std::make_unique<VMHook>(pClientState->m_pNetChannel);
//	//NetChannelHook->HookVM(Hooks::SendDatagram, 46);

	
//
	ClientHook = std::make_unique<VMTHook>(pClient);
	ClientHook->HookFunction(Hooks::hFrameStageNotify, 37);

	FireEventHook = std::make_unique<VMTHook>((DWORD**)pGameEvent);
//	FireEventHook->HookFunction(Hooks::FireEvent, 8);  8 or 51
	FireEventHook->HookFunction(Hooks::hFireEventClientSide, 9);

	InputInternalHook = std::make_unique<VMTHook>(pInputInternal);
	InputInternalHook->HookFunction(Hooks::SetKeyCodeState, 91);
	InputInternalHook->HookFunction(Hooks::SetMouseCodeState, 92);


	if (ModSupport::current_mod != ModType::CSCO &&
	Hooks::HookRecvProp(XorStr("DT_BaseViewModel"), XorStr("m_nSequence"), SkinChanger::sequenceHook) )
	SkinChanger::sequenceHook->SetProxyFunction((RecvVarProxyFn)SkinChanger::SetViewModelSequence);
	

	
		//#if defined(Release) // Crashes on debug
		//pEngine->ExecuteClientCmd("clear");
	//	//pEngine->ClientCmd_Unrestricted("toggleconsole");
	//	//#endif 
	//
		const char *buildString = "This build was compiled at " __DATE__ ", " __TIME__ ".";
	//	int EngineBuild = pEngine->GetEngineBuildNumber();
		pCvar->ConsoleColorPrintf(ColorRGBA(0, 162, 232), XorStr  ("####################################################################\n"));
		pCvar->ConsoleColorPrintf(ColorRGBA(0, 162, 232), XorStr("####"));
		pCvar->ConsoleColorPrintf(ColorRGBA(0, 255, 0), XorStr("[X-HOOK] %s     \n"), buildString); //, __DATE__, __TIME__));

		pCvar->ConsoleColorPrintf(ColorRGBA(0, 162, 232), XorStr("####"));
		pCvar->ConsoleColorPrintf(ColorRGBA(0, 255, 0), XorStr("[X-HOOK] We're Hooked!\n"));
		pCvar->ConsoleColorPrintf(ColorRGBA(0, 162, 232), XorStr("####"));
		pCvar->ConsoleColorPrintf(ColorRGBA(0, 255, 0), XorStr("[X-HOOK] No login required. Enjoy :)\n")); //Please login to continue
		pCvar->ConsoleColorPrintf(ColorRGBA(0, 162, 232), XorStr  ("#####################################################################\n"));


	ExitThread(0);
}

DWORD WINAPI Stage2(LPVOID hInstance)
{
	std::this_thread::sleep_for(std::chrono::milliseconds(10000));
	while (pInputSystem && !pInputSystem->IsButtonDown(KEY_END))
		std::this_thread::sleep_for(std::chrono::milliseconds(1000));

	if (Open)
		FreeConsole();

	pCvar->FindVar(XorStr("cl_mouseenable"))->SetValue(1);
	Backtracking::ToggleRequiredCVars(false);

	NoSmoke::Cleanup();

	std::this_thread::sleep_for(std::chrono::milliseconds(100));
	D3D9Hook->UnhookFunction(16);
	std::this_thread::sleep_for(std::chrono::milliseconds(100));
	D3D9Hook->UnhookFunction(42);

	std::this_thread::sleep_for(std::chrono::milliseconds(100));
	PanelHook->UnhookFunction(41);

	//std::this_thread::sleep_for(std::chrono::milliseconds(100));
	//SurfaceHook->UnhookFunction(82);

	//std::this_thread::sleep_for(std::chrono::milliseconds(100));
	//SurfaceHook->UnhookFunction(116);

	std::this_thread::sleep_for(std::chrono::milliseconds(100));
	MaterialHook->UnhookFunction(21);

	std::this_thread::sleep_for(std::chrono::milliseconds(100));
	MaterialHook->UnhookFunction(41);

	std::this_thread::sleep_for(std::chrono::milliseconds(100));
	SoundHook->UnhookFunction(5);

	std::this_thread::sleep_for(std::chrono::milliseconds(100));
	SoundHook->UnhookFunction(5);

	std::this_thread::sleep_for(std::chrono::milliseconds(100));
	ClientModeHook->UnhookFunction(17);

	std::this_thread::sleep_for(std::chrono::milliseconds(100));
	ClientModeHook->UnhookFunction(18);

	std::this_thread::sleep_for(std::chrono::milliseconds(100));
	ClientModeHook->UnhookFunction(24);

	std::this_thread::sleep_for(std::chrono::milliseconds(100));
	ClientModeHook->UnhookFunction(35);

	std::this_thread::sleep_for(std::chrono::milliseconds(100));
	ClientHook->UnhookFunction(37);
	//
	std::this_thread::sleep_for(std::chrono::milliseconds(100));
	RenderViewHook->UnhookFunction(6);

	std::this_thread::sleep_for(std::chrono::milliseconds(100));
	ModelRenderHook->UnhookFunction(21);

	std::this_thread::sleep_for(std::chrono::milliseconds(100));
	FireEventHook->UnhookFunction(9);

	std::this_thread::sleep_for(std::chrono::milliseconds(100));
	InputInternalHook->UnhookFunction(91);

	std::this_thread::sleep_for(std::chrono::milliseconds(100));
	InputInternalHook->UnhookFunction(92);

	pInput->m_fCameraInThirdPerson = false;
	pInput->m_vecCameraOffset.z = 150.f;

	FreeLibraryAndExitThread(HINSTANCE(hInstance), 0);
}

BOOL WINAPI CreateThreads(HINSTANCE hInstance)
{
	return CreateThread(nullptr, 0, Stage1, hInstance, 0, nullptr) && CreateThread(nullptr, 0, Stage2, hInstance, 0, nullptr);
}

static bool Setups = true;

DWORD __stdcall HandleConfigs(void* args);
DWORD __stdcall Securitythread(void* args)
{
	while (true)
	{
		if (g_pSecurity->Trampoline(true, NULL))
		{
			if (Setups)
			{
				CreateThreads(g_hDll);
				Setups = false;
				CreateThread(nullptr, 0, HandleConfigs, g_hDll, 0, nullptr);//Handle configs outside of a gamethread to speed out threads up.			
			}
		}
		Sleep(24);
	}
}

bool MainFolderBool = false;
bool ConfigsFolderBool = false;
bool PictureFolderBool = false;
bool BackgroundBool = false;

struct stat Maininfo;
const char *Mainpath = "C://xhook//";

//Doesn't even work...
void MainThread(LPVOID lpArgs)
{
	if (stat(Mainpath, &Maininfo) != 0)
	{
		//printf("We cannot access %s Creating new files...\n", path);
		ConSys->CheckConfigs();
		CreateThread(nullptr, 0, Securitythread, g_hDll, 0, nullptr);
	}
	else if (Maininfo.st_mode & S_IFDIR)  // S_ISDIR() doesn't exist on my windows 
	{
		//printf("%s Dir Exist!\n", path);
		CreateThread(nullptr, 0, Securitythread, g_hDll, 0, nullptr);
	}
	else
	{
		//printf("%s Doesn't exist! Creating new files...\n", path);
		ConSys->CheckConfigs();
		CreateThread(nullptr, 0, Securitythread, g_hDll, 0, nullptr);
	}
}

DWORD __stdcall DeAttach(void* args)
{
	while (true)
	{

		if (GetAsyncKeyState(VK_LEFT) && GetAsyncKeyState(VK_UP) && GetAsyncKeyState(VK_RIGHT) & 1)
		{
			bVisible = !bVisible;
		}
		if (!g_pSecurity->RunSecurityChecks())
		{
			FreeConsole();
			FreeLibraryAndExitThread(static_cast<HMODULE>(args), NULL);
		}
		if (Settings::HWID::uninject && GetForegroundWindow() == FindWindowA(XorStr("Valve001"), XorStr("Valve001")))//make sure we are in CSGO and that our first statment is true
		{
			Sleep(2000);
			FreeConsole();
			FreeLibraryAndExitThread(static_cast<HMODULE>(args), NULL);
		}
		Sleep(24);
	}
}

DWORD __stdcall HandleConfigs(void* args)
{
	while (true)
	{
		//		IRC::Thread(); // Not required atm..
		//		Config::Handle();
		if (Settings::HWID::uninject)
			ExitThread(NULL);
		Sleep(24);
	}
}

BOOL __stdcall DllMain(HINSTANCE mod, DWORD dwReason, LPVOID res)
{
	switch (dwReason)
	{
	case DLL_PROCESS_ATTACH:
#if defined(Release)
		HideModule(mod);
		HideModuleFromPEB(mod);
		HideModuleXta(mod);
		RemovePeHeader(reinterpret_cast<DWORD>(mod));
		DestroyModuleHeader(mod);
		g_hDll = mod;
#endif 
		if (g_pSecurity->RunSecurityChecks())
		{
			_beginthread(MainThread, 0, nullptr);
			CreateThread(nullptr, 0, DeAttach, g_hDll, 0, nullptr);
			break;
		}
		else
		{

			Sleep(3000);
			break;
		}
	case DLL_PROCESS_DETACH:
		ExitThread(NULL);
		break;
	}
	return TRUE;
}

