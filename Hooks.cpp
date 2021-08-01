#include "Hooks.h"
#include "Utils/Skins.h"
#include "GUI/Tabs/AimTabs/AimBotTab.h"
#include "SDK/SteamAPI.h"
#include "SDK/materialconfig.h"



#pragma comment(lib, "winmm.lib")
//#pragma comment(lib, "d3d9.lib")

bool Settings::Background::enable = true;
bool SetKeyCodeState::shouldListen = false;
ButtonCode_t* SetKeyCodeState::keyOutput = nullptr;

bool Settings::Misc::AntiAfk = false;

namespace Hooks
{

	HRESULT WINAPI hPresent(IDirect3DDevice9* pDevice, RECT* pSourceRect, RECT* pDestRect, HWND hDestWindowOverride, RGNDATA* pDirtyRegion) {

		if (GetAsyncKeyState(VK_SNAPSHOT)) {
			return Present(pDevice, pSourceRect, pDestRect, hDestWindowOverride, pDirtyRegion);
		}

		DWORD dwOld_D3DRS_COLORWRITEENABLE;

		return Present(pDevice, pSourceRect, pDestRect, hDestWindowOverride, pDirtyRegion);
	}

	HRESULT WINAPI hReset(LPDIRECT3DDEVICE9 pDevice, D3DPRESENT_PARAMETERS* pPresentationParameters)
	{
		static Reset_t oReset = D3D9Hook->GetOriginalFunction<Reset_t>(16);

		if (!renderer.IsReady() || !pDevice)
			return oReset(pDevice, pPresentationParameters);

		ImGui_ImplDX9_InvalidateDeviceObjects();

		HRESULT result = oReset(pDevice, pPresentationParameters);

		ImGui_ImplDX9_CreateDeviceObjects();

		return result;
	}

	void __stdcall hPaintTraverse(unsigned int VGUIPanel, bool forcerepaint, bool allowforce)
	{
		ESP::PrePaintTraverse(VGUIPanel, forcerepaint, allowforce);
		PanelHook->GetOriginalFunction<PaintTraverseFn>(41)(pPanel, VGUIPanel, forcerepaint, allowforce);


		static unsigned int drawPanel;

		if (!drawPanel)
			if (strstr(pPanel->GetName(VGUIPanel), "MatSystemTopPanel"))
				drawPanel = VGUIPanel;

		if (VGUIPanel != drawPanel)
			return;

		////Anti ss
		if (Settings::ScreenshotCleaner::enabled && pEngine->IsTakingScreenshot())
			return;

		Dlights::Paint();
		ESP::Paint();
		GrenadeHelper::Paint();
		GrenadePrediction::Paint();
		SniperCrosshair::Paint();
		Recoilcrosshair::Paint();
		Hitmarkers::Paint();

		Walkbot::update();

		//lbyindicator::Paint();
		//AngleIndicator::Paint();
		//SpeedIndicator::Paint();	
	}

	bool HookRecvProp(const char* className, const char* propertyName, std::unique_ptr<RecvPropHook>& recvPropHook) {
		// FIXME: Does not search recursively.. yet.
		// Recursion is a meme, stick to reddit mcswaggens.
		for (ClientClass* pClass = pClient->GetAllClasses(); pClass; pClass = pClass->m_pNext) {
			if (strcmp(pClass->m_pNetworkName, className) == 0) {
				RecvTable* pClassTable = pClass->m_pRecvTable;

				for (int nIndex = 0; nIndex < pClassTable->m_nProps; nIndex++) {
					RecvProp* pProp = &pClassTable->m_pProps[nIndex];

					if (!pProp || strcmp(pProp->m_pVarName, propertyName) != 0)
						continue;

					recvPropHook = std::make_unique<RecvPropHook>(pProp);

					return true;
				}

				break;
			}
		}

		return false;
	}



	bool ShouldDrawFogs(void* thisptr) {
		if (!Settings::NoFog::enabled)
			return ClientModeHook->GetOriginalFunction<ShouldDrawFogFn>(17)(thisptr);

		/* Skybox Fog is separate */
		IMatRenderContext* renderCtx = pMaterial->GetRenderContext();
		renderCtx->FogMode(MaterialFogMode_t::MATERIAL_FOG_NONE);
		renderCtx->Release();
		/* Return false for normal fog */
		return false; // uhhh, no Sweetie, don't draw that fog.
	}

	bool ShouldDrawFog(void* thisptr) {
		return ShouldDrawFogs(thisptr);
	}

	static int __fastcall SendDatagram(NetworkChannel* network, void* edx, void* datagram)
	{
		if (!Settings::Aimbot::fakeLat || datagram || !pEngine->IsInGame() || !Settings::Aimbot::backtron)
		{
			return NetDataGram->GetOriginalFunction<SendDatagramFn>(46)(network, datagram);
		}

		int instate = network->InReliableState;
		int insequencenr = network->InSequenceNr;
		int faketimeLimit = Settings::Aimbot::timeLimit; if (faketimeLimit <= 200) { faketimeLimit = 0; }
		else { faketimeLimit -= 200; }
		float delta = max(0.f, std::clamp(faketimeLimit / 1000.f, 0.f, 0.2f) - network->getLatency(0));

		Backtrack::AddLatencyToNetwork(network, delta + (delta / 20.0f));

		NetworkChannel* OrigNet = network;
		void* OrigData = datagram;

		network->InReliableState = instate;
		network->InSequenceNr = insequencenr;

		return NetDataGram->GetOriginalFunction<SendDatagramFn>(46)(OrigNet, OrigData);
	}


	bool __stdcall hCreateMove(float frametime, CUserCmd* cmd) {

		auto result = ClientModeHook->GetOriginalFunction<CreateMoveFn>(24)(pClientMode, frametime, cmd);

		if (!cmd->command_number)
			return result;

		uintptr_t* framePointer;
		__asm mov framePointer, ebp;
		bool& sendPacket = *reinterpret_cast<bool*>(*framePointer - 0x1C);

		pGlobalVars->serverTime(cmd);

		BHop::CreateMove(cmd);
		AutoStrafe::CreateMove(cmd);
		BHop::CreateMoveCircle(cmd);
		//Resolver::CreateMove(cmd);

		if (Settings::Misc::AntiAfk && cmd->command_number % 2)
		{ cmd->buttons |= 1 << 26; }	
			static void* oldPointer = nullptr;
			C_BasePlayer* localplayer = (C_BasePlayer*)pEntityList->GetClientEntity(pEngine->GetLocalPlayer());
			auto network = pEngine->GetNetChannelInfo();
			if (oldPointer != network && network && localplayer)
			{
				oldPointer = network;
				Backtrack::UpdateIncomingSequences(true);
				NetDataGram = std::make_unique<VMTHook>(network);
				NetDataGram->HookFunction(SendDatagram, 46);

			}
			Backtrack::UpdateIncomingSequences();
		
			GrenadeHelper::CreateMove(cmd);
			GrenadePrediction::CreateMove(cmd);

			Backtrack::run(cmd);
			Aimbot::CreateMove(cmd);
			Triggerbot::CreateMove(cmd);

			lbyindicator::CreateMove(cmd);		
			ReportBot::run();	
			ShowRanks::CreateMove(cmd);
			AutoDefuse::CreateMove(cmd);
			JumpThrow::CreateMove(cmd);
			EdgeJump::PrePredictionCreateMove(cmd);
			NoDuckCooldown::CreateMove(cmd);
			PredictionSystem::StartPrediction(cmd);
			Autoblock::CreateMove(cmd);
			NameChanger::fakeBan();
			AutoKnife::CreateMove(cmd);
			AntiAim::CreateMove(cmd);
			Airstuck::CreateMove(cmd);
			Fakewalk::CreateMove(cmd);
			MoonWalk::CreateMove(cmd);

			Walkbot::CreateMove(cmd);
			FakeLag::CreateMove(cmd);
			ESP::CreateMove(cmd);
			PredictionSystem::EndPrediction();
			
			AngleIndicator::PostPredictionCreateMove(cmd);
			EdgeJump::PostPredictionCreateMove(cmd);
	
		return false;
	}

	bool __stdcall Hooked_SendLobbyChatMessage(CSteamID steamIdLobby, const void* pvMsgBody, int cubMsgBody)
	{
		typedef bool(__thiscall* SendLobbyChatMessage_t)(ISteamMatchmaking*, CSteamID, const void*, int);
		static SendLobbyChatMessage_t Original_SendLobbyChatMessage = SteamHook->GetOriginalFunction<SendLobbyChatMessage_t>(26);

		
		if (!LobbyMod::Get()->InterpretLobbyMessage(steamIdLobby, pvMsgBody, cubMsgBody))
			return Original_SendLobbyChatMessage(pSteamMatchmaking, steamIdLobby, pvMsgBody, cubMsgBody);//Original_SendLobbyChatMessage(I.SteamMatchmaking(), steamIdLobby, pvMsgBody, cubMsgBody);

		return true;
	}


	using GCRetrieveMessage = EGCResult(__thiscall*)(void*, uint32_t* punMsgType, void* pubDest, uint32_t cubDest, uint32_t* pcubMsgSize);
	using GCSendMessage = EGCResult(__thiscall*)(void*, uint32_t unMsgType, const void* pubData, uint32_t cubData);
	EGCResult __fastcall hkGCRetrieveMessage(void* ecx, void*, uint32_t* punMsgType, void* pubDest, uint32_t cubDest, uint32_t* pcubMsgSize)
	{
		static auto oGCRetrieveMessage = SteamGameCoordinator->GetOriginalFunction<GCRetrieveMessage>(2);
		auto status = oGCRetrieveMessage(ecx, punMsgType, pubDest, cubDest, pcubMsgSize);

		if (status == k_EGCResultOK)
		{

			void* thisPtr = nullptr;
			__asm mov thisPtr, ebx;
			auto oldEBP = *reinterpret_cast<void**>((uint32_t)_AddressOfReturnAddress() - 4);

			uint32_t messageType = *punMsgType & 0x7FFFFFFF;
			write.ReceiveMessage(thisPtr, oldEBP, messageType, pubDest, cubDest, pcubMsgSize);
		}
	
		return status;
	}

	EGCResult __fastcall hkGCSendMessage(void* ecx, void*, uint32_t unMsgType, const void* pubData, uint32_t cubData)
	{
		static auto oGCSendMessage = SteamGameCoordinator->GetOriginalFunction<GCSendMessage>(0);
		bool sendMessage = write.PreSendMessage(unMsgType, const_cast<void*>(pubData), cubData);

		if (!sendMessage)
			return EGCResult::k_EGCResultOK;

		return oGCSendMessage(ecx, unMsgType, const_cast<void*>(pubData), cubData);
	}

	void __stdcall hDrawModelExecute(IMatRenderContext* matctx, const DrawModelState_t& state, const ModelRenderInfo_t& pInfo, matrix3x4_t* pCustomBoneToWorld)
	{

		if (!Settings::ScreenshotCleaner::enabled || !pEngine->IsTakingScreenshot()) {
			Chams::DrawModelExecute(matctx, state, pInfo, pCustomBoneToWorld);
			ThirdPerson::DrawModelExecute(matctx, state, pInfo, pCustomBoneToWorld);
		}


		ModelRenderHook->GetOriginalFunction<DrawModelExecuteFn>(21)(pModelRender, matctx, state, pInfo, pCustomBoneToWorld);
		pModelRender->ForcedMaterialOverride(nullptr);

		if (!Settings::ScreenshotCleaner::enabled || !pEngine->IsTakingScreenshot()) {
			ESP::DrawModelExecute(matctx, state, pInfo, pCustomBoneToWorld);
		}
	}
#
	bool __fastcall hkOverrideConfig(IMaterialSystem* this0, int edx, MaterialSystem_Config_t* config, bool forceUpdate)
	{
		MaterialConfig::OverrideConfig(config, forceUpdate);

		return MaterialHook->GetOriginalFunction<OverrideConfigFn>(21)(this0, config, forceUpdate); //21 is correct.
	}


	void __fastcall hFrameStageNotify(void* ecx, void* edx, ClientFrameStage_t stage)
	{
		[[maybe_unused]] static auto backtrackInit = (Backtrack::init(), true);

		if (pEngine->isConnnected() && !pEngine->IsInGame())
			NameChanger::changeName(true, nullptr, 0.0f);

		// PVS fix
		if (stage == ClientFrameStage_t::FRAME_RENDER_START)
		{
			C_BasePlayer* localplayer = (C_BasePlayer*)pEntityList->GetClientEntity(pEngine->GetLocalPlayer());		
			for (int i = 1; i < pEngine->GetMaxClients(); ++i) {
				C_BasePlayer* player = (C_BasePlayer*)pEntityList->GetClientEntity(i);

				if (!player
					|| player == localplayer
					|| player->GetDormant()
					|| !player->GetAlive()
					|| player->GetImmune())
					continue;

				IEngineClient::player_info_t playerInfo;
				pEngine->GetPlayerInfo(player->GetIndex(), &playerInfo);

				// These were hard as shit to find: https://i.imgur.com/gTqgDLO.png
				*(int*)((uintptr_t)player + 0xA30) = pGlobalVars->framecount; // Last Occlusion check frame#
				*(int*)((uintptr_t)player + 0xA28) = 0; // Occlusion flags
			}
		}
	
		static bool SpoofCvars = false;
		if (!SpoofCvars)
		{
			ConVar* sv_cheats = pCvar->FindVar("sv_cheats");
			SpoofedConvar* sv_cheats_spoofed = new SpoofedConvar(sv_cheats);
			sv_cheats_spoofed->SetInt(1);
			ConVar* ST = pCvar->FindVar("r_DrawSpecificStaticProp");
			ST->SetValue(0);
			SpoofCvars = true;
		}

		if (stage == ClientFrameStage_t::FRAME_START)
		{
			FPSWindow::update();
		}
		
		Backtrack::update(stage);

		CustomGlow::FrameStageNotify(stage);
		SkinChanger::FrameStageNotifyModels(stage);
		SkinChanger::FrameStageNotifySkins(stage);
		Noflash::FrameStageNotify(stage);
		View::FrameStageNotify(stage);
	//	Resolver::FrameStageNotify(stage);
		SkyBox::FrameStageNotify(stage);
		ASUSWalls::FrameStageNotify(stage);
		NoSmoke::FrameStageNotify(stage);
		ThirdPerson::FrameStageNotify(stage);

		
		if (SkinChanger::forceFullUpdate)
		{
			pClientState->m_nDeltaTick = -1;
			SkinChanger::forceFullUpdate = false;
		}

		ClientHook->GetOriginalFunction<FrameStageNotifyFn>(37)(ecx, stage);

	//	Resolver::PostFrameStageNotify(stage);
		View::PostFrameStageNotify(stage);
	}

	IDirect3DStateBlock9* pixel_state = NULL; IDirect3DVertexDeclaration9* vertDec; IDirect3DVertexShader9* vertShader;
	DWORD dwOld_D3DRS_COLORWRITEENABLE;
	void SaveState(IDirect3DDevice9* pDevice)
	{
		pDevice->GetRenderState(D3DRS_COLORWRITEENABLE, &dwOld_D3DRS_COLORWRITEENABLE);
		//	pDevice->CreateStateBlock(D3DSBT_PIXELSTATE, &pixel_state); // This seam not to be needed anymore because valve fixed their shit
		pDevice->GetVertexDeclaration(&vertDec);
		pDevice->GetVertexShader(&vertShader);
		pDevice->SetRenderState(D3DRS_COLORWRITEENABLE, 0xffffffff);
		pDevice->SetRenderState(D3DRS_SRGBWRITEENABLE, false);
		pDevice->SetSamplerState(NULL, D3DSAMP_ADDRESSU, D3DTADDRESS_WRAP);
		pDevice->SetSamplerState(NULL, D3DSAMP_ADDRESSV, D3DTADDRESS_WRAP);
		pDevice->SetSamplerState(NULL, D3DSAMP_ADDRESSW, D3DTADDRESS_WRAP);
		pDevice->SetSamplerState(NULL, D3DSAMP_SRGBTEXTURE, NULL);
	}

	void RestoreState(IDirect3DDevice9* pDevice) // not restoring everything. Because its not needed.
	{
		pDevice->SetRenderState(D3DRS_COLORWRITEENABLE, dwOld_D3DRS_COLORWRITEENABLE);
		pDevice->SetRenderState(D3DRS_SRGBWRITEENABLE, true);
		//pixel_state->Apply(); 
		//pixel_state->Release();
		pDevice->SetVertexDeclaration(vertDec);
		pDevice->SetVertexShader(vertShader);
	}

	HRESULT WINAPI hkEndScene(IDirect3DDevice9* device)
	{
		static EndScene_t oEndScene = D3D9Hook->GetOriginalFunction<EndScene_t>(42);

		SaveState(device);
		Menu();
		RestoreState(device);

		device->SetRenderState(D3DRS_COLORWRITEENABLE, dwOld_D3DRS_COLORWRITEENABLE);

		return oEndScene(device);
	}

	bool __fastcall hFireEventClientSide(void* ecx, void* edx, IGameEvent* pEvent)
	{

		Aimbot::FireGameEvent(pEvent);
		Hitmarkers::FireGameEvent(pEvent);
		NameStealer::FireGameEvent(pEvent);
	//	Resolver::FireGameEvent(pEvent);
		Spammer::FireGameEvent(pEvent);
		EventLogger::FireGameEvent(pEvent);
		ValveDSCheck::FireGameEvent(pEvent);

		if (pEvent)
		{
			SkinChanger::FireEventClientSide(pEvent);
			SkinChanger::FireGameEvent(pEvent);
		}
	
		return FireEventHook->GetOriginalFunction<FireEventClientSideFn>(9)(ecx, pEvent);
	}

	
	void __stdcall hBeginFrame(float frameTime)
	{

		ClanTagChanger::BeginFrame(frameTime);
		NameChanger::BeginFrame(frameTime);
		NameStealer::BeginFrame(frameTime);
		Spammer::BeginFrame(frameTime);
		Radar::BeginFrame();
		Skins::Localize();
		DisablePostProcessing::BeginFrame();

		return MaterialHook->GetOriginalFunction<BeginFrameFn>(42)(pMaterial, frameTime);
	}

	                                                
	void __fastcall Hooks::EmitSound1(void* thisptr, void* edx, IRecipientFilter& filter, int iEntIndex, int iChannel, const char* pSoundEntry,
		unsigned int nSoundEntryHash, const char* pSample, float flVolume, int nSeed,
		float flAttenuation, int iFlags, int iPitch, const Vector* pOrigin, const Vector* pDirection,
		void* pUtlVecOrigins, bool bUpdatePositions, float soundtime, int speakerentity, int Unknownn) {

		ESP::EmitSound(iEntIndex, pSample);

		SoundHook->GetOriginalFunction<EmitSound1Fn>(5)(thisptr, filter, iEntIndex, iChannel, pSoundEntry, nSoundEntryHash,
			pSample, flVolume, nSeed, flAttenuation, iFlags, iPitch, pOrigin,
			pDirection, pUtlVecOrigins, bUpdatePositions, soundtime,
			speakerentity, Unknownn);

	}
	

	
	void __fastcall Hooks::EmitSound2(void* thisptr, void* edx, void* Filter, int iEntIndex, int iChannel, const char* pSoundEntry,
		unsigned int nSoundEntryHash, const char* pSample, float flVolume, int nSeed,
		int iSoundLevel, int iFlags, int iPitch, const Vector* pOrigin,
		const Vector* pDirection, void* pUtlVecOrigins, bool bUpdatePositions, float soundtime,
		int speakerentity, void* SoundParam ) {

		ESP::EmitSound(iEntIndex, pSample);

		SoundHook->GetOriginalFunction<EmitSound2Fn>(5)(thisptr, Filter, iEntIndex, iChannel, pSoundEntry, nSoundEntryHash,
			pSample, flVolume, nSeed, iSoundLevel, iFlags, iPitch, pOrigin,
			pDirection, pUtlVecOrigins, bUpdatePositions, soundtime,
			speakerentity, SoundParam);
	}


	void __fastcall Hooks::hkEmitSound(IEngineSound* thisptr, int edx, IRecipientFilter& filter, int iEntIndex, int iChannel, const char *pSoundEntry, unsigned int nSoundEntryHash, const char *pSample,
		float flVolume, float flAttenuation, int nSeed, int iFlags, int iPitch,
		const Vector *pOrigin, const Vector *pDirection, CUtlVector< Vector >* pUtlVecOrigins, bool bUpdatePositions, float soundtime, int speakerentity, int Unknownn)
	{

		ESP::EmitSound(iEntIndex, pSample);

		auto oEmitSound = SoundHook->GetOriginalFunction<EmitSound1Fn>(5);

		oEmitSound(thisptr, filter, iEntIndex, iChannel, pSoundEntry, nSoundEntryHash, pSample, flVolume, flAttenuation, nSeed, iFlags, iPitch, pOrigin, pDirection, pUtlVecOrigins, bUpdatePositions, soundtime, speakerentity, Unknownn);
	}

	void OnScreenSizeChanged(void* thisptr, int oldwidth, int oldheight) {
		SurfaceHook->GetOriginalFunction<OnScreenSizeChangedFn>(116)(thisptr, oldwidth, oldheight);
		//Fonts::SetupFonts();
	}

	void __stdcall hPlaySounds(const char* fileName)
	{
		SurfaceHook->GetOriginalFunction<PlaySoundFn>(82)(pSurface, fileName);

		AutoAccept::PlaySound(fileName);
	}

	void	__stdcall hLockCursor()
	{
		bool Lock = G::is_renderer_active;
		if (Lock) {
			pSurface->unlockcursor(); 
			return;
		}
		SurfaceHook->GetOriginalFunction<LockCursor>(67)(pSurface);
	}


	void __fastcall Hooks::RenderSmokePostViewmodel(void* ecx, void* edx) {
		if (!NoSmoke::RenderSmokePostViewmodel())
			RenderViewHook->GetOriginalFunction<NoSmokeFn>(41)(ecx);
	}

	void __fastcall hRenderView(void* ecx, void* edx, CViewSetup &setup, CViewSetup &hudViewSetup, int nClearFlags, int whatToDraw)
	{
		RenderViewHook->GetOriginalFunction<RenderViewFn>(6)(ecx, setup, hudViewSetup, nClearFlags, whatToDraw);

		SpeedIndicator::RenderView(ecx, setup, hudViewSetup, nClearFlags, whatToDraw);

	}

	void __fastcall hOverrideView(void* _this, void* _edx, ViewSetup* setup)
	{
		if (!Settings::ScreenshotCleaner::enabled || !pEngine->IsTakingScreenshot()) {
			GrenadePrediction::OverrideView(setup);
			FOVChanger::OverrideView(setup);
			ThirdPerson::OverrideView(setup);
		}

		OverrideView::currentFOV = setup->fov;

		ClientModeHook->GetOriginalFunction<OverreideViewFn>(18)(_this, setup);
	}
	float __stdcall hGetViewModelFOV()
	{
		float fov = ClientModeHook->GetOriginalFunction<GetViewModelFOVFn>(35)();
		FOVChanger::GetViewModelFOV(fov);
		return fov;
	}


	void __fastcall Hooks::SetKeyCodeState(void* thisptr, void* edx, ButtonCode_t code, bool bPressed) {
		if (SetKeyCodeState::shouldListen && bPressed) {
			SetKeyCodeState::shouldListen = false;
			*SetKeyCodeState::keyOutput = code;
			UI::UpdateWeaponSettings();
			G::input_shouldListen = true;
		}

		if (!SetKeyCodeState::shouldListen)
			Shortcuts::SetKeyCodeState(code, bPressed);
		InputInternalHook->GetOriginalFunction<SetKeyCodeStateFn>(91)(thisptr, code, bPressed);
	}

	void __fastcall Hooks::SetMouseCodeState(void* thisptr, void* edx, ButtonCode_t code, MouseCodeState_t state) {
		if (SetKeyCodeState::shouldListen && state == MouseCodeState_t::BUTTON_PRESSED) {
			SetKeyCodeState::shouldListen = false;
			*SetKeyCodeState::keyOutput = code;
			UI::UpdateWeaponSettings();
		}

		InputInternalHook->GetOriginalFunction<SetMouseCodeStateFn>(92)(thisptr, code, state);
	}
}


