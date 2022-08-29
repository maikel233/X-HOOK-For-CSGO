#include "DllMain.h"

using namespace std::chrono_literals;

#define WaitOnFind(EXP) \
	while ((EXP) == 0) { std::this_thread::sleep_for(0.1s); }

HMODULE g_hDll;
HMODULE SteamDLLHandle = nullptr;
static bool bVisible;
bool Settings::HWID::uninject = false;


DWORD WINAPI Stage1(LPVOID hInstance)
{ 
	WaitOnFind(SteamDLLHandle = GetModuleHandle("steam_api.dll"));
	pSteamClient = (ISteamClient*)(CreateInterfaceFn(GetProcAddress(GetModuleHandleA("steamclient.dll"), "CreateInterface"))	("SteamClient017", nullptr));
	auto GetHSteamPipe = reinterpret_cast<HSteamPipe(*)()>(GetProcAddress(SteamDLLHandle, "GetHSteamPipe"));
	auto GetHSteamUser = reinterpret_cast<HSteamUser(*)()>(GetProcAddress(SteamDLLHandle, "GetHSteamUser"));
	pSteamMatchmaking = pSteamClient->GetISteamMatchmaking(GetHSteamUser(), GetHSteamPipe(), "SteamMatchMaking009");
	pSteamFriends = pSteamClient->GetISteamFriends(GetHSteamUser(), GetHSteamPipe(), "SteamFriends015");
	pSteamUser = pSteamClient->GetISteamUsers(GetHSteamUser(), GetHSteamPipe(), "SteamUser019");
	pSteamGameCoordinator = (ISteamGameCoordinator*)pSteamClient->GetISteamGenericInterfaces(GetHSteamUser(), GetHSteamPipe(), "SteamGameCoordinator001"); // We gebruiken nog geen Virtual func
	pMemAlloc = *reinterpret_cast<MemAlloc**>(GetProcAddress(GetModuleHandleA("tier0.dll"), "g_pMemAlloc"));

	
	pGlowManager = *(CGlowObjectManager **)(FindPattern(CLIENT, "0F 11 05 ? ? ? ? 83 C8 01 C7 05 ? ? ? ? 00 00 00 00") + 3);
	MsgFunc_ServerRankRevealAll = reinterpret_cast<MsgFunc_ServerRankRevealAllFn>(FindPattern(CLIENT, "55 8B EC 51 A1 ? ? ? ? 85 C0 75 37"));
	IsReadyCallback = reinterpret_cast <IsReady> ((DWORD)FindPattern(CLIENT, "55 8B EC 83 E4 F8 83 EC 08 56 8B 35 ? ? ? ? 57 83 BE"));
																																						  		
	pPredSeed = *(int**)(FindPattern(CLIENT, "8B 0D ? ? ? ? BA ? ? ? ? E8 ? ? ? ? 83 C4 04") + 0x2);    

	pClientState = *(CClientState**)(FindPattern(ENGINE, "A1 ? ? ? ? 8B 80 ? ? ? ? C3") + 1);
	offsets.sigs.LoadSky = FindPattern(ENGINE, "55 8B EC 81 EC ? ? ? ? 56 57 8B F9 C7 45");
	offsets.sigs.LineGoesThroughSmoke = FindPattern(CLIENT, "55 8B EC 83 EC 08 8B 15 ? ? ? ? 0F 57 C0");
	offsets.sigs.SubmitReport = FindPattern(CLIENT, "55 8B EC 83 E4 F8 83 EC 28 8B 4D 08");

	pGameRules = *(CSGameRulesProxy***)(FindPattern(CLIENT, "E8 ? ? ? ? A1 ? ? ? ? 85 C0 0F 84 ? ? ? ? 80 B8 ? ? ? ? ? 74 7A") + 0x6);
	csPlayerResource = *(C_CSPlayerResource***)(FindPattern(CLIENT, "8B 3D ? ? ? ? 85 FF 0F 84 ? ? ? ? 81 C7") + 2);
	pD3device = **reinterpret_cast<IDirect3DDevice9***>(FindPattern("shaderapidx9.dll", "A1 ? ? ? ? 50 8B 08 FF 51 0C") + 0x1);
	


	pInput = *(CInput**)(FindPattern(CLIENT, "B9 ? ? ? ? F3 0F 11 04 24 FF 50 10") + 1);
	pMoveHelper = **(IMoveHelper***)(FindPattern(CLIENT, "8B 0D ? ? ? ? 8B 45 ? 51 8B D4 89 02 8B 01") + 2);
	pWeaponSystem = *(IWeaponSystem**)(FindPattern(CLIENT, "8B 35 ? ? ? ? FF 10 0F B7 C0") + 2);

	pGameMovement = (IGameMovement*)(CreateInterfaceFn(GetProcAddress(GetModuleHandleA(CLIENT), "CreateInterface"))	("GameMovement001", nullptr));
	pPrediction = (IPrediction*)(CreateInterfaceFn(GetProcAddress(GetModuleHandleA(CLIENT), "CreateInterface"))	("VClientPrediction001", nullptr));
	pGameui = (CGameUI*)(CreateInterfaceFn(GetProcAddress(GetModuleHandleA(CLIENT), "CreateInterface"))		("GameUI011", nullptr));
	pClient = (IBaseClientDLL*)(CreateInterfaceFn(GetProcAddress(GetModuleHandleA(CLIENT), "CreateInterface"))		("VClient018", nullptr));
	pEntityList = (IClientEntityList*)(CreateInterfaceFn(GetProcAddress(GetModuleHandleA(CLIENT), "CreateInterface"))		("VClientEntityList003", nullptr));

	pInputSystem = (IInputSystem*)(CreateInterfaceFn(GetProcAddress(GetModuleHandleA(INPUTSYSTEM), "CreateInterface"))	("InputSystemVersion001", nullptr));

	pSurface = (ISurface*)(CreateInterfaceFn(GetProcAddress(GetModuleHandleA(VGUIMATSURFACE), "CreateInterface"))		("VGUI_Surface031", nullptr));
	pInputInternal = (IInputInternal*)(CreateInterfaceFn(GetProcAddress(GetModuleHandleA(VGUI2), "CreateInterface")) ("VGUI_InputInternal001", nullptr)); //Vgui_inputinte db 'VGUI_InputInternal001',0
	pPanel = (IVPanel*)(CreateInterfaceFn(GetProcAddress(GetModuleHandleA(VGUI2), "CreateInterface"))		("VGUI_Panel009", nullptr));
	pPhysics = (IPhysicsSurfaceProps*)(CreateInterfaceFn(GetProcAddress(GetModuleHandleA(VPHYSICS), "CreateInterface"))	("VPhysicsSurfaceProps001", nullptr));

	pMaterial = (IMaterialSystem*)(CreateInterfaceFn(GetProcAddress(GetModuleHandleA(MATERIALSYSTEM), "CreateInterface"))		("VMaterialSystem080", nullptr));
	pCvar = (ICvar*)(CreateInterfaceFn(GetProcAddress(GetModuleHandleA(MATERIALSYSTEM), "CreateInterface"))		("VEngineCvar007", nullptr));
	
	pTrace = (IEngineTrace*)(CreateInterfaceFn(GetProcAddress(GetModuleHandleA(ENGINE), "CreateInterface"))		("EngineTraceClient004", nullptr));
	pModelInfo = (IVModelInfo*)(CreateInterfaceFn(GetProcAddress(GetModuleHandleA(ENGINE), "CreateInterface"))		("VModelInfoClient004", nullptr));
	pModelRender = (IVModelRender*)(CreateInterfaceFn(GetProcAddress(GetModuleHandleA(ENGINE), "CreateInterface"))		("VEngineModel016", nullptr));
	pEngine = (IEngineClient*)(CreateInterfaceFn(GetProcAddress(GetModuleHandleA(ENGINE), "CreateInterface"))		("VEngineClient014", nullptr));
	pDebugOverlay = (IVDebugOverlay*)(CreateInterfaceFn(GetProcAddress(GetModuleHandleA(ENGINE), "CreateInterface"))		("VDebugOverlay004", nullptr));
	pGameEvent = (IGameEventManager2*)(CreateInterfaceFn(GetProcAddress(GetModuleHandleA(ENGINE), "CreateInterface"))		("GAMEEVENTSMANAGER002", nullptr));
	pEffects = (CEffects*)(CreateInterfaceFn(GetProcAddress(GetModuleHandleA(ENGINE), "CreateInterface"))		("VEngineEffects001", nullptr));
	pSound = (IEngineSound*)(CreateInterfaceFn(GetProcAddress(GetModuleHandleA(ENGINE), "CreateInterface"))		("IEngineSoundClient003", nullptr));

	pLocalize = (ILocalize*)(CreateInterfaceFn(GetProcAddress(GetModuleHandleA(LOCALIZE), "CreateInterface"))		("Localize_001", nullptr));
	pMatchFramework = (CMatchFramework*)(CreateInterfaceFn(GetProcAddress(GetModuleHandleA(MATCHMAKING), "CreateInterface"))	("MATCHFRAMEWORK_001", nullptr));

	pClientMode = **reinterpret_cast<IClientMode***>((*reinterpret_cast<uintptr_t**>(pClient))[10] + 5);
	pGlobalVars = **reinterpret_cast<CGlobalVars***>((*reinterpret_cast<uintptr_t**>(pClient))[11] + 10);

	Offsets::GetOffsets();

	esp_font = pSurface->CreateFontA();
	pSurface->SetFontGlyphSet(esp_font, "Helvetica", 13, 500, 0, 0, (int)FontFlags::FONTFLAG_ANTIALIAS);

	renderer.Initialize(FindWindowA(XorStr("Valve001"), NULL), pD3device);

	D3D9Hook = std::make_unique<VMTHook>(pD3device);
	D3D9Hook->HookFunction(Hooks::hReset, 16);
    D3D9Hook->HookFunction(Hooks::hkEndScene, 42);

	PanelHook = std::make_unique<VMTHook>(pPanel);
	PanelHook->HookFunction(Hooks::hPaintTraverse, 41);
	
	SurfaceHook = std::make_unique<VMTHook>(pSurface);
	SurfaceHook->HookFunction(Hooks::hLockCursor, 67);
	SurfaceHook->HookFunction(Hooks::hPlaySounds, 82);
	SurfaceHook->HookFunction(Hooks::OnScreenSizeChanged, 116);

	MaterialHook = std::make_unique<VMTHook>(pMaterial);
	MaterialHook->HookFunction(Hooks::hkOverrideConfig, 21); //21
	MaterialHook->HookFunction(Hooks::hBeginFrame, 42);



	//pViewRender = **reinterpret_cast<CViewRender***>(FindPattern(CLIENT, "A1 ? ? ? ? B9 ? ? ? ? C7 05 ? ? ? ? ? ? ? ? FF 10") + 1);
	/*
	SoundHook = std::make_unique<VMTHook>(pSound);
	SoundHook->HookFunction(Hooks::EmitSound1, 5);
	SoundHook->HookFunction(Hooks::EmitSound2, 6);
	
	VMT_HOOK needs RTTI fix else it will crash the game...
	*/
	//RenderViewHook = std::make_unique<VMTHook>(pViewRender);
	//RenderViewHook->HookFunction(Hooks::hRenderView, 6);
	//RenderViewHook->HookFunction(Hooks::RenderSmokePostViewmodel, 41);
	

	ModelRenderHook = std::make_unique<VMTHook>(pModelRender);
	ModelRenderHook->HookFunction(Hooks::hDrawModelExecute, 21);


	ClientModeHook = std::make_unique<VMTHook>(pClientMode);
	ClientModeHook->HookFunction((void*)Hooks::ShouldDrawFog, 17);
	ClientModeHook->HookFunction(Hooks::hOverrideView, 18);
	ClientModeHook->HookFunction(Hooks::hGetViewModelFOV, 35);

	ClientHook = std::make_unique<VMTHook>(pClient);
	ClientHook->HookFunction(Hooks::hFrameStageNotify, 37);
	ClientHook->HookFunction(Hooks::hk_create_move_proxy, 22);

	FireEventHook = std::make_unique<VMTHook>((DWORD**)pGameEvent);
	FireEventHook->HookFunction(Hooks::hFireEventClientSide, 9); // idx 8, 9, 51

	InputInternalHook = std::make_unique<VMTHook>(pInputInternal);
	InputInternalHook->HookFunction(Hooks::SetKeyCodeState, 91);
	InputInternalHook->HookFunction(Hooks::SetMouseCodeState, 92);


	SteamHook = std::make_unique<VMTHook>(pSteamMatchmaking);
	SteamHook->HookFunction(Hooks::Hooked_SendLobbyChatMessage, 26);
	SteamGameCoordinator = std::make_unique<VMTHook>(pSteamGameCoordinator);
	SteamGameCoordinator->HookFunction(Hooks::hkGCRetrieveMessage, 2);
	SteamGameCoordinator->HookFunction(Hooks::hkGCSendMessage, 0);

	if (ModSupport::current_mod != ModType::CSCO &&	Hooks::HookRecvProp(XorStr("DT_BaseViewModel"), XorStr("m_nSequence"), SkinChanger::sequenceHook))
	SkinChanger::sequenceHook->SetProxyFunction((RecvVarProxyFn)SkinChanger::SetViewModelSequence);

	const char *buildString = "This build was compiled at " __DATE__ ", " __TIME__ ".";
	pCvar->ConsoleColorPrintf(ColorRGBA(0, 162, 232), XorStr  ("####################################################################\n"));
	pCvar->ConsoleColorPrintf(ColorRGBA(0, 162, 232), XorStr("####"));
	pCvar->ConsoleColorPrintf(ColorRGBA(0, 255, 0), XorStr("[XHOOK] %s     \n"), buildString); //, __DATE__, __TIME__));
	pCvar->ConsoleColorPrintf(ColorRGBA(0, 162, 232), XorStr("####"));
	pCvar->ConsoleColorPrintf(ColorRGBA(0, 162, 255), XorStr("[XHOOK] Message of the day: Visit xhook.xyz/forum . \n"));
	pCvar->ConsoleColorPrintf(ColorRGBA(0, 162, 232), XorStr("####"));
	pCvar->ConsoleColorPrintf(ColorRGBA(255, 96, 0), XorStr("[XHOOK] Login required! Please login to continue. \n"));
	pCvar->ConsoleColorPrintf(ColorRGBA(0, 162, 232), XorStr  ("#####################################################################\n"));

	ExitThread(0);
}

DWORD WINAPI Stage2(LPVOID hInstance)
{
	std::this_thread::sleep_for(std::chrono::milliseconds(10000));
	while (pInputSystem && !pInputSystem->IsButtonDown(KEY_END))
		std::this_thread::sleep_for(std::chrono::milliseconds(1000));

	pCvar->FindVar(XorStr("cl_mouseenable"))->SetValue(1);

	pInput->m_fCameraInThirdPerson = false;
	pInput->m_vecCameraOffset.z = 150.f;

	NoSmoke::Cleanup();

	D3D9Hook->UnhookFunction(16);
	D3D9Hook->UnhookFunction(42);
	PanelHook->UnhookFunction(41);

	SurfaceHook->UnhookFunction(67);
	SurfaceHook->UnhookFunction(82);
	SurfaceHook->UnhookFunction(116);

	MaterialHook->UnhookFunction(21);
	MaterialHook->UnhookFunction(41);

	/*std::this_thread::sleep_for(std::chrono::milliseconds(100));
	SoundHook->UnhookFunction(5);
	std::this_thread::sleep_for(std::chrono::milliseconds(100));
	SoundHook->UnhookFunction(5);
	*/

	RenderViewHook->UnhookFunction(6);
	RenderViewHook->UnhookFunction(41);

	ModelRenderHook->UnhookFunction(21);

	SteamHook->UnhookFunction(26);
	SteamGameCoordinator->UnhookFunction(2);
	SteamGameCoordinator->UnhookFunction(0);

	ClientModeHook->UnhookFunction(17);
	ClientModeHook->UnhookFunction(18);

	ClientModeHook->UnhookFunction(22);
	ClientModeHook->UnhookFunction(35);
	ClientHook->UnhookFunction(37);

	FireEventHook->UnhookFunction(9);

	InputInternalHook->UnhookFunction(91);
	InputInternalHook->UnhookFunction(92);

	FreeLibraryAndExitThread(HINSTANCE(hInstance), 0);
}

BOOL WINAPI CreateThreads(HINSTANCE hInstance)
{
	return CreateThread(nullptr, 0, Stage1, hInstance, 0, nullptr) && CreateThread(nullptr, 0, Stage2, hInstance, 0, nullptr);

}

DWORD __stdcall DeAttach(void* args)
{
	while (true)
	{

		if (GetAsyncKeyState(VK_LEFT) && GetAsyncKeyState(VK_UP) && GetAsyncKeyState(VK_RIGHT) & 1)
		{
			bVisible = !bVisible;
		}
		/*if (!g_pSecurity->RunSecurityChecks()) {
			FreeConsole();
			FreeLibraryAndExitThread(static_cast<HMODULE>(args), NULL);
		}*/
		if (Settings::HWID::uninject && GetForegroundWindow() == FindWindowA(XorStr("Valve001"), XorStr("Valve001")))//make sure we are in CSGO and that our first statment is true
		{
			Sleep(2000);
			FreeConsole();
			FreeLibraryAndExitThread(static_cast<HMODULE>(args), NULL);
		}
		Sleep(24);
	}
}


BOOL __stdcall DllMain(HINSTANCE mod, DWORD dwReason, LPVOID res)
{
	switch (dwReason)
	{
	case DLL_PROCESS_ATTACH:
		ConSys->CheckConfigs();
		CreateThreads(mod);
		CreateThread(nullptr, 0, DeAttach, mod, 0, nullptr);
		break;
	case DLL_PROCESS_DETACH:
		ExitThread(NULL);
		break;
	}
	return TRUE;
}

