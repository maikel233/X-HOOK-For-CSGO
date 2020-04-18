#include "settings.h"
#include "Util_SDK.h"
#include "Util_Items.h"
#include <string>
#include <sstream>
#include "SkinChanger.h"
#include "Xorstr.hpp"
//
//ModelRenderInfo_t& G::pInfo;
CUserCmd* G::UserCmd = nullptr;
CUserCmd* G::UserCmdForBacktracking = nullptr;
INetChannel* G::test = nullptr;
C_BasePlayer* G::LocalPlayer = nullptr;
bool G::PressedKeys[256] = {};
bool G::init;
bool G::input_shouldListen = false;
bool G::is_renderer_active = false;
const char* G::text;
float G::FakeAngle;
ITexture* G::m_texture_render_target;
//

void GetVal(Json::Value &config, int* setting)
{
	if (config.isNull())
		return;

	*setting = config.asInt();
}

void GetVal(Json::Value &config, bool* setting)
{
	if (config.isNull())
		return;

	*setting = config.asBool();
}

void GetVal(Json::Value &config, float* setting)
{
	if (config.isNull())
		return;

	*setting = config.asFloat();
}

void GetVal(Json::Value &config, ImColor* setting)
{
	if (config.isNull())
		return;

	GetVal(config["r"], &setting->Value.x);
	GetVal(config["g"], &setting->Value.y);
	GetVal(config["b"], &setting->Value.z);
	GetVal(config["a"], &setting->Value.w);
}

void GetVal(Json::Value &config, char** setting)
{
	if (config.isNull())
		return;

	*setting = strdup(config.asCString());
}

void GetVal(Json::Value &config, char* setting)
{
	if (config.isNull())
		return;

	strcpy(setting, config.asCString());
}

void GetVal(Json::Value &config, ColorVar* setting)
{
	if (config.isNull())
		return;

	GetVal(config["r"], &setting->color.Value.x);
	GetVal(config["g"], &setting->color.Value.y);
	GetVal(config["b"], &setting->color.Value.z);
	GetVal(config["a"], &setting->color.Value.w);
	GetVal(config["rainbow"], &setting->rainbow);
	GetVal(config["rainbowSpeed"], &setting->rainbowSpeed);
}

void GetVal(Json::Value &config, HealthColorVar* setting)
{
	if (config.isNull())
		return;

	GetVal(config["r"], &setting->color.Value.x);
	GetVal(config["g"], &setting->color.Value.y);
	GetVal(config["b"], &setting->color.Value.z);
	GetVal(config["a"], &setting->color.Value.w);
	GetVal(config["rainbow"], &setting->rainbow);
	GetVal(config["rainbowSpeed"], &setting->rainbowSpeed);
	GetVal(config["hp"], &setting->hp);
}

template <typename Ord, Ord(*lookupFunction)(std::string)>
void GetOrdinal(Json::Value& config, Ord* setting)
{
	if (config.isNull())
		return;

	Ord value;
	if (config.isString())
		value = lookupFunction(config.asString());
	else
		value = (Ord)config.asInt();

	*setting = value;
}

void GetButtonCode(Json::Value &config, enum ButtonCode_t* setting)
{
	GetOrdinal<enum ButtonCode_t, Util::GetButtonCode>(config, setting);
}

void LoadColor(Json::Value &config, ImColor color)
{
	config["r"] = color.Value.x;
	config["g"] = color.Value.y;
	config["b"] = color.Value.z;
	config["a"] = color.Value.w;
}

void LoadColor(Json::Value &config, ColorVar color)
{
	config["r"] = color.color.Value.x;
	config["g"] = color.color.Value.y;
	config["b"] = color.color.Value.z;
	config["a"] = color.color.Value.w;
	config["rainbow"] = color.rainbow;
	config["rainbowSpeed"] = color.rainbowSpeed;
}

void LoadColor(Json::Value &config, HealthColorVar color)
{
	config["r"] = color.color.Value.x;
	config["g"] = color.color.Value.y;
	config["b"] = color.color.Value.z;
	config["a"] = color.color.Value.w;
	config["rainbow"] = color.rainbow;
	config["rainbowSpeed"] = color.rainbowSpeed;
	config["hp"] = color.hp;
}

void Settings::LoadDefaultsOrSave(std::string path)
{
	Json::Value settings;
	Json::StyledWriter styledWriter;

	LoadColor(settings[XorStr("UI")][XorStr("mainColor")], Settings::UI::mainColor);
	LoadColor(settings[XorStr("UI")][XorStr("bodyColor")], Settings::UI::bodyColor );
	LoadColor(settings[XorStr("UI")][XorStr("fontColor")], Settings::UI::fontColor);
	LoadColor(settings[XorStr("UI")][XorStr("accentColor")], Settings::UI::accentColor);
	LoadColor(settings[XorStr("UI")][XorStr("Watermark")][XorStr("color")], Settings::UI::Watermark::color);
	settings[XorStr("UI")][XorStr("Watermark")][XorStr("displayIngame")] = Settings::UI::Watermark::displayIngame;

	//settings["UI"]["Fonts"]["ESP"]["family"] = Settings::UI::Fonts::ESP::family;
	//settings["UI"]["Fonts"]["ESP"]["size"] = Settings::UI::Fonts::ESP::size;
	//settings["UI"]["Fonts"]["ESP"]["flags"] = Settings::UI::Fonts::ESP::flags;

	for (auto i : Settings::Aimbot::weapons)
	{
		// TODO this is kind of a hack and i'm too tired to find a better way to do this
		// yes i tried defining a variable, skinSetting, and giving it the same value but woooooo operator overloading
		// in C++ and weird shit
#define weaponSetting settings["Aimbot"]["weapons"][Util::Items::GetItemName((enum ItemDefinitionIndex) i.first)]
		weaponSetting["Enabled"] = i.second.enabled;
		weaponSetting["Silent"] = i.second.silent;
		weaponSetting[XorStr("pSilent")] = i.second.pSilent;
		weaponSetting[XorStr("BackTrack")] = i.second.backtrack;
		weaponSetting["Friendly"] = i.second.friendly;
		weaponSetting[XorStr("ClosestBone")] = i.second.closestBone;
		weaponSetting[XorStr("engageLock")] = i.second.engageLock;
		weaponSetting[XorStr("engageLockTR")] = i.second.engageLockTR;
		weaponSetting[XorStr("engageLockTTR")] = i.second.engageLockTTR;
		weaponSetting["TargetBone"] = (int)i.second.bone;
		weaponSetting["AimKey"] = Util::GetButtonName(i.second.aimkey);
		weaponSetting["AimKeyOnly"] = i.second.aimkeyOnly;
		weaponSetting["Smooth"]["Enabled"] = i.second.smoothEnabled;
		weaponSetting["Smooth"]["Amount"] = i.second.smoothAmount;
		weaponSetting["Smooth"]["Type"] = (int)i.second.smoothType;
		weaponSetting["Smooth"]["Salting"]["Enabled"] = i.second.smoothSaltEnabled;
		weaponSetting["Smooth"]["Salting"]["Multiplier"] = i.second.smoothSaltMultiplier;
		weaponSetting["ErrorMargin"]["Enabled"] = i.second.errorMarginEnabled;
		weaponSetting["ErrorMargin"]["Value"] = i.second.errorMarginValue;
		weaponSetting["Curve"]["Enabled"] = i.second.curveEnabled;
		weaponSetting["Curve"]["Amount"] = i.second.curveAmount;
		weaponSetting["AutoAim"]["Enabled"] = i.second.autoAimEnabled;
		weaponSetting["AutoAim"]["FOV"] = i.second.autoAimFov;
		weaponSetting[XorStr("AimStep")][XorStr("Enabled")] = i.second.aimStepEnabled;
		weaponSetting[XorStr("AimStep")][XorStr("min")] = i.second.aimStepMin;
		weaponSetting[XorStr("AimStep")][XorStr("max")] = i.second.aimStepMax;
		weaponSetting["RCS"]["Enabled"] = i.second.rcsEnabled;
		weaponSetting["RCS"]["AlwaysOn"] = i.second.rcsAlwaysOn;
		weaponSetting["RCS"]["AmountX"] = i.second.rcsAmountX;
		weaponSetting["RCS"]["AmountY"] = i.second.rcsAmountY;
		weaponSetting["AutoCockRevolver"]["Enabled"] = i.second.autoCockRevolver;
		weaponSetting["AutoPistol"]["Enabled"] = i.second.autoPistolEnabled;
		weaponSetting["AutoShoot"]["Enabled"] = i.second.autoShootEnabled;
		weaponSetting["AutoScope"]["Enabled"] = i.second.autoScopeEnabled;
		weaponSetting["NoShoot"]["Enabled"] = i.second.noShootEnabled;
		weaponSetting["IgnoreJump"]["Enabled"] = i.second.ignoreJumpEnabled;
		weaponSetting["SmokeCheck"]["Enabled"] = i.second.smokeCheck;
		weaponSetting["FlashCheck"]["Enabled"] = i.second.flashCheck;
		weaponSetting["SpreadLimit"]["Enabled"] = i.second.spreadLimitEnabled;
		weaponSetting["SpreadLimit"]["DistanceBased"] = i.second.spreadLimitDistance;
		weaponSetting["SpreadLimit"]["Value"] = i.second.spreadLimit;
		weaponSetting["AutoWall"]["Enabled"] = i.second.autoWallEnabled;
		weaponSetting["AutoWall"]["Value"] = i.second.autoWallValue;
		weaponSetting["AutoSlow"]["enabled"] = i.second.autoSlow;
		weaponSetting["Prediction"]["enabled"] = i.second.predEnabled;
		weaponSetting["moveMouse"] = i.second.moveMouse;
	/*	weaponSetting["AutoSlow"]["minDamage"] = i.second.autoSlowMinDamage;*/
		weaponSetting["HitChance"]["enabled"] = i.second.hitChanceEnabled;
		weaponSetting["HitChance"]["hitRays"] = i.second.hitChanceRays;
		weaponSetting["HitChance"]["value"] = i.second.hitChanceValue;
		weaponSetting[XorStr("RCS")][XorStr("smooth")] = i.second.smooth;
		weaponSetting[XorStr("RCS")][XorStr("smoothvalue")] = i.second.smoothvalue;

		for (int bone = (int)DesiredBones::BONE_PELVIS; bone <= (int)DesiredBones::BONE_RIGHT_SOLE; bone++)
			weaponSetting["DesiredBones"]["Bones"][bone] = i.second.desiredBones[bone];


		weaponSetting["AutoAim"]["RealDistance"] = i.second.autoAimRealDistance;

#undef weaponSetting
	}



	settings["Aimbot"]["AutoCrouch"]["enabled"] = Settings::Aimbot::AutoCrouch::enabled;
	settings["Aimbot"]["AutoShoot"]["velocityCheck"] = Settings::Aimbot::AutoShoot::velocityCheck;
//	settings["Resolver"]["resolve_all"] = Settings::Resolver::resolveAll;
	settings[XorStr("LBYIndicator")][XorStr("enabled")] = Settings::lbyindicator::enabled;
	settings[XorStr("Resolver")][XorStr("enabled")] = Settings::Resolver::enabled;
	settings[XorStr("Resolver")][XorStr("ticks")] = Settings::Resolver::ticks;
	settings[XorStr("Resolver")][XorStr("modulo")] = Settings::Resolver::modulo;
	settings[XorStr("Resolver")][XorStr("pitch")] = Settings::Resolver::pitch;
	settings[XorStr("Resolver")][XorStr("LagComp")] = Settings::Resolver::LagComp;
	settings[XorStr("Resolver")][XorStr("angleFlipEnabled")] = Settings::Resolver::angleFlipEnabled;
	settings[XorStr("Resolver")][XorStr("angleFlip")] = Util::GetButtonName(Settings::Resolver::angleFlip);
	settings[XorStr("Resolver")][XorStr("baimAfter")] = Settings::Resolver::baimAfter;
	settings[XorStr("Resolver")][XorStr("lbyOnly")] = Settings::Resolver::lbyOnly;
	settings["Triggerbot"]["enabled"] = Settings::Triggerbot::enabled;
	settings["Triggerbot"]["key"] = Util::GetButtonName(Settings::Triggerbot::key);
	settings["Triggerbot"]["Filters"]["enemies"] = Settings::Triggerbot::Filters::enemies;
	settings["Triggerbot"]["Filters"]["allies"] = Settings::Triggerbot::Filters::allies;
	settings["Triggerbot"]["Filters"]["walls"] = Settings::Triggerbot::Filters::walls;
	settings["Triggerbot"]["Filters"]["smoke_check"] = Settings::Triggerbot::Filters::smokeCheck;
	settings["Triggerbot"]["Filters"]["flash_check"] = Settings::Triggerbot::Filters::flashCheck;
	settings["Triggerbot"]["Filters"]["head"] = Settings::Triggerbot::Filters::head;
	settings["Triggerbot"]["Filters"]["chest"] = Settings::Triggerbot::Filters::chest;
	settings["Triggerbot"]["Filters"]["stomach"] = Settings::Triggerbot::Filters::stomach;
	settings["Triggerbot"]["Filters"]["arms"] = Settings::Triggerbot::Filters::arms;
	settings["Triggerbot"]["Filters"]["legs"] = Settings::Triggerbot::Filters::legs;
	settings[XorStr("Triggerbot")][XorStr("RandomDelay")][XorStr("enabled")] = Settings::Triggerbot::RandomDelay::enabled;
	settings[XorStr("Triggerbot")][XorStr("RandomDelay")][XorStr("lowBound")] = Settings::Triggerbot::RandomDelay::lowBound;
	settings[XorStr("Triggerbot")][XorStr("RandomDelay")][XorStr("highBound")] = Settings::Triggerbot::RandomDelay::highBound;

	settings[XorStr("AngleIndicator")][XorStr("enabled")] = Settings::AngleIndicator::enabled;
	settings[XorStr("AngleIndicator")][XorStr("Veloc")] = Settings::AngleIndicator::Veloc;

	settings[XorStr("EventLogger")][XorStr("enabled")] = Settings::EventLogger::enabled;

	settings[XorStr("AntiAim")][XorStr("Standing")][XorStr("Yaw")][XorStr(
		"enabled")] = Settings::AntiAim::Standing::Yaw::enabled;
	settings[XorStr("AntiAim")][XorStr("Standing")][XorStr("Yaw")][XorStr(
		"type")] = (int)Settings::AntiAim::Standing::Yaw::type;
	settings[XorStr("AntiAim")][XorStr("Standing")][XorStr("Yaw")][XorStr(
		"typeFake")] = (int)Settings::AntiAim::Standing::Yaw::typeFake;
	settings[XorStr("AntiAim")][XorStr("Standing")][XorStr("Yaw")][XorStr(
		"typeAdd")] = Settings::AntiAim::Standing::Yaw::typeAdd;
	settings[XorStr("AntiAim")][XorStr("Standing")][XorStr("Yaw")][XorStr(
		"typeFakeAdd")] = Settings::AntiAim::Standing::Yaw::typeFakeAdd;
	settings[XorStr("AntiAim")][XorStr("Standing")][XorStr("Pitch")][XorStr(
		"enabled")] = Settings::AntiAim::Standing::Pitch::enabled;
	settings[XorStr("AntiAim")][XorStr("Standing")][XorStr("Pitch")][XorStr(
		"type")] = (int)Settings::AntiAim::Standing::Pitch::type;
	settings[XorStr("AntiAim")][XorStr("Standing")][XorStr("Pitch")][XorStr(
		"custom")] = Settings::AntiAim::Standing::Pitch::custom;
	settings[XorStr("AntiAim")][XorStr("Standing")][XorStr("Roll")][XorStr(
		"enabled")] = Settings::AntiAim::Standing::Roll::enabled;
	settings[XorStr("AntiAim")][XorStr("Standing")][XorStr("Roll")][XorStr(
		"type")] = (int)Settings::AntiAim::Standing::Roll::type;
	settings[XorStr("AntiAim")][XorStr("Standing")][XorStr("SwitchAA")][XorStr(
		"enabled")] = Settings::AntiAim::Standing::SwitchAA::enabled;
	settings[XorStr("AntiAim")][XorStr("Standing")][XorStr("SwitchAA")][XorStr(
		"key")] = Settings::AntiAim::Standing::SwitchAA::key;
	settings[XorStr("AntiAim")][XorStr("Standing")][XorStr("LBY")][XorStr(
		"enabled")] = Settings::AntiAim::Standing::LBY::enabled;
	settings[XorStr("AntiAim")][XorStr("Standing")][XorStr("LBY")][XorStr(
		"type")] = (int)Settings::AntiAim::Standing::LBY::type;
	settings[XorStr("AntiAim")][XorStr("Standing")][XorStr("HeadEdge")][XorStr(
		"enabled")] = Settings::AntiAim::Standing::HeadEdge::enabled;
	settings[XorStr("AntiAim")][XorStr("Standing")][XorStr("HeadEdge")][XorStr(
		"distance")] = Settings::AntiAim::Standing::HeadEdge::distance;
	settings[XorStr("AntiAim")][XorStr("Standing")][XorStr("HeadEdge")][XorStr(
		"realAdd")] = Settings::AntiAim::Standing::HeadEdge::realAdd;
	settings[XorStr("AntiAim")][XorStr("Standing")][XorStr("HeadEdge")][XorStr(
		"fakeAdd")] = Settings::AntiAim::Standing::HeadEdge::fakeAdd;
	settings[XorStr("AntiAim")][XorStr("Standing")][XorStr(
		"antiResolver")] = Settings::AntiAim::Standing::antiResolver;
	settings[XorStr("AntiAim")][XorStr("Standing")][XorStr("dynamicAA")] = Settings::AntiAim::Standing::dynamicAA;
	settings[XorStr("AntiAim")][XorStr("Standing")][XorStr(
		"untrustedAngles")] = Settings::AntiAim::Standing::untrustedAngles;

	settings[XorStr("AntiAim")][XorStr("Moving")][XorStr("Yaw")][XorStr(
		"enabled")] = Settings::AntiAim::Moving::Yaw::enabled;
	settings[XorStr("AntiAim")][XorStr("Moving")][XorStr("Yaw")][XorStr(
		"type")] = (int)Settings::AntiAim::Moving::Yaw::type;
	settings[XorStr("AntiAim")][XorStr("Moving")][XorStr("Yaw")][XorStr(
		"typeFake")] = (int)Settings::AntiAim::Moving::Yaw::typeFake;
	settings[XorStr("AntiAim")][XorStr("Moving")][XorStr("Yaw")][XorStr(
		"typeAdd")] = Settings::AntiAim::Moving::Yaw::typeAdd;
	settings[XorStr("AntiAim")][XorStr("Moving")][XorStr("Yaw")][XorStr(
		"typeFakeAdd")] = Settings::AntiAim::Moving::Yaw::typeFakeAdd;
	settings[XorStr("AntiAim")][XorStr("Moving")][XorStr("Pitch")][XorStr(
		"enabled")] = Settings::AntiAim::Moving::Pitch::enabled;
	settings[XorStr("AntiAim")][XorStr("Moving")][XorStr("Pitch")][XorStr(
		"type")] = (int)Settings::AntiAim::Moving::Pitch::type;
	settings[XorStr("AntiAim")][XorStr("Moving")][XorStr("Pitch")][XorStr(
		"custom")] = Settings::AntiAim::Moving::Pitch::custom;
	settings[XorStr("AntiAim")][XorStr("Moving")][XorStr("Roll")][XorStr(
		"enabled")] = Settings::AntiAim::Moving::Roll::enabled;
	settings[XorStr("AntiAim")][XorStr("Moving")][XorStr("Roll")][XorStr(
		"type")] = (int)Settings::AntiAim::Moving::Roll::type;
	settings[XorStr("AntiAim")][XorStr("Moving")][XorStr("SwitchAA")][XorStr(
		"enabled")] = Settings::AntiAim::Moving::SwitchAA::enabled;
	settings[XorStr("AntiAim")][XorStr("Moving")][XorStr("SwitchAA")][XorStr(
		"key")] = Settings::AntiAim::Moving::SwitchAA::key;
	settings[XorStr("AntiAim")][XorStr("Moving")][XorStr("LBY")][XorStr(
		"enabled")] = Settings::AntiAim::Moving::LBY::enabled;
	settings[XorStr("AntiAim")][XorStr("Moving")][XorStr("LBY")][XorStr(
		"type")] = (int)Settings::AntiAim::Moving::LBY::type;
	settings[XorStr("AntiAim")][XorStr("Moving")][XorStr("HeadEdge")][XorStr(
		"enabled")] = Settings::AntiAim::Moving::HeadEdge::enabled;
	settings[XorStr("AntiAim")][XorStr("Moving")][XorStr("HeadEdge")][XorStr(
		"distance")] = Settings::AntiAim::Moving::HeadEdge::distance;
	settings[XorStr("AntiAim")][XorStr("Moving")][XorStr("HeadEdge")][XorStr(
		"realAdd")] = Settings::AntiAim::Moving::HeadEdge::realAdd;
	settings[XorStr("AntiAim")][XorStr("Moving")][XorStr("HeadEdge")][XorStr(
		"fakeAdd")] = Settings::AntiAim::Moving::HeadEdge::fakeAdd;
	settings[XorStr("AntiAim")][XorStr("Moving")][XorStr(
		"antiResolver")] = Settings::AntiAim::Moving::antiResolver;
	settings[XorStr("AntiAim")][XorStr("Moving")][XorStr("dynamicAA")] = Settings::AntiAim::Moving::dynamicAA;
	settings[XorStr("AntiAim")][XorStr("Moving")][XorStr(
		"untrustedAngles")] = Settings::AntiAim::Moving::untrustedAngles;

	settings[XorStr("AntiAim")][XorStr("Airborne")][XorStr("Yaw")][XorStr(
		"enabled")] = Settings::AntiAim::Airborne::Yaw::enabled;
	settings[XorStr("AntiAim")][XorStr("Airborne")][XorStr("Yaw")][XorStr(
		"type")] = (int)Settings::AntiAim::Airborne::Yaw::type;
	settings[XorStr("AntiAim")][XorStr("Airborne")][XorStr("Yaw")][XorStr(
		"typeFake")] = (int)Settings::AntiAim::Airborne::Yaw::typeFake;
	settings[XorStr("AntiAim")][XorStr("Airborne")][XorStr("Yaw")][XorStr(
		"typeAdd")] = Settings::AntiAim::Airborne::Yaw::typeAdd;
	settings[XorStr("AntiAim")][XorStr("Airborne")][XorStr("Yaw")][XorStr(
		"typeFakeAdd")] = Settings::AntiAim::Airborne::Yaw::typeFakeAdd;
	settings[XorStr("AntiAim")][XorStr("Airborne")][XorStr("Pitch")][XorStr(
		"enabled")] = Settings::AntiAim::Airborne::Pitch::enabled;
	settings[XorStr("AntiAim")][XorStr("Airborne")][XorStr("Pitch")][XorStr(
		"type")] = (int)Settings::AntiAim::Airborne::Pitch::type;
	settings[XorStr("AntiAim")][XorStr("Airborne")][XorStr("Pitch")][XorStr(
		"custom")] = Settings::AntiAim::Airborne::Pitch::custom;
	settings[XorStr("AntiAim")][XorStr("Airborne")][XorStr("Roll")][XorStr(
		"enabled")] = Settings::AntiAim::Airborne::Roll::enabled;
	settings[XorStr("AntiAim")][XorStr("Airborne")][XorStr("Roll")][XorStr(
		"type")] = (int)Settings::AntiAim::Airborne::Roll::type;
	settings[XorStr("AntiAim")][XorStr("Airborne")][XorStr("SwitchAA")][XorStr(
		"enabled")] = Settings::AntiAim::Airborne::SwitchAA::enabled;
	settings[XorStr("AntiAim")][XorStr("Airborne")][XorStr("SwitchAA")][XorStr(
		"key")] = Settings::AntiAim::Airborne::SwitchAA::key;
	settings[XorStr("AntiAim")][XorStr("Airborne")][XorStr("LBY")][XorStr(
		"enabled")] = Settings::AntiAim::Airborne::LBY::enabled;
	settings[XorStr("AntiAim")][XorStr("Airborne")][XorStr("LBY")][XorStr(
		"type")] = (int)Settings::AntiAim::Airborne::LBY::type;
	settings[XorStr("AntiAim")][XorStr("Airborne")][XorStr("HeadEdge")][XorStr(
		"enabled")] = Settings::AntiAim::Airborne::HeadEdge::enabled;
	settings[XorStr("AntiAim")][XorStr("Airborne")][XorStr("HeadEdge")][XorStr(
		"distance")] = Settings::AntiAim::Airborne::HeadEdge::distance;
	settings[XorStr("AntiAim")][XorStr("Airborne")][XorStr("HeadEdge")][XorStr(
		"fakeAdd")] = Settings::AntiAim::Airborne::HeadEdge::fakeAdd;
	settings[XorStr("AntiAim")][XorStr("Airborne")][XorStr("HeadEdge")][XorStr(
		"realAdd")] = Settings::AntiAim::Airborne::HeadEdge::realAdd;
	settings[XorStr("AntiAim")][XorStr("Airborne")][XorStr(
		"antiResolver")] = Settings::AntiAim::Airborne::antiResolver;
	settings[XorStr("AntiAim")][XorStr("Airborne")][XorStr("dynamicAA")] = Settings::AntiAim::Airborne::dynamicAA;
	settings[XorStr("AntiAim")][XorStr("Airborne")][XorStr(
		"untrustedAngles")] = Settings::AntiAim::Airborne::untrustedAngles;

	settings[XorStr("AntiAim")][XorStr("Misc")][XorStr("AutoDisable")][XorStr(
		"noEnemy")] = Settings::AntiAim::Misc::AutoDisable::noEnemy;
	settings[XorStr("AntiAim")][XorStr("Misc")][XorStr("AutoDisable")][XorStr(
		"knifeHeld")] = Settings::AntiAim::Misc::AutoDisable::knifeHeld;
	settings[XorStr("AntiAim")][XorStr("Misc")][XorStr("AutoDisable")][XorStr(
		"bombHeld")] = Settings::AntiAim::Misc::AutoDisable::bombHeld;
	settings[XorStr("AntiAim")][XorStr("Misc")][XorStr("AutoDisable")][XorStr(
		"freezeTime")] = Settings::AntiAim::Misc::AutoDisable::freezeTime;

	settings[XorStr("MoonWalk")][XorStr("enabled")] = Settings::MoonWalk::enabled;

	settings[XorStr("ESP")][XorStr("enabled")] = Settings::ESP::enabled;
	settings[XorStr("ESP")][XorStr("key")] = Util::GetButtonName(Settings::ESP::key);

	LoadColor(settings["ESP"]["enemy_color"], Settings::ESP::enemyColor);
	LoadColor(settings["ESP"]["enemy_visible_color"], Settings::ESP::enemyVisibleColor);
	LoadColor(settings["ESP"]["ally_color"], Settings::ESP::allyColor);
	LoadColor(settings["ESP"]["ally_visible_color"], Settings::ESP::allyVisibleColor);
	LoadColor(settings["ESP"]["t_color"], Settings::ESP::tColor);
	LoadColor(settings["ESP"]["t_visible_color"], Settings::ESP::tVisibleColor);
	LoadColor(settings["ESP"]["ct_color"], Settings::ESP::ctColor);
	LoadColor(settings["ESP"]["ct_visible_color"], Settings::ESP::ctVisibleColor);
	LoadColor(settings["ESP"]["localplayer_color"], Settings::ESP::localplayerColor);
	LoadColor(settings["ESP"]["bomb_color"], Settings::ESP::bombColor);
	LoadColor(settings["ESP"]["bomb_defusing_color"], Settings::ESP::bombDefusingColor);
	LoadColor(settings["ESP"]["hostage_color"], Settings::ESP::hostageColor);
	LoadColor(settings["ESP"]["defuser_color"], Settings::ESP::defuserColor);
	LoadColor(settings["ESP"]["weapon_color"], Settings::ESP::weaponColor);
	LoadColor(settings["ESP"]["chicken_color"], Settings::ESP::chickenColor);
	LoadColor(settings["ESP"]["fish_color"], Settings::ESP::fishColor);
	LoadColor(settings["ESP"]["smoke_color"], Settings::ESP::smokeColor);
	LoadColor(settings["ESP"]["decoy_color"], Settings::ESP::decoyColor);
	LoadColor(settings["ESP"]["flashbang_color"], Settings::ESP::flashbangColor);
	LoadColor(settings["ESP"]["grenade_color"], Settings::ESP::grenadeColor);
	LoadColor(settings["ESP"]["molotov_color"], Settings::ESP::molotovColor);
	settings["ESP"]["Glow"]["enabled"] = Settings::ESP::Glow::enabled;
	LoadColor(settings["ESP"]["Glow"]["ally_color"], Settings::ESP::Glow::allyColor);
	LoadColor(settings["ESP"]["Glow"]["enemy_color"], Settings::ESP::Glow::enemyColor);
	LoadColor(settings["ESP"]["Glow"]["enemy_visible_color"], Settings::ESP::Glow::enemyVisibleColor);
	LoadColor(settings["ESP"]["Glow"]["localplayer_color"], Settings::ESP::Glow::localplayerColor);
	LoadColor(settings["ESP"]["Glow"]["weapon_color"], Settings::ESP::Glow::weaponColor);
	LoadColor(settings["ESP"]["Glow"]["grenade_color"], Settings::ESP::Glow::grenadeColor);
	LoadColor(settings["ESP"]["Glow"]["defuser_color"], Settings::ESP::Glow::defuserColor);
	LoadColor(settings["ESP"]["Glow"]["chicken_color"], Settings::ESP::Glow::chickenColor);
	settings["ESP"]["Filters"]["legit"] = Settings::ESP::Filters::legit;
	settings["ESP"]["Filters"]["visibility_check"] = Settings::ESP::Filters::visibilityCheck;
	settings["ESP"]["Filters"]["smoke_check"] = Settings::ESP::Filters::smokeCheck;
	settings["ESP"]["Filters"]["enemies"] = Settings::ESP::Filters::enemies;
	settings["ESP"]["Filters"]["allies"] = Settings::ESP::Filters::allies;
	settings["ESP"]["Filters"]["bomb"] = Settings::ESP::Filters::bomb;
	settings["ESP"]["Filters"]["hostages"] = Settings::ESP::Filters::hostages;
	settings["ESP"]["Filters"]["defusers"] = Settings::ESP::Filters::defusers;
	settings["ESP"]["Filters"]["weapons"] = Settings::ESP::Filters::weapons;
	settings["ESP"]["Filters"]["chickens"] = Settings::ESP::Filters::chickens;
	settings["ESP"]["Filters"]["fishes"] = Settings::ESP::Filters::fishes;
	settings["ESP"]["Filters"]["throwables"] = Settings::ESP::Filters::throwables;
	settings["ESP"]["Filters"]["localplayer"] = Settings::ESP::Filters::localplayer;
	settings["ESP"]["Info"]["name"] = Settings::ESP::Info::name;
	settings["ESP"]["Info"]["clan"] = Settings::ESP::Info::clan;
	settings["ESP"]["Info"]["steam_id"] = Settings::ESP::Info::steamId;
	settings["ESP"]["Info"]["rank"] = Settings::ESP::Info::rank;
	settings["ESP"]["Info"]["health"] = Settings::ESP::Info::health;
	settings["ESP"]["Info"]["weapon"] = Settings::ESP::Info::weapon;
	settings["ESP"]["Info"]["scoped"] = Settings::ESP::Info::scoped;
	settings["ESP"]["Info"]["reloading"] = Settings::ESP::Info::reloading;
	settings["ESP"]["Info"]["flashed"] = Settings::ESP::Info::flashed;
	settings["ESP"]["Info"]["planting"] = Settings::ESP::Info::planting;
	settings["ESP"]["Info"]["has_defuser"] = Settings::ESP::Info::hasDefuser;
	settings["ESP"]["Info"]["defusing"] = Settings::ESP::Info::defusing;
	settings["ESP"]["Info"]["grabbing_hostage"] = Settings::ESP::Info::grabbingHostage;
	settings["ESP"]["Info"]["rescuing"] = Settings::ESP::Info::rescuing;
	settings["ESP"]["Info"]["location"] = Settings::ESP::Info::location;
	settings[XorStr("ESP")][XorStr("Info")][XorStr("money")] = Settings::ESP::Info::money;
	settings[XorStr("ESP")][XorStr("Info")][XorStr("lby")] = Settings::ESP::Info::lby;
	settings[XorStr("ESP")][XorStr("Info")][XorStr("resolveInfo")] = Settings::ESP::Info::resolveInfo;
	settings["ESP"]["Boxes"]["enabled"] = Settings::ESP::Boxes::enabled;
	settings["ESP"]["Boxes"]["type"] = (int)Settings::ESP::Boxes::type;
	settings["ESP"]["Skeleton"]["enabled"] = Settings::ESP::Skeleton::enabled;
	LoadColor(settings["ESP"]["Skeleton"]["color"], Settings::ESP::Skeleton::color);
	settings["ESP"]["Bars"]["enabled"] = Settings::ESP::Bars::enabled;
	settings["ESP"]["Bars"]["color_type"] = (int)Settings::ESP::Bars::colorType;
	settings["ESP"]["Bars"]["type"] = (int)Settings::ESP::Bars::type;
	settings["ESP"]["Tracers"]["enabled"] = Settings::ESP::Tracers::enabled;
	settings["ESP"]["Tracers"]["type"] = (int)Settings::ESP::Tracers::type;
	settings["ESP"]["BulletTracers"]["enabled"] = Settings::ESP::BulletTracers::enabled;
	settings["ESP"]["FOVCrosshair"]["enabled"] = Settings::ESP::FOVCrosshair::enabled;
	settings["ESP"]["FOVCrosshair"]["filled"] = Settings::ESP::FOVCrosshair::filled;
	LoadColor(settings["ESP"]["FOVCrosshair"]["color"], Settings::ESP::FOVCrosshair::color);
	settings["ESP"]["Chams"]["Arms"]["enabled"] = Settings::ESP::Chams::Arms::enabled;
	settings["ESP"]["Chams"]["Arms"]["type"] = (int)Settings::ESP::Chams::Arms::type;
	settings["ESP"]["Chams"]["Weapon"]["enabled"] = Settings::ESP::Chams::Weapon::enabled;
	LoadColor(settings["ESP"]["Chams"]["Weapon"]["color"], Settings::ESP::Chams::Weapon::color);
	LoadColor(settings["ESP"]["Chams"]["Arms"]["color"], Settings::ESP::Chams::Arms::color);
	LoadColor(settings["ESP"]["Chams"]["players_ally_color"], Settings::ESP::Chams::allyColor);
	LoadColor(settings["ESP"]["Chams"]["players_ally_visible_color"], Settings::ESP::Chams::allyVisibleColor);
	LoadColor(settings["ESP"]["Chams"]["players_enemy_color"], Settings::ESP::Chams::enemyColor);
	LoadColor(settings["ESP"]["Chams"]["players_enemy_visible_color"], Settings::ESP::Chams::enemyVisibleColor);
	LoadColor(settings["ESP"]["Chams"]["localplayer_color"], Settings::ESP::Chams::localplayerColor);
	settings["ESP"]["Chams"]["type"] = (int)Settings::ESP::Chams::type;
	settings["ESP"]["Chams"]["enabled"] = Settings::ESP::Chams::enabled;
	settings["ESP"]["Sounds"]["enabled"] = Settings::ESP::Sounds::enabled;
	settings["ESP"]["Sounds"]["time"] = Settings::ESP::Sounds::time;
	settings["ESP"]["Hitmarker"]["enabled"] = Settings::ESP::Hitmarker::enabled;
	settings["ESP"]["Hitmarker"]["enemies"] = Settings::ESP::Hitmarker::enemies;
	settings["ESP"]["Hitmarker"]["allies"] = Settings::ESP::Hitmarker::allies;
	LoadColor(settings["ESP"]["Hitmarker"]["color"], Settings::ESP::Hitmarker::color);
	settings["ESP"]["Hitmarker"]["duration"] = Settings::ESP::Hitmarker::duration;
	settings["ESP"]["Hitmarker"]["size"] = Settings::ESP::Hitmarker::size;
	settings["ESP"]["Hitmarker"]["inner_gap"] = Settings::ESP::Hitmarker::innerGap;
	settings["ESP"]["Hitmarker"]["Damage"]["enabled"] = Settings::ESP::Hitmarker::Damage::enabled;
	settings["ESP"]["HeadDot"]["enabled"] = Settings::ESP::HeadDot::enabled;
	settings["ESP"]["HeadDot"]["size"] = Settings::ESP::HeadDot::size;

	settings["Dlights"]["enabled"] = Settings::Dlights::enabled;
	settings["Dlights"]["radius"] = Settings::Dlights::radius;

	settings["Spammer"]["spammer_type"] = (int)Settings::Spammer::type;
	settings["Spammer"]["say_team"] = Settings::Spammer::say_team;

	settings["Spammer"]["KillSpammer"]["enabled"] = Settings::Spammer::KillSpammer::enabled;
	settings["Spammer"]["KillSpammer"]["say_team"] = Settings::Spammer::KillSpammer::sayTeam;
	Json::Value killSpammerMessages;
	for (auto it : Settings::Spammer::KillSpammer::messages)
		killSpammerMessages.append(it);
	settings["Spammer"]["KillSpammer"]["messages"] = killSpammerMessages;

	Json::Value normalSpammerMessages;
	for (auto it : Settings::Spammer::NormalSpammer::messages)
		normalSpammerMessages.append(it);
	settings["Spammer"]["NormalSpammer"]["messages"] = normalSpammerMessages;

	settings["Spammer"]["PositionSpammer"]["show_name"] = Settings::Spammer::PositionSpammer::showName;
	settings["Spammer"]["PositionSpammer"]["show_weapon"] = Settings::Spammer::PositionSpammer::showWeapon;
	settings["Spammer"]["PositionSpammer"]["show_rank"] = Settings::Spammer::PositionSpammer::showRank;
	settings["Spammer"]["PositionSpammer"]["show_wins"] = Settings::Spammer::PositionSpammer::showWins;
	settings["Spammer"]["PositionSpammer"]["show_health"] = Settings::Spammer::PositionSpammer::showHealth;
	settings["Spammer"]["PositionSpammer"]["show_money"] = Settings::Spammer::PositionSpammer::showMoney;
	settings["Spammer"]["PositionSpammer"]["show_lastplace"] = Settings::Spammer::PositionSpammer::showLastplace;

	settings["BHop"]["enabled"] = Settings::BHop::enabled;
	settings[XorStr("BHop")][XorStr("Chance")][XorStr("enabled")] = Settings::BHop::Chance::enabled;
	settings[XorStr("BHop")][XorStr("Chance")][XorStr("value")] = Settings::BHop::Chance::value;
	settings[XorStr("BHop")][XorStr("Hops")][XorStr("enabledMax")] = Settings::BHop::Hops::enabledMax;
	settings[XorStr("BHop")][XorStr("Hops")][XorStr("Max")] = Settings::BHop::Hops::Max;
	settings[XorStr("BHop")][XorStr("Hops")][XorStr("enabledMin")] = Settings::BHop::Hops::enabledMin;
	settings[XorStr("BHop")][XorStr("Hops")][XorStr("Min")] = Settings::BHop::Hops::Min;
	settings["AutoStrafe"]["enabled"] = Settings::AutoStrafe::enabled;
	settings["AutoStrafe"]["type"] = (int)Settings::AutoStrafe::type;
	settings["AutoStrafe"]["silent"] = Settings::AutoStrafe::silent;

	settings["Noflash"]["enabled"] = Settings::Noflash::enabled;
	settings["Noflash"]["value"] = Settings::Noflash::value;

	settings["Radar"]["enabled"] = Settings::Radar::enabled;
	settings["Radar"]["zoom"] = Settings::Radar::zoom;
	settings["Radar"]["enemies"] = Settings::Radar::enemies;
	settings["Radar"]["allies"] = Settings::Radar::allies;
	settings["Radar"]["legit"] = Settings::Radar::legit;
	settings["Radar"]["visibility_check"] = Settings::Radar::visibilityCheck;
	settings["Radar"]["smoke_check"] = Settings::Radar::smokeCheck;
	settings["Radar"]["InGame"]["enabled"] = Settings::Radar::InGame::enabled;
	LoadColor(settings["Radar"]["enemy_color"], Settings::Radar::enemyColor);
	LoadColor(settings["Radar"]["enemy_visible_color"], Settings::Radar::enemyVisibleColor);
	LoadColor(settings["Radar"]["ally_color"], Settings::Radar::allyColor);
	LoadColor(settings["Radar"]["ally_visible_color"], Settings::Radar::allyVisibleColor);
	LoadColor(settings["Radar"]["t_color"], Settings::Radar::tColor);
	LoadColor(settings["Radar"]["t_visible_color"], Settings::Radar::tVisibleColor);
	LoadColor(settings["Radar"]["ct_color"], Settings::Radar::ctColor);
	LoadColor(settings["Radar"]["ct_visible_color"], Settings::Radar::ctVisibleColor);
	LoadColor(settings["Radar"]["bomb_color"], Settings::Radar::bombColor);
	LoadColor(settings["Radar"]["bomb_defusing_color"], Settings::Radar::bombDefusingColor);
	settings["Radar"]["icons_scale"] = Settings::Radar::iconsScale;

	settings["Recoilcrosshair"]["enabled"] = Settings::Recoilcrosshair::enabled;

	settings["Recoilcrosshair"]["showOnlyWhenShooting"] = Settings::Recoilcrosshair::showOnlyWhenShooting;

	settings["FOVChanger"]["enabled"] = Settings::FOVChanger::enabled;
	settings["FOVChanger"]["value"] = Settings::FOVChanger::value;
	settings["FOVChanger"]["viewmodel_enabled"] = Settings::FOVChanger::viewmodelEnabled;
	settings["FOVChanger"]["viewmodel_value"] = Settings::FOVChanger::viewmodelValue;
	settings["FOVChanger"]["ignore_scope"] = Settings::FOVChanger::ignoreScope;

	//steam_sdk inventory changer
	settings["InventoryChanger"]["inventorychanger"] = &Settings::InventoryChanger::enabled;
	settings["InventoryChanger"]["Medalchanger"] = &Settings::InventoryChanger::medalsEnabled;
	settings["InventoryChanger"]["rank"] = &Settings::Misc::misc_Rank;
	settings["InventoryChanger"]["level"] = &Settings::Misc::misc_Level;
	settings["InventoryChanger"]["xp"] = &Settings::Misc::misc_XP;
	settings["InventoryChanger"]["wins"] = &Settings::Misc::misc_Wins;
	settings["InventoryChanger"]["friendly"] = &Settings::Misc::misc_Friendly;
	settings["InventoryChanger"]["leader"] = &Settings::Misc::misc_Leader;
	settings["InventoryChanger"]["teacher"] = &Settings::Misc::misc_Teacher;
//	settings["InventoryChanger"]["medals"] = (int*)&Settings::InventoryChanger::medals;
	
	settings["Airstuck"]["enabled"] = Settings::Airstuck::enabled;
	settings["Airstuck"]["key"] = Util::GetButtonName(Settings::Airstuck::key);

	settings[XorStr("FakewalkAW")][XorStr("enabled")] = Settings::FakewalkAW::enabled;
	settings[XorStr("FakewalkAW")][XorStr("key")] = Util::GetButtonName(Settings::FakewalkAW::key);
	settings[XorStr("Fakewalk")][XorStr("enabled")] = Settings::Fakewalk::enabled;
	settings[XorStr("Fakewalk")][XorStr("key")] = Util::GetButtonName(Settings::Fakewalk::key);
	settings[XorStr("ChokePackets")][XorStr("enabled")] = Settings::ChokePackets::enabled;
	settings[XorStr("ChokePackets")][XorStr("key")] = Util::GetButtonName(Settings::ChokePackets::key);
	settings[XorStr("SlowMo")][XorStr("enabled")] = Settings::SlowMo::enabled;
	settings[XorStr("SlowMo")][XorStr("key")] = Util::GetButtonName(Settings::SlowMo::key);

	settings["SkinChanger"]["Skins"]["enabled"] = Settings::Skinchanger::Skins::enabled;
	settings["SkinChanger"]["Models"]["enabled"] = Settings::Skinchanger::Models::enabled;
	settings["SkinChanger"]["Skins"]["perTeam"] = Settings::Skinchanger::Skins::perTeam;

	for (const auto& item : Settings::Skinchanger::skinsCT)
	{
		const AttribItem_t& skin = item.second;

#define skinSetting settings["SkinChanger"]["skinsCT"][Util::Items::GetItemConfigEntityName(item.first)]
		skinSetting["ItemDefinitionIndex"] = Util::Items::GetItemConfigEntityName(skin.itemDefinitionIndex);
		skinSetting["PaintKit"] = skin.fallbackPaintKit;
		skinSetting["Wear"] = skin.fallbackWear;
		skinSetting["Seed"] = skin.fallbackSeed;
		skinSetting["StatTrak"] = skin.fallbackStatTrak;
		skinSetting["CustomName"] = skin.customName;
#undef skinSetting
	}

	for (const auto& item : Settings::Skinchanger::skinsT)
	{
		const AttribItem_t& skin = item.second;

#define skinSetting settings["SkinChanger"]["skinsT"][Util::Items::GetItemConfigEntityName(item.first)]
		skinSetting["ItemDefinitionIndex"] = Util::Items::GetItemConfigEntityName(skin.itemDefinitionIndex);
		skinSetting["PaintKit"] = skin.fallbackPaintKit;
		skinSetting["Wear"] = skin.fallbackWear;
		skinSetting["Seed"] = skin.fallbackSeed;
		skinSetting["StatTrak"] = skin.fallbackStatTrak;
		skinSetting["CustomName"] = skin.customName;
#undef skinSetting
	}

	settings["ShowRanks"]["enabled"] = Settings::ShowRanks::enabled;

	settings["ShowSpectators"]["enabled"] = Settings::ShowSpectators::enabled;

	settings[XorStr("ClanTagChanger")][XorStr("preset")] = (int)Settings::ClanTagChanger::preset;
	settings[XorStr("ClanTagChanger")][XorStr("value")] = Settings::ClanTagChanger::value;
	settings[XorStr("ClanTagChanger")][XorStr("enabled")] = Settings::ClanTagChanger::enabled;
	settings[XorStr("ClanTagChanger")][XorStr("animation")] = Settings::ClanTagChanger::animation;
	settings[XorStr("ClanTagChanger")][XorStr("animation_speed")] = Settings::ClanTagChanger::animationSpeed;
	settings[XorStr("ClanTagChanger")][XorStr("type")] = (int)Settings::ClanTagChanger::type;

	settings["View"]["NoViewPunch"]["enabled"] = Settings::View::NoViewPunch::enabled;
	settings["View"]["NoAimPunch"]["enabled"] = Settings::View::NoAimPunch::enabled;

//	settings["Teleport"]["enabled"] = Settings::Teleport::enabled;
//	settings["Teleport"]["key"] = Settings::Teleport::key;

	settings[XorStr("FakeLag")][XorStr("type")] = (int)Settings::FakeLag::type;
	settings[XorStr("FakeLag")][XorStr("value")] = Settings::FakeLag::value;
	settings[XorStr("spinFactor")][XorStr("value")] = Settings::spinFactor::value;
	settings[XorStr("customYaw")][XorStr("value")] = Settings::customYaw::value;
	settings[XorStr("customYaw2")][XorStr("value")] = Settings::customYaw2::value;
	settings[XorStr("customYawLby")][XorStr("enabled")] = Settings::customYaw::lby;
	settings[XorStr("customYaw2Lby")][XorStr("enabled")] = Settings::customYaw2::lby;

	settings["AutoAccept"]["enabled"] = Settings::AutoAccept::enabled;

	settings["NoSky"]["enabled"] = Settings::NoSky::enabled;
	LoadColor(settings["NoSky"]["color"], Settings::NoSky::color);

	settings[XorStr("SkyBox")][XorStr("enabled")] = Settings::SkyBox::enabled;
	settings[XorStr("SkyBox")][XorStr("skyBoxNumber")] = Settings::SkyBox::skyBoxNumber;

	settings["ASUSWalls"]["enabled"] = Settings::ASUSWalls::enabled;
	LoadColor(settings["ASUSWalls"]["color"], Settings::ASUSWalls::color);

	settings["NoScopeBorder"]["enabled"] = Settings::NoScopeBorder::enabled;

	settings["SniperCrosshair"]["enabled"] = Settings::SniperCrosshair::enabled;

	settings["Autoblock"]["enabled"] = Settings::Autoblock::enabled;
	settings["Autoblock"]["key"] = Settings::Autoblock::key;

	settings["AutoDefuse"]["enabled"] = Settings::AutoDefuse::enabled;
	settings["AutoDefuse"]["silent"] = Settings::AutoDefuse::silent;

	settings["NoSmoke"]["enabled"] = Settings::NoSmoke::enabled;

	settings["ScreenshotCleaner"]["enabled"] = Settings::ScreenshotCleaner::enabled;

	settings["EdgeJump"]["enabled"] = Settings::EdgeJump::enabled;
	settings["EdgeJump"]["key"] = Util::GetButtonName(Settings::EdgeJump::key);



	settings["NameStealer"]["enabled"] = Settings::NameStealer::enabled;
	settings["NameStealer"]["team"] = Settings::NameStealer::team;

	settings[XorStr("ThirdPerson")][XorStr("enabled")] = Settings::ThirdPerson::enabled;
	settings[XorStr("ThirdPerson")][XorStr("mode")] = (int)Settings::ThirdPerson::mode;
	settings[XorStr("ThirdPerson")][XorStr("distance")] = Settings::ThirdPerson::distance;
	settings[XorStr("ThirdPerson")][XorStr("key")] = Settings::ThirdPerson::key;

	settings[XorStr("JumpThrow")][XorStr("enabled")] = Settings::JumpThrow::enabled;
	settings[XorStr("JumpThrow")][XorStr("key")] = Util::GetButtonName(Settings::JumpThrow::key);

	settings[XorStr("DisablePostProcessing")][XorStr("enabled")] = Settings::DisablePostProcessing::enabled;

	settings[XorStr("GrenadeHelper")][XorStr("enabled")] = Settings::GrenadeHelper::enabled;
	settings[XorStr("GrenadeHelper")][XorStr("aimAssist")] = Settings::GrenadeHelper::aimAssist;
	settings[XorStr("GrenadeHelper")][XorStr("OnlyMatching")] = Settings::GrenadeHelper::onlyMatchingInfos;
	settings[XorStr("GrenadeHelper")][XorStr("aimStep")] = Settings::GrenadeHelper::aimStep;
	settings[XorStr("GrenadeHelper")][XorStr("aimDistance")] = Settings::GrenadeHelper::aimDistance;
	settings[XorStr("GrenadeHelper")][XorStr("aimFov")] = Settings::GrenadeHelper::aimFov;
	LoadColor(settings[XorStr("GrenadeHelper")][XorStr("aimDot")], Settings::GrenadeHelper::aimDot);
	LoadColor(settings[XorStr("GrenadeHelper")][XorStr("aimLine")], Settings::GrenadeHelper::aimLine);
	LoadColor(settings[XorStr("GrenadeHelper")][XorStr("infoHe")], Settings::GrenadeHelper::infoHE);
	LoadColor(settings[XorStr("GrenadeHelper")][XorStr("infoSmoke")], Settings::GrenadeHelper::infoSmoke);
	LoadColor(settings[XorStr("GrenadeHelper")][XorStr("infoMolotov")], Settings::GrenadeHelper::infoMolotov);
	LoadColor(settings[XorStr("GrenadeHelper")][XorStr("infoFlash")], Settings::GrenadeHelper::infoFlash);

	settings[XorStr("GrenadePrediction")][XorStr("enabled")] = Settings::GrenadePrediction::enabled;
	LoadColor(settings[XorStr("GrenadePrediction")][XorStr("color")], Settings::GrenadePrediction::color);

	settings[XorStr("AutoKnife")][XorStr("enabled")] = Settings::AutoKnife::enabled;
	settings[XorStr("AutoKnife")][XorStr("Filters")][XorStr("enemies")] = Settings::AutoKnife::Filters::enemies;
	settings[XorStr("AutoKnife")][XorStr("Filters")][XorStr("allies")] = Settings::AutoKnife::Filters::allies;
	settings[XorStr("AutoKnife")][XorStr("onKey")] = Settings::AutoKnife::onKey;

//	settings[XorStr("AntiAimInfos")][XorStr("enabled")] = Settings::AntiAimInfos::enabled;

	std::ofstream(path) << styledWriter.write(settings);
}
inline bool exists_test3(const std::string& name) {
	struct stat buffer;
	return (stat(name.c_str(), &buffer) == 0);
}
void Settings::LoadConfig(std::string path)
{
	if (!exists_test3(path))
	{
		//Settings::LoadDefaultsOrSave(path);
		return;
	}

	Json::Value settings;
	std::ifstream configDoc(path, std::ifstream::binary);
	configDoc >> settings;

	GetVal(settings[XorStr("UI")][XorStr("mainColor")], &Settings::UI::mainColor);
	GetVal(settings[XorStr("UI")][XorStr("bodyColor")], &Settings::UI::bodyColor);
	GetVal(settings[XorStr("UI")][XorStr("fontColor")], &Settings::UI::fontColor);
	GetVal(settings[XorStr("UI")][XorStr("Watermark")][XorStr("color")], &Settings::UI::Watermark::color);
	GetVal(settings[XorStr("UI")][XorStr("Watermark")][XorStr("displayIngame")],
		&Settings::UI::Watermark::displayIngame);
	//GetVal(settings["UI"]["Fonts"]["ESP"]["family"], &Settings::UI::Fonts::ESP::family);
	//GetVal(settings["UI"]["Fonts"]["ESP"]["size"], &Settings::UI::Fonts::ESP::size);
	//GetVal(settings["UI"]["Fonts"]["ESP"]["flags"], &Settings::UI::Fonts::ESP::flags);

	//Fonts::SetupFonts();
	//14 e 15

	Settings::Aimbot::weapons = {
		{ ItemDefinitionIndex::INVALID,{ false, false, false, false, false, false, false, 700, Bone::BONE_HEAD, ButtonCode_t::MOUSE_MIDDLE, false, false, 1.0f, SmoothType::SLOW_END, false, 0.0f, false, 0.0f, false, 0.0f, true, 180.0f, false, 25.0f, 35.0f, false, false, 2.0f, 2.0f, false, false, false, false, false, false, false, false, false, false, 0.1f, false, 10.0f, false, false, false, false, false, false , 1 , 1.0f, false, false, 1.0f } } };


	for (Json::ValueIterator itr = settings["Aimbot"]["weapons"].begin(); itr != settings["Aimbot"]["weapons"].end(); itr++)
	{
		std::string weaponDataKey = itr.key().asString();
		auto weaponSetting = settings["Aimbot"]["weapons"][weaponDataKey];

		// XXX Using exception handling to deal with this is stupid, but I don't care to find a better solution
		// XXX We can't use GetOrdinal() since the key type is a string...
		ItemDefinitionIndex weaponID;
		try
		{
			weaponID = (ItemDefinitionIndex)std::stoi(weaponDataKey);
		}
		catch (std::invalid_argument) // Not a number
		{
			weaponID = Util::Items::GetItemIndex(weaponDataKey);
		}

		if (Settings::Aimbot::weapons.find(weaponID) == Settings::Aimbot::weapons.end())
			Settings::Aimbot::weapons[weaponID] = AimbotWeapon_t();


		
		AimbotWeapon_t weapon = {
			weaponSetting["Enabled"].asBool(),
			weaponSetting["Silent"].asBool(),
			weaponSetting[XorStr("pSilent")].asBool(),
			weaponSetting["Friendly"].asBool(),
			weaponSetting[XorStr("ClosestBone")].asBool(),
			weaponSetting[XorStr("engageLock")].asBool(),
			weaponSetting[XorStr("engageLockTR")].asBool(),
			weaponSetting[XorStr("engageLockTTR")].asInt(),
			(Bone)weaponSetting["TargetBone"].asInt(),
			Util::GetButtonCode(weaponSetting["AimKey"].asCString()),
			weaponSetting["AimKeyOnly"].asBool(),
			weaponSetting["Smooth"]["Enabled"].asBool(),
			weaponSetting["Smooth"]["Amount"].asFloat(),
			(SmoothType)weaponSetting["Smooth"]["Type"].asInt(),
			weaponSetting["Smooth"]["Salting"]["Enabled"].asBool(),
			weaponSetting["Smooth"]["Salting"]["Multiplier"].asFloat(),
			weaponSetting["ErrorMargin"]["Enabled"].asBool(),
			weaponSetting["ErrorMargin"]["Value"].asFloat(),
			weaponSetting["Curve"]["Enabled"].asBool(),
			weaponSetting["Curve"]["Amount"].asFloat(),
			weaponSetting["AutoAim"]["Enabled"].asBool(),
			weaponSetting["AutoAim"]["FOV"].asFloat(),
			weaponSetting[XorStr("AimStep")][XorStr("Enabled")].asBool(),
			weaponSetting[XorStr("AimStep")][XorStr("min")].asFloat(),
			weaponSetting[XorStr("AimStep")][XorStr("max")].asFloat(),
			weaponSetting["RCS"]["Enabled"].asBool(),
			weaponSetting["RCS"]["AlwaysOn"].asBool(),
			weaponSetting["RCS"]["AmountX"].asFloat(),
			weaponSetting["RCS"]["AmountY"].asFloat(),
			weaponSetting["AutoCockRevolver"]["Enabled"].asBool(),
			weaponSetting["AutoPistol"]["Enabled"].asBool(),
			weaponSetting["AutoShoot"]["Enabled"].asBool(),
			weaponSetting["AutoScope"]["Enabled"].asBool(),
			weaponSetting["NoShoot"]["Enabled"].asBool(),
			weaponSetting["IgnoreJump"]["Enabled"].asBool(),
			weaponSetting["SmokeCheck"]["Enabled"].asBool(),
			weaponSetting["FlashCheck"]["Enabled"].asBool(),
			weaponSetting["SpreadLimit"]["Enabled"].asBool(),
			weaponSetting["SpreadLimit"]["DistanceBased"].asBool(),
			weaponSetting["SpreadLimit"]["Value"].asFloat(),
			weaponSetting["AutoWall"]["Enabled"].asBool(),
			weaponSetting["AutoWall"]["Value"].asFloat(),
			weaponSetting["AutoAim"]["RealDistance"].asBool(),
			weaponSetting["AutoSlow"]["enabled"].asBool(),
			weaponSetting["moveMouse"].asBool(),
			weaponSetting["Prediction"]["enabled"].asBool(),
			weaponSetting[XorStr("BackTrack")].asBool(),
			weaponSetting["HitChance"]["enabled"].asBool(),
			weaponSetting["HitChance"]["hitRays"].asInt(),
			weaponSetting["HitChance"]["value"].asFloat(),
			weaponSetting["Velocitycheck"]["enabled"].asBool(),
			weaponSetting[XorStr("RCS")][XorStr("smooth")].asBool(),
			weaponSetting[XorStr("RCS")][XorStr("smoothvalue")].asFloat()

			//autoWallValue, autoAimRealDistance, autoSlow, moveMouse, predEnabled, backtrack, hitChanceEnabled, hitChanceRays, hitChanceValue
		};

		for (int bone = (int)DesiredBones::BONE_PELVIS; bone <= (int)DesiredBones::BONE_RIGHT_SOLE; bone++)
			weapon.desiredBones[bone] = weaponSetting["DesiredBones"]["Bones"][bone].asBool();
		Settings::Aimbot::weapons.at(weaponID) = weapon;
	}

	GetVal(settings["Aimbot"]["AutoCrouch"]["enabled"], &Settings::Aimbot::AutoCrouch::enabled);
	GetVal(settings["Aimbot"]["AutoShoot"]["velocityCheck"], &Settings::Aimbot::AutoShoot::velocityCheck);

	GetVal(settings[XorStr("Resolver")][XorStr("enabled")], &Settings::Resolver::enabled);
	GetVal(settings[XorStr("Resolver")][XorStr("ticks")], &Settings::Resolver::ticks);
	GetVal(settings[XorStr("Resolver")][XorStr("modulo")], &Settings::Resolver::modulo);
	GetVal(settings[XorStr("Resolver")][XorStr("pitch")], &Settings::Resolver::pitch);
	GetVal(settings[XorStr("Resolver")][XorStr("LagComp")], &Settings::Resolver::LagComp);
	GetVal(settings[XorStr("Resolver")][XorStr("angleFlipEnabled")], &Settings::Resolver::angleFlipEnabled);
	GetButtonCode(settings[XorStr("Resolver")][XorStr("angleFlip")], &Settings::Resolver::angleFlip);
	GetVal(settings[XorStr("Resolver")][XorStr("baimAfter")], &Settings::Resolver::baimAfter);
	GetVal(settings[XorStr("Resolver")]["lbyOnly"], &Settings::Resolver::lbyOnly);
	GetVal(settings[XorStr("AngleIndicator")][XorStr("enabled")], &Settings::AngleIndicator::enabled);
	GetVal(settings[XorStr("AngleIndicator")][XorStr("Veloc")], &Settings::AngleIndicator::Veloc);
	GetVal(settings[XorStr("EventLogger")][XorStr("enabled")], &Settings::EventLogger::enabled);

	GetVal(settings["Triggerbot"]["enabled"], &Settings::Triggerbot::enabled);
	GetButtonCode(settings["Triggerbot"]["key"], &Settings::Triggerbot::key);
	GetVal(settings["Triggerbot"]["Filters"]["enemies"], &Settings::Triggerbot::Filters::enemies);
	GetVal(settings["Triggerbot"]["Filters"]["allies"], &Settings::Triggerbot::Filters::allies);
	GetVal(settings["Triggerbot"]["Filters"]["walls"], &Settings::Triggerbot::Filters::walls);
	GetVal(settings["Triggerbot"]["Filters"]["smoke_check"], &Settings::Triggerbot::Filters::smokeCheck);
	GetVal(settings["Triggerbot"]["Filters"]["flash_check"], &Settings::Triggerbot::Filters::flashCheck);
	GetVal(settings["Triggerbot"]["Filters"]["head"], &Settings::Triggerbot::Filters::head);
	GetVal(settings["Triggerbot"]["Filters"]["chest"], &Settings::Triggerbot::Filters::chest);
	GetVal(settings["Triggerbot"]["Filters"]["stomach"], &Settings::Triggerbot::Filters::stomach);
	GetVal(settings["Triggerbot"]["Filters"]["arms"], &Settings::Triggerbot::Filters::arms);
	GetVal(settings["Triggerbot"]["Filters"]["legs"], &Settings::Triggerbot::Filters::legs);
	GetVal(settings[XorStr("Triggerbot")][XorStr("RandomDelay")][XorStr("enabled")], &Settings::Triggerbot::RandomDelay::enabled);
	GetVal(settings[XorStr("Triggerbot")][XorStr("RandomDelay")][XorStr("lowBound")], &Settings::Triggerbot::RandomDelay::lowBound);
	GetVal(settings[XorStr("Triggerbot")][XorStr("RandomDelay")][XorStr("highBound")], &Settings::Triggerbot::RandomDelay::highBound);

	GetVal(settings[XorStr("AntiAim")][XorStr("Standing")][XorStr("Yaw")][XorStr("enabled")],
		&Settings::AntiAim::Standing::Yaw::enabled);
	GetVal(settings[XorStr("AntiAim")][XorStr("Standing")][XorStr("Yaw")][XorStr("type")],
		(int*)&Settings::AntiAim::Standing::Yaw::type);
	GetVal(settings[XorStr("AntiAim")][XorStr("Standing")][XorStr("Yaw")][XorStr("typeFake")],
		(int*)&Settings::AntiAim::Standing::Yaw::typeFake);
	GetVal(settings[XorStr("AntiAim")][XorStr("Standing")][XorStr("Yaw")][XorStr("typeAdd")],
		&Settings::AntiAim::Standing::Yaw::typeAdd);
	GetVal(settings[XorStr("AntiAim")][XorStr("Standing")][XorStr("Yaw")][XorStr("typeFakeAdd")],
		&Settings::AntiAim::Standing::Yaw::typeFakeAdd);
	GetVal(settings[XorStr("AntiAim")][XorStr("Standing")][XorStr("Pitch")][XorStr("enabled")],
		&Settings::AntiAim::Standing::Pitch::enabled);
	GetVal(settings[XorStr("AntiAim")][XorStr("Standing")][XorStr("Pitch")][XorStr("type")],
		(int*)&Settings::AntiAim::Standing::Pitch::type);
	GetVal(settings[XorStr("AntiAim")][XorStr("Standing")][XorStr("Pitch")][XorStr("custom")],
		&Settings::AntiAim::Standing::Pitch::custom);
	GetVal(settings[XorStr("AntiAim")][XorStr("Standing")][XorStr("Roll")][XorStr("enabled")],
		&Settings::AntiAim::Standing::Roll::enabled);
	GetVal(settings[XorStr("AntiAim")][XorStr("Standing")][XorStr("Roll")][XorStr("type")],
		(int*)&Settings::AntiAim::Standing::Roll::type);
	GetVal(settings[XorStr("AntiAim")][XorStr("Standing")][XorStr("SwitchAA")][XorStr("enabled")],
		&Settings::AntiAim::Standing::SwitchAA::enabled);
	GetButtonCode(settings[XorStr("AntiAim")][XorStr("Standing")][XorStr("SwitchAA")][XorStr("key")],
		&Settings::AntiAim::Standing::SwitchAA::key);
	GetVal(settings[XorStr("AntiAim")][XorStr("Standing")][XorStr("LBY")][XorStr("enabled")],
		&Settings::AntiAim::Standing::LBY::enabled);
	GetVal(settings[XorStr("AntiAim")][XorStr("Standing")][XorStr("LBY")][XorStr("type")],
		(int*)&Settings::AntiAim::Standing::LBY::type);
	GetVal(settings[XorStr("AntiAim")][XorStr("Standing")][XorStr("HeadEdge")][XorStr("enabled")],
		&Settings::AntiAim::Standing::HeadEdge::enabled);
	GetVal(settings[XorStr("AntiAim")][XorStr("Standing")][XorStr("HeadEdge")][XorStr("distance")],
		&Settings::AntiAim::Standing::HeadEdge::distance);
	GetVal(settings[XorStr("AntiAim")][XorStr("Standing")][XorStr("HeadEdge")][XorStr("realAdd")],
		&Settings::AntiAim::Standing::HeadEdge::realAdd);
	GetVal(settings[XorStr("AntiAim")][XorStr("Standing")][XorStr("HeadEdge")][XorStr("fakeAdd")],
		&Settings::AntiAim::Standing::HeadEdge::fakeAdd);
	GetVal(settings[XorStr("AntiAim")][XorStr("Standing")][XorStr("antiResolver")],
		&Settings::AntiAim::Standing::antiResolver);
	GetVal(settings[XorStr("AntiAim")][XorStr("Standing")][XorStr("dynamicAA")],
		&Settings::AntiAim::Standing::dynamicAA);
	GetVal(settings[XorStr("AntiAim")][XorStr("Standing")][XorStr("untrustedAngles")],
		&Settings::AntiAim::Standing::untrustedAngles);

	GetVal(settings[XorStr("AntiAim")][XorStr("Moving")][XorStr("Yaw")][XorStr("enabled")],
		&Settings::AntiAim::Moving::Yaw::enabled);
	GetVal(settings[XorStr("AntiAim")][XorStr("Moving")][XorStr("Yaw")][XorStr("type")],
		(int*)&Settings::AntiAim::Moving::Yaw::type);
	GetVal(settings[XorStr("AntiAim")][XorStr("Moving")][XorStr("Yaw")][XorStr("typeFake")],
		(int*)&Settings::AntiAim::Moving::Yaw::typeFake);
	GetVal(settings[XorStr("AntiAim")][XorStr("Moving")][XorStr("Yaw")][XorStr("typeAdd")],
		&Settings::AntiAim::Moving::Yaw::typeAdd);
	GetVal(settings[XorStr("AntiAim")][XorStr("Moving")][XorStr("Yaw")][XorStr("typeFakeAdd")],
		&Settings::AntiAim::Moving::Yaw::typeFakeAdd);
	GetVal(settings[XorStr("AntiAim")][XorStr("Moving")][XorStr("Pitch")][XorStr("enabled")],
		&Settings::AntiAim::Moving::Pitch::enabled);
	GetVal(settings[XorStr("AntiAim")][XorStr("Moving")][XorStr("Pitch")][XorStr("type")],
		(int*)&Settings::AntiAim::Moving::Pitch::type);
	GetVal(settings[XorStr("AntiAim")][XorStr("Moving")][XorStr("Pitch")][XorStr("custom")],
		&Settings::AntiAim::Moving::Pitch::custom);
	GetVal(settings[XorStr("AntiAim")][XorStr("Moving")][XorStr("Roll")][XorStr("enabled")],
		&Settings::AntiAim::Moving::Roll::enabled);
	GetVal(settings[XorStr("AntiAim")][XorStr("Moving")][XorStr("Roll")][XorStr("type")],
		(int*)&Settings::AntiAim::Moving::Roll::type);
	GetVal(settings[XorStr("AntiAim")][XorStr("Moving")][XorStr("SwitchAA")][XorStr("enabled")],
		&Settings::AntiAim::Moving::SwitchAA::enabled);
	GetButtonCode(settings[XorStr("AntiAim")][XorStr("Moving")][XorStr("SwitchAA")][XorStr("key")],
		&Settings::AntiAim::Moving::SwitchAA::key);
	GetVal(settings[XorStr("AntiAim")][XorStr("Moving")][XorStr("LBY")][XorStr("enabled")],
		&Settings::AntiAim::Moving::LBY::enabled);
	GetVal(settings[XorStr("AntiAim")][XorStr("Moving")][XorStr("LBY")][XorStr("type")],
		(int*)&Settings::AntiAim::Moving::LBY::type);
	GetVal(settings[XorStr("AntiAim")][XorStr("Moving")][XorStr("HeadEdge")][XorStr("enabled")],
		&Settings::AntiAim::Moving::HeadEdge::enabled);
	GetVal(settings[XorStr("AntiAim")][XorStr("Moving")][XorStr("HeadEdge")][XorStr("distance")],
		&Settings::AntiAim::Moving::HeadEdge::distance);
	GetVal(settings[XorStr("AntiAim")][XorStr("Moving")][XorStr("HeadEdge")][XorStr("realAdd")],
		&Settings::AntiAim::Moving::HeadEdge::realAdd);
	GetVal(settings[XorStr("AntiAim")][XorStr("Moving")][XorStr("HeadEdge")][XorStr("fakeAdd")],
		&Settings::AntiAim::Moving::HeadEdge::fakeAdd);
	GetVal(settings[XorStr("AntiAim")][XorStr("Moving")][XorStr("antiResolver")],
		&Settings::AntiAim::Moving::antiResolver);
	GetVal(settings[XorStr("AntiAim")][XorStr("Moving")][XorStr("dynamicAA")],
		&Settings::AntiAim::Moving::dynamicAA);
	GetVal(settings[XorStr("AntiAim")][XorStr("Moving")][XorStr("untrustedAngles")],
		&Settings::AntiAim::Moving::untrustedAngles);

	GetVal(settings[XorStr("AntiAim")][XorStr("Airborne")][XorStr("Yaw")][XorStr("enabled")],
		&Settings::AntiAim::Airborne::Yaw::enabled);
	GetVal(settings[XorStr("AntiAim")][XorStr("Airborne")][XorStr("Yaw")][XorStr("type")],
		(int*)&Settings::AntiAim::Airborne::Yaw::type);
	GetVal(settings[XorStr("AntiAim")][XorStr("Airborne")][XorStr("Yaw")][XorStr("typeFake")],
		(int*)&Settings::AntiAim::Airborne::Yaw::typeFake);
	GetVal(settings[XorStr("AntiAim")][XorStr("Airborne")][XorStr("Yaw")][XorStr("typeAdd")],
		&Settings::AntiAim::Airborne::Yaw::typeAdd);
	GetVal(settings[XorStr("AntiAim")][XorStr("Airborne")][XorStr("Yaw")][XorStr("typeFakeAdd")],
		&Settings::AntiAim::Airborne::Yaw::typeFakeAdd);
	GetVal(settings[XorStr("AntiAim")][XorStr("Airborne")][XorStr("Pitch")][XorStr("enabled")],
		&Settings::AntiAim::Airborne::Pitch::enabled);
	GetVal(settings[XorStr("AntiAim")][XorStr("Airborne")][XorStr("Pitch")][XorStr("type")],
		(int*)&Settings::AntiAim::Airborne::Pitch::type);
	GetVal(settings[XorStr("AntiAim")][XorStr("Airborne")][XorStr("Pitch")][XorStr("custom")],
		&Settings::AntiAim::Airborne::Pitch::custom);
	GetVal(settings[XorStr("AntiAim")][XorStr("Airborne")][XorStr("Roll")][XorStr("enabled")],
		&Settings::AntiAim::Airborne::Roll::enabled);
	GetVal(settings[XorStr("AntiAim")][XorStr("Airborne")][XorStr("Roll")][XorStr("type")],
		(int*)&Settings::AntiAim::Airborne::Roll::type);
	GetVal(settings[XorStr("AntiAim")][XorStr("Airborne")][XorStr("SwitchAA")][XorStr("enabled")],
		&Settings::AntiAim::Airborne::SwitchAA::enabled);
	GetButtonCode(settings[XorStr("AntiAim")][XorStr("Airborne")][XorStr("SwitchAA")][XorStr("key")],
		&Settings::AntiAim::Airborne::SwitchAA::key);
	GetVal(settings[XorStr("AntiAim")][XorStr("Airborne")][XorStr("LBY")][XorStr("enabled")],
		&Settings::AntiAim::Airborne::LBY::enabled);
	GetVal(settings[XorStr("AntiAim")][XorStr("Airborne")][XorStr("LBY")][XorStr("type")],
		(int*)&Settings::AntiAim::Airborne::LBY::type);
	GetVal(settings[XorStr("AntiAim")][XorStr("Airborne")][XorStr("HeadEdge")][XorStr("enabled")],
		&Settings::AntiAim::Airborne::HeadEdge::enabled);
	GetVal(settings[XorStr("AntiAim")][XorStr("Airborne")][XorStr("HeadEdge")][XorStr("distance")],
		&Settings::AntiAim::Airborne::HeadEdge::distance);
	GetVal(settings[XorStr("AntiAim")][XorStr("Airborne")][XorStr("HeadEdge")][XorStr("fakeAdd")],
		&Settings::AntiAim::Airborne::HeadEdge::fakeAdd);
	GetVal(settings[XorStr("AntiAim")][XorStr("Airborne")][XorStr("HeadEdge")][XorStr("realAdd")],
		&Settings::AntiAim::Airborne::HeadEdge::realAdd);
	GetVal(settings[XorStr("AntiAim")][XorStr("Airborne")][XorStr("antiResolver")],
		&Settings::AntiAim::Airborne::antiResolver);
	GetVal(settings[XorStr("AntiAim")][XorStr("Airborne")][XorStr("dynamicAA")],
		&Settings::AntiAim::Airborne::dynamicAA);
	GetVal(settings[XorStr("AntiAim")][XorStr("Airborne")][XorStr("untrustedAngles")],
		&Settings::AntiAim::Airborne::untrustedAngles);

	GetVal(settings[XorStr("AntiAim")][XorStr("Misc")][XorStr("AutoDisable")][XorStr("noEnemy")],
		&Settings::AntiAim::Misc::AutoDisable::noEnemy);
	GetVal(settings[XorStr("AntiAim")][XorStr("Misc")][XorStr("AutoDisable")][XorStr("knifeHeld")],
		&Settings::AntiAim::Misc::AutoDisable::knifeHeld);
	GetVal(settings[XorStr("AntiAim")][XorStr("Misc")][XorStr("AutoDisable")][XorStr("bombHeld")],
		&Settings::AntiAim::Misc::AutoDisable::bombHeld);
	GetVal(settings[XorStr("AntiAim")][XorStr("Misc")][XorStr("AutoDisable")][XorStr("freezeTime")],
		&Settings::AntiAim::Misc::AutoDisable::freezeTime);

	GetVal(settings[XorStr("MoonWalk")][XorStr("enabled")], &Settings::MoonWalk::enabled);
	GetVal(settings[XorStr("ESP")][XorStr("enabled")], &Settings::ESP::enabled);
	GetButtonCode(settings[XorStr("ESP")][XorStr("key")], &Settings::ESP::key);
	GetVal(settings["ESP"]["enemy_color"], &Settings::ESP::enemyColor);
	GetVal(settings["ESP"]["enemy_visible_color"], &Settings::ESP::enemyVisibleColor);
	GetVal(settings["ESP"]["ally_color"], &Settings::ESP::allyColor);
	GetVal(settings["ESP"]["ally_visible_color"], &Settings::ESP::allyVisibleColor);
	GetVal(settings["ESP"]["t_color"], &Settings::ESP::tColor);
	GetVal(settings["ESP"]["t_visible_color"], &Settings::ESP::tVisibleColor);
	GetVal(settings["ESP"]["ct_color"], &Settings::ESP::ctColor);
	GetVal(settings["ESP"]["ct_visible_color"], &Settings::ESP::ctVisibleColor);
	GetVal(settings["ESP"]["localplayer_color"], &Settings::ESP::localplayerColor);
	GetVal(settings["ESP"]["bomb_color"], &Settings::ESP::bombColor);
	GetVal(settings["ESP"]["bomb_defusing_color"], &Settings::ESP::bombDefusingColor);
	GetVal(settings["ESP"]["hostage_color"], &Settings::ESP::hostageColor);
	GetVal(settings["ESP"]["defuser_color"], &Settings::ESP::defuserColor);
	GetVal(settings["ESP"]["weapon_color"], &Settings::ESP::weaponColor);
	GetVal(settings["ESP"]["chicken_color"], &Settings::ESP::chickenColor);
	GetVal(settings["ESP"]["fish_color"], &Settings::ESP::fishColor);
	GetVal(settings["ESP"]["smoke_color"], &Settings::ESP::smokeColor);
	GetVal(settings["ESP"]["decoy_color"], &Settings::ESP::decoyColor);
	GetVal(settings["ESP"]["flashbang_color"], &Settings::ESP::flashbangColor);
	GetVal(settings["ESP"]["grenade_color"], &Settings::ESP::grenadeColor);
	GetVal(settings["ESP"]["molotov_color"], &Settings::ESP::molotovColor);
	GetVal(settings["ESP"]["Glow"]["enabled"], &Settings::ESP::Glow::enabled);
	GetVal(settings["ESP"]["Glow"]["ally_color"], &Settings::ESP::Glow::allyColor);
	GetVal(settings["ESP"]["Glow"]["enemy_color"], &Settings::ESP::Glow::enemyColor);
	GetVal(settings["ESP"]["Glow"]["enemy_visible_color"], &Settings::ESP::Glow::enemyVisibleColor);
	GetVal(settings["ESP"]["Glow"]["localplayer_color"], &Settings::ESP::Glow::localplayerColor);
	GetVal(settings["ESP"]["Glow"]["weapon_color"], &Settings::ESP::Glow::weaponColor);
	GetVal(settings["ESP"]["Glow"]["grenade_color"], &Settings::ESP::Glow::grenadeColor);
	GetVal(settings["ESP"]["Glow"]["defuser_color"], &Settings::ESP::Glow::defuserColor);
	GetVal(settings["ESP"]["Glow"]["chicken_color"], &Settings::ESP::Glow::chickenColor);
	GetVal(settings["ESP"]["Filters"]["legit"], &Settings::ESP::Filters::legit);
	GetVal(settings["ESP"]["Filters"]["visibility_check"], &Settings::ESP::Filters::visibilityCheck);
	GetVal(settings["ESP"]["Filters"]["smoke_check"], &Settings::ESP::Filters::smokeCheck);
	GetVal(settings["ESP"]["Filters"]["enemies"], &Settings::ESP::Filters::enemies);
	GetVal(settings["ESP"]["Filters"]["allies"], &Settings::ESP::Filters::allies);
	GetVal(settings["ESP"]["Filters"]["bomb"], &Settings::ESP::Filters::bomb);
	GetVal(settings["ESP"]["Filters"]["hostages"], &Settings::ESP::Filters::hostages);
	GetVal(settings["ESP"]["Filters"]["defusers"], &Settings::ESP::Filters::defusers);
	GetVal(settings["ESP"]["Filters"]["weapons"], &Settings::ESP::Filters::weapons);
	GetVal(settings["ESP"]["Filters"]["chickens"], &Settings::ESP::Filters::chickens);
	GetVal(settings["ESP"]["Filters"]["fishes"], &Settings::ESP::Filters::fishes);
	GetVal(settings["ESP"]["Filters"]["throwables"], &Settings::ESP::Filters::throwables);
	GetVal(settings["ESP"]["Filters"]["localplayer"], &Settings::ESP::Filters::localplayer);
	GetVal(settings["ESP"]["Info"]["name"], &Settings::ESP::Info::name);
	GetVal(settings["ESP"]["Info"]["clan"], &Settings::ESP::Info::clan);
	GetVal(settings["ESP"]["Info"]["steam_id"], &Settings::ESP::Info::steamId);
	GetVal(settings["ESP"]["Info"]["rank"], &Settings::ESP::Info::rank);
	GetVal(settings["ESP"]["Info"]["health"], &Settings::ESP::Info::health);
	GetVal(settings["ESP"]["Info"]["weapon"], &Settings::ESP::Info::weapon);
	GetVal(settings["ESP"]["Info"]["scoped"], &Settings::ESP::Info::scoped);
	GetVal(settings["ESP"]["Info"]["reloading"], &Settings::ESP::Info::reloading);
	GetVal(settings["ESP"]["Info"]["flashed"], &Settings::ESP::Info::flashed);
	GetVal(settings["ESP"]["Info"]["planting"], &Settings::ESP::Info::planting);
	GetVal(settings["ESP"]["Info"]["has_defuser"], &Settings::ESP::Info::hasDefuser);
	GetVal(settings["ESP"]["Info"]["defusing"], &Settings::ESP::Info::defusing);
	GetVal(settings["ESP"]["Info"]["grabbing_hostage"], &Settings::ESP::Info::grabbingHostage);
	GetVal(settings["ESP"]["Info"]["rescuing"], &Settings::ESP::Info::rescuing);
	GetVal(settings["ESP"]["Info"]["location"], &Settings::ESP::Info::location);
	GetVal(settings[XorStr("ESP")][XorStr("Info")][XorStr("money")], &Settings::ESP::Info::money);
	GetVal(settings[XorStr("ESP")][XorStr("Info")][XorStr("lby")], &Settings::ESP::Info::lby);
	GetVal(settings[XorStr("ESP")][XorStr("Info")][XorStr("resolveInfo")], &Settings::ESP::Info::resolveInfo);
	GetVal(settings["ESP"]["Boxes"]["enabled"], &Settings::ESP::Boxes::enabled);
	GetVal(settings["ESP"]["Boxes"]["type"], (int*)& Settings::ESP::Boxes::type);
	GetVal(settings["ESP"]["Skeleton"]["enabled"], &Settings::ESP::Skeleton::enabled);
	GetVal(settings["ESP"]["Skeleton"]["color"], &Settings::ESP::Skeleton::color);
	GetVal(settings["ESP"]["Bars"]["enabled"], &Settings::ESP::Bars::enabled);
	GetVal(settings["ESP"]["Bars"]["color_type"], (int*)& Settings::ESP::Bars::colorType);
	GetVal(settings["ESP"]["Bars"]["type"], (int*)& Settings::ESP::Bars::type);
	GetVal(settings["ESP"]["Tracers"]["enabled"], &Settings::ESP::Tracers::enabled);
	GetVal(settings["ESP"]["Tracers"]["type"], (int*)& Settings::ESP::Tracers::type);
	GetVal(settings["ESP"]["BulletTracers"]["enabled"], &Settings::ESP::BulletTracers::enabled);
	GetVal(settings["ESP"]["FOVCrosshair"]["enabled"], &Settings::ESP::FOVCrosshair::enabled);
	GetVal(settings["ESP"]["FOVCrosshair"]["filled"], &Settings::ESP::FOVCrosshair::filled);
	GetVal(settings["ESP"]["FOVCrosshair"]["color"], &Settings::ESP::FOVCrosshair::color);
	GetVal(settings["ESP"]["Chams"]["Arms"]["enabled"], &Settings::ESP::Chams::Arms::enabled);
	GetVal(settings["ESP"]["Chams"]["Arms"]["type"], (int*)& Settings::ESP::Chams::Arms::type);
	GetVal(settings["ESP"]["Chams"]["Arms"]["color"], &Settings::ESP::Chams::Arms::color);
	GetVal(settings["ESP"]["Chams"]["Weapon"]["enabled"], &Settings::ESP::Chams::Weapon::enabled);
	GetVal(settings["ESP"]["Chams"]["Weapon"]["color"], &Settings::ESP::Chams::Weapon::color);
	GetVal(settings["ESP"]["Chams"]["players_ally_color"], &Settings::ESP::Chams::allyColor);
	GetVal(settings["ESP"]["Chams"]["players_ally_visible_color"], &Settings::ESP::Chams::allyVisibleColor);
	GetVal(settings["ESP"]["Chams"]["players_enemy_color"], &Settings::ESP::Chams::enemyColor);
	GetVal(settings["ESP"]["Chams"]["players_enemy_visible_color"], &Settings::ESP::Chams::enemyVisibleColor);
	GetVal(settings["ESP"]["Chams"]["localplayer_color"], &Settings::ESP::Chams::localplayerColor);
	GetVal(settings["ESP"]["Chams"]["type"], (int*)& Settings::ESP::Chams::type);
	GetVal(settings["ESP"]["Chams"]["enabled"], &Settings::ESP::Chams::enabled);
	GetVal(settings["ESP"]["Sounds"]["enabled"], &Settings::ESP::Sounds::enabled);
	GetVal(settings["ESP"]["Sounds"]["time"], &Settings::ESP::Sounds::time);
	GetVal(settings["ESP"]["Hitmarker"]["enabled"], &Settings::ESP::Hitmarker::enabled);
	GetVal(settings["ESP"]["Hitmarker"]["enemies"], &Settings::ESP::Hitmarker::enemies);
	GetVal(settings["ESP"]["Hitmarker"]["allies"], &Settings::ESP::Hitmarker::allies);
	GetVal(settings["ESP"]["Hitmarker"]["color"], &Settings::ESP::Hitmarker::color);
	GetVal(settings["ESP"]["Hitmarker"]["duration"], &Settings::ESP::Hitmarker::duration);
	GetVal(settings["ESP"]["Hitmarker"]["size"], &Settings::ESP::Hitmarker::size);
	GetVal(settings["ESP"]["Hitmarker"]["inner_gap"], &Settings::ESP::Hitmarker::innerGap);
	GetVal(settings["ESP"]["Hitmarker"]["Damage"]["enabled"], &Settings::ESP::Hitmarker::Damage::enabled);
	GetVal(settings["ESP"]["HeadDot"]["enabled"], &Settings::ESP::HeadDot::enabled);
	GetVal(settings["ESP"]["HeadDot"]["size"], &Settings::ESP::HeadDot::size);
	GetVal(settings[XorStr("ESP")][XorStr("Spread")][XorStr("enabled")], &Settings::ESP::Spread::enabled);
	GetVal(settings[XorStr("ESP")][XorStr("Spread")][XorStr("spreadLimit")], &Settings::ESP::Spread::spreadLimit);
	GetVal(settings[XorStr("ESP")][XorStr("Spread")][XorStr("color")], &Settings::ESP::Spread::color);
	GetVal(settings[XorStr("ESP")][XorStr("Spread")][XorStr("spreadLimitColor")], &Settings::ESP::Spread::spreadLimitColor);

	GetVal(settings[XorStr("LBYIndicator")][XorStr("enabled")], &Settings::lbyindicator::enabled);
	GetVal(settings[XorStr("lbyindicator")][XorStr("enabled")], &Settings::lbyindicator::enabled);


	GetVal(settings["Dlights"]["enabled"], &Settings::Dlights::enabled);
	GetVal(settings["Dlights"]["radius"], &Settings::Dlights::radius);

	GetVal(settings["Spammer"]["spammer_type"], (int*)& Settings::Spammer::type);
	GetVal(settings["Spammer"]["say_team"], &Settings::Spammer::say_team);
	GetVal(settings["Spammer"]["KillSpammer"]["enabled"], &Settings::Spammer::KillSpammer::enabled);
	GetVal(settings["Spammer"]["KillSpammer"]["say_team"], &Settings::Spammer::KillSpammer::sayTeam);
	if (!settings["Spammer"]["KillSpammer"]["messages"].isNull())
	{
		Settings::Spammer::KillSpammer::messages.clear();
		for (const Json::Value& message : settings[XorStr("Spammer")][XorStr("KillSpammer")][XorStr("messages")])
			Settings::Spammer::KillSpammer::messages.push_back(message.asString());
	}
	if (!settings["Spammer"]["NormalSpammer"]["messages"].isNull())
	{
		Settings::Spammer::NormalSpammer::messages.clear();
		for (const Json::Value& message : settings["Spammer"]["NormalSpammer"]["messages"])
			Settings::Spammer::NormalSpammer::messages.push_back(message.asString());
	}
	GetVal(settings["Spammer"]["PositionSpammer"]["show_name"], &Settings::Spammer::PositionSpammer::showName);
	GetVal(settings["Spammer"]["PositionSpammer"]["show_weapon"], &Settings::Spammer::PositionSpammer::showWeapon);
	GetVal(settings["Spammer"]["PositionSpammer"]["show_rank"], &Settings::Spammer::PositionSpammer::showRank);
	GetVal(settings["Spammer"]["PositionSpammer"]["show_wins"], &Settings::Spammer::PositionSpammer::showWins);
	GetVal(settings["Spammer"]["PositionSpammer"]["show_health"], &Settings::Spammer::PositionSpammer::showHealth);
	GetVal(settings["Spammer"]["PositionSpammer"]["show_money"], &Settings::Spammer::PositionSpammer::showMoney);
	GetVal(settings["Spammer"]["PositionSpammer"]["show_lastplace"], &Settings::Spammer::PositionSpammer::showLastplace);

	GetVal(settings["BHop"]["enabled"], &Settings::BHop::enabled);
	GetVal(settings[XorStr("BHop")][XorStr("Chance")][XorStr("enabled")], &Settings::BHop::Chance::enabled);
	GetVal(settings[XorStr("BHop")][XorStr("Chance")][XorStr("value")], &Settings::BHop::Chance::value);
	GetVal(settings[XorStr("BHop")][XorStr("Hops")][XorStr("enabledMax")], &Settings::BHop::Hops::enabledMax);
	GetVal(settings[XorStr("BHop")][XorStr("Hops")][XorStr("Max")], &Settings::BHop::Hops::Max);
	GetVal(settings[XorStr("BHop")][XorStr("Hops")][XorStr("enabledMin")], &Settings::BHop::Hops::enabledMin);
	GetVal(settings[XorStr("BHop")][XorStr("Hops")][XorStr("Min")], &Settings::BHop::Hops::Min);

	GetVal(settings["AutoStrafe"]["enabled"], &Settings::AutoStrafe::enabled);
	GetVal(settings["AutoStrafe"]["type"], (int*)& Settings::AutoStrafe::type);
	GetVal(settings["AutoStrafe"]["silent"], &Settings::AutoStrafe::silent);

	GetVal(settings["Noflash"]["enabled"], &Settings::Noflash::enabled);
	GetVal(settings["Noflash"]["value"], &Settings::Noflash::value);

	GetVal(settings["Radar"]["enabled"], &Settings::Radar::enabled);
	GetVal(settings["Radar"]["zoom"], &Settings::Radar::zoom);
	GetVal(settings["Radar"]["enemies"], &Settings::Radar::enemies);
	GetVal(settings["Radar"]["allies"], &Settings::Radar::allies);
	GetVal(settings["Radar"]["legit"], &Settings::Radar::legit);
	GetVal(settings["Radar"]["visibility_check"], &Settings::Radar::visibilityCheck);
	GetVal(settings["Radar"]["smoke_check"], &Settings::Radar::smokeCheck);
	GetVal(settings["Radar"]["InGame"]["enabled"], &Settings::Radar::InGame::enabled);
	GetVal(settings["Radar"]["enemy_color"], &Settings::Radar::enemyColor);
	GetVal(settings["Radar"]["enemy_visible_color"], &Settings::Radar::enemyVisibleColor);
	GetVal(settings["Radar"]["ally_color"], &Settings::Radar::allyColor);
	GetVal(settings["Radar"]["ally_visible_color"], &Settings::Radar::allyVisibleColor);
	GetVal(settings["Radar"]["t_color"], &Settings::Radar::tColor);
	GetVal(settings["Radar"]["t_visible_color"], &Settings::Radar::tVisibleColor);
	GetVal(settings["Radar"]["ct_color"], &Settings::Radar::ctColor);
	GetVal(settings["Radar"]["ct_visible_color"], &Settings::Radar::ctVisibleColor);
	GetVal(settings["Radar"]["bomb_color"], &Settings::Radar::bombColor);
	GetVal(settings["Radar"]["bomb_defusing_color"], &Settings::Radar::bombDefusingColor);
	GetVal(settings["Radar"]["icons_scale"], &Settings::Radar::iconsScale);


	GetVal(settings["Recoilcrosshair"]["enabled"], &Settings::Recoilcrosshair::enabled);
	GetVal(settings["Recoilcrosshair"]["showOnlyWhenShooting"], &Settings::Recoilcrosshair::showOnlyWhenShooting);

	GetVal(settings["FOVChanger"]["enabled"], &Settings::FOVChanger::enabled);
	GetVal(settings["FOVChanger"]["value"], &Settings::FOVChanger::value);
	GetVal(settings["FOVChanger"]["viewmodel_enabled"], &Settings::FOVChanger::viewmodelEnabled);
	GetVal(settings["FOVChanger"]["viewmodel_value"], &Settings::FOVChanger::viewmodelValue);
	GetVal(settings["FOVChanger"]["ignore_scope"], &Settings::FOVChanger::ignoreScope);


	//steam_sdk inventory changer
	GetVal(settings["InventoryChanger"]["inventorychanger"], &Settings::InventoryChanger::enabled);
	GetVal(settings["InventoryChanger"]["Medalchanger"], &Settings::InventoryChanger::medalsEnabled);
	GetVal(settings["InventoryChanger"]["rank"], &Settings::Misc::misc_Rank);
	GetVal(settings["InventoryChanger"]["level"], &Settings::Misc::misc_Level);
	GetVal(settings["InventoryChanger"]["xp"], &Settings::Misc::misc_XP);
	GetVal(settings["InventoryChanger"]["wins"], &Settings::Misc::misc_Wins);
	GetVal(settings["InventoryChanger"]["friendly"], &Settings::Misc::misc_Friendly);
	GetVal(settings["InventoryChanger"]["leader"], &Settings::Misc::misc_Leader);
	GetVal(settings["InventoryChanger"]["teacher"], &Settings::Misc::misc_Teacher);
//	GetVal(settings["InventoryChanger"]["medals"], (int*)&Settings::InventoryChanger::medals);

	GetVal(settings["Airstuck"]["enabled"], &Settings::Airstuck::enabled);
	GetButtonCode(settings["Airstuck"]["key"], &Settings::Airstuck::key);
	GetVal(settings[XorStr("FakewalkAW")][XorStr("enabled")], &Settings::FakewalkAW::enabled);
	GetButtonCode(settings[XorStr("FakewalkAW")][XorStr("key")], &Settings::FakewalkAW::key);
	GetVal(settings[XorStr("Fakewalk")][XorStr("enabled")], &Settings::Fakewalk::enabled);
	GetButtonCode(settings[XorStr("Fakewalk")][XorStr("key")], &Settings::Fakewalk::key);
	GetVal(settings[XorStr("ChokePackets")][XorStr("enabled")], &Settings::ChokePackets::enabled);
	GetButtonCode(settings[XorStr("ChokePackets")][XorStr("key")], &Settings::ChokePackets::key);
	GetVal(settings[XorStr("SlowMo")][XorStr("enabled")], &Settings::SlowMo::enabled);
	GetButtonCode(settings[XorStr("SlowMo")][XorStr("key")], &Settings::SlowMo::key);


	Settings::Skinchanger::Skins::enabled = false;
	Settings::Skinchanger::skinsCT.clear();
	Settings::Skinchanger::skinsT.clear();

	for (Json::ValueIterator itr = settings["SkinChanger"]["skinsCT"].begin(); itr != settings["SkinChanger"]["skinsCT"].end(); itr++)
	{
		std::string skinDataKey = itr.key().asString();
		auto skinSetting = settings["SkinChanger"]["skinsCT"][skinDataKey];

		// XXX Using exception handling to deal with this is stupid, but I don't care to find a better solution
		// XXX We can't use GetOrdinal() since the key type is a string...
		unsigned int weaponID;

		try
		{
			weaponID = std::stoi(skinDataKey);
		}
		catch (std::invalid_argument)
		{
			weaponID = (int)Util::Items::GetItemIndex(skinDataKey);
		}

		ItemDefinitionIndex defIndex;
		GetOrdinal<ItemDefinitionIndex, Util::Items::GetItemIndex>(skinSetting["ItemDefinitionIndex"], &defIndex);

		if (Settings::Skinchanger::skinsCT.find((ItemDefinitionIndex)weaponID) == Settings::Skinchanger::skinsCT.end())
			Settings::Skinchanger::skinsCT[(ItemDefinitionIndex)weaponID] = AttribItem_t();

		AttribItem_t skin = {
			defIndex,
			skinSetting["PaintKit"].asInt(),
			skinSetting["Wear"].asFloat(),
			skinSetting["Seed"].asInt(),
			skinSetting["StatTrak"].asInt(),
			-1,
			skinSetting["CustomName"].asString(),
		};

		Settings::Skinchanger::skinsCT.at((ItemDefinitionIndex)weaponID) = skin;
	}

	for (Json::ValueIterator itr = settings["SkinChanger"]["skinsT"].begin(); itr != settings["SkinChanger"]["skinsT"].end(); itr++)
	{
		std::string skinDataKey = itr.key().asString();
		auto skinSetting = settings["SkinChanger"]["skinsT"][skinDataKey];

		// XXX Using exception handling to deal with this is stupid, but I don't care to find a better solution
		// XXX We can't use GetOrdinal() since the key type is a string...
		unsigned int weaponID;

		try
		{
			weaponID = std::stoi(skinDataKey);
		}
		catch (std::invalid_argument)
		{
			weaponID = (int)Util::Items::GetItemIndex(skinDataKey);
		}

		ItemDefinitionIndex defIndex;
		GetOrdinal<ItemDefinitionIndex, Util::Items::GetItemIndex>(skinSetting["ItemDefinitionIndex"], &defIndex);

		if (Settings::Skinchanger::skinsT.find((ItemDefinitionIndex)weaponID) == Settings::Skinchanger::skinsT.end())
			Settings::Skinchanger::skinsT[(ItemDefinitionIndex)weaponID] = AttribItem_t();

		AttribItem_t skin = {
			defIndex,
			skinSetting["PaintKit"].asInt(),
			skinSetting["Wear"].asFloat(),
			skinSetting["Seed"].asInt(),
			skinSetting["StatTrak"].asInt(),
			-1,
			skinSetting["CustomName"].asString(),
		};

		Settings::Skinchanger::skinsT.at((ItemDefinitionIndex)weaponID) = skin;
	}

	SkinChanger::forceFullUpdate = true;

	GetVal(settings["SkinChanger"]["Skins"]["enabled"], &Settings::Skinchanger::Skins::enabled);
	GetVal(settings["SkinChanger"]["Models"]["enabled"], &Settings::Skinchanger::Models::enabled);
	GetVal(settings["SkinChanger"]["Skins"]["perTeam"], &Settings::Skinchanger::Skins::perTeam);

	GetVal(settings[XorStr("UI")][XorStr("Windows")][XorStr("Config")][XorStr("posX")],
		&Settings::UI::Windows::Config::posX);
	GetVal(settings[XorStr("UI")][XorStr("Windows")][XorStr("Config")][XorStr("posY")],
		&Settings::UI::Windows::Config::posY);
	GetVal(settings[XorStr("UI")][XorStr("Windows")][XorStr("Config")][XorStr("sizeX")],
		&Settings::UI::Windows::Config::sizeX);
	GetVal(settings[XorStr("UI")][XorStr("Windows")][XorStr("Config")][XorStr("sizeY")],
		&Settings::UI::Windows::Config::sizeY);
	GetVal(settings[XorStr("UI")][XorStr("Windows")][XorStr("Config")][XorStr("open")],
		&Settings::UI::Windows::Config::open);


	Settings::UI::Windows::Config::reload = true;

	GetVal(settings["ShowRanks"]["enabled"], &Settings::ShowRanks::enabled);

	GetVal(settings["ShowSpectators"]["enabled"], &Settings::ShowSpectators::enabled);

	GetVal(settings["ClanTagChanger"]["value"], (char *)& Settings::ClanTagChanger::value);
	GetVal(settings["ClanTagChanger"]["enabled"], &Settings::ClanTagChanger::enabled);
	GetVal(settings["ClanTagChanger"]["animation"], &Settings::ClanTagChanger::animation);
	GetVal(settings["ClanTagChanger"]["animation_speed"], &Settings::ClanTagChanger::animationSpeed);
	GetVal(settings["ClanTagChanger"]["type"], (int*)& Settings::ClanTagChanger::type);
	//::ClanTagChanger::UpdateClanTagCallback();

	GetVal(settings["View"]["NoViewPunch"]["enabled"], &Settings::View::NoViewPunch::enabled);
	GetVal(settings["View"]["NoAimPunch"]["enabled"], &Settings::View::NoAimPunch::enabled);

//	GetVal(settings["Teleport"]["enabled"], &Settings::Teleport::enabled);
//	GetButtonCode(settings["Teleport"]["key"], &Settings::Teleport::key);

	GetVal(settings[XorStr("FakeLag")][XorStr("type")], (int*)&Settings::FakeLag::type);
	GetVal(settings[XorStr("FakeLag")][XorStr("value")], &Settings::FakeLag::value);
	GetVal(settings[XorStr("spinFactor")][XorStr("value")], &Settings::spinFactor::value);

	GetVal(settings["AutoAccept"]["enabled"], &Settings::AutoAccept::enabled);

	GetVal(settings["NoSky"]["enabled"], &Settings::NoSky::enabled);
	GetVal(settings["NoSky"]["color"], &Settings::NoSky::color);

	GetVal(settings[XorStr("SkyBox")][XorStr("enabled")], &Settings::SkyBox::enabled);
	GetVal(settings[XorStr("SkyBox")][XorStr("skyBoxNumber")], &Settings::SkyBox::skyBoxNumber);

	GetVal(settings["ASUSWalls"]["enabled"], &Settings::ASUSWalls::enabled);
	GetVal(settings["ASUSWalls"]["color"], &Settings::ASUSWalls::color);

	GetVal(settings["NoScopeBorder"]["enabled"], &Settings::NoScopeBorder::enabled);

	GetVal(settings["SniperCrosshair"]["enabled"], &Settings::SniperCrosshair::enabled);

	GetVal(settings[XorStr("Autoblock")][XorStr("enabled")], &Settings::Autoblock::enabled);
	GetButtonCode(settings[XorStr("Autoblock")][XorStr("key")], &Settings::Autoblock::key);

	GetVal(settings["AutoDefuse"]["enabled"], &Settings::AutoDefuse::enabled);
	GetVal(settings["AutoDefuse"]["silent"], &Settings::AutoDefuse::silent);

	GetVal(settings["NoSmoke"]["enabled"], &Settings::NoSmoke::enabled);

	GetVal(settings["ScreenshotCleaner"]["enabled"], &Settings::ScreenshotCleaner::enabled);


	GetVal(settings["EdgeJump"]["enabled"], &Settings::EdgeJump::enabled);
	GetButtonCode(settings["EdgeJump"]["key"], &Settings::EdgeJump::key);

	GetVal(settings["NameStealer"]["enabled"], &Settings::NameStealer::enabled);
	GetVal(settings["NameStealer"]["team"], &Settings::NameStealer::team);

	GetVal(settings[XorStr("ThirdPerson")][XorStr("enabled")], &Settings::ThirdPerson::enabled);
	GetVal(settings[XorStr("ThirdPerson")][XorStr("mode")], (int*)&Settings::ThirdPerson::mode);
	GetVal(settings[XorStr("ThirdPerson")][XorStr("distance")], &Settings::ThirdPerson::distance);
	GetButtonCode(settings[XorStr("ThirdPerson")][XorStr("key")], &Settings::ThirdPerson::key);
	GetVal(settings[XorStr("ThirdPerson")][XorStr("transparency")], &Settings::ThirdPerson::transparency);

	GetVal(settings[XorStr("DisablePostProcessing")][XorStr("enabled")], &Settings::DisablePostProcessing::enabled);

	GetVal(settings[XorStr("GrenadeHelper")][XorStr("enabled")], &Settings::GrenadeHelper::enabled);
	GetVal(settings[XorStr("GrenadeHelper")][XorStr("aimAssist")], &Settings::GrenadeHelper::aimAssist);
	GetVal(settings[XorStr("GrenadeHelper")][XorStr("OnlyMatching")],
		&Settings::GrenadeHelper::onlyMatchingInfos);
	GetVal(settings[XorStr("GrenadeHelper")][XorStr("aimStep")], &Settings::GrenadeHelper::aimStep);
	GetVal(settings[XorStr("GrenadeHelper")][XorStr("aimDistance")], &Settings::GrenadeHelper::aimDistance);
	GetVal(settings[XorStr("GrenadeHelper")][XorStr("aimFov")], &Settings::GrenadeHelper::aimFov);

	GetVal(settings[XorStr("GrenadeHelper")][XorStr("aimDot")], &Settings::GrenadeHelper::aimDot);
	GetVal(settings[XorStr("GrenadeHelper")][XorStr("aimLine")], &Settings::GrenadeHelper::aimLine);
	GetVal(settings[XorStr("GrenadeHelper")][XorStr("infoHE")], &Settings::GrenadeHelper::infoHE);
	GetVal(settings[XorStr("GrenadeHelper")][XorStr("infoSmoke")], &Settings::GrenadeHelper::infoSmoke);
	GetVal(settings[XorStr("GrenadeHelper")][XorStr("infoFlash")], &Settings::GrenadeHelper::infoFlash);
	GetVal(settings[XorStr("GrenadeHelper")][XorStr("infoMolotov")], &Settings::GrenadeHelper::infoMolotov);

	GetVal(settings[XorStr("GrenadePrediction")][XorStr("enabled")], &Settings::GrenadePrediction::enabled);
	GetVal(settings[XorStr("GrenadePrediction")][XorStr("color")], &Settings::GrenadePrediction::color);

	GetVal(settings[XorStr("AutoKnife")][XorStr("enabled")], &Settings::AutoKnife::enabled);
	GetVal(settings[XorStr("AutoKnife")][XorStr("Filters")][XorStr("enemies")],
		&Settings::AutoKnife::Filters::enemies);
	GetVal(settings[XorStr("AutoKnife")][XorStr("Filters")][XorStr("allies")],
		&Settings::AutoKnife::Filters::allies);
	GetVal(settings[XorStr("AutoKnife")][XorStr("onKey")], &Settings::AutoKnife::onKey);

//	GetVal(settings[XorStr("AntiAimInfos")][XorStr("enabled")], &Settings::AntiAimInfos::enabled);
}
#include <Shlobj.h>



char paths[255];
void Settings::LoadSettings()
{
	//static CHAR path[MAX_PATH];
	strcat(paths, "C:\\Xhook\\");
	std::string folder, file;

	//if (SUCCEEDED(SHGetFolderPathA(NULL, CSIDL_APPDATA, NULL, 0, path)))
	//{
		folder = std::string(paths) + "\\Configs\\";

	//}

	CreateDirectoryA(folder.c_str(), NULL);
}


void Settings::DeleteConfig(std::string path)
{
	//remove_directory(path.c_str());
}


void Settings::SaveGrenadeInfo(std::string path) {
	Json::Value grenadeInfos;
	for (auto grenadeInfo = GrenadeHelper::grenadeInfos.begin();
		grenadeInfo != GrenadeHelper::grenadeInfos.end(); grenadeInfo++) {
		Json::Value act;
		act[XorStr("name")] = grenadeInfo->name.c_str();
		act[XorStr("gType")] = grenadeInfo->gType;
		act[XorStr("tType")] = grenadeInfo->tType;
		act[XorStr("pos")][XorStr("x")] = grenadeInfo->pos.x;
		act[XorStr("pos")][XorStr("y")] = grenadeInfo->pos.y;
		act[XorStr("pos")][XorStr("z")] = grenadeInfo->pos.z;

		act[XorStr("angle")][XorStr("x")] = grenadeInfo->angle.x;
		act[XorStr("angle")][XorStr("y")] = grenadeInfo->angle.y;

		grenadeInfos.append(act);
	}

	Json::Value data;
	Json::StyledWriter styledWriter;

	data[XorStr("smokeinfos")] = grenadeInfos;

	std::ofstream(path) << styledWriter.write(data);
}

	void Settings::LoadGrenadeInfo(std::string path) {
		/*if (!std::ifstream(path).good() || !DoesFileExist(path.c_str()))
			return;*/
		Json::Value data;
		std::ifstream configDoc(path, std::ifstream::binary);
		try {
			configDoc >> data;
		}
		catch (...) {
			pCvar->ConsoleDPrintf(XorStr("Error parsing the config file.\n"));
			return;
		}

		Json::Value array = data[XorStr("smokeinfos")];
		Settings::GrenadeHelper::grenadeInfos = {};

		for (Json::Value::iterator it = array.begin(); it != array.end(); ++it) {
			Json::Value act = *it;
			const char* name = act[XorStr("name")].asCString();
			GrenadeType gType = (GrenadeType)act[XorStr("gType")].asInt();
			ThrowType tType = (ThrowType)act[XorStr("tType")].asInt();
			Json::Value pos = act[XorStr("pos")];
			Vector posVec = Vector(pos[XorStr("x")].asFloat(), pos[XorStr("y")].asFloat(),
				pos[XorStr("z")].asFloat());
			Json::Value angle = act[XorStr("angle")];
			Vector vAngle = Vector(angle[XorStr("x")].asFloat(), angle[XorStr("y")].asFloat(), 0);
			Settings::GrenadeHelper::grenadeInfos.push_back(GrenadeInfo(gType, posVec, vAngle, tType, std::string(name)));
	}
}