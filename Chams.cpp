#include "Features.h"
#include "Util_SDK.h"
#include "Hooks.h"

bool Settings::ESP::Chams::enabled = false;
bool Settings::ESP::Chams::Arms::enabled = false;
bool Settings::ESP::Chams::Weapon::enabled = false;
bool Settings::ESP::Chams::Weapon::DroppedWeapons = false;
ArmsType Settings::ESP::Chams::Arms::type = ArmsType::DEFAULT;
HealthColorVar Settings::ESP::Chams::allyColor = ImColor(0, 0, 255, 255);
HealthColorVar Settings::ESP::Chams::allyVisibleColor = ImColor(0, 255, 0, 255);
HealthColorVar Settings::ESP::Chams::enemyColor = ImColor(255, 0, 0, 255);
HealthColorVar Settings::ESP::Chams::enemyVisibleColor = ImColor(255, 255, 0, 255);

HealthColorVar Settings::ESP::Chams::enemyBacktrackColor = ImColor(0, 255, 0, 255);
HealthColorVar Settings::ESP::Chams::enemyBacktrackVisibleColor = ImColor(0, 0, 255, 255);
HealthColorVar Settings::ESP::Chams::allyBacktrackColor = ImColor(0, 0, 255, 255);
HealthColorVar Settings::ESP::Chams::allyBacktracVisibleColor = ImColor(0, 255, 0, 255);

HealthColorVar Settings::ESP::Chams::localplayerColor = ImColor(0, 255, 255, 255);
ColorVar Settings::ESP::Chams::Arms::color = ImColor(255, 255, 255, 255);
ColorVar Settings::ESP::Chams::Weapon::color = ImColor(255, 255, 255, 255);
ChamsType Settings::ESP::Chams::type = ChamsType::CHAMS;

IMaterial* Chams::chamsMaterial = nullptr;
IMaterial* Chams::ignoredChamsMaterial = nullptr;

IMaterial* materialChams;
IMaterial* materialChamsIgnorez;
IMaterial* materialChamsFlat;
IMaterial* materialChamsFlatIgnorez;
IMaterial* materialChamsGlass;
IMaterial* materialChamsCrystal;
IMaterial* materialChamsBlueCrystal;
IMaterial* materialChamsGold;
IMaterial* materialChamsVelvet;
IMaterial* materialChamsTree;
IMaterial* materialChamsSpeechInfo;
IMaterial* materialChamsFishNet;
IMaterial* materialChamsLetters;
IMaterial* materialChamsGloss;
IMaterial* materialChamsAnimated;
IMaterial* materialChamsBlink;
IMaterial* materialChamsGlow;
IMaterial* materialChamsGuerilla;
IMaterial* materialChamsArms;
IMaterial* materialChamsWeapons;
IMaterial* materialFakeChams;


static void DrawBacktrackedPlayer(IMatRenderContext* matctx, const DrawModelState_t& state, const ModelRenderInfo_t& pInfo, matrix3x4_t* pCustomBoneToWorld)
{
	if (!Settings::ESP::Chams::enabled)
		return;

	/*for (unsigned int i = 1; i < 16; i++) {*/

		C_BasePlayer* localplayer = (C_BasePlayer*)pEntityList->GetClientEntity(pEngine->GetLocalPlayer());
		if (!localplayer)
			return;

		C_BasePlayer* entity = (C_BasePlayer*)pEntityList->GetClientEntity(pInfo.entity_index);
		if (!entity
			|| entity->GetDormant()
			|| !entity->GetAlive())
			return;
		
		if (entity == localplayer && !Settings::ESP::Filters::localplayer)
			return;

		if (entity->GetTeam() != localplayer->GetTeam() && !Settings::ESP::Filters::enemies)
			return;

		if (entity != localplayer && entity->GetTeam() == localplayer->GetTeam() && !Settings::ESP::Filters::allies)
			return;

		for (unsigned int i = 1; i < Backtracking::lagRecords[entity->GetIndex()].size(); i++) {

		IMaterial* visible_material;
		IMaterial* hidden_material;

		switch (Settings::ESP::Chams::type) {
		case ChamsType::CHAMS:
		case ChamsType::CHAMS_XQZ:
			visible_material = materialChams;
			hidden_material = materialChamsIgnorez;
			break;
		case ChamsType::CHAMS_FLAT:
		case ChamsType::CHAMS_FLAT_XQZ:
			visible_material = materialChamsFlat;
			hidden_material = materialChamsFlatIgnorez;
			break;
		case ChamsType::CHAMS_GLASS:
			visible_material = materialChamsGlass;
			hidden_material = materialChamsGlass;
			break;
		case ChamsType::CHAMS_CRYSTAL:
			visible_material = materialChamsCrystal;
			hidden_material = materialChamsCrystal;
			break;
		case ChamsType::CHAMS_CRYSTALBLUE:
			visible_material = materialChamsBlueCrystal;
			hidden_material = materialChamsBlueCrystal;
			break;
		case ChamsType::CHAMS_GOLD:
			visible_material = materialChamsGold;
			hidden_material = materialChamsGold;
			break;
		case ChamsType::CHAMS_VELVET:
			visible_material = materialChamsVelvet;
			hidden_material = materialChamsVelvet;
			break;
		case ChamsType::CHAMS_TREE:
			visible_material = materialChamsTree;
			hidden_material = materialChamsTree;
			break;
		case ChamsType::CHAMS_SPEECHINFO:
			visible_material = materialChamsSpeechInfo;
			hidden_material = materialChamsSpeechInfo;
			break;
		case ChamsType::CHAMS_FISHNET:
			visible_material = materialChamsFishNet;
			hidden_material = materialChamsFishNet;
			break;
		case ChamsType::CHAMS_LETTERS:
			visible_material = materialChamsLetters;
			hidden_material = materialChamsLetters;
			break;
		case ChamsType::CHAMS_GLOSS:
			visible_material = materialChamsGloss;
			hidden_material = materialChamsGloss;
			break;
		case ChamsType::CHAMS_ANIMATED:
			visible_material = materialChamsAnimated;
			hidden_material = materialChamsAnimated;
			break;
		case ChamsType::CHAMS_BLINK:
			visible_material = materialChamsBlink;
			hidden_material = materialChamsBlink;
			break;
		case ChamsType::CHAMS_GLOW:
			visible_material = materialChamsGlow;
			hidden_material = materialChamsGlow;
			break;
		case ChamsType::CHAMS_GUERILLA:
			visible_material = materialChamsGuerilla;
			hidden_material = materialChamsGuerilla;
			break;
		}


		visible_material->AlphaModulate(1.0f);
		hidden_material->AlphaModulate(1.0f);

		if (entity == localplayer) {
			Color visColor = Color::FromImColor(Settings::ESP::Chams::localplayerColor.Color(entity));
			Color color = visColor;
			color *= 0.45f;

			visible_material->ColorModulate(visColor.r / 255.f, visColor.g / 255.f, visColor.b / 255.f);
			hidden_material->ColorModulate(color.r / 255.f, color.g / 255.f, color.b / 255.f);
		}
		else if (entity->GetTeam() == localplayer->GetTeam()) {
			Color visColor = Color::FromImColor(Settings::ESP::Chams::allyBacktracVisibleColor.Color(entity));
			Color color = Color::FromImColor(Settings::ESP::Chams::allyBacktrackColor.Color(entity));

			visible_material->ColorModulate(visColor.r / 255.f, visColor.g / 255.f, visColor.b / 255.f);
			hidden_material->ColorModulate(color.r / 255.f, color.g / 255.f, color.b / 255.f);
		}
		else if (entity->GetTeam() != localplayer->GetTeam()) {
			Color visColor = Color::FromImColor(Settings::ESP::Chams::enemyBacktrackVisibleColor.Color(entity));
			Color color = Color::FromImColor(Settings::ESP::Chams::enemyBacktrackColor.Color(entity));

			visible_material->ColorModulate(visColor.r / 255.f, visColor.g / 255.f, visColor.b / 255.f);
			hidden_material->ColorModulate(color.r / 255.f, color.g / 255.f, color.b / 255.f);
		}
		else {
			return;
		}

		if (entity->GetImmune()) {
			visible_material->AlphaModulate(0.5f);
			hidden_material->AlphaModulate(0.5f);
		}

		if (!Settings::ESP::Filters::legit && (Settings::ESP::Chams::type == ChamsType::CHAMS_XQZ ||
			Settings::ESP::Chams::type == ChamsType::CHAMS_FLAT_XQZ)) {
			if (Settings::Aimbot::LegitBackTrack)
			{
				if (localplayer->GetAlive())
				{
					pModelRender->ForcedMaterialOverride(hidden_material);
					ModelRenderHook->GetOriginalFunction<DrawModelExecuteFn>(21)(pModelRender, matctx, state, pInfo, Backtracking::lagRecords[entity->GetIndex()][i].pBoneToWorldOut);
					hidden_material->SetMaterialVarFlag(MATERIAL_VAR_IGNOREZ, true);
				}
				else
				{
					Backtracking::lagRecords.clear();
					pModelRender->ForcedMaterialOverride(visible_material);
					ModelRenderHook->GetOriginalFunction<DrawModelExecuteFn>(21)(pModelRender, matctx, state, pInfo, pCustomBoneToWorld);
				}
			}
			else if (!Settings::Aimbot::LegitBackTrack)
			{
				Backtracking::lagRecords.clear();
				pModelRender->ForcedMaterialOverride(hidden_material);
				ModelRenderHook->GetOriginalFunction<DrawModelExecuteFn>(21)(pModelRender, matctx, state, pInfo, pCustomBoneToWorld);
			}
		}
		if (Settings::ThirdPerson::enabled && entity == localplayer && localplayer->IsScoped())
		{
			visible_material->AlphaModulate(Settings::ThirdPerson::transparency);
		}
		if (Settings::Aimbot::LegitBackTrack)
		{
			if (localplayer->GetAlive())
			{
				pModelRender->ForcedMaterialOverride(visible_material);
				ModelRenderHook->GetOriginalFunction<DrawModelExecuteFn>(21)(pModelRender, matctx, state, pInfo, Backtracking::lagRecords[entity->GetIndex()][i].pBoneToWorldOut);
			
			}
			else
			{
				Backtracking::lagRecords.clear();
				pModelRender->ForcedMaterialOverride(visible_material);
				ModelRenderHook->GetOriginalFunction<DrawModelExecuteFn>(21)(pModelRender, matctx, state, pInfo, pCustomBoneToWorld);
			}
		}
		else if (!Settings::Aimbot::LegitBackTrack)
		{
			Backtracking::lagRecords.clear();
			pModelRender->ForcedMaterialOverride(visible_material);
			ModelRenderHook->GetOriginalFunction<DrawModelExecuteFn>(21)(pModelRender, matctx, state, pInfo, pCustomBoneToWorld);
		}
	}
}


static void DrawPlayer(IMatRenderContext* matctx, const DrawModelState_t& state, const ModelRenderInfo_t& pInfo, matrix3x4_t* pCustomBoneToWorld)
{
	if (!Settings::ESP::Chams::enabled)
		return;

	//DrawBacktrackedPlayer(matctx, state, pInfo, pCustomBoneToWorld);

		C_BasePlayer* localplayer = (C_BasePlayer*)pEntityList->GetClientEntity(pEngine->GetLocalPlayer());
		if (!localplayer)
			return;

		C_BasePlayer* entity = (C_BasePlayer*)pEntityList->GetClientEntity(pInfo.entity_index);
		if (!entity
			|| entity->GetDormant()
			|| !entity->GetAlive())
			return;

		if (entity == localplayer && !Settings::ESP::Filters::localplayer)
			return;

		if (entity->GetTeam() != localplayer->GetTeam() && !Settings::ESP::Filters::enemies)
			return;

		if (entity != localplayer && entity->GetTeam() == localplayer->GetTeam() && !Settings::ESP::Filters::allies)
			return;

		IMaterial* visible_material;
		IMaterial* hidden_material;

		switch (Settings::ESP::Chams::type) {
		case ChamsType::CHAMS:
		case ChamsType::CHAMS_XQZ:
			visible_material = materialChams;
			hidden_material = materialChamsIgnorez;
			break;
		case ChamsType::CHAMS_FLAT:
		case ChamsType::CHAMS_FLAT_XQZ:
			visible_material = materialChamsFlat;
			hidden_material = materialChamsFlatIgnorez;
			break;
		case ChamsType::CHAMS_GLASS:
			visible_material = materialChamsGlass;
			hidden_material = materialChamsGlass;
			break;
		case ChamsType::CHAMS_CRYSTAL:
			visible_material = materialChamsCrystal;
			hidden_material = materialChamsCrystal;
			break;
		case ChamsType::CHAMS_CRYSTALBLUE:
			visible_material = materialChamsBlueCrystal;
			hidden_material = materialChamsBlueCrystal;
			break;
		case ChamsType::CHAMS_GOLD:
			visible_material = materialChamsGold;
			hidden_material = materialChamsGold;
			break;
		case ChamsType::CHAMS_VELVET:
			visible_material = materialChamsVelvet;
			hidden_material = materialChamsVelvet;
			break;
		case ChamsType::CHAMS_TREE:
			visible_material = materialChamsTree;
			hidden_material = materialChamsTree;
			break;
		case ChamsType::CHAMS_SPEECHINFO:
			visible_material = materialChamsSpeechInfo;
			hidden_material = materialChamsSpeechInfo;
			break;
		case ChamsType::CHAMS_FISHNET:
			visible_material = materialChamsFishNet;
			hidden_material = materialChamsFishNet;
			break;
		case ChamsType::CHAMS_LETTERS:
			visible_material = materialChamsLetters;
			hidden_material = materialChamsLetters;
			break;
		case ChamsType::CHAMS_GLOSS:
			visible_material = materialChamsGloss;
			hidden_material = materialChamsGloss;
			break;
		case ChamsType::CHAMS_ANIMATED:
			visible_material = materialChamsAnimated;
			hidden_material = materialChamsAnimated;
			break;
		case ChamsType::CHAMS_BLINK:
			visible_material = materialChamsBlink;
			hidden_material = materialChamsBlink;
			break;
		case ChamsType::CHAMS_GLOW:
			visible_material = materialChamsGlow;
			hidden_material = materialChamsGlow;
			break;
		case ChamsType::CHAMS_GUERILLA:
			visible_material = materialChamsGuerilla;
			hidden_material = materialChamsGuerilla;
			break;
		}


		visible_material->AlphaModulate(1.0f);
		hidden_material->AlphaModulate(1.0f);

		if (entity == localplayer) {
			Color visColor = Color::FromImColor(Settings::ESP::Chams::localplayerColor.Color(entity));
			Color color = visColor;
			color *= 0.45f;

			visible_material->ColorModulate(visColor.r / 255.f, visColor.g / 255.f, visColor.b / 255.f);
			hidden_material->ColorModulate(color.r / 255.f, color.g / 255.f, color.b / 255.f);
		}
		else if (entity->GetTeam() == localplayer->GetTeam()) {
			Color visColor = Color::FromImColor(Settings::ESP::Chams::allyVisibleColor.Color(entity));
			Color color = Color::FromImColor(Settings::ESP::Chams::allyColor.Color(entity));

			visible_material->ColorModulate(visColor.r / 255.f, visColor.g / 255.f, visColor.b / 255.f);
			hidden_material->ColorModulate(color.r / 255.f, color.g / 255.f, color.b / 255.f);
		}
		else if (entity->GetTeam() != localplayer->GetTeam()) {
			Color visColor = Color::FromImColor(Settings::ESP::Chams::enemyVisibleColor.Color(entity));
			Color color = Color::FromImColor(Settings::ESP::Chams::enemyColor.Color(entity));

			visible_material->ColorModulate(visColor.r / 255.f, visColor.g / 255.f, visColor.b / 255.f);
			hidden_material->ColorModulate(color.r / 255.f, color.g / 255.f, color.b / 255.f);
		}
		else {
			return;
		}

		if (entity->GetImmune()) {
			visible_material->AlphaModulate(0.5f);
			hidden_material->AlphaModulate(0.5f);
		}

		if (!Settings::ESP::Filters::legit && (Settings::ESP::Chams::type == ChamsType::CHAMS_XQZ ||
			Settings::ESP::Chams::type == ChamsType::CHAMS_FLAT_XQZ)) {
			pModelRender->ForcedMaterialOverride(hidden_material);
			ModelRenderHook->GetOriginalFunction<DrawModelExecuteFn>(21)(pModelRender, matctx, state, pInfo, pCustomBoneToWorld);
		}
		if (Settings::ThirdPerson::enabled && entity == localplayer && localplayer->IsScoped())
		{
			visible_material->AlphaModulate(Settings::ThirdPerson::transparency);
		}
		pModelRender->ForcedMaterialOverride(visible_material);
		ModelRenderHook->GetOriginalFunction<DrawModelExecuteFn>(21)(pModelRender, matctx, state, pInfo, pCustomBoneToWorld);
}
static void DrawWeapon(const ModelRenderInfo_t& pInfo) {
	if (!Settings::ESP::Chams::Weapon::enabled)
		return;

	std::string modelName = pModelInfo->GetModelName(pInfo.pModel);
	IMaterial* mat = materialChamsWeapons;

	if (!Settings::ESP::Chams::Weapon::enabled)
		mat = pMaterial->FindMaterial(modelName.c_str(), TEXTURE_GROUP_MODEL);

	mat->AlphaModulate(1.0f);
	mat->ColorModulate(Settings::ESP::Chams::Weapon::color.Color().Value.x, Settings::ESP::Chams::Weapon::color.Color().Value.y, Settings::ESP::Chams::Weapon::color.Color().Value.z);

	pModelRender->ForcedMaterialOverride(mat);
}



static void DrawArms(const ModelRenderInfo_t& pInfo) {
	if (!Settings::ESP::Chams::Arms::enabled)
		return;

	std::string modelName = pModelInfo->GetModelName(pInfo.pModel);
	IMaterial* mat = materialChamsArms;

	if (!Settings::ESP::Chams::Arms::enabled)
		mat = pMaterial->FindMaterial(modelName.c_str(), TEXTURE_GROUP_MODEL);
	switch (Settings::ESP::Chams::Arms::type) {
	default:
		break;
	case ArmsType::DEFAULT:
		mat->AlphaModulate(1.0f);
		mat->ColorModulate(Settings::ESP::Chams::Arms::color.Color().Value.x, Settings::ESP::Chams::Arms::color.Color().Value.y, Settings::ESP::Chams::Arms::color.Color().Value.z);
		break;
	case ArmsType::GLASS:
		mat = pMaterial->FindMaterial("models/inventory_items/cologne_prediction/cologne_prediction_glass",
			TEXTURE_GROUP_OTHER);
		mat->AlphaModulate(1.0f);
		mat->ColorModulate(Settings::ESP::Chams::Arms::color.Color().Value.x, Settings::ESP::Chams::Arms::color.Color().Value.y, Settings::ESP::Chams::Arms::color.Color().Value.z);
		break;
	case ArmsType::CRYSTAL:
		mat = pMaterial->FindMaterial("models/inventory_items/trophy_majors/crystal_clear", TEXTURE_GROUP_OTHER);
		mat->AlphaModulate(1.0f);
		mat->ColorModulate(Settings::ESP::Chams::Arms::color.Color().Value.x, Settings::ESP::Chams::Arms::color.Color().Value.y, Settings::ESP::Chams::Arms::color.Color().Value.z);
		break;
	case ArmsType::CRYSTALBLUE:
		mat = pMaterial->FindMaterial("models/inventory_items/trophy_majors/crystal_blue", TEXTURE_GROUP_OTHER);
		mat->AlphaModulate(1.0f);
		mat->ColorModulate(Settings::ESP::Chams::Arms::color.Color().Value.x, Settings::ESP::Chams::Arms::color.Color().Value.y, Settings::ESP::Chams::Arms::color.Color().Value.z);
		break;
	case ArmsType::GOLD:
		mat = pMaterial->FindMaterial("models/inventory_items/wildfire_gold/wildfire_gold_detail",
			TEXTURE_GROUP_OTHER);
		mat->AlphaModulate(1.0f);
		mat->ColorModulate(Settings::ESP::Chams::Arms::color.Color().Value.x, Settings::ESP::Chams::Arms::color.Color().Value.y, Settings::ESP::Chams::Arms::color.Color().Value.z);
		break;
	case ArmsType::VELVET:
		mat = pMaterial->FindMaterial("models/inventory_items/trophy_majors/velvet", TEXTURE_GROUP_OTHER);
		mat->AlphaModulate(1.0f);
		mat->ColorModulate(Settings::ESP::Chams::Arms::color.Color().Value.x, Settings::ESP::Chams::Arms::color.Color().Value.y, Settings::ESP::Chams::Arms::color.Color().Value.z);
		break;
	case ArmsType::TREE:
		mat = pMaterial->FindMaterial("models/props_foliage/urban_tree03_branches", TEXTURE_GROUP_OTHER);
		mat->AlphaModulate(1.0f);
		mat->ColorModulate(Settings::ESP::Chams::Arms::color.Color().Value.x, Settings::ESP::Chams::Arms::color.Color().Value.y, Settings::ESP::Chams::Arms::color.Color().Value.z);
		break;
	case ArmsType::SPEECHINFO:
		mat = pMaterial->FindMaterial("models/extras/speech_info", TEXTURE_GROUP_OTHER);
		mat->AlphaModulate(1.0f);
		mat->ColorModulate(Settings::ESP::Chams::Arms::color.Color().Value.x, Settings::ESP::Chams::Arms::color.Color().Value.y, Settings::ESP::Chams::Arms::color.Color().Value.z);
		break;
	case ArmsType::FISHNET:
		mat = pMaterial->FindMaterial("models/props_shacks/fishing_net01", TEXTURE_GROUP_OTHER);
		mat->AlphaModulate(1.0f);
		mat->ColorModulate(Settings::ESP::Chams::Arms::color.Color().Value.x, Settings::ESP::Chams::Arms::color.Color().Value.y, Settings::ESP::Chams::Arms::color.Color().Value.z);
		break;
	case ArmsType::LETTERS:
		mat = pMaterial->FindMaterial("models/inventory_items/contributor_map_tokens/contributor_charset_color",
			TEXTURE_GROUP_OTHER);
		mat->AlphaModulate(1.0f);
		mat->ColorModulate(Settings::ESP::Chams::Arms::color.Color().Value.x, Settings::ESP::Chams::Arms::color.Color().Value.y, Settings::ESP::Chams::Arms::color.Color().Value.z);
		break;
	case ArmsType::GLOSS:
		mat = pMaterial->FindMaterial("models/inventory_items/trophy_majors/gloss", TEXTURE_GROUP_OTHER);
		mat->AlphaModulate(1.0f);
		mat->ColorModulate(Settings::ESP::Chams::Arms::color.Color().Value.x, Settings::ESP::Chams::Arms::color.Color().Value.y, Settings::ESP::Chams::Arms::color.Color().Value.z);
		break;
	case ArmsType::WIREFRAME:	
			mat->SetMaterialVarFlag(MATERIAL_VAR_WIREFRAME, Settings::ESP::Chams::Arms::type == ArmsType::WIREFRAME);
			mat->AlphaModulate(1.0f);
			mat->ColorModulate(Settings::ESP::Chams::Arms::color.Color().Value.x, Settings::ESP::Chams::Arms::color.Color().Value.y, Settings::ESP::Chams::Arms::color.Color().Value.z);
	}

	mat->SetMaterialVarFlag(MATERIAL_VAR_WIREFRAME, Settings::ESP::Chams::Arms::type == ArmsType::WIREFRAME);
	mat->SetMaterialVarFlag(MATERIAL_VAR_NO_DRAW, Settings::ESP::Chams::Arms::type == ArmsType::NONE);
	pModelRender->ForcedMaterialOverride(mat);
}

static void DrawDroppedWeapon(const ModelRenderInfo_t& pInfo) {

	if (!Settings::ESP::Chams::Weapon::DroppedWeapons)
		return;


	std::string modelName = pModelInfo->GetModelName(pInfo.pModel);
	IMaterial* mat = materialChamsWeapons;

	if (!Settings::ESP::Chams::Weapon::DroppedWeapons)
		mat = pMaterial->FindMaterial(modelName.c_str(), TEXTURE_GROUP_OTHER);

	mat->AlphaModulate(1.0f);
	mat->ColorModulate(Settings::ESP::Chams::Weapon::color.Color().Value.x, Settings::ESP::Chams::Weapon::color.Color().Value.y, Settings::ESP::Chams::Weapon::color.Color().Value.z);

	pModelRender->ForcedMaterialOverride(mat);
}

void Chams::DrawModelExecute(IMatRenderContext* matctx, const DrawModelState_t& state, const ModelRenderInfo_t& pInfo, matrix3x4_t* pCustomBoneToWorld)
{
	if (!pEngine->IsInGame())
		return;

	if (!Settings::ESP::enabled)
		return;

	if (!pInfo.pModel)
		return;

	static bool materialsCreated = false;
	if (!materialsCreated) {
		materialChams = Util::CreateMaterial(XorStr("VertexLitGeneric"), XorStr("VGUI/white_additive"), false,
			true, true, true, true);
		materialFakeChams = Util::CreateMaterial(XorStr("VertexLitGeneric"), XorStr("VGUI/white_additive"), false,
			true, true, true, true);
		materialChamsIgnorez = Util::CreateMaterial(XorStr("VertexLitGeneric"), XorStr("VGUI/white_additive"),
			true, true, true, true, true);
		materialChamsFlat = Util::CreateMaterial(XorStr("UnlitGeneric"), XorStr("VGUI/white_additive"), false,
			true, true, true, true);
		materialChamsFlatIgnorez = Util::CreateMaterial(XorStr("UnlitGeneric"), XorStr("VGUI/white_additive"),
			true, true, true, true, true);
		materialChamsArms = Util::CreateMaterial(XorStr("VertexLitGeneric"), XorStr("VGUI/white_additive"), false,
			true, true, true, true);
		materialChamsWeapons = Util::CreateMaterial(XorStr("VertexLitGeneric"), XorStr("VGUI/white_additive"),
			false, true, true, true, true);
		materialChamsGlass = pMaterial->FindMaterial(
			"models/inventory_items/cologne_prediction/cologne_prediction_glass", TEXTURE_GROUP_OTHER);
		materialChamsCrystal = pMaterial->FindMaterial("models/inventory_items/trophy_majors/crystal_clear",
			TEXTURE_GROUP_OTHER);
		materialChamsBlueCrystal = pMaterial->FindMaterial("models/inventory_items/trophy_majors/crystal_blue",
			TEXTURE_GROUP_OTHER);
		materialChamsGold = pMaterial->FindMaterial("models/inventory_items/wildfire_gold/wildfire_gold_detail",
			TEXTURE_GROUP_OTHER);
		materialChamsVelvet = pMaterial->FindMaterial("models/inventory_items/trophy_majors/velvet",
			TEXTURE_GROUP_OTHER);
		materialChamsTree = pMaterial->FindMaterial("models/props_foliage/urban_tree03_branches", TEXTURE_GROUP_OTHER);
		materialChamsSpeechInfo = pMaterial->FindMaterial("models/extras/speech_info", TEXTURE_GROUP_OTHER);
		materialChamsFishNet = pMaterial->FindMaterial("models/props_shacks/fishing_net01", TEXTURE_GROUP_OTHER);
		materialChamsLetters = pMaterial->FindMaterial(
			"models/inventory_items/contributor_map_tokens/contributor_charset_color", TEXTURE_GROUP_OTHER);
		materialChamsGloss = pMaterial->FindMaterial("models/inventory_items/trophy_majors/gloss",
			TEXTURE_GROUP_OTHER);
		materialChamsAnimated = pMaterial->FindMaterial("models/inventory_items/music_kit/darude_01/mp3_detail", TEXTURE_GROUP_OTHER);

		materialChamsBlink = pMaterial->FindMaterial("models/inventory_items/dogtags/dogtags_outline", TEXTURE_GROUP_OTHER);

		materialChamsGlow = pMaterial->FindMaterial("vgui/achievements/glow", TEXTURE_GROUP_OTHER);

		materialChamsGuerilla = pMaterial->FindMaterial("models/player/t_guerilla/t_guerilla", TEXTURE_GROUP_OTHER);

		materialsCreated = true;

		Chams::chamsMaterial = materialChams;
		Chams::ignoredChamsMaterial = materialChamsIgnorez;
	}

	std::string modelName = pModelInfo->GetModelName(pInfo.pModel);

	if (modelName.find(XorStr("models/player")) != std::string::npos)
		DrawBacktrackedPlayer(matctx, state, pInfo, pCustomBoneToWorld);
	else if (modelName.find(XorStr("arms")) != std::string::npos)
		DrawArms(pInfo);
	else if (modelName.find(XorStr("weapon")) != std::string::npos)
		DrawWeapon(pInfo);
	else if (modelName.find(XorStr("_dropped.mdl")) != std::string::npos)
		DrawDroppedWeapon(pInfo);
}
