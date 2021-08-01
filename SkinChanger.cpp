#include "skinchanger.h"
#include "stickers.h"
bool Settings::Skinchanger::Skins::enabled = false;
bool Settings::Skinchanger::Models::enabled = false;

bool Settings::Skinchanger::Skins::perTeam = false;

bool Settings::Skinchanger::Skins::stickersenabled = false;
int Settings::Skinchanger::Skins::sticker1 = 0;
int Settings::Skinchanger::Skins::sticker2 = 0;
int Settings::Skinchanger::Skins::sticker3 = 0;
int Settings::Skinchanger::Skins::sticker4 = 0;

std::unordered_map<ItemDefinitionIndex, AttribItem_t, Util::IntHash<ItemDefinitionIndex>> Settings::Skinchanger::skinsCT = {
	{ ItemDefinitionIndex::WEAPON_AK47 /*WeaponID*/,{ ItemDefinitionIndex::INVALID /*itemDefinitionIndex*/, 524 /*fallbackPaintKit*/, 0.0005f /*fallbackWear*/, -1 /*fallbackSeed*/, 1337/*fallbackStatTrak*/, -1/*fallbackEntityQuality*/, "XHOOK.XYZ"/*customName*/ } },
	{ ItemDefinitionIndex::WEAPON_KNIFE,{ ItemDefinitionIndex::WEAPON_KNIFE_M9_BAYONET, -1, -1, -1, -1, -1, "" } },
	{ ItemDefinitionIndex::GLOVE_CT_SIDE,{ ItemDefinitionIndex::GLOVE_SPECIALIST, 10006, 0.0005f, -1, -1, -1, "" } },
	{ ItemDefinitionIndex::GLOVE_T_SIDE,{ ItemDefinitionIndex::GLOVE_STUDDED_BLOODHOUND, 10006, 0.0005f, -1, -1, -1, "" } },
	{ ItemDefinitionIndex::GLOVE_STUDDED_BLOODHOUND,{ ItemDefinitionIndex::INVALID, 10006, 0.0005f, -1, -1, -1, "" } },
	{ ItemDefinitionIndex::GLOVE_SPORTY,{ ItemDefinitionIndex::INVALID, 10018, 0.0005f, -1, -1, -1, "" } },
	{ ItemDefinitionIndex::GLOVE_SLICK,{ ItemDefinitionIndex::INVALID, 10013, 0.0005f, -1, -1, -1, "" } },
	{ ItemDefinitionIndex::GLOVE_MOTORCYCLE,{ ItemDefinitionIndex::INVALID, 10024, 0.0005f, -1, -1, -1, "" } },
	{ ItemDefinitionIndex::GLOVE_LEATHER_WRAP,{ ItemDefinitionIndex::INVALID, 10009, 0.0005f, -1, -1, -1, "" } },
	{ ItemDefinitionIndex::GLOVE_SPECIALIST,{ ItemDefinitionIndex::INVALID, 10033, 0.0005f, -1, -1, -1, "" } },
	{ ItemDefinitionIndex::GLOVE_HYDRA,{ ItemDefinitionIndex::INVALID, 10057, 0.0005f, -1, -1, -1, "" } },
	{ ItemDefinitionIndex::WEAPON_KNIFE_M9_BAYONET,{ ItemDefinitionIndex::INVALID, 417, 0.0005f, -1, 1337, -1, "XHOOK.XYZ" } },
	{ ItemDefinitionIndex::WEAPON_KNIFE_KARAMBIT,{ ItemDefinitionIndex::INVALID, 417, 0.0005f, -1, -1, -1, "" } },
	{ ItemDefinitionIndex::WEAPON_BAYONET,{ ItemDefinitionIndex::INVALID, 417, 0.0005f, -1, -1, -1, "" } },
	{ ItemDefinitionIndex::WEAPON_KNIFE_FLIP,{ ItemDefinitionIndex::INVALID, 417, 0.0005f, -1, -1, -1, "" } },
	{ ItemDefinitionIndex::WEAPON_KNIFE_GUT,{ ItemDefinitionIndex::INVALID, 417, 0.0005f, -1, -1, -1, "" } },
	{ ItemDefinitionIndex::WEAPON_KNIFE_BUTTERFLY,{ ItemDefinitionIndex::INVALID, 417, 0.0005f, -1, -1, -1, "" } },
	{ ItemDefinitionIndex::WEAPON_KNIFE_TACTICAL,{ ItemDefinitionIndex::INVALID, 417, 0.0005f, -1, -1, -1, "" } },
	{ ItemDefinitionIndex::WEAPON_KNIFE_PUSH,{ ItemDefinitionIndex::INVALID, 417, 0.0005f, -1, -1, -1, "" } },
	{ ItemDefinitionIndex::WEAPON_KNIFE_CORD,{ ItemDefinitionIndex::INVALID, 417, 0.0005f, -1, -1, -1, "" } },
	{ ItemDefinitionIndex::WEAPON_KNIFE_CANIS,{ ItemDefinitionIndex::INVALID, 417, 0.0005f, -1, -1, -1, "" } },
	{ ItemDefinitionIndex::WEAPON_KNIFE_SURVIVAL_BOWIE,{ ItemDefinitionIndex::INVALID, 417, 0.0005f, -1, -1, -1, "" } },
	{ ItemDefinitionIndex::WEAPON_KNIFE_URSUS,{ ItemDefinitionIndex::INVALID, 417, 0.0005f, -1, -1, -1, "" } },
	{ ItemDefinitionIndex::WEAPON_KNIFE_GYPSY_JACKKNIFE,{ ItemDefinitionIndex::INVALID, 417, 0.0005f, -1, -1, -1, "" } },
	{ ItemDefinitionIndex::WEAPON_KNIFE_OUTDOOR,{ ItemDefinitionIndex::INVALID, 417, 0.0005f, -1, -1, -1, "" } },
	{ ItemDefinitionIndex::WEAPON_KNIFE_STILETTO,{ ItemDefinitionIndex::INVALID, 417, 0.0005f, -1, -1, -1, "" } },
	{ ItemDefinitionIndex::WEAPON_KNIFE_WIDOWMAKER,{ ItemDefinitionIndex::INVALID, 417, 0.0005f, -1, -1, -1, "" } },
	{ ItemDefinitionIndex::WEAPON_USP_SILENCER,{ ItemDefinitionIndex::INVALID, 2, 0.0005f, -1, -1, -1, "" } },
};

std::unordered_map<ItemDefinitionIndex, AttribItem_t, Util::IntHash<ItemDefinitionIndex>> Settings::Skinchanger::skinsT = {
	{ ItemDefinitionIndex::WEAPON_AK47 /*WeaponID*/,{ ItemDefinitionIndex::INVALID /*itemDefinitionIndex*/, 524 /*fallbackPaintKit*/, 0.0005f /*fallbackWear*/, -1 /*fallbackSeed*/, 1337/*fallbackStatTrak*/, -1/*fallbackEntityQuality*/, "XHOOK"/*customName*/ } },
	{ ItemDefinitionIndex::WEAPON_KNIFE_T,{ ItemDefinitionIndex::WEAPON_KNIFE_KARAMBIT, -1, -1, -1, -1, -1, "" } },
	{ ItemDefinitionIndex::GLOVE_T_SIDE,{ ItemDefinitionIndex::GLOVE_STUDDED_BLOODHOUND, 10006, 0.0005f, -1, -1, -1, "" } },
	{ ItemDefinitionIndex::GLOVE_STUDDED_BLOODHOUND,{ ItemDefinitionIndex::INVALID, 10006, 0.0005f, -1, -1, -1, "" } },
	{ ItemDefinitionIndex::GLOVE_SPORTY,{ ItemDefinitionIndex::INVALID, 10018, 0.0005f, -1, -1, -1, "" } },
	{ ItemDefinitionIndex::GLOVE_SLICK,{ ItemDefinitionIndex::INVALID, 10013, 0.0005f, -1, -1, -1, "" } },
	{ ItemDefinitionIndex::GLOVE_MOTORCYCLE,{ ItemDefinitionIndex::INVALID, 10024, 0.0005f, -1, -1, -1, "" } },
	{ ItemDefinitionIndex::GLOVE_LEATHER_WRAP,{ ItemDefinitionIndex::INVALID, 10009, 0.0005f, -1, -1, -1, "" } },
	{ ItemDefinitionIndex::GLOVE_SPECIALIST,{ ItemDefinitionIndex::INVALID, 10033, 0.0005f, -1, -1, -1, "" } },
	{ ItemDefinitionIndex::GLOVE_HYDRA,{ ItemDefinitionIndex::INVALID, 10057, 0.0005f, -1, -1, -1, "" } },
	{ ItemDefinitionIndex::WEAPON_KNIFE_M9_BAYONET,{ ItemDefinitionIndex::INVALID, 417, 0.0005f, -1, -1, -1, "" } },
	{ ItemDefinitionIndex::WEAPON_KNIFE_KARAMBIT,{ ItemDefinitionIndex::INVALID, 417, 0.0005f, -1, 1337, -1, "XHOOK.XYZ" } },
	{ ItemDefinitionIndex::WEAPON_BAYONET,{ ItemDefinitionIndex::INVALID, 417, 0.0005f, -1, -1, -1, "" } },
	{ ItemDefinitionIndex::WEAPON_KNIFE_FLIP,{ ItemDefinitionIndex::INVALID, 417, 0.0005f, -1, -1, -1, "" } },
	{ ItemDefinitionIndex::WEAPON_KNIFE_GUT,{ ItemDefinitionIndex::INVALID, 417, 0.0005f, -1, -1, -1, "" } },
	{ ItemDefinitionIndex::WEAPON_KNIFE_BUTTERFLY,{ ItemDefinitionIndex::INVALID, 417, 0.0005f, -1, -1, -1, "" } },
	{ ItemDefinitionIndex::WEAPON_KNIFE_TACTICAL,{ ItemDefinitionIndex::INVALID, 417, 0.0005f, -1, -1, -1, "" } },
	{ ItemDefinitionIndex::WEAPON_KNIFE_PUSH,{ ItemDefinitionIndex::INVALID, 417, 0.0005f, -1, -1, -1, "" } },
	{ ItemDefinitionIndex::WEAPON_KNIFE_CORD,{ ItemDefinitionIndex::INVALID, 417, 0.0005f, -1, -1, -1, "" } },
	{ ItemDefinitionIndex::WEAPON_KNIFE_CANIS,{ ItemDefinitionIndex::INVALID, 417, 0.0005f, -1, -1, -1, "" } },
	{ ItemDefinitionIndex::WEAPON_KNIFE_SURVIVAL_BOWIE,{ ItemDefinitionIndex::INVALID, 417, 0.0005f, -1, -1, -1, "" } },
	{ ItemDefinitionIndex::WEAPON_KNIFE_URSUS,{ ItemDefinitionIndex::INVALID, 417, 0.0005f, -1, -1, -1, "" } },
	{ ItemDefinitionIndex::WEAPON_KNIFE_GYPSY_JACKKNIFE,{ ItemDefinitionIndex::INVALID, 417, 0.0005f, -1, -1, -1, "" } },
	{ ItemDefinitionIndex::WEAPON_KNIFE_OUTDOOR,{ ItemDefinitionIndex::INVALID, 417, 0.0005f, -1, -1, -1, "" } },
	{ ItemDefinitionIndex::WEAPON_KNIFE_STILETTO,{ ItemDefinitionIndex::INVALID, 417, 0.0005f, -1, -1, -1, "" } },
	{ ItemDefinitionIndex::WEAPON_KNIFE_WIDOWMAKER,{ ItemDefinitionIndex::INVALID, 417, 0.0005f, -1, -1, -1, "" } },

};

std::unordered_map<std::string, std::string> killIcons = {};

std::unique_ptr<RecvPropHook> SkinChanger::sequenceHook;


bool SkinChanger::forceFullUpdate = true;
bool SkinChanger::glovesUpdated = false;

bool NeedsUpdate = false;

void SkinChanger::FrameStageNotifyModels(ClientFrameStage_t stage)
{
	if (!Settings::Skinchanger::Models::enabled)
		return;


		if (!pEngine->IsInGame())
			return;

		if (stage != ClientFrameStage_t::FRAME_NET_UPDATE_POSTDATAUPDATE_START)
			return;

		C_BasePlayer* localplayer = (C_BasePlayer*)pEntityList->GetClientEntity(pEngine->GetLocalPlayer());
		if (!localplayer)
			return;

		IEngineClient::player_info_t localplayer_info;
		if (!pEngine->GetPlayerInfo(pEngine->GetLocalPlayer(), &localplayer_info))
			return;


		if(!localplayer->GetAlive())
		{
			auto glove = static_cast<C_BaseAttributableItem*>(pEntityList->GetClientEntityFromHandle(*localplayer->GetWearables()));
			if (!glove)
				return;

			glove->GetNetworkable()->SetDestroyedOnRecreateEntities();
			glove->GetNetworkable()->Release();

			return;
		}
		

		auto weapons = localplayer->GetWeapons();

		for (size_t i = 0; weapons[i] != 0xFFFFFFFF; i++) //0xFFFFFFF
		{
			if (localplayer->GetWeapons()[i] == -1)
				continue;

			

			auto weapon = static_cast<C_BaseAttributableItem*>(pEntityList->GetClientEntityFromHandle(weapons[i]));
			if (!weapon)
				continue;

			auto keyExists = localplayer->GetTeam() == TeamID::TEAM_COUNTER_TERRORIST ? Settings::Skinchanger::skinsCT.find(*weapon->GetItemDefinitionIndex()) : Settings::Skinchanger::skinsT.find(*weapon->GetItemDefinitionIndex());

			if (keyExists == (localplayer->GetTeam() == TeamID::TEAM_COUNTER_TERRORIST ? Settings::Skinchanger::skinsCT.end() : Settings::Skinchanger::skinsT.end()))
				continue;

			const AttribItem_t &currentModel = localplayer->GetTeam() == TeamID::TEAM_COUNTER_TERRORIST ? Settings::Skinchanger::skinsCT.at(*weapon->GetItemDefinitionIndex()) : Settings::Skinchanger::skinsT.at(*weapon->GetItemDefinitionIndex());

			if (currentModel.itemDefinitionIndex != ItemDefinitionIndex::INVALID && ItemDefinitionIndexMap.find(currentModel.itemDefinitionIndex) != ItemDefinitionIndexMap.end())
			{
				*weapon->GetModelIndex() = pModelInfo->GetModelIndex(ItemDefinitionIndexMap.at(currentModel.itemDefinitionIndex).entityModel);
				if (ItemDefinitionIndexMap.find(*weapon->GetItemDefinitionIndex()) != ItemDefinitionIndexMap.end())
				{
					killIcons[ItemDefinitionIndexMap.at(*weapon->GetItemDefinitionIndex()).killIcon] = ItemDefinitionIndexMap.at(currentModel.itemDefinitionIndex).killIcon;
					*weapon->GetItemDefinitionIndex() = currentModel.itemDefinitionIndex;
				}
			}
		}

			C_BaseViewModel *viewmodel = (C_BaseViewModel *)pEntityList->GetClientEntityFromHandle(localplayer->GetViewModel());
			if (!viewmodel)
				return;

			C_BaseCombatWeapon *activeWeapon = (C_BaseCombatWeapon *)pEntityList->GetClientEntityFromHandle(localplayer->GetActiveWeapon());
			if (!activeWeapon)
				return;

			if (ItemDefinitionIndexMap.find(*activeWeapon->GetItemDefinitionIndex()) != ItemDefinitionIndexMap.end())
				if (Settings::Skinchanger::Models::enabled)
					*viewmodel->GetModelIndex() = pModelInfo->GetModelIndex(ItemDefinitionIndexMap.at(*activeWeapon->GetItemDefinitionIndex()).entityModel);


			if (!pEntityList->GetClientEntity(localplayer->GetWearables()[0] & 0xFFF))
			{
				for (ClientClass *pClass = pClient->GetAllClasses(); pClass; pClass = pClass->m_pNext)
				{
					if (pClass->m_ClassID != EClassIds::ceconwearable)
						continue;

					int entry = (pEntityList->GetHighestEntityIndex() + 1);
					int serial = Util::RandomInt(0x0, 0xFFF);

					pClass->m_pCreateFn(entry, serial);
					localplayer->GetWearables()[0] = entry | (serial << 16);

					SkinChanger::forceFullUpdate = true;
					SkinChanger::glovesUpdated = true;
					//ForceRefresh();
					//NeedsUpdate = true;

					break;
				}
			}
			auto glove = static_cast<C_BaseAttributableItem*>(pEntityList->GetClientEntityFromHandle(*localplayer->GetWearables()));
		//	C_BaseAttributableItem* glove = (C_BaseAttributableItem*)pEntityList->GetClientEntity(localplayer->GetWearables()[0] & 0xFFF);
			if (!glove)
				return;

			auto keyExists = localplayer->GetTeam() == TeamID::TEAM_COUNTER_TERRORIST ? Settings::Skinchanger::skinsCT.find(ItemDefinitionIndex::GLOVE_CT_SIDE) : Settings::Skinchanger::skinsT.find(ItemDefinitionIndex::GLOVE_T_SIDE);

			if (keyExists != (localplayer->GetTeam() == TeamID::TEAM_COUNTER_TERRORIST ? Settings::Skinchanger::skinsCT.end() : Settings::Skinchanger::skinsT.end()))
			{
				const AttribItem_t &currentModel = localplayer->GetTeam() == TeamID::TEAM_COUNTER_TERRORIST ? Settings::Skinchanger::skinsCT.at(ItemDefinitionIndex::GLOVE_CT_SIDE) : Settings::Skinchanger::skinsT.at(ItemDefinitionIndex::GLOVE_T_SIDE);

				if (currentModel.itemDefinitionIndex != ItemDefinitionIndex::INVALID && ItemDefinitionIndexMap.find(currentModel.itemDefinitionIndex) != ItemDefinitionIndexMap.end())
				{
					if (currentModel.itemDefinitionIndex == ItemDefinitionIndex::GLOVE_CT_SIDE || currentModel.itemDefinitionIndex == ItemDefinitionIndex::GLOVE_T_SIDE)
						return;

					if (*glove->GetItemDefinitionIndex() != currentModel.itemDefinitionIndex)
					{
						glove->SetModelIndex(pModelInfo->GetModelIndex(ItemDefinitionIndexMap.at(currentModel.itemDefinitionIndex).entityModel));
						*glove->GetItemDefinitionIndex() = currentModel.itemDefinitionIndex;
					}
				}
			}
}

void SkinChanger::FrameStageNotifySkins(ClientFrameStage_t stage)
{
	if (!Settings::Skinchanger::Skins::enabled)
		return;
	
		if (!pEngine->IsInGame())
			return;


		if (stage != ClientFrameStage_t::FRAME_NET_UPDATE_POSTDATAUPDATE_START)
			return;

		C_BasePlayer* localplayer = (C_BasePlayer*)pEntityList->GetClientEntity(pEngine->GetLocalPlayer());
		if (!localplayer || !localplayer->GetAlive())
			return;

		IEngineClient::player_info_t localplayer_info;

		if (!pEngine->GetPlayerInfo(pEngine->GetLocalPlayer(), &localplayer_info))
			return;

		auto weapons = localplayer->GetWeapons();

		for (size_t i = 0; weapons[i] != 0xFFFFFFFF; i++) //0xFFFFFFF
		{
			if (localplayer->GetWeapons()[i] == -1)
				continue;

			//C_BaseAttributableItem *weapon = (C_BaseAttributableItem *)pEntityList->GetClientEntity(localplayer->GetWeapons()[i] & 0xFFF);

			auto weapon = static_cast<C_BaseAttributableItem*>(pEntityList->GetClientEntityFromHandle(weapons[i]));

			auto keyExists = localplayer->GetTeam() == TeamID::TEAM_COUNTER_TERRORIST || !Settings::Skinchanger::Skins::perTeam ? Settings::Skinchanger::skinsCT.find(*weapon->GetItemDefinitionIndex()) : Settings::Skinchanger::skinsT.find(*weapon->GetItemDefinitionIndex());

			if (keyExists == (localplayer->GetTeam() == TeamID::TEAM_COUNTER_TERRORIST || !Settings::Skinchanger::Skins::perTeam ? Settings::Skinchanger::skinsCT.end() : Settings::Skinchanger::skinsT.end()))
				continue;

			const AttribItem_t &skin = localplayer->GetTeam() == TeamID::TEAM_COUNTER_TERRORIST || !Settings::Skinchanger::Skins::perTeam ? Settings::Skinchanger::skinsCT.at(*weapon->GetItemDefinitionIndex()) : Settings::Skinchanger::skinsT.at(*weapon->GetItemDefinitionIndex());


			if (skin.fallbackPaintKit != -1)
				*weapon->GetFallbackPaintKit() = skin.fallbackPaintKit;

			if (skin.fallbackSeed != -1)
				*weapon->GetFallbackSeed() = skin.fallbackSeed;

			if (skin.fallbackWear != -1)
				*weapon->GetFallbackWear() = skin.fallbackWear;

			if (skin.fallbackStatTrak != -1)
				*weapon->GetFallbackStatTrak() = skin.fallbackStatTrak;

			if (skin.entityQuality != -1)
				*weapon->GetEntityQuality() = skin.entityQuality;

			if (Settings::Skinchanger::Skins::stickersenabled)
			{
				Stickers::ApplyStickerHooks(weapon);  //ApplyStickers	
			}

			if (skin.customName != "")
				snprintf(weapon->GetCustomName(), 32, "%s", skin.customName.c_str());

			*weapon->GetItemIDHigh() = -1;
			*weapon->GetAccountID() = localplayer_info.xuidlow;
		}


		auto glove = static_cast<C_BaseAttributableItem*>(pEntityList->GetClientEntityFromHandle(*localplayer->GetWearables()));
		//C_BaseAttributableItem* glove = (C_BaseAttributableItem*)pEntityList->GetClientEntity(localplayer->GetWearables()[0] & 0xFFF);
		if (!glove)
			return;


		if (localplayer->GetTeam() == TeamID::TEAM_COUNTER_TERRORIST || !Settings::Skinchanger::Skins::perTeam)
		{
			if (Settings::Skinchanger::skinsCT.find(localplayer->GetTeam() == TeamID::TEAM_COUNTER_TERRORIST ? ItemDefinitionIndex::GLOVE_CT_SIDE : ItemDefinitionIndex::GLOVE_T_SIDE) != Settings::Skinchanger::skinsCT.end())
			{
				const AttribItem_t &currentSkinOrig = Settings::Skinchanger::skinsCT.at(localplayer->GetTeam() == TeamID::TEAM_COUNTER_TERRORIST ? ItemDefinitionIndex::GLOVE_CT_SIDE : ItemDefinitionIndex::GLOVE_T_SIDE);

				if (Settings::Skinchanger::skinsCT.find(currentSkinOrig.itemDefinitionIndex) != Settings::Skinchanger::skinsCT.end())
				{
					const AttribItem_t &currentSkin = Settings::Skinchanger::skinsCT.at(currentSkinOrig.itemDefinitionIndex);

					if (currentSkin.fallbackPaintKit != -1)
						*glove->GetFallbackPaintKit() = currentSkin.fallbackPaintKit;

					if (currentSkin.fallbackWear != -1)
						*glove->GetFallbackWear() = currentSkin.fallbackWear;

					*glove->GetFallbackSeed() = 0;
					*glove->GetFallbackStatTrak() = -1;
					*glove->GetEntityQuality() = 4;
					*glove->GetItemIDHigh() = -1;
					*glove->GetAccountID() = localplayer_info.xuidlow;

					if (SkinChanger::glovesUpdated)
					{
						SkinChanger::glovesUpdated = false;
						glove->GetNetworkable()->PreDataUpdate(DATA_UPDATE_CREATED);
					
					//	glove->GetNetworkable()->PreDataUpdate(ERROR_CLUSTER_DATABASE_UPDATE_CONDITION_FAILED);
					}

			

				}
			}
		}
		else if (localplayer->GetTeam() == TeamID::TEAM_TERRORIST && Settings::Skinchanger::Skins::perTeam)
		{
			if (Settings::Skinchanger::skinsT.find(ItemDefinitionIndex::GLOVE_T_SIDE) != Settings::Skinchanger::skinsT.end())
			{
				const AttribItem_t &currentSkinOrig = Settings::Skinchanger::skinsT.at(ItemDefinitionIndex::GLOVE_T_SIDE);

				if (Settings::Skinchanger::skinsT.find(currentSkinOrig.itemDefinitionIndex) != Settings::Skinchanger::skinsT.end())
				{
					const AttribItem_t &currentSkin = Settings::Skinchanger::skinsT.at(currentSkinOrig.itemDefinitionIndex);

					if (currentSkin.fallbackPaintKit != -1)
						*glove->GetFallbackPaintKit() = currentSkin.fallbackPaintKit;

					if (currentSkin.fallbackWear != -1)
						*glove->GetFallbackWear() = currentSkin.fallbackWear;

					*glove->GetFallbackSeed() = 0;
					*glove->GetFallbackStatTrak() = -1;
					*glove->GetEntityQuality() = 4;
					*glove->GetItemIDHigh() = -1;
					*glove->GetAccountID() = localplayer_info.xuidlow;

					if (SkinChanger::glovesUpdated)
					{
						glove->GetNetworkable()->PreDataUpdate(DATA_UPDATE_CREATED);
						SkinChanger::glovesUpdated = false;
					}
				}
			}
		}
}

void SkinChanger::ForceRefresh()
{
	if (!pEngine->IsInGame())
		return;

	pClientState->ForceFullUpdate();
}

void SkinChanger::FireEventClientSide(IGameEvent* event)
{
	if (!Settings::Skinchanger::Models::enabled)
		return;

	if (!pEngine->IsInGame())
		return;

	if (!event || strcmp(event->GetName(), "player_death") != 0)
		return;

	if (!event->GetInt("attacker") || pEngine->GetPlayerForUserID(event->GetInt("attacker")) != pEngine->GetLocalPlayer())
		return;

	std::string weapon = event->GetString("weapon");

	event->SetString("weapon", killIcons.find(weapon) != killIcons.end() ? killIcons.at(weapon).c_str() : weapon.c_str());
}

void SkinChanger::FireGameEvent(IGameEvent* event)
{
	if (!Settings::Skinchanger::Models::enabled)
		return;

	if (!event || strcmp(event->GetName(), "switch_team") != 0)
		return;

	if (!(*csPlayerResource))
		return;

	static TeamID oldTeam = TeamID::TEAM_UNASSIGNED;
	TeamID currentTeam = (*csPlayerResource)->GetTeam(pEngine->GetLocalPlayer());

	if (oldTeam != currentTeam)
	{
		oldTeam = currentTeam;
		SkinChanger::forceFullUpdate = true;
		//ForceRefresh();
	}
}


void SkinChanger::SetViewModelSequence(const CRecvProxyData* pDataConst, void* pStruct, void* pOut) {
	//if (ModSupport::current_mod == ModType::CSCO)
	//	return;

	// Make the incoming data editable.
	CRecvProxyData* pData = const_cast<CRecvProxyData*> (pDataConst);

	// Confirm that we are replacing our view model and not someone elses.
	C_BaseViewModel* pViewModel = (C_BaseViewModel*)pStruct;

	if (pViewModel) {
		IClientEntity* pOwner = (IClientEntity*)pEntityList->GetClientEntity(pViewModel->GetOwner() & 0xFFF);

		// Compare the owner entity of this view model to the local player entity.
		if (pOwner && pOwner->GetIndex() == pEngine->GetLocalPlayer()) {
			// Get the filename of the current view model.
			model_t* pModel = pModelInfo->GetModel(*pViewModel->GetModelIndex());
			std::string szModel = pModelInfo->GetModelName(pModel);

			// Store the current sequence.
			int m_nSequence = pData->m_Value.m_Int;
			if (szModel == "models/weapons/v_knife_butterfly.mdl") {
				// Fix animations for the Butterfly Knife.
				switch (m_nSequence) {
				case SEQUENCE_DEFAULT_DRAW:
					m_nSequence = Util::RandomInt(SEQUENCE_BUTTERFLY_DRAW, SEQUENCE_BUTTERFLY_DRAW2);
					break;
				case SEQUENCE_DEFAULT_LOOKAT01:
					m_nSequence = Util::RandomInt(SEQUENCE_BUTTERFLY_LOOKAT01, SEQUENCE_BUTTERFLY_LOOKAT03);
					break;
				default:
					m_nSequence++;
				}
			}
			else if (szModel == "models/weapons/v_knife_falchion_advanced.mdl") {
				// Fix animations for the Falchion Knife.
				switch (m_nSequence) {
				case SEQUENCE_DEFAULT_IDLE2:
					m_nSequence = SEQUENCE_FALCHION_IDLE1;
					break;
				case SEQUENCE_DEFAULT_HEAVY_MISS1:
					m_nSequence = Util::RandomInt(SEQUENCE_FALCHION_HEAVY_MISS1,
						SEQUENCE_FALCHION_HEAVY_MISS1_NOFLIP);
					break;
				case SEQUENCE_DEFAULT_LOOKAT01:
					m_nSequence = Util::RandomInt(SEQUENCE_FALCHION_LOOKAT01, SEQUENCE_FALCHION_LOOKAT02);
					break;
				case SEQUENCE_DEFAULT_DRAW:
				case SEQUENCE_DEFAULT_IDLE1:
					break;
				default:
					m_nSequence--;
				}
			}
			else if (szModel == "models/weapons/v_knife_push.mdl") {
				// Fix animations for the Shadow Daggers.
				switch (m_nSequence) {
				case SEQUENCE_DEFAULT_IDLE2:
					m_nSequence = SEQUENCE_DAGGERS_IDLE1;
					break;
				case SEQUENCE_DEFAULT_LIGHT_MISS1:
				case SEQUENCE_DEFAULT_LIGHT_MISS2:
					m_nSequence = Util::RandomInt(SEQUENCE_DAGGERS_LIGHT_MISS1, SEQUENCE_DAGGERS_LIGHT_MISS5);
					break;
				case SEQUENCE_DEFAULT_HEAVY_MISS1:
					m_nSequence = Util::RandomInt(SEQUENCE_DAGGERS_HEAVY_MISS2, SEQUENCE_DAGGERS_HEAVY_MISS1);
					break;
				case SEQUENCE_DEFAULT_HEAVY_HIT1:
				case SEQUENCE_DEFAULT_HEAVY_BACKSTAB:
				case SEQUENCE_DEFAULT_LOOKAT01:
					m_nSequence += 3;
					break;
				case SEQUENCE_DEFAULT_DRAW:
				case SEQUENCE_DEFAULT_IDLE1:
					break;
				default:
					m_nSequence += 2;
				}
			}
			else if (szModel == "models/weapons/v_knife_survival_bowie.mdl") {
				// Fix animations for the Bowie Knife.
				switch (m_nSequence) {
				case SEQUENCE_DEFAULT_DRAW:
				case SEQUENCE_DEFAULT_IDLE1:
					break;
				case SEQUENCE_DEFAULT_IDLE2:
					m_nSequence = SEQUENCE_BOWIE_IDLE1;
					break;
				default:
					m_nSequence--;
				}
			}

			// Set the fixed sequence.
			pData->m_Value.m_Int = m_nSequence;
		}
	}

	// Call original function with the modified data.
	if (!fnSequenceProxyFn)
		fnSequenceProxyFn = sequenceHook->GetOriginalFunction();

	fnSequenceProxyFn(pData, pStruct, pOut);
}
