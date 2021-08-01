#include "Menu.h"
#include "../ImGui/imgui_internal.h"
#include "../settings.h"
#include "Tabs/aimtab.h"
#include "Tabs/HvHTab.h"
#include "Tabs/HvHTabs/standinghvhtab.h"
#include "Tabs/SkinsTab.h"
#include "Windows/skinmodelchanger.h"
#include "Tabs/ModelsTab.h"
#include "../Features.h"
#include "../SDK/ICvar.h"
#include "../SDK/IClientEntity.h"

#include "../Shared.h"

CSteamID Settings::steamIdLobby;

bool Settings::UI::MainUI = false;
bool Settings::UI::middle = true;
bool Settings::UI::right = false;

ImVec2 center = { 100, 100 };

void UI::KeyBindButton(ButtonCode_t* key) {
	G::text = pInputSystem->ButtonCodeToString(*key);

	if (SetKeyCodeState::shouldListen && SetKeyCodeState::keyOutput == key)
	{
		G::text = XorStr("-- press a key --");
	}
	else
		G::text = (G::text);

	if (ImGui::Button(G::text, ImVec2(-1, 0))) {
		SetKeyCodeState::shouldListen = true;
		G::input_shouldListen = true;
		SetKeyCodeState::keyOutput = key;
	}
}


static int currentPlayer = -1;
bool Settings::ScreenshotCleaner::enabled = false;

ColorVar Settings::UI::accentColor = ImColor(43, 115, 178, 74);  //    |-»
ColorVar Settings::UI::mainColor = ImColor(46, 133, 200, 255);
ColorVar Settings::UI::bodyColor = ImColor(28, 33, 32, 228);
ColorVar Settings::UI::fontColor = ImColor(255, 255, 255, 255);
const char* BoxTypes[] = { "Flat 2D", "Frame 2D", "Box 3D", "Hitbox" };
const char* TracerTypes[] = { "Bottom", "Cursor" };
const char* BarTypes[] = { "Vertical Left", "Vertical Right", "Horizontal Below", "Horizontal Above", "Interwebz" };
const char* ArmorBarTypes[] = { "Vertical Left", "Vertical Right", "Horizontal Below", "Horizontal Above" };
const char* BarColorTypes[] = { "Static", "Health Based" };
const char* TeamColorTypes[] = { "Absolute", "Relative" };
const char* ChamsTypes[] = { "Normal", "Normal - XQZ", "Flat", "Flat - XQZ", "Glass", "Crystal", "BlueCrystal",
"Gold", "Velvet", "Tree", "SpeechInfo", "FishNet", "Letters", "Gloss" ,"Animated", "Blink", "Glow", "Guerilla" };
const char* ArmsTypes[] = { "Default", "Glass", "Crystal", "BlueCrystal", "Gold", "Velvet", "Tree", "SpeechInfo",
"FishNet", "Letters", "Gloss", "Wireframe", "None" };
const char* tpModes[] = { "Fake", "Real", "Lower Body Yaw"};
const char* ChamModes[] = { "Fake", "Real", "Lower Body Yaw", "Ghost" };
const char* strafeTypes[] = { "Forwards", "Backwards", "Left", "Right", "Rage" };
const char* animationTypes[] = { "Static", "Current Time", "Marquee", "Words", "Letters" };
const char* spammerTypes[] = { "None", "Normal", "Positions" };
const char* presetTypes[] = { "XHOOK.XYZ", "Dogeii Gang", "Custom" };
const char* lagTypes[] = { "Off", "AimTux", "Normal", "Step", "Reactive", "Nuclear", "Lunico" };
const char* teams[] = { "Allies", "Enemies", "Both" };
const char* grenadeTypes[] = { "Flashbang", "Smoke Grenade", "Molotov", "HE Grenade" };
const char* throwTypes[] = { "NORMAL", "RUN", "JUMP", "WALK" };
static char nickname[127] = "xhook";
static char BanMsg[127] = "";
const char* Sounds[] = { "None", "kickmetaldoor2", "default_doorstop", "metal", "dont_leave!", "thunder", "beep", "bass", "Sk00ter", "MedKit" };
const char* RadioSpammer[] = { "None", "Normal", "Rare"};

const char *tracerEffectNames[] = {
	"Assault Rifle", // 0
	"Pistol",
	"SMG",
	"Rifle",
	"Kisak Snot",
	"Machine Gun",
	"Shotgun",
	"Kisak Snot Fallback",
	"Kisak Snot Fallback2",
	"Wire1A",
	"Wire2",
	"Wire1B",
	"Original",
	"Backup",
	".50 Cal",
	".50 Cal Glow",
	".50 Cal Low",
	".50 Cal Low Glow", // 17
};

static const char* ranklist[] =
{
	"OFF",
	"Silver I",
	"Silver II",
	"Silver III",
	"Silver IV",
	"Silver Elite",
	"Silver Elite Master",

	"Gold Nova I",
	"Gold Nova II",
	"Gold Nova III",
	"Gold Nova Master",
	"Master Guardian I",
	"Master Guardian II",

	"Master Guardian Elite",
	"Distinguished Master Guardian",
	"Legendary Eagle",
	"Legendary Eagle Master",
	"Supreme Master First Class",
	"Global Elite"

};

const char* SkyBoxes[] = {
	"cs_baggage_skybox_", // 0
	"cs_tibet",
	"embassy",
	"italy",
	"jungle",
	"office",
	"nukeblank",
	"sky_venice",
	"sky_cs15_daylight01_hdr",
	"sky_cs15_daylight02_hdr",
	"sky_cs15_daylight03_hdr",
	"sky_cs15_daylight04_hdr",
	"sky_csgo_cloudy01",
	"sky_csgo_night_flat",
	"sky_csgo_night02",
	"sky_csgo_night02b",
	"sky_day02_05",
	"sky_day02_05_hdr",
	"sky_dust",
	"vertigo",
	"vertigo_hdr",
	"vertigoblue_hdr",
	"vietnam" // 21
};

const char* tabs[] = {
	"Aim",
	"Visuals",
	"HvH",
	"Player List",
	"Misc",
	"Colors",
	"Skin changer",
	"Steam spoofer"
};

static int listbox_item_current = 1;
static int listbox_item_currentRanks = 1;
static int listbox_item_currentMedals = 1;
struct stat GNadeInfo; // for the dirs

struct ColorListVar
{
	const char* name;
	union
	{
		ColorVar* colorVarPtr;
		HealthColorVar* healthColorVarPtr;
	};
	enum
	{
		COLORVAR_TYPE,
		HEALTHCOLORVAR_TYPE
	} type;

	ColorListVar(const char* name, ColorVar* ptr)
	{
		this->name = name;
		this->colorVarPtr = ptr;
		this->type = COLORVAR_TYPE;
	}

	ColorListVar(const char* name, HealthColorVar* ptr)
	{
		this->name = name;
		this->healthColorVarPtr = ptr;
		this->type = HEALTHCOLORVAR_TYPE;
	}
};

ColorListVar colors[] = {
	{ "UI Main", &Settings::UI::mainColor },
	{ "UI Body", &Settings::UI::bodyColor },
	{ "UI Font", &Settings::UI::fontColor },
	{ "FOV Circle", &Settings::ESP::FOVCrosshair::color },
	{ "Sniper Crosshair", &Settings::SniperCrosshair::color },
	{ "Spread", &Settings::ESP::Spread::color },
	{ "SpreadLimit", &Settings::ESP::Spread::spreadLimitColor },
	{ "Hitmarker", &Settings::ESP::Hitmarker::color },
	{ "ESP - Enemy", &Settings::ESP::enemyColor },
	{ "ESP - Team", &Settings::ESP::allyColor },
	{ "ESP - Enemy Visible", &Settings::ESP::enemyVisibleColor },
	{ "ESP - Team Visible", &Settings::ESP::allyVisibleColor },
	{ "ESP - CT", &Settings::ESP::ctColor },
	{ "ESP - T", &Settings::ESP::tColor },
	{ "ESP - CT Visible", &Settings::ESP::ctVisibleColor },
	{ "ESP - T Visible", &Settings::ESP::tVisibleColor },
	{ "ESP - LocalPlayer", &Settings::ESP::localplayerColor },
	{ "ESP - Bomb", &Settings::ESP::bombColor },
	{ "ESP - Bomb Defusing", &Settings::ESP::bombDefusingColor },
	{ "ESP - Hostage", &Settings::ESP::hostageColor },
	{ "ESP - Defuser", &Settings::ESP::defuserColor },
	{ "ESP - Weapon", &Settings::ESP::weaponColor },
	{ "ESP - Chicken", &Settings::ESP::chickenColor },
	{ "ESP - Fish", &Settings::ESP::fishColor },
	{ "ESP - Smoke", &Settings::ESP::smokeColor },
	{ "ESP - Decoy", &Settings::ESP::decoyColor },
	{ "ESP - Flashbang", &Settings::ESP::flashbangColor },
	{ "ESP - Grenade", &Settings::ESP::grenadeColor },
	{ "ESP - Molotov", &Settings::ESP::molotovColor },
	{ "ESP - Skeleton", &Settings::ESP::Skeleton::color },
	{ "ESP - Backtracked Skeleton", &Settings::ESP::SkeletonBacktrack::color },
	{ "Chams - Team", &Settings::ESP::Chams::allyColor },
	{ "Chams - Team Visible", &Settings::ESP::Chams::allyVisibleColor },
	{ "Chams - Team Backtracked", &Settings::ESP::Chams::allyBacktrackColor },
	{ "Chams - Team Visible Backtracked", &Settings::ESP::Chams::allyBacktracVisibleColor },
	{ "Chams - Enemy", &Settings::ESP::Chams::enemyColor },
	{ "Chams - Enemy Visible", &Settings::ESP::Chams::enemyVisibleColor },
	{ "Chams - Enemy Backtracked", &Settings::ESP::Chams::enemyBacktrackColor },
	{ "Chams - Enemy Visible Backtracked", &Settings::ESP::Chams::enemyBacktrackVisibleColor },
	{ "Chams - LocalPlayer", &Settings::ESP::Chams::localplayerColor },
	{ "Chams - Arms", &Settings::ESP::Chams::Arms::color },
	{ "Chams - Weapon", &Settings::ESP::Chams::Weapon::color },
	{ "Grenade Helper - Aim Line", &Settings::GrenadeHelper::aimLine },
	{ "Grenade Helper - Aim Dot", &Settings::GrenadeHelper::aimDot },
	{ "Grenade Helper - HE Info", &Settings::GrenadeHelper::infoHE },
	{ "Grenade Helper - Smoke Info", &Settings::GrenadeHelper::infoSmoke },
	{ "Grenade Helper - Molotov Info", &Settings::GrenadeHelper::infoMolotov },
	{ "Grenade Helper - Flash Info", &Settings::GrenadeHelper::infoFlash },
	{ "Grenade Prediction - Aim Line", &Settings::GrenadePrediction::color },
	{ "Radar - Enemy", &Settings::Radar::enemyColor },
	{ "Radar - Team", &Settings::Radar::allyColor },
	{ "Radar - Enemy Visible", &Settings::Radar::enemyVisibleColor },
	{ "Radar - Team Visible", &Settings::Radar::allyVisibleColor },
	{ "Radar - CT", &Settings::Radar::ctColor },
	{ "Radar - T", &Settings::Radar::tColor },
	{ "Radar - CT Visible", &Settings::Radar::ctVisibleColor },
	{ "Radar - T Visible", &Settings::Radar::tVisibleColor },
	{ "Radar - Bomb", &Settings::Radar::bombColor },
	{ "Radar - Bomb Defusing", &Settings::Radar::bombDefusingColor },
	{ "Glow - Team", &Settings::ESP::Glow::allyColor },
	{ "Glow - Enemy", &Settings::ESP::Glow::enemyColor },
	{ "Glow - Enemy Visible", &Settings::ESP::Glow::enemyVisibleColor },
	{ "Glow - LocalPlayer", &Settings::ESP::Glow::localplayerColor },
	{ "Glow - Weapon", &Settings::ESP::Glow::weaponColor },
	{ "Glow - Grenade", &Settings::ESP::Glow::grenadeColor },
	{ "Glow - Defuser", &Settings::ESP::Glow::defuserColor },
	{ "Glow - Chicken", &Settings::ESP::Glow::chickenColor },
	{ "Sky", &Settings::NoSky::color },
	{ "Walls", &Settings::ASUSWalls::color },
};

namespace ImGui
{


	static auto vector_getter = [](void* vec, int idx, const char** out_text)
	{
		auto& vector = *static_cast<std::vector<std::string>*>(vec);
		if (idx < 0 || idx >= static_cast<int>(vector.size())) { return false; }
		*out_text = vector.at(idx).c_str();
		return true;
	};

	inline bool Combo(const char* label, int* currIndex, std::vector<std::string>& values) {
		if (values.empty()) { return false; }
		return Combo(label, currIndex, vector_getter,
			static_cast<void*>(&values), values.size());
	}

	inline bool
		ListBox(const char* label, int* currIndex, std::vector<std::string>& values, int height_in_items = -1) {
		if (values.empty()) { return false; }
		return ListBox(label, currIndex, vector_getter,
			static_cast<void*>(&values), values.size(), height_in_items);
	}


	class Tab
	{
	private:

		std::vector<std::string> labels;

	public:

		void add(std::string name)
		{
			labels.push_back(name);
		}

		void draw(int *selected)
		{
			ImGuiStyle &style = GetStyle();
			ImVec4 color = style.Colors[ImGuiCol_Button];
			ImVec4 colorActive = style.Colors[ImGuiCol_ButtonActive];
			ImVec4 colorHover = style.Colors[ImGuiCol_ButtonHovered];
			ImVec2 max = GetContentRegionMax();
			float size_x = max.x / labels.size() - 20.f;
			float size_y = max.y / labels.size() - 40.f;

			for (size_t i = 0; i < labels.size(); i++)
			{
				if (i == *selected)
				{
					style.Colors[ImGuiCol_Button] = colorActive;
					style.Colors[ImGuiCol_ButtonActive] = colorActive;
					style.Colors[ImGuiCol_ButtonHovered] = colorActive;
				}
				else
				{
					style.Colors[ImGuiCol_Button] = color;
					style.Colors[ImGuiCol_ButtonActive] = colorActive;
					style.Colors[ImGuiCol_ButtonHovered] = colorHover;
				}

				if (Button(labels.at(i).c_str(), { size_x, size_y }))
					*selected = i;

				/*if (i != labels.size() - 1)
				SameLine();*/
			}

			style.Colors[ImGuiCol_Button] = color;
			style.Colors[ImGuiCol_ButtonActive] = colorActive;
			style.Colors[ImGuiCol_ButtonHovered] = colorHover;
		}
	};
}


const char* colorNames[IM_ARRAYSIZE(colors)];
char ConfigNames[128] = "";
bool ColorPicker(float* col, bool alphabar)
{
	const int EDGE_SIZE = 200; // = int(ImGui::GetWindowWidth() * 0.75f);
	const ImVec2 SV_PICKER_SIZE = ImVec2(EDGE_SIZE, EDGE_SIZE);
	const float SPACING = ImGui::GetStyle().ItemInnerSpacing.x;
	const float HUE_PICKER_WIDTH = 20.f;
	const float CROSSHAIR_SIZE = 7.0f;

	ImColor color(col[0], col[1], col[2]);
	bool value_changed = false;

	ImDrawList* draw_list = ImGui::GetWindowDrawList();

	ImVec2 picker_pos = ImGui::GetCursorScreenPos();

	float hue, saturation, value;
	ImGui::ColorConvertRGBtoHSV(color.Value.x, color.Value.y, color.Value.z, hue, saturation, value);

	ImColor colors[] = {
		ImColor(255, 0, 0),
		ImColor(255, 255, 0),
		ImColor(0, 255, 0),
		ImColor(0, 255, 255),
		ImColor(0, 0, 255),
		ImColor(255, 0, 255),
		ImColor(255, 0, 0)
	};

	for (int i = 0; i < 6; i++)
	{
		draw_list->AddRectFilledMultiColor(
			ImVec2(picker_pos.x + SV_PICKER_SIZE.x + SPACING, picker_pos.y + i * (SV_PICKER_SIZE.y / 6)),
			ImVec2(picker_pos.x + SV_PICKER_SIZE.x + SPACING + HUE_PICKER_WIDTH,
				picker_pos.y + (i + 1) * (SV_PICKER_SIZE.y / 6)),
			colors[i],
			colors[i],
			colors[i + 1],
			colors[i + 1]
		);
	}

	draw_list->AddLine(
		ImVec2(picker_pos.x + SV_PICKER_SIZE.x + SPACING - 2, picker_pos.y + hue * SV_PICKER_SIZE.y),
		ImVec2(picker_pos.x + SV_PICKER_SIZE.x + SPACING + 2 + HUE_PICKER_WIDTH, picker_pos.y + hue * SV_PICKER_SIZE.y),
		ImColor(255, 255, 255)
	);

	if (alphabar)
	{
		float alpha = col[3];

		draw_list->AddRectFilledMultiColor(
			ImVec2(picker_pos.x + SV_PICKER_SIZE.x + 2 * SPACING + HUE_PICKER_WIDTH, picker_pos.y),
			ImVec2(picker_pos.x + SV_PICKER_SIZE.x + 2 * SPACING + 2 * HUE_PICKER_WIDTH, picker_pos.y + SV_PICKER_SIZE.y),
			ImColor(0, 0, 0), ImColor(0, 0, 0), ImColor(255, 255, 255), ImColor(255, 255, 255)
		);

		draw_list->AddLine(
			ImVec2(picker_pos.x + SV_PICKER_SIZE.x + 2 * (SPACING - 2) + HUE_PICKER_WIDTH, picker_pos.y + alpha * SV_PICKER_SIZE.y),
			ImVec2(picker_pos.x + SV_PICKER_SIZE.x + 2 * (SPACING + 2) + 2 * HUE_PICKER_WIDTH, picker_pos.y + alpha * SV_PICKER_SIZE.y),
			ImColor(255.f - alpha, 255.f, 255.f)
		);
	}

	const ImU32 c_oColorBlack = ImGui::ColorConvertFloat4ToU32(ImVec4(0.f, 0.f, 0.f, 1.f));
	const ImU32 c_oColorBlackTransparent = ImGui::ColorConvertFloat4ToU32(ImVec4(0.f, 0.f, 0.f, 0.f));
	const ImU32 c_oColorWhite = ImGui::ColorConvertFloat4ToU32(ImVec4(1.f, 1.f, 1.f, 1.f));

	ImVec4 cHueValue(1, 1, 1, 1);
	ImGui::ColorConvertHSVtoRGB(hue, 1, 1, cHueValue.x, cHueValue.y, cHueValue.z);
	ImU32 oHueColor = ImGui::ColorConvertFloat4ToU32(cHueValue);

	draw_list->AddRectFilledMultiColor(
		ImVec2(picker_pos.x, picker_pos.y),
		ImVec2(picker_pos.x + SV_PICKER_SIZE.x, picker_pos.y + SV_PICKER_SIZE.y),
		c_oColorWhite,
		oHueColor,
		oHueColor,
		c_oColorWhite
	);

	draw_list->AddRectFilledMultiColor(
		ImVec2(picker_pos.x, picker_pos.y),
		ImVec2(picker_pos.x + SV_PICKER_SIZE.x, picker_pos.y + SV_PICKER_SIZE.y),
		c_oColorBlackTransparent,
		c_oColorBlackTransparent,
		c_oColorBlack,
		c_oColorBlack
	);

	float x = saturation * SV_PICKER_SIZE.x;
	float y = (1 - value) * SV_PICKER_SIZE.y;
	ImVec2 p(picker_pos.x + x, picker_pos.y + y);
	draw_list->AddLine(ImVec2(p.x - CROSSHAIR_SIZE, p.y), ImVec2(p.x - 2, p.y), ImColor(255, 255, 255));
	draw_list->AddLine(ImVec2(p.x + CROSSHAIR_SIZE, p.y), ImVec2(p.x + 2, p.y), ImColor(255, 255, 255));
	draw_list->AddLine(ImVec2(p.x, p.y + CROSSHAIR_SIZE), ImVec2(p.x, p.y + 2), ImColor(255, 255, 255));
	draw_list->AddLine(ImVec2(p.x, p.y - CROSSHAIR_SIZE), ImVec2(p.x, p.y - 2), ImColor(255, 255, 255));

	ImGui::InvisibleButton("saturation_value_selector", SV_PICKER_SIZE);

	if (ImGui::IsItemActive() && ImGui::GetIO().MouseDown[0])
	{
		ImVec2 mouse_pos_in_canvas = ImVec2(ImGui::GetIO().MousePos.x - picker_pos.x, ImGui::GetIO().MousePos.y - picker_pos.y);

		if (mouse_pos_in_canvas.x <	0)
			mouse_pos_in_canvas.x = 0;
		else if (mouse_pos_in_canvas.x >= SV_PICKER_SIZE.x - 1)
			mouse_pos_in_canvas.x = SV_PICKER_SIZE.x - 1;

		if (mouse_pos_in_canvas.y < 0)
			mouse_pos_in_canvas.y = 0;
		else if (mouse_pos_in_canvas.y >= SV_PICKER_SIZE.y - 1)
			mouse_pos_in_canvas.y = SV_PICKER_SIZE.y - 1;

		value = 1 - (mouse_pos_in_canvas.y / (SV_PICKER_SIZE.y - 1));
		saturation = mouse_pos_in_canvas.x / (SV_PICKER_SIZE.x - 1);
		value_changed = true;
	}

	// hue bar logic
	ImGui::SetCursorScreenPos(ImVec2(picker_pos.x + SPACING + SV_PICKER_SIZE.x, picker_pos.y));
	ImGui::InvisibleButton("hue_selector", ImVec2(HUE_PICKER_WIDTH, SV_PICKER_SIZE.y));

	if (ImGui::GetIO().MouseDown[0] && (ImGui::IsItemHovered() || ImGui::IsItemActive()))
	{
		ImVec2 mouse_pos_in_canvas = ImVec2(ImGui::GetIO().MousePos.x - picker_pos.x, ImGui::GetIO().MousePos.y - picker_pos.y);

		if (mouse_pos_in_canvas.y <	0)
			mouse_pos_in_canvas.y = 0;
		else if (mouse_pos_in_canvas.y >= SV_PICKER_SIZE.y - 1)
			mouse_pos_in_canvas.y = SV_PICKER_SIZE.y - 1;

		hue = mouse_pos_in_canvas.y / (SV_PICKER_SIZE.y - 1);
		value_changed = true;
	}

	if (alphabar)
	{
		ImGui::SetCursorScreenPos(ImVec2(picker_pos.x + SPACING * 2 + HUE_PICKER_WIDTH + SV_PICKER_SIZE.x, picker_pos.y));
		ImGui::InvisibleButton("alpha_selector", ImVec2(HUE_PICKER_WIDTH, SV_PICKER_SIZE.y));

		if (ImGui::GetIO().MouseDown[0] && (ImGui::IsItemHovered() || ImGui::IsItemActive()))
		{
			ImVec2 mouse_pos_in_canvas = ImVec2(ImGui::GetIO().MousePos.x - picker_pos.x, ImGui::GetIO().MousePos.y - picker_pos.y);

			if (mouse_pos_in_canvas.y <	0)
				mouse_pos_in_canvas.y = 0;
			else if (mouse_pos_in_canvas.y >= SV_PICKER_SIZE.y - 1)
				mouse_pos_in_canvas.y = SV_PICKER_SIZE.y - 1;

			float alpha = mouse_pos_in_canvas.y / (SV_PICKER_SIZE.y - 1);
			col[3] = alpha;
			value_changed = true;
		}
	}

	color = ImColor::HSV(hue >= 1 ? hue - 10 * 1e-6 : hue, saturation > 0 ? saturation : 10 * 1e-6, value > 0 ? value : 1e-6);
	col[0] = color.Value.x;
	col[1] = color.Value.y;
	col[2] = color.Value.z;

	bool widget_used;
	ImGui::PushItemWidth((alphabar ? SPACING + HUE_PICKER_WIDTH : 0) + SV_PICKER_SIZE.x + SPACING + HUE_PICKER_WIDTH - 2 * ImGui::GetStyle().FramePadding.x);
	widget_used = alphabar ? ImGui::ColorEdit4("", col) : ImGui::ColorEdit3("", col);
	ImGui::PopItemWidth();

	float new_hue, new_sat, new_val;
	ImGui::ColorConvertRGBtoHSV(col[0], col[1], col[2], new_hue, new_sat, new_val);

	if (new_hue <= 0 && hue > 0)
	{
		if (new_val <= 0 && value != new_val)
		{
			color = ImColor::HSV(hue, saturation, new_val <= 0 ? value * 0.5f : new_val);
			col[0] = color.Value.x;
			col[1] = color.Value.y;
			col[2] = color.Value.z;
		}
		else if (new_sat <= 0)
		{
			color = ImColor::HSV(hue, new_sat <= 0 ? saturation * 0.5f : new_sat, new_val);
			col[0] = color.Value.x;
			col[1] = color.Value.y;
			col[2] = color.Value.z;
		}
	}

	return value_changed | widget_used;
}
bool ColorPicker4(float col[4])
{
	return ColorPicker(col, true);
}


static char fName[128] = "default";
bool Settings::HWID::LoggedIn = true;
static char Pass[256] = "";
static int miss = 0;

static bool security;

//void Authentication()
//{
//	ImGui::SetNextWindowPos(ImVec2(1, 1));
//	ImGui::Begin("Login", nullptr, ImVec2(0, 0), 0.5f, ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoResize);
//
//	if (!Settings::HWID::LoggedIn) {
//		if (g_pSecurity->isVerified()) {
//			ImGui::OpenPopup(XorStr("XHOOK"));
//		}
//		else {
//			g_pSecurity->isVerified();
//			ImGui::OpenPopup(XorStr("HWID Error."));
//		}
//	}
//	if (miss >= 4) {
//		Settings::HWID::uninject = true;
//		ExitProcess(NULL);
//	}
//	if (ImGui::BeginPopupModal(XorStr("HWID Error."))) {
//		ImGui::Text(
//			XorStr(" Oooops looks like something went wrong \n close this window and open csgo console for more info ")
//		);
//		ImGui::Spacing();
//
//		ImGui::Separator();
//		ImGui::PushItemWidth(188);
//
//		if (ImGui::Button(XorStr("Ok"))) {
//			pCvar->ConsoleColorPrintf(ColorRGBA(255, 0, 0),
//				XorStr("[XHOOK] This device is not recognized. Wait one hour and try again \n[XHOOK] Visit xhook.xyz/forum for more information."));
//			g_pSecurity->SendHWID = true;
//			g_pSecurity->Thread();
//			ImGui::CloseCurrentPopup();
//		}
//		ImGui::EndPopup();
//	}
//
//	ImGui::Columns(1);
//
//	if (ImGui::BeginPopupModal(XorStr("XHOOK"), NULL, ImGuiWindowFlags_NoResize)) {
//		ImGui::Text(
//			XorStr("Welcome to Project XHOOK. \nPlease enter your Verification ID: ")
//		);
//		ImGui::SameLine();
//		if (ImGui::Button(" ", ImVec2(1, 1))) {
//			Settings::HWID::LoggedIn = true;
//
//			ImGui::CloseCurrentPopup();
//		}
//		ImGui::Spacing();
//		ImGui::BulletText(" Verification-ID ");
//		ImGui::Separator();
//		ImGui::PushItemWidth(440);
//		ImGui::Image(UsericoTexture, ImVec2(16, 16));
//		ImGui::SameLine();
//		ImGui::InputText("", Pass, IM_ARRAYSIZE(Pass),
//			ImGuiInputTextFlags_CharsNoBlank | ImGuiInputTextFlags_Password |
//			ImGuiInputTextFlags_AutoSelectAll);
//		ImGui::PopItemWidth();
//		ImGui::Separator();
//		if (ImGui::Button(XorStr("Login"))) {
//			if (g_pSecurity->pwmatch(Pass)) {
//				Settings::HWID::LoggedIn = true;
//				ImGui::CloseCurrentPopup();
//				miss = 0;
//				g_pSecurity->timerstart();
//
//			}
//			else {
//				Settings::HWID::LoggedIn = false;
//				miss += 1;
//				ImGui::CloseCurrentPopup();
//
//				ImGui::OpenPopup("whoops");
//			}
//
//		}
//		ImGui::EndPopup();
//	}
//	ImGui::End();
//}

void DrawUserPanel()
{
	ImGui::SetNextWindowPos(ImVec2(10, 10));
	ImGui::Begin("UserPanel", nullptr, ImVec2(0, 0), 0.5f, ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoResize);

	ImGui::Checkbox("AntiAfk", &Settings::Misc::AntiAfk);
	ImGui::Checkbox("Reportbot", &Settings::Reportbot::Enabled);
	ImGui::SameLine();
	ImGui::PushID("Reportbot");

	if (ImGui::Button("..."))
		ImGui::OpenPopup("");

	if (ImGui::BeginPopup("")) {
		ImGui::PushItemWidth(80.0f);
		ImGui::Combo("Target", &Settings::Reportbot::target, "Enemies\0Allies\0All\0");
		ImGui::InputInt("Delay (s)", &Settings::Reportbot::delay);
		Settings::Reportbot::delay = (std::max)(Settings::Reportbot::delay, 1);
		ImGui::InputInt("Rounds", &Settings::Reportbot::rounds);
		Settings::Reportbot::rounds = (std::max)(Settings::Reportbot::rounds, 1);
		ImGui::PopItemWidth();
		ImGui::Checkbox("Abusive Communications", &Settings::Reportbot::textAbuse);
		ImGui::Checkbox("Griefing", &Settings::Reportbot::griefing);
		ImGui::Checkbox("Wall Hacking", &Settings::Reportbot::wallhack);
		ImGui::Checkbox("Aim Hacking", &Settings::Reportbot::aimbot);
		ImGui::Checkbox("Other Hacking", &Settings::Reportbot::other);
		if (ImGui::Button("Reset"))
			ReportBot::resetReportbot();
		ImGui::EndPopup();
	}
	ImGui::PopID();
	//if (ImGui::Button(XorStr("Logout  "),
	//	ImVec2(ImGui::CalcTextSize(XorStr("Logout  "), NULL, true).x, 0.0f))) {
	//	Settings::HWID::LoggedIn = false;
	//	Configs::showWindow = false;
	//	Settings::Aimbot::enabled = false;
	//	Settings::ESP::enabled = false;
	//}

	ImGui::Separator();
	if (ImGui::Button(XorStr("RQUIT  "),
		ImVec2(ImGui::CalcTextSize(XorStr("RQUIT  "), NULL, true).x, 0.0f)))
	{
		Settings::HWID::uninject = true;
		ExitProcess(NULL);
	}

	ImGui::End();
}

void Menu()
{
	static int colorSelected = 0;
	static bool mouse_enabled, Visuals, VisualsTab, Misc, MiscTab, Aim, AimTab, HvH, HvHTab, Skins, SkinsTab, Models, ModelsTab, steamspoofer, steamspoofertab;

	G::is_renderer_active = renderer.IsActive();

	if (G::is_renderer_active) {
		if (mouse_enabled) {
			//pEngine->ClientCmd_Unrestricted("toggleconsole");
			//pCvar->FindVar("cl_mouseenable")->SetValue(mouse_enabled);
			//pInputSystem->EnableInput(!mouse_enabled);
			//pInputSystem->ResetInputState();
		//	pEngine->ClientCmd_Unrestricted("cl_mouseenable 0");
			mouse_enabled = false;
		}
	}
	else {
		if (!mouse_enabled) {
		//	pEngine->ClientCmd_Unrestricted("toggleconsole");
		/*	pCvar->FindVar("cl_mouseenable")->SetValue(!mouse_enabled);
			pInputSystem->EnableInput(!mouse_enabled);
			pInputSystem->ResetInputState();*/
			mouse_enabled = true;
		}
	}
	ImGui::GetIO().MouseDrawCursor = G::is_renderer_active;
	Settings::UI::MainUI = G::is_renderer_active;

	ImGui_ImplDX9_NewFrame();

	ImVec2 Pos;// The location of the Doge Picture 
	static ImGuiStyle* style = &ImGui::GetStyle();
	static bool once;
	if (!once)
	{
		ImGuiStyle& style = ImGui::GetStyle();

		ImVec4 mainColorHovered = ImVec4(Settings::UI::mainColor.Color().Value.x + 0.1f, Settings::UI::mainColor.Color().Value.y + 0.1f, Settings::UI::mainColor.Color().Value.z + 0.1f, Settings::UI::mainColor.Color().Value.w);
		ImVec4 mainColorActive = ImVec4(Settings::UI::mainColor.Color().Value.x + 0.2f, Settings::UI::mainColor.Color().Value.y + 0.2f, Settings::UI::mainColor.Color().Value.z + 0.2f, Settings::UI::mainColor.Color().Value.w);
		ImVec4 menubarColor = ImVec4(Settings::UI::bodyColor.Color().Value.x, Settings::UI::bodyColor.Color().Value.y, Settings::UI::bodyColor.Color().Value.z, Settings::UI::bodyColor.Color().Value.w - 0.8f);
		ImVec4 frameBgColor = ImVec4(Settings::UI::bodyColor.Color().Value.x, Settings::UI::bodyColor.Color().Value.y, Settings::UI::bodyColor.Color().Value.z, Settings::UI::bodyColor.Color().Value.w + .1f);
		ImVec4 tooltipBgColor = ImVec4(Settings::UI::bodyColor.Color().Value.x, Settings::UI::bodyColor.Color().Value.y, Settings::UI::bodyColor.Color().Value.z, Settings::UI::bodyColor.Color().Value.w + .05f);

		style.Alpha = 1.0f;
		style.WindowPadding = ImVec2(8, 8);
		style.WindowMinSize = ImVec2(32, 32);
		style.WindowRounding = 3.0f;
		style.WindowTitleAlign = ImVec2(0.5f, 0.5f);
		style.ChildWindowRounding = 3.0f;
		style.FramePadding = ImVec2(4, 3);
		style.FrameRounding = 2.0f;
		style.ItemSpacing = ImVec2(8, 4);
		style.ItemInnerSpacing = ImVec2(4, 4);
		style.TouchExtraPadding = ImVec2(0, 0);
		style.IndentSpacing = 21.0f;
		style.ColumnsMinSpacing = 3.0f;
		style.ScrollbarSize = 12.0f;
		style.ScrollbarRounding = 0.0f;
		style.GrabMinSize = 5.0f;
		style.GrabRounding = 2.0f;
		style.ButtonTextAlign = ImVec2(0.5f, 0.5f);
		style.DisplayWindowPadding = ImVec2(22, 22);
		style.DisplaySafeAreaPadding = ImVec2(4, 4);
		style.AntiAliasedLines = true;
		style.AntiAliasedShapes = true;
		style.CurveTessellationTol = 1.25f;

		style.Colors[ImGuiCol_Text] = Settings::UI::fontColor.Color();
		style.Colors[ImGuiCol_TextDisabled] = ImVec4(0.24f, 0.23f, 0.29f, 1.00f);
		style.Colors[ImGuiCol_WindowBg] = Settings::UI::bodyColor.Color();
		style.Colors[ImGuiCol_ChildWindowBg] = ImVec4(.0f, .0f, .0f, .0f);
		style.Colors[ImGuiCol_PopupBg] = tooltipBgColor;
		style.Colors[ImGuiCol_Border] = Settings::UI::mainColor.Color();
		style.Colors[ImGuiCol_BorderShadow] = ImVec4(0.92f, 0.91f, 0.88f, 0.00f);
		style.Colors[ImGuiCol_FrameBg] = frameBgColor;
		style.Colors[ImGuiCol_FrameBgHovered] = mainColorHovered;
		style.Colors[ImGuiCol_FrameBgActive] = mainColorActive;
		style.Colors[ImGuiCol_TitleBg] = Settings::UI::mainColor.Color();
		style.Colors[ImGuiCol_TitleBgCollapsed] = ImVec4(1.00f, 0.98f, 0.95f, 0.75f);
		style.Colors[ImGuiCol_TitleBgActive] = Settings::UI::mainColor.Color();
		style.Colors[ImGuiCol_MenuBarBg] = menubarColor;
		style.Colors[ImGuiCol_ScrollbarBg] = ImVec4(frameBgColor.x + .05f, frameBgColor.y + .05f, frameBgColor.z + .05f, frameBgColor.w);
		style.Colors[ImGuiCol_ScrollbarGrab] = Settings::UI::mainColor.Color();
		style.Colors[ImGuiCol_ScrollbarGrabHovered] = mainColorHovered;
		style.Colors[ImGuiCol_ScrollbarGrabActive] = mainColorActive;
		style.Colors[ImGuiCol_Separator] = mainColorActive;
		style.Colors[ImGuiCol_ComboBg] = frameBgColor;
		style.Colors[ImGuiCol_CheckMark] = Settings::UI::mainColor.Color();
		style.Colors[ImGuiCol_SliderGrab] = mainColorHovered;
		style.Colors[ImGuiCol_SliderGrabActive] = mainColorActive;
		style.Colors[ImGuiCol_Button] = Settings::UI::mainColor.Color();
		style.Colors[ImGuiCol_ButtonHovered] = mainColorHovered;
		style.Colors[ImGuiCol_ButtonActive] = mainColorActive;
		style.Colors[ImGuiCol_Header] = Settings::UI::mainColor.Color();
		style.Colors[ImGuiCol_HeaderHovered] = mainColorHovered;
		style.Colors[ImGuiCol_HeaderActive] = mainColorActive;
		style.Colors[ImGuiCol_Column] = Settings::UI::mainColor.Color();
		style.Colors[ImGuiCol_ColumnHovered] = mainColorHovered;
		style.Colors[ImGuiCol_ColumnActive] = mainColorActive;
		style.Colors[ImGuiCol_ResizeGrip] = Settings::UI::mainColor.Color();
		style.Colors[ImGuiCol_ResizeGripHovered] = mainColorHovered;
		style.Colors[ImGuiCol_ResizeGripActive] = mainColorActive;
		style.Colors[ImGuiCol_CloseButton] = Settings::UI::mainColor.Color();
		style.Colors[ImGuiCol_CloseButtonHovered] = mainColorHovered;
		style.Colors[ImGuiCol_CloseButtonActive] = mainColorActive;
		style.Colors[ImGuiCol_PlotLines] = Settings::UI::mainColor.Color();
		style.Colors[ImGuiCol_PlotLinesHovered] = mainColorHovered;
		style.Colors[ImGuiCol_PlotHistogram] = Settings::UI::mainColor.Color();
		style.Colors[ImGuiCol_PlotHistogramHovered] = mainColorHovered;
		style.Colors[ImGuiCol_TextSelectedBg] = mainColorHovered;
		style.Colors[ImGuiCol_ModalWindowDarkening] = ImVec4(0.0f, 0.0f, 0.0f, 0.75f);


	}

	 // TEST
	if (Settings::HWID::LoggedIn) {
		Radar::RenderWindow();
		ShowSpectators::RenderWindow();
		ResolverInfo::RenderWindow();
		EventLogger::PaintImGui();
		FPSWindow::Info();
	//	SpeedIndicator::Paint();
	}


	if (G::is_renderer_active)
	{

		static int page = 0;
		ImGui::SetNextWindowSize(ImVec2(1080, 720), ImGuiSetCond_FirstUseEver);
		static bool MenuBool;


	/*	if (!Settings::HWID::LoggedIn) {
			Authentication();
		}*/

		if (Settings::HWID::LoggedIn)
		{
			//IRC::RenderWindow();
			Configs::RenderWindow();
			
			DrawUserPanel(); 

			ImGui::Begin("XHOOK V1.33", &MenuBool, ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_ShowBorders);
			{
				Pos = ImGui::GetWindowPos();
				for (int i = 0; i < IM_ARRAYSIZE(tabs); i++)
				{
					int distance = i == page ? 0 : i > page ? i - page : page - i;

					if (ImGui::Button(tabs[i], ImVec2(ImGui::GetWindowSize().x / IM_ARRAYSIZE(tabs) - 10, 0)))
						page = i;

					if (i < IM_ARRAYSIZE(tabs) - 1)
						ImGui::SameLine();
				}

				ImGui::Separator();


				if (!Settings::Background::enable)
				{
					//Background Image
					ImVec2 pos = ImGui::GetCursorScreenPos();
					ImVec2 maxPos = ImVec2(pos.x + ImGui::GetWindowSize().x, pos.y + ImGui::GetWindowSize().y);

					ImGui::GetWindowDrawList()->AddImage(
						(void *)BackgroundTexture,
						ImVec2(pos.x, pos.y),
						ImVec2(maxPos),
						ImVec2(1, 0), ImVec2(0, 1)
					);

				}

				int currentwidth = ImGui::GetWindowSize().x;
				int nextwidth = (currentwidth - (currentwidth * 2));

				switch (page)
				{
				case 0:
					AimTab::RenderTab();
					break;
				case 1:


					ImGui::Checkbox("Enabled", &Settings::ESP::enabled);
					ImGui::NextColumn();
					{
						UI::KeyBindButton(&Settings::ESP::key);
					}
					ImGui::Separator();

					ImGui::Columns(2, NULL, true);
					{
						ImGui::BeginChild("COL1", ImVec2(0, 0), true);
						{
							ImGui::Text("ESP");
							ImGui::BeginChild("ESP", ImVec2(0, 0), true);
							ImGui::Text("Type");
							ImGui::Separator();
							ImGui::Columns(2, NULL, true);
							{
								ImGui::Checkbox("Outline Box", &Settings::ESP::Boxes::enabled);
								SetTooltip("Outline players with a box");
								ImGui::Checkbox("Chams", &Settings::ESP::Chams::enabled);
								SetTooltip("Change players' model texture");
								ImGui::Checkbox("Health", &Settings::ESP::Bars::enabled);
								SetTooltip("Show a players health with a bar");
								ImGui::Checkbox(XorStr("Armor"), &Settings::ESP::ArmorBars::enabled);
								ImGui::Checkbox("Tracers", &Settings::ESP::Tracers::enabled);
								SetTooltip("Draws a line to each player");
								ImGui::ItemSize(ImVec2(0.0f, 0.0f), 0.0f);
								ImGui::Text("Bar Color");
								ImGui::ItemSize(ImVec2(0.0f, 0.0f), 0.0f);
								ImGui::Text("Team Color");
								ImGui::ItemSize(ImVec2(0.0f, 0.0f), 0.0f);
								ImGui::Checkbox("Bullet Tracers", &Settings::ESP::BulletTracers::enabled);
								SetTooltip("Adds a line showing where a player is aiming");
								ImGui::Checkbox("Head Dot", &Settings::ESP::HeadDot::enabled);
								SetTooltip("Adds a Dot on the Head of a player");
								ImGui::Checkbox("Skeleton", &Settings::ESP::Skeleton::enabled);
								SetTooltip("Will draw skeleton.");
							}
							ImGui::NextColumn();
							{
								ImGui::PushItemWidth(-1);
								ImGui::Combo("##BOXTYPE", (int*)& Settings::ESP::Boxes::type, BoxTypes, IM_ARRAYSIZE(BoxTypes));
								ImGui::Combo("##CHAMSTYPE", (int*)& Settings::ESP::Chams::type, ChamsTypes, IM_ARRAYSIZE(ChamsTypes));
								ImGui::Combo("##BARTYPE", (int*)& Settings::ESP::Bars::type, BarTypes, IM_ARRAYSIZE(BarTypes));
								ImGui::Combo(XorStr("##ARMORBARTYPE"), (int*)&Settings::ESP::ArmorBars::type, ArmorBarTypes, IM_ARRAYSIZE(ArmorBarTypes));
								ImGui::Combo("##TRACERTYPE", (int*)& Settings::ESP::Tracers::type, TracerTypes, IM_ARRAYSIZE(TracerTypes));
								ImGui::Combo("##BARCOLTYPE", (int*)& Settings::ESP::Bars::colorType, BarColorTypes, IM_ARRAYSIZE(BarColorTypes));
								ImGui::Combo("##TEAMCOLTYPE", (int*)& Settings::ESP::teamColorType, TeamColorTypes, IM_ARRAYSIZE(TeamColorTypes));
								ImGui::PopItemWidth();
								ImGui::Checkbox("Draw Ticks", &Settings::ESP::SkeletonBacktrack::enabled);
								SetTooltip("Show BT ticks");
								ImGui::PushItemWidth(-1);
								ImGui::SliderFloat("##HDOTSIZE", &Settings::ESP::HeadDot::size, 1.f, 10.f, "Size: %0.3f");
								ImGui::PopItemWidth();
							}
							ImGui::Columns(1);
							ImGui::Separator();
							ImGui::Text("Filter");
							ImGui::Separator();
							ImGui::Columns(2, NULL, true);
							{
								ImGui::Checkbox("Enemies", &Settings::ESP::Filters::enemies);
								SetTooltip("Show enemies");
								ImGui::Checkbox("Chickens", &Settings::ESP::Filters::chickens);
								SetTooltip("Show chickens");
								ImGui::Checkbox("LocalPlayer", &Settings::ESP::Filters::localplayer);
								SetTooltip("Show localplayer");
								ImGui::Checkbox("Legit Mode", &Settings::ESP::Filters::legit);
								SetTooltip("Hide enemies behind walls");
							}
							ImGui::NextColumn();
							{
								ImGui::Checkbox("Allies", &Settings::ESP::Filters::allies);
								SetTooltip("Show team mates");
								ImGui::Checkbox("Fish", &Settings::ESP::Filters::fishes);
								SetTooltip("Show fish");
								ImGui::Checkbox("Smoke Check", &Settings::ESP::Filters::smokeCheck);
								SetTooltip("Mark players behind smokes as invisible");
								ImGui::Checkbox("Visiblity Check", &Settings::ESP::Filters::visibilityCheck);
								SetTooltip("Change color of outlined box based on whether you see them");
							}
							ImGui::Columns(1);
							ImGui::Separator();
							ImGui::Text("Player Information");
							ImGui::Separator();
							ImGui::Columns(2, NULL, true);
							{
								ImGui::Checkbox("Clan", &Settings::ESP::Info::clan);
								SetTooltip("Show clan");
								ImGui::Checkbox("Rank", &Settings::ESP::Info::rank);
								SetTooltip("Show rank");
								ImGui::PushID(1);
								ImGui::Checkbox("Health", &Settings::ESP::Info::health);
								SetTooltip("Show health");
								ImGui::Checkbox("Distance", &Settings::ESP::Info::distance);
								SetTooltip("Show the distance of a player");
								ImGui::PopID();
								ImGui::Checkbox("Scoped", &Settings::ESP::Info::scoped);
								SetTooltip("Show whether they are scoped");
								ImGui::Checkbox("Flashed", &Settings::ESP::Info::flashed);
								SetTooltip("Show whether they are flashed");
								ImGui::Checkbox("Defuse Kit", &Settings::ESP::Info::hasDefuser);
								SetTooltip("Show whether they have a defuse kit");
								ImGui::Checkbox("Grabbing Hostage", &Settings::ESP::Info::grabbingHostage);
								SetTooltip("Show whether they are grabbing a hostage");
								ImGui::Checkbox("Location", &Settings::ESP::Info::location);
								SetTooltip("Show location");
							}
							ImGui::NextColumn();
							{
								ImGui::Checkbox("Name", &Settings::ESP::Info::name);
								SetTooltip("Show name");
								ImGui::Checkbox("Steam ID", &Settings::ESP::Info::steamId);
								SetTooltip("Show Steam ID");
								ImGui::Checkbox("Weapon", &Settings::ESP::Info::weapon);
								SetTooltip("Show held weapon");

								SetTooltip("Show held weapon");
								ImGui::Checkbox("Reloading", &Settings::ESP::Info::reloading);
								SetTooltip("Show whether they are reloading");
								ImGui::Checkbox("Planting", &Settings::ESP::Info::planting);
								SetTooltip("Show whether they are planting");
								ImGui::Checkbox("Defusing", &Settings::ESP::Info::defusing);
								SetTooltip("Show whether they are defusing");
								ImGui::Checkbox("Rescuing Hostage", &Settings::ESP::Info::rescuing);
								SetTooltip("Show whether they are rescuing a hostage");
								ImGui::Checkbox(XorStr("LBY Updated / Backtracked"), &Settings::ESP::Info::lby);
								ImGui::Checkbox(XorStr("Resolver Info"), &Settings::ESP::Info::resolveInfo);
							}

							ImGui::Columns(1);
							ImGui::Separator();
							ImGui::Text("World");
							ImGui::Separator();
							ImGui::Columns(2, NULL, true);
							{
								ImGui::Checkbox("Weapons", &Settings::ESP::Filters::weapons);
								SetTooltip("Show weapons on floor");
								ImGui::Checkbox("Throwables", &Settings::ESP::Filters::throwables);
								SetTooltip("Show throwables");
								ImGui::Checkbox("Entity Glow", &Settings::ESP::Glow::enabled);
								SetTooltip("Show a glow around entities");
							}
							ImGui::NextColumn();
							{
								ImGui::Checkbox("Bomb", &Settings::ESP::Filters::bomb);
								SetTooltip("Show bomb when planted");
								ImGui::Checkbox("Defuse Kits", &Settings::ESP::Filters::defusers);
								SetTooltip("Show defuse kits on floor");
								ImGui::Checkbox("Hostages", &Settings::ESP::Filters::hostages);
								SetTooltip("Show hostages");
							}
							ImGui::Columns(1);
							ImGui::EndChild();
							ImGui::EndChild();
						}
					}

					ImGui::NextColumn();
					{
						ImGui::BeginChild("Chams", ImVec2(0, 0), true);
						{
							ImGui::Text("Crosshair");
							ImGui::Separator();
							ImGui::Columns(2, NULL, true);
							{
								ImGui::Checkbox("Recoil Crosshair", &Settings::Recoilcrosshair::enabled);
								SetTooltip("Show a crosshair based on recoil");
								ImGui::Checkbox("FOV Circle", &Settings::ESP::FOVCrosshair::enabled);
								ImGui::Checkbox("Show Spread", &Settings::ESP::Spread::enabled);
								SetTooltip("Show where your bullets can possibly Land");
							}
							ImGui::NextColumn();
							{
								ImGui::Checkbox("Only When Shooting", &Settings::Recoilcrosshair::showOnlyWhenShooting);
								SetTooltip("Only show recoil crosshair when shooting");
								ImGui::Checkbox("Filled", &Settings::ESP::FOVCrosshair::filled);
								SetTooltip("Fill the fov circle");
								ImGui::Checkbox("Show SpreadLimit", &Settings::ESP::Spread::spreadLimit);
								SetTooltip("Show Spread Limit threshold");
							}

							ImGui::Columns(1);
							ImGui::Separator();
							ImGui::Text(XorStr("Skybox Changer"));
							ImGui::Separator();
							ImGui::Checkbox(XorStr("##SKYBOXENABLE"), &Settings::SkyBox::enabled);
							ImGui::SameLine();
							ImGui::Combo(XorStr("##SKYBOX"), &Settings::SkyBox::skyBoxNumber, SkyBoxes, IM_ARRAYSIZE(SkyBoxes));
							ImGui::Separator();
							ImGui::Text("Other Visual Settings");
							ImGui::Separator();
							ImGui::Columns(2, NULL, true);
							{
								ImGui::Checkbox("Arms", &Settings::ESP::Chams::Arms::enabled);
								SetTooltip("Apply chams to arms");
								ImGui::SameLine();
								ImGui::Checkbox("Weapons", &Settings::ESP::Chams::Weapon::enabled);
								SetTooltip("Apply chams to weapons");
								ImGui::SameLine();
								ImGui::Checkbox("Dropped Weapons", &Settings::ESP::Chams::Weapon::DroppedWeapons);
								SetTooltip("Apply chams to Dropped weapons");
								ImGui::Checkbox("Dlights", &Settings::Dlights::enabled);
								SetTooltip("Adds a light source to players");
								ImGui::Checkbox("No Flash", &Settings::Noflash::enabled);
								SetTooltip("Hide flashbang effect");
								ImGui::Checkbox("Show Footsteps", &Settings::ESP::Sounds::enabled);
							SetTooltip("Shows you footsteps in 3D space");
								ImGui::Checkbox("No View Punch", &Settings::View::NoViewPunch::enabled);
								SetTooltip("Disables view punch when shooting");
								ImGui::Checkbox("No Aim Punch", &Settings::View::NoAimPunch::enabled);
								SetTooltip("Disables aim punch when shooting");
								ImGui::Checkbox("No Sky", &Settings::NoSky::enabled);
								SetTooltip("Allows for the skybox to be colored or disabled");
								ImGui::Checkbox("No Smoke", &Settings::NoSmoke::enabled);
								SetTooltip("Disables smoke rendering");
								ImGui::Checkbox("No Fog", &Settings::NoFog::enabled);
								ImGui::Checkbox("ASUS Walls", &Settings::ASUSWalls::enabled);
								SetTooltip("Makes wall textures transparent");
							
								ImGui::Checkbox("Show Watermark", &Settings::UI::Watermark::showwatermark); 
								ImGui::Checkbox("Player indicator", &Settings::ESP::PlayerIndicator::enabled);
								SetTooltip("A test indicator...");
							}
							
							if (ImGui::Button(XorStr("Material Config"), ImVec2(-1, 0)))
								ImGui::OpenPopup(XorStr("##MaterialConfigWindow"));
							SetTooltip(XorStr("Advanced CSGO Gfx Settings\nExpect a short delay when changed"));
							ImGui::SetNextWindowSize(ImVec2(320, 640), ImGuiSetCond_Always);
							if (ImGui::BeginPopup(XorStr("##MaterialConfigWindow"))) {
								ImGui::PushItemWidth(-1);
								if (ImGui::Button(XorStr("Reset Changes"))) {
									Settings::MaterialConfig::config = MaterialConfig::backupConfig;
								}
								ImGui::Checkbox(XorStr("Changes Enabled?"), &Settings::MaterialConfig::enabled);
								SetTooltip(XorStr("Expect a Short delay when changing these settings."));
								ImGui::SliderFloat(XorStr("##MONITORGAMMA"), &Settings::MaterialConfig::config.m_fMonitorGamma, 0.1f, 12.0f, XorStr("Gamma: %.3f"));
								ImGui::SliderFloat(XorStr("##GAMMATVRANGEMIN"), &Settings::MaterialConfig::config.m_fGammaTVRangeMin, 0.1f, min(300.0f, Settings::MaterialConfig::config.m_fGammaTVRangeMax), XorStr("TVRangeMin: %.3f"));
								ImGui::SliderFloat(XorStr("##GAMMATVRANGEMAX"), &Settings::MaterialConfig::config.m_fGammaTVRangeMax, Settings::MaterialConfig::config.m_fGammaTVRangeMin, 300.0f, XorStr("TVRangeMax: %.3f"));
								ImGui::SliderFloat(XorStr("##GAMMATVEXPONENT"), &Settings::MaterialConfig::config.m_fGammaTVExponent, 0.1f, 3.0f, XorStr("TV Exponent: %.3f"));
								ImGui::Checkbox(XorStr("GammaTVEnabled"), &Settings::MaterialConfig::config.m_bGammaTVEnabled);
								ImGui::Text(XorStr("Width:"));
								ImGui::SameLine();
								ImGui::InputInt(XorStr("##GAMEWIDTH"), &Settings::MaterialConfig::config.m_VideoMode.m_Width);

								ImGui::Text(XorStr("Height:"));
								ImGui::SameLine();
								ImGui::InputInt(XorStr("##GAMEHEIGHT"), &Settings::MaterialConfig::config.m_VideoMode.m_Height);

								ImGui::Text(XorStr("Refresh Rate:"));
								ImGui::SameLine();
								ImGui::InputInt(XorStr("##GAMEREFRESHRATE"), &Settings::MaterialConfig::config.m_VideoMode.m_RefreshRate);

								ImGui::Checkbox(XorStr("TripleBuffered"), &Settings::MaterialConfig::config.m_bTripleBuffered);
								ImGui::SliderInt(XorStr("##AASAMPLES"), &Settings::MaterialConfig::config.m_nAASamples, 0, 16, XorStr("AA Samples: %1.f"));
								ImGui::SliderInt(XorStr("##FORCEANISOTROPICLEVEL"), &Settings::MaterialConfig::config.m_nForceAnisotropicLevel, 0, 8, XorStr("Anisotropic Level: %1.f"));
								ImGui::SliderInt(XorStr("##SKIPMIPLEVELS"), &Settings::MaterialConfig::config.skipMipLevels, 1, 16, XorStr("SkipMipLevels: %1.f"));
								if (ImGui::Button(XorStr("Flags"), ImVec2(-1, 0)))
									ImGui::OpenPopup(XorStr("##MaterialConfigFlags"));
								ImGui::SetNextWindowSize(ImVec2(320, 240), ImGuiSetCond_Always);
								if (ImGui::BeginPopup(XorStr("##MaterialConfigFlags"))) {
									ImGui::PushItemWidth(-1);

									static bool localFlags[] = {
										(Settings::MaterialConfig::config.m_Flags & MaterialSystem_Config_Flags_t::MATSYS_VIDCFG_FLAGS_WINDOWED) != 0, // ( 1 << 0 )
										(Settings::MaterialConfig::config.m_Flags & MaterialSystem_Config_Flags_t::MATSYS_VIDCFG_FLAGS_RESIZING) != 0, // ( 1 << 1 )
										false, // ( 1 << 2 ) is not used.
										(Settings::MaterialConfig::config.m_Flags & MaterialSystem_Config_Flags_t::MATSYS_VIDCFG_FLAGS_NO_WAIT_FOR_VSYNC) != 0, // ( 1 << 3 )
										(Settings::MaterialConfig::config.m_Flags & MaterialSystem_Config_Flags_t::MATSYS_VIDCFG_FLAGS_STENCIL) != 0, // ...
										(Settings::MaterialConfig::config.m_Flags & MaterialSystem_Config_Flags_t::MATSYS_VIDCFG_FLAGS_FORCE_TRILINEAR) != 0,
										(Settings::MaterialConfig::config.m_Flags & MaterialSystem_Config_Flags_t::MATSYS_VIDCFG_FLAGS_FORCE_HWSYNC) != 0,
										(Settings::MaterialConfig::config.m_Flags & MaterialSystem_Config_Flags_t::MATSYS_VIDCFG_FLAGS_DISABLE_SPECULAR) != 0,
										(Settings::MaterialConfig::config.m_Flags & MaterialSystem_Config_Flags_t::MATSYS_VIDCFG_FLAGS_DISABLE_BUMPMAP) != 0,
										(Settings::MaterialConfig::config.m_Flags & MaterialSystem_Config_Flags_t::MATSYS_VIDCFG_FLAGS_ENABLE_PARALLAX_MAPPING) != 0,
										(Settings::MaterialConfig::config.m_Flags & MaterialSystem_Config_Flags_t::MATSYS_VIDCFG_FLAGS_USE_Z_PREFILL) != 0,
										(Settings::MaterialConfig::config.m_Flags & MaterialSystem_Config_Flags_t::MATSYS_VIDCFG_FLAGS_REDUCE_FILLRATE) != 0,
										(Settings::MaterialConfig::config.m_Flags & MaterialSystem_Config_Flags_t::MATSYS_VIDCFG_FLAGS_ENABLE_HDR) != 0,
										(Settings::MaterialConfig::config.m_Flags & MaterialSystem_Config_Flags_t::MATSYS_VIDCFG_FLAGS_LIMIT_WINDOWED_SIZE) != 0,
										(Settings::MaterialConfig::config.m_Flags & MaterialSystem_Config_Flags_t::MATSYS_VIDCFG_FLAGS_SCALE_TO_OUTPUT_RESOLUTION) != 0,
										(Settings::MaterialConfig::config.m_Flags & MaterialSystem_Config_Flags_t::MATSYS_VIDCFG_FLAGS_USING_MULTIPLE_WINDOWS) != 0,
										(Settings::MaterialConfig::config.m_Flags & MaterialSystem_Config_Flags_t::MATSYS_VIDCFG_FLAGS_DISABLE_PHONG) != 0,
										(Settings::MaterialConfig::config.m_Flags & MaterialSystem_Config_Flags_t::MATSYS_VIDCFG_FLAGS_VR_MODE) != 0
									};

									ImGui::Checkbox(XorStr("Windowed"), &localFlags[0]);
									ImGui::Checkbox(XorStr("Resizing"), &localFlags[1]);
									ImGui::Checkbox(XorStr("No VSYNC Wait"), &localFlags[3]);
									ImGui::Checkbox(XorStr("Stencil"), &localFlags[4]);
									ImGui::Checkbox(XorStr("Force Tri-Linear"), &localFlags[5]);
									ImGui::Checkbox(XorStr("Force HW Sync"), &localFlags[6]);
									ImGui::Checkbox(XorStr("Disable Specular"), &localFlags[7]);
									ImGui::Checkbox(XorStr("Disable Bumpmap"), &localFlags[8]);
									ImGui::Checkbox(XorStr("Disable Phong"), &localFlags[16]);
									ImGui::Checkbox(XorStr("Parallax Mapping"), &localFlags[9]);
									ImGui::Checkbox(XorStr("Use Z-Prefill"), &localFlags[10]);
									ImGui::Checkbox(XorStr("Reduce FillRate"), &localFlags[11]);
									ImGui::Checkbox(XorStr("HDR"), &localFlags[12]);
									ImGui::Checkbox(XorStr("Limit Windowed Size"), &localFlags[13]);
									ImGui::Checkbox(XorStr("Scale to Output Resolution"), &localFlags[14]);
									ImGui::Checkbox(XorStr("Using Multiple Windows"), &localFlags[15]);
									ImGui::Checkbox(XorStr("VR-Mode"), &localFlags[17]);

									if (ImGui::Button(XorStr("Apply "))) {
										for (unsigned short i = 0; i < 18; i++) {
											if (i == 2) // ( 1 << 2 ) not used.
												continue;
											Settings::MaterialConfig::config.SetFlag((unsigned int)(1 << i), localFlags[i]);
										}
									}

									ImGui::PopItemWidth();
									ImGui::EndPopup();
								}
								//m_flags
								ImGui::Checkbox(XorStr("EditMode"), &Settings::MaterialConfig::config.bEditMode);
								//proxiesTestMode
								ImGui::Checkbox(XorStr("Compressed Textures"), &Settings::MaterialConfig::config.bCompressedTextures);
								ImGui::Checkbox(XorStr("Filter Lightmaps"), &Settings::MaterialConfig::config.bFilterLightmaps);
								ImGui::Checkbox(XorStr("Filter Textures"), &Settings::MaterialConfig::config.bFilterTextures);
								ImGui::Checkbox(XorStr("Reverse Depth"), &Settings::MaterialConfig::config.bReverseDepth);
								ImGui::Checkbox(XorStr("Buffer Primitives"), &Settings::MaterialConfig::config.bBufferPrimitives);
								ImGui::Checkbox(XorStr("Draw Flat"), &Settings::MaterialConfig::config.bDrawFlat);
								ImGui::Checkbox(XorStr("Measure Fill-Rate"), &Settings::MaterialConfig::config.bMeasureFillRate);
								ImGui::Checkbox(XorStr("Visualize Fill-Rate"), &Settings::MaterialConfig::config.bVisualizeFillRate);
								ImGui::Checkbox(XorStr("No Transparency"), &Settings::MaterialConfig::config.bNoTransparency);
								ImGui::Checkbox(XorStr("Software Lighting"), &Settings::MaterialConfig::config.bSoftwareLighting); // Crashes game
																																   //AllowCheats ?
								ImGui::SliderInt(XorStr("##MIPLEVELS"), (int*)&Settings::MaterialConfig::config.nShowMipLevels, 0, 127, XorStr("ShowMipLevels: %1.f"));
								ImGui::Checkbox(XorStr("Show Low-Res Image"), &Settings::MaterialConfig::config.bShowLowResImage);
								ImGui::Checkbox(XorStr("Show Normal Map"), &Settings::MaterialConfig::config.bShowNormalMap);
								ImGui::Checkbox(XorStr("MipMap Textures"), &Settings::MaterialConfig::config.bMipMapTextures);
								ImGui::SliderInt(XorStr("##NFULLBRIGHT"), (int*)&Settings::MaterialConfig::config.nFullbright, 0, 127, XorStr("nFullBright: %1.f"));
								ImGui::Checkbox(XorStr("Fast NoBump"), &Settings::MaterialConfig::config.m_bFastNoBump);
								ImGui::Checkbox(XorStr("Suppress Rendering"), &Settings::MaterialConfig::config.m_bSuppressRendering);
								ImGui::Checkbox(XorStr("Draw Gray"), &Settings::MaterialConfig::config.m_bDrawGray);
								ImGui::Checkbox(XorStr("Show Specular"), &Settings::MaterialConfig::config.bShowSpecular);
								ImGui::Checkbox(XorStr("Show Defuse"), &Settings::MaterialConfig::config.bShowDiffuse);
								ImGui::SliderInt(XorStr("##AAQUALITY"), &Settings::MaterialConfig::config.m_nAAQuality, 0, 16, XorStr("AAQuality: %1.f"));
								ImGui::Checkbox(XorStr("Shadow Depth Texture"), &Settings::MaterialConfig::config.m_bShadowDepthTexture);
								ImGui::Checkbox(XorStr("Motion Blur"), &Settings::MaterialConfig::config.m_bMotionBlur);
								ImGui::Checkbox(XorStr("Support Flashlight"), &Settings::MaterialConfig::config.m_bSupportFlashlight);
								ImGui::Checkbox(XorStr("Paint Enabled"), &Settings::MaterialConfig::config.m_bPaintEnabled);
								// VRMode Adapter?
								ImGui::PopItemWidth();
								ImGui::EndPopup();
							}

						

							ImGui::NextColumn();
							{
								ImGui::PushItemWidth(-1);
								ImGui::Combo("##ARMSTYPE", (int*)& Settings::ESP::Chams::Arms::type, ArmsTypes, IM_ARRAYSIZE(ArmsTypes));
								ImGui::SliderFloat("##DLIGHTRADIUS", &Settings::Dlights::radius, 0, 1000, "Radius: %0.f");
								ImGui::SliderFloat("##NOFLASHAMOUNT", &Settings::Noflash::value, 0, 255, "Amount: %0.f");
								ImGui::SliderInt("##SOUNDSTIME", &Settings::ESP::Sounds::time, 250, 5000, "Timeout: %0.f");
								ImGui::SliderFloat("##MINIMALVOLUME", &Settings::ESP::Sounds::MinimumVolume, 0.f, 1.f, "Size: %0.f");
								ImGui::PopItemWidth();								
								ImGui::Checkbox("No Scope Border", &Settings::NoScopeBorder::enabled);
								SetTooltip("Disables black scope silhouette");
								//ImGui::Checkbox(XorStr("AntiAim Infos"), &Settings::AntiAimInfos::enabled);
								ImGui::Checkbox(XorStr("Grenade Prediction"), &Settings::GrenadePrediction::enabled);
								ImGui::Checkbox("Autowall Debug", &Settings::ESP::AutoWall::debugView);
								ImGui::Checkbox(XorStr("AimSpot Debug"), &Settings::ESP::AutoAim::drawTarget);
								ImGui::Checkbox(XorStr("BoneMap Debug"), &Settings::ESP::BoneMap::draw);
								if (Settings::ESP::BoneMap::draw)
									ImGui::Checkbox(XorStr("Just Dots"), &Settings::ESP::BoneMap::justDrawDots);
							}



							ImGui::Columns(1);
							ImGui::Separator();
							ImGui::Text("Other Windows");
							ImGui::Separator();
							ImGui::Columns(2, NULL, true);
							{
								ImGui::Checkbox("Spectator list", &Settings::ShowSpectators::enabled);
								SetTooltip("Shows a spectator GUI");
							}
							ImGui::NextColumn();
							{
								ImGui::Checkbox("Show resolver info", &Settings::Resolver::info);
								SetTooltip("Shows Resolver information");
								ImGui::SameLine();
/*								ImGui::Checkbox("IRC", &Settings::IRC::Enabled);
								SetTooltip("Lets you talk to other x-hook users who are playing");	*/			
							}
							ImGui::Columns(1);
							ImGui::Separator();
							ImGui::Text("Radar");
							ImGui::Separator();
							ImGui::Columns(2, NULL, true);
							{
								ImGui::Checkbox("Radar", &Settings::Radar::enabled);
								SetTooltip("Shows players on the custom radar");
								ImGui::PushItemWidth(-1);
								ImGui::Combo("##RADARTEAMCOLTYPE", (int*)& Settings::Radar::teamColorType, TeamColorTypes, IM_ARRAYSIZE(TeamColorTypes));
								ImGui::PopItemWidth();
								
								ImGui::Checkbox("Allies", &Settings::Radar::allies);
								ImGui::SameLine();
								ImGui::Checkbox("Enemies", &Settings::Radar::enemies);
								ImGui::Checkbox("Bomb", &Settings::Radar::bomb);
								ImGui::Checkbox("Legit", &Settings::Radar::legit);
								ImGui::Checkbox("In-game Radar", &Settings::Radar::InGame::enabled);
								SetTooltip("Shows players on the in-game radar");
							}
							ImGui::NextColumn();
							{
								ImGui::PushItemWidth(-1);
								ImGui::SliderFloat("##RADARZOOM", &Settings::Radar::zoom, 0.f, 100.f, "Zoom: %0.f");
								ImGui::SliderFloat("##RADARICONSSCALE", &Settings::Radar::iconsScale, 2, 16, "Icons Scale: %0.1f");
								ImGui::PopItemWidth();
								ImGui::Checkbox("Names", &Settings::Radar::name);
								ImGui::Checkbox("Defuser", &Settings::Radar::defuser);
								ImGui::Checkbox("Visibility Check", &Settings::Radar::visibilityCheck);
								ImGui::Checkbox("Smoke Check", &Settings::Radar::smokeCheck);
							}
							ImGui::Columns(1);
							ImGui::Separator();

							ImGui::Text("Hitmarkers");
							ImGui::Separator();
							ImGui::Columns(2, NULL, true);
							{
								ImGui::Checkbox("Hitmarkers", &Settings::ESP::Hitmarker::enabled);
								SetTooltip("Notify when you hit another player");

								ImGui::Checkbox("Enemies##HITMARKERS", &Settings::ESP::Hitmarker::enemies);
								ImGui::Checkbox("Allies##HITMARKERS", &Settings::ESP::Hitmarker::allies);
								ImGui::Checkbox(XorStr("Sounds##HITMARKERS"), &Settings::ESP::Hitmarker::Sounds::enabled);
								ImGui::Checkbox("Damage##HITMARKERS", &Settings::ESP::Hitmarker::Damage::enabled);
								SetTooltip("Show dealt damage next to the hitmarker");
							}
							ImGui::NextColumn();
							{
								ImGui::PushItemWidth(-1);
								ImGui::SliderInt("##HITMARKERDUR", &Settings::ESP::Hitmarker::duration, 250, 3000, "Timeout: %0.f");
								ImGui::SliderInt("##HITMARKERSIZE", &Settings::ESP::Hitmarker::size, 1, 32, "Size: %0.f");
								ImGui::SliderInt("##HITMARKERGAP", &Settings::ESP::Hitmarker::innerGap, 1, 16, "Gap: %0.f");
								ImGui::Combo(XorStr("Sounds"), (int*)&Settings::ESP::Hitmarker::Sounds::sound, Sounds, IM_ARRAYSIZE(Sounds));
								ImGui::PopItemWidth();
							}
							ImGui::Columns(1);
							ImGui::Separator();

							ImGui::EndChild();
						}
					}
					ImGui::Columns(1);
					break;
				case 2:
					HvH::RenderTab();
					break;
				case 3:

					if (!pEngine->IsInGame() || (*csPlayerResource && !(*csPlayerResource)->GetConnected(currentPlayer)))
						currentPlayer = -1;

					ImGui::ListBoxHeader("##PLAYERS", ImVec2(-1, (ImGui::GetWindowSize().y - 95)));
					if (pEngine->IsInGame() && *csPlayerResource)
					{
						ImGui::Columns(7);


						ImGui::Text("ID");
						ImGui::NextColumn();

						ImGui::Text("Nickname");
						ImGui::NextColumn();

						ImGui::Text("Team");
						ImGui::NextColumn();

						//		ImGui::Text("STEAMID");
						//			ImGui::NextColumn();

						ImGui::Text("Clan tag");
						ImGui::NextColumn();

						ImGui::Text("Stats");
						ImGui::NextColumn();

						ImGui::Text("Rank");
						ImGui::NextColumn();

						ImGui::Text("Wins");
						ImGui::NextColumn();

						std::unordered_map<TeamID, std::vector<int>, Util::IntHash<TeamID>> players = {
							{ TeamID::TEAM_UNASSIGNED,{} },
							{ TeamID::TEAM_SPECTATOR,{} },
							{ TeamID::TEAM_TERRORIST,{} },
							{ TeamID::TEAM_COUNTER_TERRORIST,{} },
						};

						for (int i = 1; i < pEngine->GetMaxClients(); i++)
						{
							if (i == pEngine->GetLocalPlayer())
								continue;

							if (!(*csPlayerResource)->GetConnected(i))
								continue;

							players[(*csPlayerResource)->GetTeam(i)].push_back(i);
						}

						for (int team = (int)TeamID::TEAM_UNASSIGNED; team <= (int)TeamID::TEAM_COUNTER_TERRORIST; team++)
						{
							char* teamName = strdup("");
							switch ((TeamID)team)
							{
							case TeamID::TEAM_UNASSIGNED:
								teamName = strdup("Unassigned");
								break;
							case TeamID::TEAM_SPECTATOR:
								teamName = strdup("Spectator");
								break;
							case TeamID::TEAM_TERRORIST:
								teamName = strdup("Terrorist");
								break;
							case TeamID::TEAM_COUNTER_TERRORIST:
								teamName = strdup("Counter Terrorist");
								break;
							}

							for (auto it : players[(TeamID)team])
							{
								std::string id = std::to_string(it);

								IEngineClient::player_info_t entityInformation;
								pEngine->GetPlayerInfo(it, &entityInformation);

								//if (entityInformation.ishltv)
								//	continue;

								ImGui::Separator();


								if (ImGui::Selectable(id.c_str(), it == currentPlayer, ImGuiSelectableFlags_SpanAllColumns))
									currentPlayer = it;
								ImGui::NextColumn();

								ImGui::Text("%s", entityInformation.name);
								ImGui::NextColumn();

								ImGui::Text("%s", teamName);
								ImGui::NextColumn();

								//			ImGui::Text("%s", entityInformation.guid);
								//			ImGui::NextColumn();					

								ImGui::Text("%s", (*csPlayerResource)->GetClan(it));
								ImGui::NextColumn();

								ImGui::Text("%d/%d/%d", (*csPlayerResource)->GetKills(it), (*csPlayerResource)->GetAssists(it), (*csPlayerResource)->GetDeaths(it));
								ImGui::NextColumn();

								ImGui::Text("%s", ESP::ranks[*(*csPlayerResource)->GetCompetitiveRanking(it)]);
								ImGui::NextColumn();

								ImGui::Text("%d", *(*csPlayerResource)->GetCompetitiveWins(it));
								ImGui::NextColumn();

							}
						}
					}
					ImGui::ListBoxFooter();

					if (currentPlayer != -1)
					{
						IEngineClient::player_info_t entityInformation;
						pEngine->GetPlayerInfo(currentPlayer, &entityInformation);

						
						ImGui::Columns(2);
						{
							bool isFriendly = std::find(Aimbot::friends.begin(), Aimbot::friends.end(), entityInformation.xuid) != Aimbot::friends.end();
							if (ImGui::Checkbox("Friend", &isFriendly))
							{
								if (isFriendly)
									Aimbot::friends.push_back(entityInformation.xuid);
								else
									Aimbot::friends.erase(std::find(Aimbot::friends.begin(), Aimbot::friends.end(), entityInformation.xuid));
							}

							bool shouldResolve = std::find(Resolver::playerAngleLogs.begin(), Resolver::playerAngleLogs.end(),
								entityInformation.xuid) != Resolver::playerAngleLogs.end();
							if (ImGui::Checkbox(XorStr("Resolver"), &shouldResolve)) {
								if (shouldResolve)
									Resolver::playerAngleLogs.push_back(entityInformation.xuid);
								else
									Resolver::playerAngleLogs.erase(
										std::find(Resolver::playerAngleLogs.begin(), Resolver::playerAngleLogs.end(),
											entityInformation.xuid));
							}
						}
						ImGui::NextColumn();
						{
							if (ImGui::Button("Steal name"))
							{
								std::string name;
								const char* ass = entityInformation.name;
								name = ass;

								strcpy(nickname, name.c_str());
								NameChanger::SetName(name.c_str());
							}

							const char* clanTag = (*csPlayerResource)->GetClan(currentPlayer);
							if (strlen(clanTag) > 0 && ImGui::Button("Steal clan tag"))
							{
								Settings::ClanTagChanger::enabled = true;
								strcpy(Settings::ClanTagChanger::value, clanTag);
								Settings::ClanTagChanger::type = ClanTagType::STATIC;

								ClanTagChanger::UpdateClanTagCallback();
							}

							if (ImGui::Button("Debug information*"))
							{
								XorCompileTime::w_printf(XorStr("\n=====\nPlayer informations:\n[%s] %s \nSteamID: %s\n=====\n"), (*csPlayerResource)->GetClan(currentPlayer), entityInformation.name, entityInformation.guid);
							}
						}
						ImGui::NextColumn();
						{
							bool isPreferBaim = std::find(Aimbot::baim.begin(), Aimbot::baim.end(), entityInformation.xuid) !=
								Aimbot::baim.end();
							if (ImGui::Checkbox(XorStr("Prefer Baim"), &isPreferBaim)) {
								if (isPreferBaim)
									Aimbot::baim.push_back(entityInformation.xuid);
								else
									Aimbot::baim.erase(
										std::find(Aimbot::baim.begin(), Aimbot::baim.end(), entityInformation.xuid));
							}

							if (ImGui::Button(XorStr("Print information"))) {
								pCvar->ConsoleColorPrintf(ColorRGBA(255, 255, 255),
									XorStr("\n=====\nPlayer informations:\n[%s] %s \nSteamID: %s\n=====\n"),
									(*csPlayerResource)->GetClan(currentPlayer), entityInformation.name,
									entityInformation.guid);
							}
						}
					}
					break;
				case 4:
					ImGui::Columns(2, NULL, true);
					{
						ImGui::BeginChild("Child1", ImVec2(0, 0), true);
						{
							ImGui::Text("Movement");
							ImGui::Separator();

							ImGui::Checkbox("Bunny Hop", &Settings::BHop::enabled);
							SetTooltip("Auto bunny hop");
							ImGui::Separator();

							if (Settings::BHop::enabled)
							{
								ImGui::Text("Humanize");
								ImGui::Separator();
								ImGui::Columns(2, NULL, true);
								{
									ImGui::Checkbox("Hop Chance", &Settings::BHop::Chance::enabled);
									ImGui::Checkbox("Min Hops", &Settings::BHop::Hops::enabledMin);
									ImGui::Checkbox("Max Hops", &Settings::BHop::Hops::enabledMax);
								}
								ImGui::NextColumn();
								{
									ImGui::PushItemWidth(-1);
									ImGui::SliderInt("##BHOPCHANCE", &Settings::BHop::Chance::value, 0, 100);
									ImGui::SliderInt("##BHOPMIN", &Settings::BHop::Hops::Min, 0, 20);
									ImGui::SliderInt("##BHOPMAX", &Settings::BHop::Hops::Max, 0, 20);
								}
								ImGui::Separator();
							}

							ImGui::Columns(2, NULL, true);
							{
								ImGui::Checkbox("Edge Jump", &Settings::EdgeJump::enabled);
								ImGui::Checkbox("CircleStrafe", &Settings::CircleStrafe::enabled);
								SetTooltip("Jumps off the edges");
								ImGui::Checkbox("NoDuck", &Settings::NoDuckCooldown::enabled);
								SetTooltip("Remove the duck cooldown.");
							}
							ImGui::NextColumn();
							{
								UI::KeyBindButton(&Settings::EdgeJump::key);
								UI::KeyBindButton(&Settings::CircleStrafe::key);
								//	ImGui::Combo("##edgejumpkey", (int*)&Settings::EdgeJump::key, ButtonNames, IM_ARRAYSIZE(ButtonNames));
								//	ImGui::Combo("##circlestrafekey", (int*)&Settings::CircleStrafe::key, ButtonNames, IM_ARRAYSIZE(ButtonNames));
							}
							ImGui::Columns(1);

							ImGui::Separator();
							ImGui::Columns(2, NULL, true);
							{
								ImGui::Checkbox("Auto Strafe", &Settings::AutoStrafe::enabled);
								SetTooltip("Auto strafe when bunny hopping");
							}
							ImGui::NextColumn();
							{
								ImGui::PushItemWidth(-1);
								ImGui::Combo("##STRAFETYPE", (int*)& Settings::AutoStrafe::type, strafeTypes, IM_ARRAYSIZE(strafeTypes));
								ImGui::PopItemWidth();
							}

							if (Settings::AutoStrafe::type == AutostrafeType::AS_RAGE)
							{
								ImGui::Checkbox("Silent", &Settings::AutoStrafe::silent);
								SetTooltip("Strafes won't be visible for spectators");
							}

							ImGui::Columns(1);
							ImGui::Separator();
							ImGui::Text("Spammer");
							ImGui::Separator();

							ImGui::Columns(1, NULL, true);
							{
								ImGui::Checkbox("Chat Kill", &Settings::Spammer::KillSpammer::enabled);
							}
							ImGui::NextColumn();
							{
								ImGui::Checkbox("Team Chat###SAY_TEAM1", &Settings::Spammer::KillSpammer::sayTeam);
								SetTooltip("Only show kill message in team chat");
							}
							ImGui::NextColumn();
							{
								if (ImGui::Button("Options###KILL"))
									ImGui::OpenPopup("options_kill");

								ImGui::SetNextWindowSize(ImVec2(565, 268), ImGuiSetCond_Always);
								if (ImGui::BeginPopup("options_kill"))
								{
									static int killSpammerMessageCurrent = -1;
									static char killSpammerMessageBuf[126];

									ImGui::PushItemWidth(445);
									ImGui::InputText("###SPAMMERMESSAGE", killSpammerMessageBuf, IM_ARRAYSIZE(killSpammerMessageBuf));
									ImGui::PopItemWidth();
									ImGui::SameLine();

									if (ImGui::Button("Add"))
									{
										if (strlen(killSpammerMessageBuf) > 0)
											Settings::Spammer::KillSpammer::messages.push_back(std::string(killSpammerMessageBuf));

										strcpy(killSpammerMessageBuf, "");
									}
									ImGui::SameLine();

									if (ImGui::Button("Remove"))
										if (killSpammerMessageCurrent > -1 && (int)Settings::Spammer::KillSpammer::messages.size() > killSpammerMessageCurrent)
											Settings::Spammer::KillSpammer::messages.erase(Settings::Spammer::KillSpammer::messages.begin() + killSpammerMessageCurrent);

									ImGui::PushItemWidth(550);
									ImGui::ListBox("", &killSpammerMessageCurrent, Settings::Spammer::KillSpammer::messages, 10);
									ImGui::PopItemWidth();

									ImGui::EndPopup();
								}
							}

							ImGui::Columns(3, NULL, true);
							{
								ImGui::Combo(XorStr("####RadioCommands"), (int*)&Settings::Spammer::RadioSpammer::radio, RadioSpammer, IM_ARRAYSIZE(RadioSpammer));
							}
							ImGui::NextColumn();
							{
								ImGui::Text("Radio Commands");
							}
							ImGui::Columns(3, NULL, true);
							{
								ImGui::Combo("###SPAMMERYPE", (int*)&Settings::Spammer::type, spammerTypes, IM_ARRAYSIZE(spammerTypes));
							}
							ImGui::NextColumn();
							{
								ImGui::Checkbox("Team Chat###SAY_TEAM2", &Settings::Spammer::say_team);
								SetTooltip("Only spam messages in team chat");
							}
							ImGui::NextColumn();
							{
								if (Settings::Spammer::type != SpammerType::SPAMMER_NONE && ImGui::Button("Options###SPAMMER"))
									ImGui::OpenPopup("options_spammer");

								if (Settings::Spammer::type == SpammerType::SPAMMER_NORMAL)
									ImGui::SetNextWindowSize(ImVec2(565, 268), ImGuiSetCond_Always);

								else if (Settings::Spammer::type == SpammerType::SPAMMER_POSITIONS)
									ImGui::SetNextWindowSize(ImVec2(200, 240), ImGuiSetCond_Always);

								if (Settings::Spammer::type != SpammerType::SPAMMER_NONE && ImGui::BeginPopup("options_spammer"))
								{
									if (Settings::Spammer::type == SpammerType::SPAMMER_NORMAL)
									{
										static int spammerMessageCurrent = -1;
										static char spammerMessageBuf[126];

										ImGui::PushItemWidth(445);
										ImGui::InputText("###SPAMMERMESSAGE", spammerMessageBuf, IM_ARRAYSIZE(spammerMessageBuf));
										ImGui::PopItemWidth();
										ImGui::SameLine();

										if (ImGui::Button("Add"))
										{
											if (strlen(spammerMessageBuf) > 0)
												Settings::Spammer::NormalSpammer::messages.push_back(std::string(spammerMessageBuf));

											strcpy(spammerMessageBuf, "");
										}
										ImGui::SameLine();

										if (ImGui::Button("Remove"))
											if (spammerMessageCurrent > -1 && (int)Settings::Spammer::NormalSpammer::messages.size() > spammerMessageCurrent)
												Settings::Spammer::NormalSpammer::messages.erase(Settings::Spammer::NormalSpammer::messages.begin() + spammerMessageCurrent);

										ImGui::PushItemWidth(550);
										ImGui::ListBox("", &spammerMessageCurrent, Settings::Spammer::NormalSpammer::messages, 10);
										ImGui::PopItemWidth();
									}
									else if (Settings::Spammer::type == SpammerType::SPAMMER_POSITIONS)
									{
										ImGui::PushItemWidth(185);
										ImGui::Combo("###POSITIONSTEAM", &Settings::Spammer::PositionSpammer::team, teams, IM_ARRAYSIZE(teams));
										ImGui::PopItemWidth();
										ImGui::Separator();
										ImGui::Checkbox("Show Name", &Settings::Spammer::PositionSpammer::showName);
										ImGui::Checkbox("Show Weapon", &Settings::Spammer::PositionSpammer::showWeapon);
										ImGui::Checkbox("Show Rank", &Settings::Spammer::PositionSpammer::showRank);
										ImGui::Checkbox("Show Wins", &Settings::Spammer::PositionSpammer::showWins);
										ImGui::Checkbox("Show Health", &Settings::Spammer::PositionSpammer::showHealth);
										ImGui::Checkbox("Show Money", &Settings::Spammer::PositionSpammer::showMoney);
										ImGui::Checkbox("Show Last Place", &Settings::Spammer::PositionSpammer::showLastplace);
									}

									ImGui::EndPopup();
								}
							}
							ImGui::Columns(1);
							ImGui::Separator();
							ImGui::Text("FOV");
							ImGui::Separator();
							ImGui::Columns(2, NULL, true);
							{
								ImGui::Checkbox("FOV", &Settings::FOVChanger::enabled);
								SetTooltip("Change camera FOV");
								ImGui::Checkbox("Viewmodel FOV", &Settings::FOVChanger::viewmodelEnabled);
								SetTooltip("Change viewmodel FOV");
								ImGui::Checkbox("Ignore Scope", &Settings::FOVChanger::ignoreScope);
								SetTooltip("Disable FOV Changer while scoping");
							}
							ImGui::NextColumn();
							{
								ImGui::PushItemWidth(-1);
								ImGui::SliderFloat("##FOVAMOUNT", &Settings::FOVChanger::value, 0, 180);
								ImGui::SliderFloat("##MODELFOVAMOUNT", &Settings::FOVChanger::viewmodelValue, 0, 360);
								ImGui::PopItemWidth();
							}
							ImGui::Columns(1);
							ImGui::Separator();

							ImGui::Checkbox(XorStr("Third Person"), &Settings::ThirdPerson::enabled);
							{
								ImGui::Separator();

								ImGui::Columns(2, NULL, true);
								{
									ImGui::ItemSize(ImVec2(0.0f, 0.0f), 0.0f);
									ImGui::Text(XorStr("Mode"));
									ImGui::ItemSize(ImVec2(0.0f, 0.0f), 0.0f);
									ImGui::Text(XorStr("Camera Offset"));
									ImGui::ItemSize(ImVec2(0.0f, 0.0f), 0.0f);
									ImGui::Text(XorStr("Key"));
									ImGui::ItemSize(ImVec2(0.0f, 0.0f), 0.0f);
									ImGui::Text(XorStr("Transparency (Scoped)"));
									ImGui::ItemSize(ImVec2(0.0f, 0.0f), 0.0f);
								}
								ImGui::NextColumn();
								{
									ImGui::PushItemWidth(-3);
									ImGui::Combo(XorStr("##TPMODE"), (int*)&Settings::ThirdPerson::mode,
										tpModes, (int)ThirdPersonMode::NUMBER_OF_TYPES);


									ImGui::PopItemWidth();
									ImGui::PushItemWidth(-1);
									ImGui::SliderFloat(XorStr("##TPCAMOFFSET"), &Settings::ThirdPerson::distance, 0.0f, 500.0f);
									ImGui::PopItemWidth();
									//ImGui::Combo("##Thirdpersonkey", (int*)&Settings::ThirdPerson::key, ButtonNames, IM_ARRAYSIZE(ButtonNames));
									UI::KeyBindButton(&Settings::ThirdPerson::key);
									ImGui::PushItemWidth(-1);
									ImGui::SliderFloat(XorStr("##TRANSPARENCY"), &Settings::ThirdPerson::transparency, 0.1f,
										0.6f, XorStr("%0.1f"));
									ImGui::PopItemWidth();
								}
								ImGui::Columns(1);
							}
							ImGui::Columns(1);
							ImGui::Separator();
							ImGui::Text("Grenade Helper");
							ImGui::Separator();
							ImGui::Columns(2, NULL, true);
							{
								ImGui::Checkbox("Enabled ###ghenabled", &Settings::GrenadeHelper::enabled);
								SetTooltip("Show Saved Grenade Throws, Visuals required.");
							}
							ImGui::NextColumn();
							{
								ImGui::Checkbox("Only matching ###match", &Settings::GrenadeHelper::onlyMatchingInfos);
								SetTooltip("Shows only the info's for your equipped Grenade.");
							}

							ImGui::Columns(2);
							{
								if (ImGui::Button("Aimassist", ImVec2(-1, 0)))
									ImGui::OpenPopup("optionAimAssist");
								ImGui::SetNextWindowSize(ImVec2(200, 120), ImGuiSetCond_Always);
								if (ImGui::BeginPopup("optionAimAssist"))
								{
									ImGui::PushItemWidth(-1);
									ImGui::Checkbox("Enabled", &Settings::GrenadeHelper::aimAssist);
									SetTooltip("Whether the Aimassist will help you throw Grenades.");
									ImGui::SliderFloat("###aimstep", &Settings::GrenadeHelper::aimStep, 0, 10, "Speed: %0.3f");
									SetTooltip("The speed at which the Aimassist helps.");
									ImGui::SliderFloat("###aimfov", &Settings::GrenadeHelper::aimFov, 0, 180, "Fov: %0.2f");
									SetTooltip("The Fov at which the Aimassist starts.");
									ImGui::SliderFloat("###aimdistance", &Settings::GrenadeHelper::aimDistance, 0, 100, "Distance: %0.2f");
									SetTooltip("The distance at which the Aimassist starts.");
									ImGui::PopItemWidth();

									ImGui::EndPopup();
								}
							}
							ImGui::NextColumn();
							{
								if (ImGui::Button("Add Info", ImVec2(-1, 0)))
									ImGui::OpenPopup("addinfo_throw");

								ImGui::SetNextWindowSize(ImVec2(565, 268), ImGuiSetCond_Always);
								if (ImGui::BeginPopup("addinfo_throw"))
								{
									static int throwMessageCurrent = -1;
									static char inputName[40];
									static int tType = (int)ThrowType::NORMAL;
									static int gType = (int)GrenadeType::SMOKE;

									if (pEngine->IsInGame())
									{
										C_BasePlayer* localPlayer = (C_BasePlayer*)pEntityList->GetClientEntity(pEngine->GetLocalPlayer());
										if (localPlayer)
										{
											C_BaseCombatWeapon *activeWeapon = (C_BaseCombatWeapon *)pEntityList->GetClientEntityFromHandle(
												localPlayer->GetActiveWeapon());
											if (activeWeapon &&
												activeWeapon->GetCSWpnData()->GetWeaponType() == CSWeaponType::WEAPONTYPE_GRENADE)
												gType = (int)getGrenadeType(activeWeapon);
										}
									}
									ImGui::Columns(1);
									ImGui::PushItemWidth(500);
									ImGui::InputText("", inputName, sizeof(inputName));
									ImGui::PopItemWidth();
									ImGui::SameLine();
									if (ImGui::Button("Add") && pEngine->IsInGame() && Settings::GrenadeHelper::actMapName.length() > 0)
									{
										C_BasePlayer* localPlayer = (C_BasePlayer*)pEntityList->GetClientEntity(pEngine->GetLocalPlayer());
										if (strlen(inputName) > 0)
										{
											GrenadeInfo n = GrenadeInfo((GrenadeType)gType, localPlayer->GetEyePosition(), *localPlayer->GetVAngles(), (ThrowType)tType, inputName);
											Settings::GrenadeHelper::grenadeInfos.push_back(n);

											//	std::string GetGhConfigDirectory(); // Get nade file
											const char *GetGhConfigDirectory = "C://xhook//configs//GrenadeHelper";

											std::ostringstream path;
											path << GetGhConfigDirectory << Settings::GrenadeHelper::actMapName;

											char NadePath[255];
											const char * GetPathInconst = path.str().c_str();
											if (stat(GetPathInconst, &GNadeInfo) != 0)
											{

												CreateDirectoryA(GetPathInconst, nullptr);

												path << XorStr("/config.json");
												Settings::SaveGrenadeInfo(path.str());

											}
											else if (GNadeInfo.st_mode & S_IFDIR)  // S_ISDIR() doesn't exist on my windows 
											{

											}
											else
											{
												CreateDirectoryA(GetPathInconst, nullptr);

												path << XorStr("/config.json");
												Settings::SaveGrenadeInfo(path.str());
											}
										}
										strcpy(inputName, "");
									}
									ImGui::Columns(2);
									ImGui::Combo("###Throwtype", &tType, throwTypes, IM_ARRAYSIZE(throwTypes));
									ImGui::NextColumn();
									ImGui::Combo("###Grenadetype", &gType, grenadeTypes, IM_ARRAYSIZE(grenadeTypes));
									ImGui::Columns(1);
									ImGui::Separator();
									ImGui::PushItemWidth(550);
									auto lambda = [](void* data, int idx, const char** out_text)
									{
										*out_text = Settings::GrenadeHelper::grenadeInfos.at(idx).name.c_str();
										return *out_text != NULL;
									};
									ImGui::ListBox("", &throwMessageCurrent, lambda, NULL, Settings::GrenadeHelper::grenadeInfos.size(), 7);
									ImGui::PopItemWidth();
									ImGui::Columns(1);
									if (ImGui::Button("Remove", ImVec2(ImGui::GetWindowWidth(), 30)))
										if (throwMessageCurrent > -1 && (int)Settings::GrenadeHelper::grenadeInfos.size() > throwMessageCurrent)
										{
											Settings::GrenadeHelper::grenadeInfos.erase(Settings::GrenadeHelper::grenadeInfos.begin() + throwMessageCurrent);

											const char *GetGhConfigDirectory = "C://xhook//configs//GrenadeHelper";
											std::ostringstream path;
											path << GetGhConfigDirectory << Settings::GrenadeHelper::actMapName;

											const char * GetPathInconst = path.str().c_str();
											if (stat(GetPathInconst, &GNadeInfo) != 0)
											{
												const char * GetPathInconst = path.str().c_str();
												CreateDirectoryA(GetPathInconst, nullptr);
												path << XorStr("/config.json");
												Settings::SaveGrenadeInfo(path.str());
											}
											else if (GNadeInfo.st_mode & S_IFDIR)  // S_ISDIR() doesn't exist on my windows 
											{

											}
											else
											{

											}
										}
									ImGui::EndPopup();
								}
							}
							
							ImGui::EndChild();
						}
					}
					ImGui::NextColumn();
					{
						ImGui::BeginChild(XorStr("Child2"), ImVec2(0, 0), true);
						{
							ImGui::Text(XorStr("Clantag"));
							ImGui::Separator();
							ImGui::Checkbox(XorStr("Enabled"), &Settings::ClanTagChanger::enabled);
							ImGui::Separator();
							ImGui::Columns(1);

							ImGui::Separator();
							ImGui::Columns(2, NULL, true);
							{
								ImGui::PushItemWidth(-1);
								if (ImGui::Combo(XorStr("##PRESETTYPE"), (int*)&Settings::ClanTagChanger::preset,
									presetTypes,
									IM_ARRAYSIZE(presetTypes)))
									ClanTagChanger::UpdateClanTagCallback();
								ImGui::PopItemWidth();

								ImGui::ItemSize(ImVec2(0.0f, 0.0f), 0.0f);
								ImGui::Text(XorStr("Animation Speed"));
								ImGui::ItemSize(ImVec2(0.0f, 0.0f), 0.0f);

								if (Settings::ClanTagChanger::preset == valueType::CUSTOM) {
									ImGui::Text(XorStr("Custom Clan Tag"));
									ImGui::ItemSize(ImVec2(0.0f, 0.0f), 0.0f);
								}
							}
							ImGui::NextColumn();
							{
								ImGui::PushItemWidth(-1);
								if (ImGui::Combo(XorStr("##ANIMATIONTYPE"), (int*)&Settings::ClanTagChanger::type,
									animationTypes, IM_ARRAYSIZE(animationTypes)))
									ClanTagChanger::UpdateClanTagCallback();
								if (ImGui::SliderInt(XorStr("##ANIMATIONSPEED"), &Settings::ClanTagChanger::animationSpeed, 0,
									2000))
									ClanTagChanger::UpdateClanTagCallback();

								if (Settings::ClanTagChanger::preset == valueType::CUSTOM) {
									if (ImGui::InputText(XorStr("##CLANTAG"), Settings::ClanTagChanger::value, 30)) {
										if (Settings::ClanTagChanger::value[1] != 0)
											ClanTagChanger::UpdateClanTagCallback();

									}
								}
								ImGui::PopItemWidth();
							}
							ImGui::Columns(1);
							ImGui::Separator();
							ImGui::Combo("", &Settings::Misc::banColor, "White\0Red\0Purple\0Green\0Light green\0Turquoise\0Light red\0Gray\0Yellow\0Gray 2\0Light blue\0Gray/Purple\0Blue\0Pink\0Dark orange\0Orange\0");
							ImGui::InputText(XorStr("##BANTEXT"), BanMsg, 127);
							ImGui::SameLine();
							if (ImGui::Button("Custom"))
							{
								Settings::Misc::banText = (std::string(BanMsg).c_str());
								NameChanger::fakeBan(true);
							}
							ImGui::SameLine();
							if (ImGui::Button("Setup fake ban"))
							{			
								Settings::Misc::banText = { "Cheater has been permanently banned from official CS:GO servers." };
								NameChanger::fakeBan(true);
							}



							ImGui::Text("Nickname");
							ImGui::Separator();
							ImGui::InputText("##NICKNAME", nickname, 127);
							ImGui::SameLine();
							if (ImGui::Button("Set Nickname", ImVec2(-1, 0)))
								NameChanger::SetName(std::string(nickname).c_str());
							if (ImGui::Button("Glitch Name"))
								NameChanger::SetName("\n\xAD\xAD\xAD");

							//	SetTooltip("Allows you to change your name an infinite amount of times and doesn't show in server message");
							ImGui::SameLine();
							if (ImGui::Button(XorStr("Infinite Name Spam")))
								NameChanger::InitColorChange(NameChanger::type = NC_Type::NC_RAINBOW);
							ImGui::SameLine();
							if (ImGui::Button("XHOOK"))
							{
								NameChanger::changes = 0;
								NameChanger::type = NC_Type::NC_TROLL;
							}
							ImGui::SameLine();
							if (ImGui::Button("XHOOK"))
								NameChanger::changes = 0;
							NameChanger::type = NC_Type::NC_NORMAL;
							ImGui::SameLine();
							if (ImGui::Button("Colorize Name", ImVec2(-1, 0)))
								ImGui::OpenPopup("optionColorizeName");
							ImGui::SetNextWindowSize(ImVec2(150, 300), ImGuiSetCond_Always);
							if (ImGui::BeginPopup("optionColorizeName"))
							{
								ImGui::PushItemWidth(-1);
								for (auto& it : NameChanger::colors)
								{
									if (ImGui::Button(it.second, ImVec2(-1, 0)))
										NameChanger::InitColorChange(NC_Type::NC_SOLID, it.first);
								}
								ImGui::PopItemWidth();

								ImGui::EndPopup();
							}
							ImGui::Columns(2, NULL, true);
							{					
								if (ImGui::Checkbox("Name Stealer", &Settings::NameStealer::enabled))
									NameStealer::entityId = -1;
							}
							ImGui::NextColumn();
							{
								ImGui::Combo("", &Settings::NameStealer::team, teams, IM_ARRAYSIZE(teams));
							}
							ImGui::Columns(1);
							ImGui::Separator();
							ImGui::Text("Other");
							ImGui::Separator();
							ImGui::Columns(2, NULL, true);
							{						
								ImGui::Checkbox("Auto Accept", &Settings::AutoAccept::enabled);
								ImGui::Checkbox("Disable post-processing", &Settings::DisablePostProcessing::enabled);
								ImGui::Checkbox("AirStuck", &Settings::Airstuck::enabled);
								SetTooltip("Stops tickrate so you freeze in place");
								ImGui::Checkbox("Autoblock", &Settings::Autoblock::enabled);
								SetTooltip("Allows you to block players from moving forwards by standing in front of them and mirroring their moves - great for griefing");					
								ImGui::Checkbox("Jump Throw", &Settings::JumpThrow::enabled);
								//	SetTooltip("Hold to prime grenade, release to perform perfect jump throw. Good for executing map smokes.");
								ImGui::Checkbox("Auto Defuse", &Settings::AutoDefuse::enabled);
								//	SetTooltip("Will automatically defuse the bomb with 0.5 seconds remaining - starts at 5.5 seconds until explosion with kit and 10.5 without");
								ImGui::Checkbox("Silent Defuse", &Settings::AutoDefuse::silent);
							}
							ImGui::NextColumn();
							{
								ImGui::Checkbox("Show Ranks", &Settings::ShowRanks::enabled);
								SetTooltip("Displays competitive rank of all players in the scoreboard next to their name during a competitive match");
								ImGui::Checkbox("Screenshot Cleaner", &Settings::ScreenshotCleaner::enabled);
								SetTooltip("Prevents XHOOK visuals from appearing in screenshots taken");
								UI::KeyBindButton(&Settings::Airstuck::key);
								UI::KeyBindButton(&Settings::Autoblock::key);
								UI::KeyBindButton(&Settings::JumpThrow::key);
								ImGui::Checkbox("Sniper Crosshair", &Settings::SniperCrosshair::enabled);
								ImGui::Checkbox(XorStr("Event Logger"), &Settings::EventLogger::enabled);
								ImGui::Checkbox(XorStr("Debug Anim overlay"), &Settings::Debug::AnimLayers::draw);
							}
							ImGui::NextColumn();
							{
								ImGui::Columns(1);
								ImGui::Separator();
								ImGui::Text(XorStr("Fake Lag"));
								ImGui::Separator();
								ImGui::Columns(2, NULL, true);
								{

									ImGui::ItemSize(ImVec2(0.0f, 0.0f), 0.0f);
									ImGui::Text(XorStr("Type"));
									ImGui::ItemSize(ImVec2(0.0f, 0.0f), 0.0f);
									if (Settings::FakeLag::type != FakeLagType::LUNICO)
										ImGui::Text(XorStr("Choke Amount"));
									ImGui::ItemSize(ImVec2(0.0f, 0.0f), 0.0f);
									ImGui::Checkbox("Enable", &Settings::FakeLag::enablefakelatency);
								}
								ImGui::NextColumn();
								{
									ImGui::PushItemWidth(-3);
									ImGui::Combo(XorStr("##FAKELAGTYPE"), (int*)&Settings::FakeLag::type,
										lagTypes, IM_ARRAYSIZE(lagTypes));
									ImGui::PopItemWidth();
									ImGui::PushItemWidth(-1);
									if (Settings::FakeLag::type != FakeLagType::LUNICO) {
										ImGui::SliderInt(XorStr("##FAKELAGAMOUNT"), &Settings::FakeLag::value, 0, 16,
											XorStr("Amount: %0.f"));
									}
									
									ImGui::SliderFloat("##fakelatency", &Settings::FakeLag::fakelatency, 0.f, 100.f, "fakelatency: %0.f");
									ImGui::PopItemWidth();
								}
							}
							ImGui::NextColumn();
							{
								ImGui::Columns(1);
								ImGui::Separator();
								ImGui::Text("WalkBot");
								ImGui::Separator();
								ImGui::Columns(1, NULL, true);
								{
									ImGui::Text("Walkbot - Experimental");

									ImGui::Checkbox("Enable", &Settings::WalkBot::enabled);
									UI::KeyBindButton(&Settings::WalkBot::Key);

									if (ImGui::Button("Reset")) {
										Settings::WalkBot::forceReset = true;
									}
									ImGui::SameLine();
									if (ImGui::Button("Clear markers")) {
										Walkbot::Clear();
										Settings::WalkBot::forceReset = true;
									}
								}
								ImGui::Columns(1);
								ImGui::Separator();
							}
							ImGui::PushStyleVar(ImGuiStyleVar_WindowMinSize, ImVec2(210, 85));
							if (ImGui::BeginPopupModal("Error###UNTRUSTED_FEATURE"))
							{
								ImGui::Text("You cannot use this feature on a VALVE server.");
								//ImGui::Checkbox("This is not a VALVE server", &ValveDSCheck::forceUT);
								if (ImGui::Button("OK"))
									ImGui::CloseCurrentPopup();
								ImGui::EndPopup();
							}
							ImGui::PopStyleVar();
							ImGui::EndChild();
						}
					}
					break;
				case 5:
					for (int i = 0; i < IM_ARRAYSIZE(colors); i++)
						colorNames[i] = colors[i].name;

					ImGui::SetNextWindowSize(ImVec2(540, 325), ImGuiSetCond_Always);
					ImGui::Columns(2, NULL, true);
					{
						ImGui::PushItemWidth(-1);
						ImGui::ListBox("##COLORSELECTION", &colorSelected, colorNames, IM_ARRAYSIZE(colorNames), 12);
						ImGui::PopItemWidth();			
					}
					ImGui::NextColumn();
					{
						if (colors[colorSelected].type == ColorListVar::HEALTHCOLORVAR_TYPE)
						{
							ColorPicker4((float*)colors[colorSelected].healthColorVarPtr);
							ImGui::Checkbox("Rainbow", &colors[colorSelected].healthColorVarPtr->rainbow);
							SetTooltip("Makes the color an animated rainbow.");
							ImGui::SameLine();
							ImGui::Checkbox("Health-Based", &colors[colorSelected].healthColorVarPtr->hp);
							SetTooltip("Takes color from entity health, i.e. 100 - green, 50 - yellow.");
							ImGui::PushItemWidth(-1);
							ImGui::SliderFloat("##RAINBOWSPEED", &colors[colorSelected].healthColorVarPtr->rainbowSpeed, 0.f, 1.f, "Rainbow Speed: %0.3f");
							ImGui::PopItemWidth();
						}
						else
						{
							ColorPicker4((float*)colors[colorSelected].colorVarPtr);
							ImGui::Checkbox("Rainbow", &colors[colorSelected].colorVarPtr->rainbow);
							SetTooltip("Makes the color an animated rainbow.");
							ImGui::PushItemWidth(-1);
							ImGui::SliderFloat("##RAINBOWSPEED", &colors[colorSelected].colorVarPtr->rainbowSpeed, 0.f, 1.f, "Rainbow Speed: %0.3f");

							ImGui::Checkbox("Background", &Settings::Background::enable);

							SetTooltip("Custom Background! Located at: C://Xhook//pictures//bg.jpg!");
							ImGui::SameLine();
							if (ImGui::Button("Reload Background"))
							{
								if (D3DXCreateTextureFromFile(pD3device, "C://xhook//Pictures//bg.jpg", &BackgroundTexture) != D3D_OK) {
									ImGui::OpenPopup("Success###Picture Reloaded!");
								}
							}
							SetTooltip("Reload Background! Located at: C://Xhook//pictures//bg.jpg");
							ImGui::SameLine();
							if (ImGui::Button("Delete Background"))
							{
								if (D3DXCreateTexture(pD3device, 1080, 720, 0, 0, D3DFMT_A8R8G8B8, D3DPOOL_MANAGED, &BackgroundTexture) != D3D_OK) {
									ImGui::OpenPopup("Success###Picture loaded!");
								}
							}
						}
					}
					ImGui::NextColumn();
					{
						ImGui::Text("Config");
						ImGui::Separator();
						ImGui::PushItemWidth(138);
						ImGui::Checkbox("Config window", &Configs::showWindow);
					}
					break;
				case 6:
					SkinModelChanger::RenderWindow();
					break;
				case 7:
					pProfileChanger->RenderWindow(true);
					break;

				//	ImGui::Columns(2, nullptr, false);
				//	ImGui::Checkbox("Enable Medal Changer", &Settings::InventoryChanger::medalsEnabled);
				//	static int medal_id = 0;
				//	ImGui::InputInt("Medal ID", &medal_id);
				//	if (ImGui::Button("Add") && medal_id != 0) {
				//		Settings::InventoryChanger::medals.insert(Settings::InventoryChanger::medals.end(), medal_id);
				//		medal_id = 0;
				//	}
				//	ImGui::SameLine();
				//	if (ImGui::Button("All medals"))
				//	{				
				//		Settings::InventoryChanger::medals = { 874,875,879,883,889,890,896,897,903,904,910,913,916,917,923,926,927,931,937,940,941,948,955,958,961,1003,1318,1329,1332,1338,1341,1344,1363,1372,4356,6021,6020,6019,6012,6032,6027,6002 }; // MEDAL IDS
				//	}
				//	ImGui::SameLine();
				//	if (ImGui::Button("Clear Medal listbox"))
				//	{
				//		Settings::InventoryChanger::medals = { };
				//	}
				//	ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(1, 1, 1, 1));
				//	ImGui::ListBoxHeader("Medal List");
				//	for (int m = 0; m < Settings::InventoryChanger::medals.size(); m++) {
				//		if (ImGui::Selectable(std::to_string(Settings::InventoryChanger::medals[m]).c_str())) {
				//			if (Settings::InventoryChanger::equipped_medal == Settings::InventoryChanger::medals[m]) {
				//				Settings::InventoryChanger::equipped_medal = 0;
				//				Settings::InventoryChanger::equipped_medal_override = false;
				//			}
				//			Settings::InventoryChanger::medals.erase(Settings::InventoryChanger::medals.begin() + m);
				//		}
				//	}
				//	ImGui::ListBoxFooter();
				//	ImGui::PopStyleColor();
				//	ImGui::Checkbox("Equipped Medal Override", &Settings::InventoryChanger::equipped_medal_override);
				//	if (Settings::InventoryChanger::equipped_medal_override) {
				//		static int equipped_medal = 0;
				//		ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(1, 1, 1, 1));
				//		if (ImGui::Combo("Equipped Medal", &equipped_medal, [](void* data, int idx, const char** out_text)
				//		{
				//			*out_text = std::to_string(Settings::InventoryChanger::medals[idx]).c_str();
				//			return true;
				//		}, nullptr, Settings::InventoryChanger::medals.size(), 5)) {
				//			Settings::InventoryChanger::equipped_medal = Settings::InventoryChanger::medals[equipped_medal];
				//		}
				//		ImGui::PopStyleColor();
				
					}
			}
		ImGui::End();
	   
	 }
	 

  }
  ImGui::Render();
}