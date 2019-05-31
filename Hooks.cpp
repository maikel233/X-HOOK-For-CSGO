#include "Hooks.h"
#include "Sprite.h"
#include "Skins.h"
//#include "glhook.h"
#include "Icons.h"
#include "AimBotTab.h"
#include "Features.h"
#include "util_sdk.h"
#include "util.h"
#include "ImGui\imgui.h"
#include "ImGui/imgui_internal.h"
#include <d3dx9.h>
#include <d3d9.h>
#include <d3d9types.h>

#pragma comment(lib, "winmm.lib")
#pragma comment(lib, "d3d9.lib")

bool Settings::Background::enable = true;
bool SetKeyCodeState::shouldListen = false;
ButtonCode_t* SetKeyCodeState::keyOutput = nullptr;

VMTHook* HNetchan = nullptr;
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

		C_BasePlayer* localplayer = (C_BasePlayer*)pEntityList->GetClientEntity(pEngine->GetLocalPlayer());
		if (pEngine->IsInGame() && strstr(XorStr("HudZoom"), pPanel->GetName(VGUIPanel)) && Settings::NoScopeBorder::enabled && localplayer && localplayer->IsScoped())
			return;

		PanelHook->GetOriginalFunction<PaintTraverseFn>(41)(pPanel, VGUIPanel, forcerepaint, allowforce);


		static unsigned int drawPanel; 

		if (!drawPanel)
			if (strstr(pPanel->GetName(VGUIPanel), "MatSystemTopPanel"))
				drawPanel = VGUIPanel;

		if (VGUIPanel != drawPanel)
			return;

		//Anti ss
		if (Settings::ScreenshotCleaner::enabled && pEngine->IsTakingScreenshot())
			return;


		Info();
		Dlights::Paint();
		ESP::Paint();
		GrenadeHelper::Paint();
		GrenadePrediction::Paint();
		SniperCrosshair::Paint();
		Recoilcrosshair::Paint();
		Hitmarkers::Paint();

		Walkbot::update();

		lbyindicator::Paint();
		AngleIndicator::Paint();
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
		IMatRenderContext *renderCtx = pMaterial->GetRenderContext();
		renderCtx->FogMode(MaterialFogMode_t::MATERIAL_FOG_NONE);
		renderCtx->Release();
		/* Return false for normal fog */
		return false; // uhhh, no Sweetie, don't draw that fog.
	}

	bool ShouldDrawFog(void *thisptr) {
		return ShouldDrawFogs(thisptr);
	}



#define FAKE_LATENCY_AMOUNT 0.046875f
	SendDatagramFn oSendDatagram = NULL;
	using SendDatagram_t = int(__thiscall *)(void *, void *);
	int __fastcall SendDatagram(void* netchan, void*, void *datagram)
	{

		static auto original_fn = NetChannelHook->GetOriginalFunction<SendDatagram_t>(46);
	

		INetChannel* channel = (INetChannel*)HNetchan;
		bf_write* data = (bf_write*)datagram;

		if (!Settings::Aimbot::LegitBackTrack || datagram)
		{
			return original_fn(channel, data);
		}
		//	return original_fn(net_channel, datagram);
		
		//auto * channel = reinterpret_cast< INetChannel * >(net_channel);
		gLagCompensation.AddLatencyToNetchan(channel, FAKE_LATENCY_AMOUNT);
	//	lagcompensation::add_latency(chan, FAKE_LATENCY_AMOUNT);



		int instate = channel->m_nInReliableState;
		int insequencenr = channel->m_nInSequenceNr;

		int ret = original_fn(channel, data);

		channel->m_nInReliableState = instate;
		channel->m_nInSequenceNr = insequencenr;

		//pCvar->ConsoleColorPrintf(ColorRGBA(0, 162, 232), XorStr(" very sad nothing is happening... \n"));

		return ret;
	}



	

	static bool hooked = false;
	bool __stdcall hCreateMove(float frametime, CUserCmd* cmd)
	{
		ClientModeHook->GetOriginalFunction<CreateMoveFn>(24)(pClientMode, frametime, cmd);
		if (cmd && cmd->command_number)
		{
			PDWORD pEBP;
			__asm mov pEBP, ebp;
			bool& bSendPacket = *(bool*)(*pEBP - 0x1C);

			bSendPacket = SendPacket;
			bSendPackett = bSendPacket;
			SendPacket = true;
			lbyindicator::CreateMove(cmd);
			Resolver::CreateMove(cmd);
			BHop::CreateMove(cmd);
			AutoStrafe::CreateMove(cmd);
			ShowRanks::CreateMove(cmd);
			AutoDefuse::CreateMove(cmd);
			JumpThrow::CreateMove(cmd);
			GrenadeHelper::CreateMove(cmd);
			GrenadePrediction::CreateMove(cmd);
			EdgeJump::PrePredictionCreateMove(cmd);
			NoDuckCooldown::CreateMove(cmd);
			BHop::CreateMoveCircle(cmd);
			PredictionSystem::StartPrediction(cmd);
			Autoblock::CreateMove(cmd);
			Aimbot::CreateMove(cmd);
			Triggerbot::CreateMove(cmd);
			AutoKnife::CreateMove(cmd);
			AntiAim::CreateMove(cmd);
			Airstuck::CreateMove(cmd);
			Fakewalk::CreateMove(cmd);
			MoonWalk::CreateMove(cmd);
			backtracking->legitBackTrack(cmd);
			Walkbot::CreateMove(cmd);
			FakeLag::CreateMove(cmd);
			ESP::CreateMove(cmd);
			PredictionSystem::EndPrediction();
			//TracerEffect::CreateMove(cmd);
			AngleIndicator::PostPredictionCreateMove(cmd);
			EdgeJump::PostPredictionCreateMove(cmd);

		/*	if (Settings::Aimbot::LegitBackTrack)

			{*/
			//	gLagCompensation.UpdateIncomingSequences();
		//	}

			//if (Settings::Aimbot::LegitBackTrack)
			//{
			//	if (!HNetchan)
			//	{
			//		DWORD ClientState = *(DWORD*)pClientState;
			//		if (ClientState)
			//		{
			//			DWORD NetChannel = *(DWORD*)(*(DWORD*)pClientState + 0x9C);
			//			if (NetChannel)
			//			{
			//				/*	NetChannelHook = std::make_unique<VMTHook>(pClientState->m_pNetChannel);
			//					NetChannelHook->HookFunction(Hooks::SendDatagram, 46);*/



			//				HNetchan = new VMTHook((DWORD**)NetChannel);
			//				oSendDatagram = (SendDatagramFn)HNetchan->HookFunction((void*)SendDatagram, 46);
			//			}
			//		}
			//	}

			//	//if (HNetchan)
			//	//{

			//	//	DWORD ClientState = *(DWORD*)pClientState;
			//	//	if (ClientState)
			//	//	{
			//	//		DWORD NetChannel = *(DWORD*)(*(DWORD*)pClientState + 0x9C);
			//	//		if (NetChannel)
			//	//		{
			//	//			//Real_m_nInSequencenumber = 0.0f;
			//	//			delete HNetchan;
			//	//			HNetchan = nullptr;
			//	//		}
			//	//		else
			//	//		{
			//	//			//LocalPlayer.Real_m_nInSequencenumber = 0.0f;
			//	//			//HNetchan->ClearClassBase();
			//	//			
			//	//			delete HNetchan;
			//	//			HNetchan = nullptr;
			//	//		}
			//	//	}
			//	//	else
			//	//	{
			//	//	
			//	//		delete HNetchan;
			//	//		HNetchan = nullptr;
			//	//	}
			////	}
			//
			//	
			//

		}
		else
			return ClientModeHook->GetOriginalFunction<CreateMoveFn>(24)(pClientMode, frametime, cmd);

		return false;
	}
	void __stdcall hDrawModelExecute(IMatRenderContext* matctx, const DrawModelState_t& state, const ModelRenderInfo_t& pInfo, matrix3x4_t* pCustomBoneToWorld)
	{

		if (!Settings::ScreenshotCleaner::enabled || !pEngine->IsTakingScreenshot()) {
			Chams::DrawModelExecute(matctx, state, pInfo, pCustomBoneToWorld);
			ThirdPerson::DrawModelExecute(matctx, state, pInfo, pCustomBoneToWorld);
		}


		ModelRenderHook->GetOriginalFunction<DrawModelExecuteFn>(21)(pModelRender, matctx, state, pInfo, pCustomBoneToWorld);
		pModelRender->ForcedMaterialOverride(nullptr);

	}

	bool __fastcall hkOverrideConfig(IMaterialSystem* this0, int edx, MaterialSystem_Config_t* config, bool forceUpdate)
	{
		MaterialConfig::OverrideConfig(config, forceUpdate);

		return MaterialHook->GetOriginalFunction<OverrideConfigFn>(21)(this0, config, forceUpdate); //21 is correct.
	}

	void __fastcall hFrameStageNotify(void* ecx, void* edx, ClientFrameStage_t stage)
	{
		// PVS Fix
		if (stage == ClientFrameStage_t::FRAME_RENDER_START)
		{
			for (int i = 1; i <= pGlobalVars->maxClients; i++)
			{
				if (i == pEngine->GetLocalPlayer()) continue;
				IClientEntity* pCurEntity = pEntityList->GetClientEntity(i);
				if (!pCurEntity) continue;

				*(int*)((uintptr_t)pCurEntity + 0xA30) = pGlobalVars->framecount; //we'll skip occlusion checks now
				*(int*)((uintptr_t)pCurEntity + 0xA28) = 0;//clear occlusion flags
			}
		}
		else if (stage == ClientFrameStage_t::FRAME_NET_UPDATE_POSTDATAUPDATE_START)
		{
			backtracking->Update(pGlobalVars->tickcount);
			//lagcompensation::Get().update_sequence();
		//	gLagCompensation.UpdateIncomingSequences();
			
		}

		static bool Rekt = false;
		if (!Rekt)
		{
			ConVar* sv_cheats = pCvar->FindVar("sv_cheats");
			SpoofedConvar* sv_cheats_spoofed = new SpoofedConvar(sv_cheats);
			sv_cheats_spoofed->SetInt(1);
			ConVar* ST = pCvar->FindVar("r_DrawSpecificStaticProp");
			ST->SetValue(0);
			Rekt = true;
		}

		if (Settings::Aimbot::LegitBackTrack)

		{
		//	gLagCompensation.UpdateIncomingSequences();
		}
		Backtracking::FrameStageNotify(stage);
		CustomGlow::FrameStageNotify(stage);
		SkinChanger::FrameStageNotifyModels(stage);
		SkinChanger::FrameStageNotifySkins(stage);
		Noflash::FrameStageNotify(stage);
		View::FrameStageNotify(stage);
		Resolver::FrameStageNotify(stage);
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

		Resolver::PostFrameStageNotify(stage);
		View::PostFrameStageNotify(stage);
	}
	bool Once = true;
	bool InitImages = true;
	static int width = 0;
	static int height = 0;


	HRESULT WINAPI hkEndScene(IDirect3DDevice9* device)
	{
		static EndScene_t oEndScene = D3D9Hook->GetOriginalFunction<EndScene_t>(42);

		DWORD dwOld_D3DRS_COLORWRITEENABLE;
		device->GetRenderState(D3DRS_COLORWRITEENABLE, &dwOld_D3DRS_COLORWRITEENABLE);
		device->SetRenderState(D3DRS_COLORWRITEENABLE, 0xffffffff);



		if (!Settings::Background::enable)
		{
			if (Once == true)
			{

				if (D3DXCreateTextureFromFile(pD3device, "C://xhook//Pictures//bg.jpg", &BackgroundTexture) != D3D_OK) {
					ImGui::OpenPopup("Success###Picture loaded!");

				}
				Once = false;
			}
		}

		if (InitImages == true)
		{

			if (width == 0) {
				pEngine->GetScreenSize(width, height);
			}


			if (UsericoTexture == nullptr)D3DXCreateTextureFromFileInMemoryEx(pD3device
				, &userico, sizeof(userico),
				16, 16, D3DX_DEFAULT, 0, D3DFMT_UNKNOWN, D3DPOOL_MANAGED, D3DX_DEFAULT, D3DX_DEFAULT, 0, NULL, NULL, &UsericoTexture);
			if (gasTexture == nullptr)D3DXCreateTextureFromFileInMemoryEx(pD3device
				, &gas, sizeof(gas),
				(width * 0.05f), (height * 0.75f), D3DX_DEFAULT, 0, D3DFMT_UNKNOWN, D3DPOOL_MANAGED, D3DX_DEFAULT, D3DX_DEFAULT, 0, NULL, NULL, &gasTexture);

			if (needleTexture == nullptr)D3DXCreateTextureFromFileInMemoryEx(pD3device
				, &needle, sizeof(needle),
				(width * 0.05f) + 90, (height * 0.75f) + 113, D3DX_DEFAULT, 0, D3DFMT_UNKNOWN, D3DPOOL_MANAGED, D3DX_DEFAULT, D3DX_DEFAULT, 0, NULL, NULL, &needleTexture);

			if (oilTexture == nullptr)D3DXCreateTextureFromFileInMemoryEx(pD3device
				, &oil, sizeof(oil),
				(width * 0.95f) - 225, (height * 0.75f), D3DX_DEFAULT, 0, D3DFMT_UNKNOWN, D3DPOOL_MANAGED, D3DX_DEFAULT, D3DX_DEFAULT, 0, NULL, NULL, &oilTexture);

			if (rearmirror == nullptr)D3DXCreateTextureFromFileInMemoryEx(pD3device
				, &rearmirror, sizeof(rearmirror),
				(width / 2) - 266, 0, D3DX_DEFAULT, 0, D3DFMT_UNKNOWN, D3DPOOL_MANAGED, D3DX_DEFAULT, D3DX_DEFAULT, 0, NULL, NULL, &rearmirrorTexture);

			if (speedoTexture == nullptr)D3DXCreateTextureFromFileInMemoryEx(pD3device
				, &speedometer, sizeof(speedometer),
				(width * 0.70f), (height * 0.75f), D3DX_DEFAULT, 0, D3DFMT_UNKNOWN, D3DPOOL_MANAGED, D3DX_DEFAULT, D3DX_DEFAULT, 0, NULL, NULL, &speedoTexture);

			if (wheelTexture == nullptr)D3DXCreateTextureFromFileInMemoryEx(pD3device
				, &wheel, sizeof(wheel),
				(width * 0.5f) - (width / 8), height - (width / 8), D3DX_DEFAULT, 0, D3DFMT_UNKNOWN, D3DPOOL_MANAGED, D3DX_DEFAULT, D3DX_DEFAULT, 0, NULL, NULL, &wheelTexture);

			InitImages = false;
		}

		Menu();

		device->SetRenderState(D3DRS_COLORWRITEENABLE, dwOld_D3DRS_COLORWRITEENABLE);

		return oEndScene(device);
	}
	int Kills = 0;
	bool __fastcall hFireEventClientSide(void* ecx, void* edx, IGameEvent* pEvent)
	{

		Aimbot::FireGameEvent(pEvent);
		Hitmarkers::FireGameEvent(pEvent);
		NameStealer::FireGameEvent(pEvent);
		Resolver::FireGameEvent(pEvent);
		Spammer::FireGameEvent(pEvent);
		EventLogger::FireGameEvent(pEvent);
		ValveDSCheck::FireGameEvent(pEvent);

		if (pEvent)
		{
			SkinChanger::FireEventClientSide(pEvent);
			SkinChanger::FireGameEvent(pEvent);
		}

		if (Settings::Spammer::KillSpammer::enabledSync) {
			if (strcmp(pEvent->GetName(), "round_end") == 0) {
				if (Settings::Spammer::KillSpammer::enableCustomSounds)
				{
					PlaySoundA(XorStr("C:\\xhook\\Hitsounds\\RoundEnd.wav"), nullptr, SND_ASYNC);
				}
			}

			else if ((strcmp(pEvent->GetName(), "game_newmap") == 0) || (strcmp(pEvent->GetName(), "game_start") == 0) || (strcmp(pEvent->GetName(), "game_end") == 0)) {
				Kills = 0;
			}

			else if (strcmp(pEvent->GetName(), "round_start") == 0) {
				char Buffer[128];

				SYSTEMTIME str_t;
				GetLocalTime(&str_t);
				sprintf(Buffer, "say [X-HOOK]: Time: %d:%d:%d Hey Here we go again :)", str_t.wHour, str_t.wMinute, str_t.wSecond); // our current round						

				Settings::WalkBot::marker = 0;

				pEngine->ExecuteClientCmd(Buffer); 
			}

			else if (strcmp(pEvent->GetName(), "round_start") == 0) {
				char Buffer[128];

				SYSTEMTIME str_t;
				GetLocalTime(&str_t);
				sprintf(Buffer, "say [X-HOOK]: Time: %d:%d:%d Hey Here we go again :)", str_t.wHour, str_t.wMinute, str_t.wSecond); // our current round						

				pEngine->ExecuteClientCmd(Buffer); 
			}

			else if (strcmp(pEvent->GetName(), "player_connect") == 0) {
				int PlayerID = pEngine->GetPlayerForUserID(pEvent->GetInt("userid"));

				IEngineClient::player_info_t PlayerInfo;
				if (pEngine->GetPlayerInfo(PlayerID, &PlayerInfo)) {
					char Buffer[128];
					sprintf(Buffer, "say [X-HOOK] %s just connected", PlayerInfo.name);
					pEngine->ExecuteClientCmd(Buffer); 
				}
			}

			else if ((strcmp(pEvent->GetName(), "player_death") == 0) || (strcmp(pEvent->GetName(), "entity_killed") == 0)) {
				int PlayerID = pEngine->GetPlayerForUserID(pEvent->GetInt("userid"));
				int AttackerID = pEngine->GetPlayerForUserID(pEvent->GetInt("attacker"));
				if (Settings::Spammer::KillSpammer::enabledSync) {

					IEngineClient::player_info_t PlayerInfo;
					IEngineClient::player_info_t AttackerInfo;

					if (pEngine->GetPlayerInfo(PlayerID, &PlayerInfo) &&
						pEngine->GetPlayerInfo(AttackerID, &AttackerInfo)) {


						if (PlayerID == pEngine->GetLocalPlayer())
							Kills = 0;

						if (PlayerID != pEngine->GetLocalPlayer() && AttackerID == pEngine->GetLocalPlayer()) {
							Kills++;

							char Buffer[128];

							switch (Kills) {
							case 1: sprintf(Buffer, "say [X-HOOK] First kill! %s just got owned [KS: 1]", PlayerInfo.name);
								if (Settings::Spammer::KillSpammer::enableCustomSounds)
								{
									PlaySoundA(XorStr("C:\\xhook\\Hitsounds\\Firstblood.wav"), nullptr, SND_ASYNC);
								}
								break;
							case 2:	sprintf(Buffer, "say [X-HOOK] Double kill! %s just got owned [KS: 2]", PlayerInfo.name);
								if (Settings::Spammer::KillSpammer::enableCustomSounds)
								{
									PlaySoundA(XorStr("C:\\xhook\\Hitsounds\\Doubekill.wav"), nullptr, SND_ASYNC);
								}
								break;
							case 3:	sprintf(Buffer, "say [X-HOOK] Triple kill! %s just got owned [KS: 3]", PlayerInfo.name);
								if (Settings::Spammer::KillSpammer::enableCustomSounds)
								{
									PlaySoundA(XorStr("C:\\xhook\\Hitsounds\\Triplekill.wav"), nullptr, SND_ASYNC);
								}
								break;
							case 4:	sprintf(Buffer, "say [X-HOOK] Ultra kill! %s just got owned [KS: 4]", PlayerInfo.name);
								if (Settings::Spammer::KillSpammer::enableCustomSounds)
								{
									PlaySoundA(XorStr("C:\\xhook\\Hitsounds\\Ultrakill.wav"), nullptr, SND_ASYNC);
								}
								break;
							case 5:	sprintf(Buffer, "say [X-HOOK] MMMONSTERKILL! %s just got owned [KS: 5]", PlayerInfo.name);
								if (Settings::Spammer::KillSpammer::enableCustomSounds)
								{
									PlaySoundA(XorStr("C:\\xhook\\Hitsounds\\Monsterkill.wav"), nullptr, SND_ASYNC);
								}
								break;
							case 6:	sprintf(Buffer, "say [X-HOOK] Unbelievable! %s just got owned [KS: 6]", PlayerInfo.name);
								if (Settings::Spammer::KillSpammer::enableCustomSounds)
								{
									PlaySoundA(XorStr("C:\\xhook\\Hitsounds\\MegaKill.wav"), nullptr, SND_ASYNC);
								}
								break;
							case 7:	sprintf(Buffer, "say [X-HOOK] Ludicrouskill! %s just got owned [KS: 7]", PlayerInfo.name);
								if (Settings::Spammer::KillSpammer::enableCustomSounds)
								{
									PlaySoundA(XorStr("C:\\xhook\\Hitsounds\\ludicrouskill.wav"), nullptr, SND_ASYNC);
								}
							case 8:	sprintf(Buffer, "say [X-HOOK] HolyShit! %s just got owned [KS: 8]", PlayerInfo.name);
								if (Settings::Spammer::KillSpammer::enableCustomSounds)
								{
									PlaySoundA(XorStr("C:\\xhook\\Hitsounds\\holyshit.wav"), nullptr, SND_ASYNC);
								}
								break;
							case 9: sprintf(Buffer, "say [X-HOOK] KILLING SPREE! %s just got owned [KS: 9]", PlayerInfo.name);
								if (Settings::Spammer::KillSpammer::enableCustomSounds)
								{
									PlaySoundA(XorStr("C:\\xhook\\Hitsounds\\Killingspree.wav"), nullptr, SND_ASYNC);
								}
								break;
							case 10: sprintf(Buffer, "say [X-HOOK] Perfect! %s just got owned [KS: 10]", PlayerInfo.name);
								if (Settings::Spammer::KillSpammer::enableCustomSounds)
								{
									PlaySoundA(XorStr("C:\\xhook\\Hitsounds\\Perfect.wav"), nullptr, SND_ASYNC);
								}
								break;
							case 11: sprintf(Buffer, "say [X-HOOK] %s just got owned [KS: 12]", PlayerInfo.name);
								if (Settings::Spammer::KillSpammer::enableCustomSounds)
								{
									PlaySoundA(XorStr("C:\\xhook\\Hitsounds\\Ownage.wav"), nullptr, SND_ASYNC);
								}
								break;
							case 12: sprintf(Buffer, "say [X-HOOK] Wicked sick! %s just got owned [KS: 11]", PlayerInfo.name);
								if (Settings::Spammer::KillSpammer::enableCustomSounds)
								{
									PlaySoundA(XorStr("C:\\xhook\\Hitsounds\\WickedSick.wav"), nullptr, SND_ASYNC);
								}
								break;
							case 13: sprintf(Buffer, "say [X-HOOK] Wicked sick! %s just got owned [KS: 13]", PlayerInfo.name);
								if (Settings::Spammer::KillSpammer::enableCustomSounds)
								{
									PlaySoundA(XorStr("C:\\xhook\\Hitsounds\\ComboWhore.wav"), nullptr, SND_ASYNC);
								}
								break;


							case 15: sprintf(Buffer, "say [X-HOOK] RAMPAGE! %s just got owned [KS: 15]", PlayerInfo.name);
								if (Settings::Spammer::KillSpammer::enableCustomSounds)
								{
									PlaySoundA(XorStr("C:\\xhook\\Hitsounds\\Rampage.wav"), nullptr, SND_ASYNC);
								}
								break;
							case 21: sprintf(Buffer, "say [X-HOOK] DOMINATING! %s just got owned [KS: 21]", PlayerInfo.name);
								if (Settings::Spammer::KillSpammer::enableCustomSounds)
								{
									PlaySoundA(XorStr("C:\\xhook\\Hitsounds\\Dominating.wav"), nullptr, SND_ASYNC);
								}
								break;
							case 26: sprintf(Buffer, "say [X-HOOK]UNSTOPPABLE! %s just got owned [KS: 26]", PlayerInfo.name);
								if (Settings::Spammer::KillSpammer::enableCustomSounds)
								{
									PlaySoundA(XorStr("C:\\xhook\\Hitsounds\\unstoppable.wav"), nullptr, SND_ASYNC);
								}
								break;
							case 37: sprintf(Buffer, "say [X-HOOK] GODLIKE! %s just got owned [KS: 37]", PlayerInfo.name);
								if (Settings::Spammer::KillSpammer::enableCustomSounds)
								{
									PlaySoundA(XorStr("C:\\xhook\\Hitsounds\\Godlike.wav"), nullptr, SND_ASYNC);
								}
								break;
							case 40: sprintf(Buffer, "say [X-HOOK] BEYOND GODLIKE! %s just got owned [KS: 40]", PlayerInfo.name); break;
							case 50: sprintf(Buffer, "say [X-HOOK] KILLPOCALYPSE! %s just got owned [KS: 50]", PlayerInfo.name); break;
							case 55: sprintf(Buffer, "say [X-HOOK] KILLIONAIRE! [KS: 55]", PlayerInfo.name); break;
							default: sprintf(Buffer, "say [X-HOOK] %s got rekt! [KS: %i]", PlayerInfo.name, Kills); break;
							}

							pEngine->ExecuteClientCmd(Buffer); // Send our Kill Message.

						}
					}
				}
			}
			else if (strcmp(pEvent->GetName(), "player_hurt") == 0) {
				int PlayerID = pEngine->GetPlayerForUserID(pEvent->GetInt("userid"));
				int AttackerID = pEngine->GetPlayerForUserID(pEvent->GetInt("attacker"));

				IEngineClient::player_info_t PlayerInfo;
				IEngineClient::player_info_t AttackerInfo;

				if (pEngine->GetPlayerInfo(PlayerID, &PlayerInfo) &&
					pEngine->GetPlayerInfo(AttackerID, &AttackerInfo)) {

					if (AttackerID == pEngine->GetLocalPlayer()) {
						return FireEventHook->GetOriginalFunction<FireEventClientSideFn>(9)(ecx, pEvent);
					}

					if (Settings::Spammer::KillSpammer::enabledHurt) {

						char Buffer[128];
						sprintf(Buffer, "say %s just hurt %s", AttackerInfo.name, PlayerInfo.name);
						pEngine->ExecuteClientCmd(Buffer);
					}

				}
			}

			else if (strcmp(pEvent->GetName(), "player_disconnect") == 0) {
				int PlayerID = pEngine->GetPlayerForUserID(pEvent->GetInt("userid"));
				IEngineClient::player_info_t PlayerInfo;
				if (pEngine->GetPlayerInfo(PlayerID, &PlayerInfo)) {
					char Buffer[128];
					sprintf(Buffer, "say [X-HOOK] %s just ragequit :>", PlayerInfo.name);
					pEngine->ExecuteClientCmd(Buffer);
				}
			}
		}
		return FireEventHook->GetOriginalFunction<FireEventClientSideFn>(9)(ecx, pEvent);
	}

	
	void __stdcall hBeginFrame(float frameTime)
	{

//		ClanTagChanger::BeginFrame(frameTime);
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
		SoundHook->GetOriginalFunction<EmitSound1Fn>(5)(thisptr, filter, iEntIndex, iChannel, pSoundEntry, nSoundEntryHash,
			pSample, flVolume, nSeed, flAttenuation, iFlags, iPitch, pOrigin,
			pDirection, pUtlVecOrigins, bUpdatePositions, soundtime,
			speakerentity, Unknownn);
	}
	//
	void __fastcall Hooks::EmitSound2(void* thisptr, void* edx, IRecipientFilter& filter, int iEntIndex, int iChannel, const char* pSoundEntry,
		unsigned int nSoundEntryHash, const char* pSample, float flVolume, int nSeed,
		soundlevel_t iSoundLevel, int iFlags, int iPitch, const Vector* pOrigin,
		const Vector* pDirection, void* pUtlVecOrigins, bool bUpdatePositions, float soundtime,
		int speakerentity, int Unknownn) {

		ESP::EmitSound(iEntIndex, pSample);

		SoundHook->GetOriginalFunction<EmitSound2Fn>(5)(thisptr, filter, iEntIndex, iChannel, pSoundEntry, nSoundEntryHash,
			pSample, flVolume, nSeed, iSoundLevel, iFlags, iPitch, pOrigin,
			pDirection, pUtlVecOrigins, bUpdatePositions, soundtime,
			speakerentity, Unknownn);
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
			pSurface->unlockcursor(); // IDX 66
			return;
		}
		SurfaceHook->GetOriginalFunction<LockCursor>(67)(pSurface);
	}
	
	void __fastcall Hooks::RenderSmokePostViewmodel(void* ecx, void* edx) {
		if (!NoSmoke::RenderSmokePostViewmodel())
			RenderViewHook->GetOriginalFunction<NoSmokeFn>(41)(ecx);
	}


	//		MirrorTexture Not Set.
	//		Tried BeginRenderTargetAllocation after game startup.If I let you do this, all users would suffer.
	//		ImageFormat: 12


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


