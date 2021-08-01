// ImGui - standalone example application for DirectX 9
// If you are new to ImGui, see examples/README.txt and documentation at the top of imgui.cpp.

#include <imgui.h>
#include "imgui_impl_dx9.h"
#include <d3d9.h>
#define DIRECTINPUT_VERSION 0x0800
#include <dinput.h>
#include <tchar.h>
#include "Settings.h"
#include "XorStr.hpp"
#define IM_ARRAYSIZE(_ARR)      ((int)(sizeof(_ARR)/sizeof(*_ARR)))



//Settings...
//--AIMBOT--//
bool Settings::Aimbot::AutoSlow::goingToSlow;
bool Settings::Aimbot::enabled = false;
bool Settings::Aimbot::pSilent = false;
bool Settings::Aimbot::silent = false;
bool Settings::Aimbot::friendly = false;
//Bone Settings::Aimbot::bone = Bone::BONE_HEAD;
//ButtonCode_t Settings::Aimbot::aimkey = ButtonCode_t::MOUSE_MIDDLE;
bool Settings::Aimbot::aimkeyOnly = false;
bool Settings::Aimbot::Smooth::enabled = false;
float Settings::Aimbot::Smooth::value = 0.5f;
SmoothType Settings::Aimbot::Smooth::type = SmoothType::SLOW_END;
bool Settings::Aimbot::ErrorMargin::enabled = false;
float Settings::Aimbot::ErrorMargin::value = 0.0f;
//curve
bool Settings::Aimbot::Curve::enabled = false;
float Settings::Aimbot::Curve::value = 0.5f;
bool Settings::Aimbot::AutoAim::enabled = false;
float Settings::Aimbot::AutoAim::fov = 180.0f;
bool Settings::Aimbot::AutoAim::realDistance = false;
bool Settings::SmartAim::enabled = false;
bool Settings::Aimbot::AutoAim::closestBone = false;
bool Settings::Aimbot::AutoAim::desiredBones[] =
{
	true, true, true, true, true, true, true, // center mass
	false, false, false, false, false, false, false, // left arm
	false, false, false, false, false, false, false, // right arm
	false, false, false, false, false, // left leg
	false, false, false, false, false  // right leg
};
bool Settings::Aimbot::AutoAim::engageLock = false;
bool Settings::Aimbot::AutoAim::engageLockTR = false; // engage lock Target Reacquisition ( re-target after getting a kill when spraying ).
int Settings::Aimbot::AutoAim::engageLockTTR = 700; // Time to Target Reacquisition in ms


bool Settings::Aimbot::SpreadLimit::enabled = false;
bool Settings::Aimbot::SpreadLimit::distanceBased = false;
float Settings::Aimbot::SpreadLimit::value = 0.1f;
bool Settings::Aimbot::AutoWall::enabled = false;
float Settings::Aimbot::AutoWall::value = 10.0f;
bool Settings::Aimbot::AutoWall::bones[] = { true, false, false, false, false, false };
bool Settings::Aimbot::AimStep::enabled = false;
float Settings::Aimbot::AimStep::min = 25.0f;
float Settings::Aimbot::AimStep::max = 35.0f;
bool Settings::Aimbot::AutoPistol::enabled = false;
bool Settings::Aimbot::AutoShoot::enabled = false;
bool Settings::Aimbot::AutoShoot::velocityCheck = false;
bool Settings::Aimbot::AutoShoot::autoscope = false;
bool Settings::Aimbot::RCS::enabled = false;
bool Settings::Aimbot::RCS::always_on = false;
float Settings::Aimbot::RCS::valueX = 2.0f;
float Settings::Aimbot::RCS::valueY = 2.0f;
bool Settings::Aimbot::RCS::smooth = false;
float Settings::Aimbot::RCS::smoothvalue = 1.0f;

bool Settings::Aimbot::AutoCrouch::enabled = false;
bool Settings::Aimbot::NoShoot::enabled = false;
bool Settings::Aimbot::IgnoreJump::enabled = false;
bool Settings::Aimbot::SmokeCheck::enabled = false;
bool Settings::Aimbot::FlashCheck::enabled = false;
bool Settings::Aimbot::Smooth::Salting::enabled = false;
float Settings::Aimbot::Smooth::Salting::multiplier = 0.0f;
bool Settings::Aimbot::AutoSlow::enabled = false;
bool Settings::Aimbot::Prediction::enabled = false;
bool Settings::Aimbot::AutoCockRevolver::enabled = false;
bool Settings::Aimbot::AutoWall::AutoWallType = false;
bool Settings::Aimbot::HitChance::enabled = false;
int Settings::Aimbot::HitChance::hitRays = 100;
float Settings::Aimbot::HitChance::value = 0.5f;
bool Settings::Aimbot::moveMouse = false;

bool Settings::Aimbot::backtrack = false;
bool Settings::Aimbot::legitMode = false;

//bool Aimbot::aimStepInProgress = false;
int Settings::Aimbot::AutoAim::aftershots = 0;
//std::vector<int64_t> Aimbot::friends = {};
//std::vector<int64_t> Aimbot::baim = {};
std::vector<long> killTimes = { 0 }; // the Epoch time from when we kill someone
float autoCockDifference = 0.0f;

int missedShots = 0;
bool sentShotToTarget;

bool shouldAim;
//Vector AimStepLastAngle;
//Vector RCSLastPunch;
//
//int Aimbot::targetAimbot = -1;
////bool Aimbot::useAbTarget = false;

const int HEAD_VECTORS = 11;

//bool Aimbot::shootingRevolver = false;


//--ANTIAIM--//
bool Settings::AntiAim::Misc::LegitAA = false;
#ifndef REGION_ANTIAIM_STANDING_SETTINGS
bool Settings::AntiAim::Standing::Yaw::enabled = false;
AntiAimType_Y Settings::AntiAim::Standing::Yaw::type = AntiAimType_Y::NOAA;
AntiAimType_Y Settings::AntiAim::Standing::Yaw::typeFake = AntiAimType_Y::NOAA;
float Settings::AntiAim::Standing::Yaw::typeAdd = 0.0f;
float Settings::AntiAim::Standing::Yaw::typeFakeAdd = 0.0f;

bool Settings::AntiAim::Standing::Pitch::enabled = false;
AntiAimType_X Settings::AntiAim::Standing::Pitch::type = AntiAimType_X::STATIC_DOWN;
float Settings::AntiAim::Standing::Pitch::custom = 0.0f;

bool Settings::AntiAim::Standing::Roll::enabled = false;
AntiAimType_Z Settings::AntiAim::Standing::Roll::type = AntiAimType_Z::TEST;

bool Settings::AntiAim::Standing::SwitchAA::enabled = false;
//ButtonCode_t Settings::AntiAim::Standing::SwitchAA::key = ButtonCode_t::KEY_COMMA;

bool Settings::AntiAim::Standing::LBY::enabled = false;
AntiAimType_LBY Settings::AntiAim::Standing::LBY::type = AntiAimType_LBY::ONE;

bool Settings::AntiAim::Standing::HeadEdge::enabled = false;
float Settings::AntiAim::Standing::HeadEdge::distance = 25.0f;
float Settings::AntiAim::Standing::HeadEdge::fakeAdd = 0.f;
float Settings::AntiAim::Standing::HeadEdge::realAdd = 0.f;

bool Settings::AntiAim::Standing::antiResolver = false;
bool Settings::AntiAim::Standing::dynamicAA = false;
bool Settings::AntiAim::Standing::untrustedAngles = false;
#endif
#ifndef REGION_ANTIAIM_MOVING_SETTINGS
bool Settings::AntiAim::Moving::Yaw::enabled = false;
AntiAimType_Y Settings::AntiAim::Moving::Yaw::type = AntiAimType_Y::NOAA;
AntiAimType_Y Settings::AntiAim::Moving::Yaw::typeFake = AntiAimType_Y::NOAA;
float Settings::AntiAim::Moving::Yaw::typeAdd = 0.0f;
float Settings::AntiAim::Moving::Yaw::typeFakeAdd = 0.0f;

bool Settings::AntiAim::Moving::Pitch::enabled = false;
AntiAimType_X Settings::AntiAim::Moving::Pitch::type = AntiAimType_X::STATIC_DOWN;
float Settings::AntiAim::Moving::Pitch::custom = 0.0f;

bool Settings::AntiAim::Moving::Roll::enabled = false;
AntiAimType_Z Settings::AntiAim::Moving::Roll::type = AntiAimType_Z::TEST;

bool Settings::AntiAim::Moving::SwitchAA::enabled = false;
//ButtonCode_t Settings::AntiAim::Moving::SwitchAA::key = ButtonCode_t::KEY_COMMA;

bool Settings::AntiAim::Moving::LBY::enabled = false;
AntiAimType_LBY Settings::AntiAim::Moving::LBY::type = AntiAimType_LBY::ONE;

bool Settings::AntiAim::Moving::HeadEdge::enabled = false;
float Settings::AntiAim::Moving::HeadEdge::distance = 25.0f;
float Settings::AntiAim::Moving::HeadEdge::fakeAdd = 0.f;
float Settings::AntiAim::Moving::HeadEdge::realAdd = 0.f;

bool Settings::AntiAim::Moving::antiResolver = false;
bool Settings::AntiAim::Moving::dynamicAA = false;
bool Settings::AntiAim::Moving::untrustedAngles = false;
#endif
#ifndef REGION_ANTIAIM_AIRBORNE_SETTINGS
bool Settings::AntiAim::Airborne::Yaw::enabled = false;
AntiAimType_Y Settings::AntiAim::Airborne::Yaw::type = AntiAimType_Y::NOAA;
AntiAimType_Y Settings::AntiAim::Airborne::Yaw::typeFake = AntiAimType_Y::NOAA;
float Settings::AntiAim::Airborne::Yaw::typeAdd = 0.0f;
float Settings::AntiAim::Airborne::Yaw::typeFakeAdd = 0.0f;

bool Settings::AntiAim::Airborne::Pitch::enabled = false;
AntiAimType_X Settings::AntiAim::Airborne::Pitch::type = AntiAimType_X::STATIC_DOWN;
float Settings::AntiAim::Airborne::Pitch::custom = 0.0f;

bool Settings::AntiAim::Airborne::Roll::enabled = false;
AntiAimType_Z Settings::AntiAim::Airborne::Roll::type = AntiAimType_Z::TEST;

bool Settings::AntiAim::Airborne::SwitchAA::enabled = false;
//ButtonCode_t Settings::AntiAim::Airborne::SwitchAA::key = ButtonCode_t::KEY_COMMA;

bool Settings::AntiAim::Airborne::LBY::enabled = false;
AntiAimType_LBY Settings::AntiAim::Airborne::LBY::type = AntiAimType_LBY::ONE;

bool Settings::AntiAim::Airborne::HeadEdge::enabled = false;
float Settings::AntiAim::Airborne::HeadEdge::distance = 25.0f;
float Settings::AntiAim::Airborne::HeadEdge::fakeAdd = 0.f;
float Settings::AntiAim::Airborne::HeadEdge::realAdd = 0.f;

bool Settings::AntiAim::Airborne::antiResolver = false;
bool Settings::AntiAim::Airborne::dynamicAA = false;
bool Settings::AntiAim::Airborne::untrustedAngles = false;
#endif
#ifndef REGION_ANTIAIM_MISC_SETTINGS
bool Settings::AntiAim::Misc::AutoDisable::noEnemy = false;
bool Settings::AntiAim::Misc::AutoDisable::knifeHeld = true;
bool Settings::AntiAim::Misc::AutoDisable::bombHeld = false;
bool Settings::AntiAim::Misc::AutoDisable::freezeTime = true;
#endif
//float AntiAim::lastRealYaw = 0.0f;
//float AntiAim::lastFakeYaw = 0.0f;
//bool AntiAim::isAntiAiming = false;
//long lastPress = 0;
//
//bool AntiAim::canEdge = false;
//bool AntiAim::fakeTp = false;




//--ESP--//
bool Settings::ESP::enabled = false;
//ButtonCode_t Settings::ESP::key = ButtonCode_t::KEY_Z;
TeamColorType Settings::ESP::teamColorType = TeamColorType::RELATIVE_;
//HealthColorVar Settings::ESP::enemyColor = ImColor(240, 60, 60, 255);
//HealthColorVar Settings::ESP::enemyVisibleColor = ImColor(240, 185, 60, 255);
//HealthColorVar Settings::ESP::allyColor = ImColor(60, 60, 240, 255);
//HealthColorVar Settings::ESP::allyVisibleColor = ImColor(60, 185, 240, 255);
//HealthColorVar Settings::ESP::tColor = ImColor(240, 60, 60, 255);
//HealthColorVar Settings::ESP::tVisibleColor = ImColor(240, 185, 60, 255);
//HealthColorVar Settings::ESP::ctColor = ImColor(60, 60, 240, 255);
//HealthColorVar Settings::ESP::ctVisibleColor = ImColor(60, 185, 240, 255);
//HealthColorVar Settings::ESP::localplayerColor = ImColor(117, 43, 73, 255);
//ColorVar Settings::ESP::bombColor = ImColor(242, 48, 193, 255);
//ColorVar Settings::ESP::bombDefusingColor = ImColor(100, 48, 242, 255);
//ColorVar Settings::ESP::hostageColor = ImColor(242, 48, 193, 255);
//ColorVar Settings::ESP::defuserColor = ImColor(132, 48, 242, 255);
//ColorVar Settings::ESP::weaponColor = ImColor(255, 255, 255, 255);
//ColorVar Settings::ESP::chickenColor = ImColor(255, 255, 255, 255);
//ColorVar Settings::ESP::fishColor = ImColor(255, 255, 255, 255);
//ColorVar Settings::ESP::smokeColor = ImColor(32, 224, 22, 255);
//ColorVar Settings::ESP::decoyColor = ImColor(32, 224, 22, 255);
//ColorVar Settings::ESP::flashbangColor = ImColor(224, 207, 22, 255);
//ColorVar Settings::ESP::grenadeColor = ImColor(224, 22, 22, 255);
//ColorVar Settings::ESP::molotovColor = ImColor(224, 22, 22, 255);
//ColorVar Settings::ESP::Skeleton::color = ImColor(255, 255, 255, 255);
//ColorVar Settings::ESP::Spread::color = ImColor(15, 200, 45, 255);
//ColorVar Settings::ESP::Spread::spreadLimitColor = ImColor(20, 5, 150, 255);
//bool Settings::ESP::Glow::enabled = false;
//HealthColorVar Settings::ESP::Glow::allyColor = ImColor(0, 50, 200, 200);
//HealthColorVar Settings::ESP::Glow::enemyColor = ImColor(200, 0, 50, 200);
//HealthColorVar Settings::ESP::Glow::enemyVisibleColor = ImColor(200, 200, 50, 200);
//HealthColorVar Settings::ESP::Glow::localplayerColor = ImColor(117, 43, 73, 200);
//ColorVar Settings::ESP::Glow::weaponColor = ImColor(200, 0, 50, 200);
//ColorVar Settings::ESP::Glow::grenadeColor = ImColor(200, 0, 50, 200);
//ColorVar Settings::ESP::Glow::defuserColor = ImColor(100, 100, 200, 200);
//ColorVar Settings::ESP::Glow::chickenColor = ImColor(100, 200, 100, 200);
bool Settings::ESP::Filters::legit = false;
bool Settings::ESP::Filters::visibilityCheck = false;
bool Settings::ESP::Filters::smokeCheck = false;
bool Settings::ESP::Filters::enemies = false;
bool Settings::ESP::Filters::allies = false;
bool Settings::ESP::Filters::bomb = false;
bool Settings::ESP::Filters::hostages = false;
bool Settings::ESP::Filters::defusers = false;
bool Settings::ESP::Filters::weapons = false;
bool Settings::ESP::Filters::chickens = false;
bool Settings::ESP::Filters::fishes = false;
bool Settings::ESP::Filters::throwables = false;
bool Settings::ESP::Filters::localplayer = false;
bool Settings::ESP::Info::name = false;
bool Settings::ESP::Info::clan = false;
bool Settings::ESP::Info::steamId = false;
bool Settings::ESP::Info::rank = false;
bool Settings::ESP::Info::health = false;
bool Settings::ESP::Info::weapon = false;
bool Settings::ESP::Info::Ammo = false;
bool Settings::ESP::Info::scoped = false;
bool Settings::ESP::Info::reloading = false;
bool Settings::ESP::Info::flashed = false;
bool Settings::ESP::Info::planting = false;
bool Settings::ESP::Info::hasDefuser = false;
bool Settings::ESP::Info::defusing = false;
bool Settings::ESP::Info::distance = false;
bool Settings::ESP::Info::grabbingHostage = false;
bool Settings::ESP::Info::rescuing = false;
bool Settings::ESP::Info::location = false;
bool Settings::ESP::Info::lby = false;
bool Settings::ESP::Info::resolveInfo = false;
bool Settings::ESP::Boxes::enabled = false;
BoxType Settings::ESP::Boxes::type = BoxType::FRAME_2D;
bool Settings::ESP::Bars::enabled = false;
BarColorType Settings::ESP::Bars::colorType = BarColorType::HEALTH_BASED;
BarType Settings::ESP::Bars::type = BarType::HORIZONTAL;
bool Settings::ESP::Tracers::enabled = false;
TracerType Settings::ESP::Tracers::type = TracerType::BOTTOM;
bool Settings::ESP::BulletTracers::enabled = false;
bool Settings::ESP::FOVCrosshair::enabled = false;
bool Settings::ESP::FOVCrosshair::filled = false;
//ColorVar Settings::ESP::FOVCrosshair::color = ImColor(180, 50, 50, 255);
bool Settings::ESP::Skeleton::enabled = false;
bool Settings::ESP::SkeletonBacktrack::enabled = false;
bool Settings::ESP::Sounds::enabled = false;
int Settings::ESP::Sounds::time = 1000;
float Settings::ESP::Sounds::MinimumVolume = 0;
bool Settings::NoScopeBorder::enabled = false;
bool Settings::ESP::HeadDot::enabled = false;
float Settings::ESP::HeadDot::size = 2.f;

bool Settings::ESP::AutoWall::debugView = false;
bool Settings::ESP::AutoAim::drawTarget = false;
//Vector Settings::ESP::AutoAim::target = { 0, 0, 0 };
bool Settings::ESP::BoneMap::draw = false;
bool Settings::ESP::BoneMap::justDrawDots = false;

bool Settings::ESP::Spread::enabled = false;
bool Settings::ESP::Spread::spreadLimit = false;

//Test variable to test stuff out.....
bool Settings::ESP::Test::testfunction1 = false;
bool Settings::ESP::Test::testfunction2 = false;
bool Settings::ESP::Test::testfunction3 = false;



bool Settings::Airstuck::enabled = false;
//ButtonCode_t Settings::Airstuck::key = ButtonCode_t::KEY_F;

FakeLagType Settings::FakeLag::type = FakeLagType::OFF;
int Settings::FakeLag::value = 9;
//bool FakeLag::bFlipping = false;



//teleport//
bool Settings::Teleport::enabled = false;
//edgejump//
bool Settings::EdgeJump::enabled = false;
//ddi.cpp//

bool Settings::ESP::Info::FPS = false;
std::string CurrentFPS;

//ColorVar Settings::UI::Watermark::color = ImColor(26, 104, 173, 255);
bool Settings::UI::Watermark::displayIngame = false;
bool Settings::UI::Watermark::showwatermark = false;
//Spotify
bool Settings::Spotify::ShowControls = false;
bool Settings::Spotify::ShowCurrentlyPlaying = false;


bool Settings::View::NoAimPunch::enabled = false;
bool Settings::View::NoViewPunch::enabled = false;
//show ranks and spec
bool Settings::ShowRanks::enabled = false;
bool Settings::ShowSpectators::enabled = false;
//jumpthrow
bool Settings::JumpThrow::enabled = false;
//EVENTLOGGER//
bool Settings::EventLogger::enabled = false;
bool Settings::EventLogger::OnFreezeTime = false;

bool Settings::FakewalkAW::enabled;
//ButtonCode_t Settings::FakewalkAW::key = ButtonCode_t::KEY_C;
bool Settings::Fakewalk::enabled;
//ButtonCode_t Settings::Fakewalk::key = ButtonCode_t::KEY_Y;
bool Settings::ChokePackets::enabled;
//ButtonCode_t Settings::ChokePackets::key = ButtonCode_t::KEY_X;
bool Settings::SlowMo::enabled;
//ButtonCode_t Settings::SlowMo::key = ButtonCode_t::KEY_C;


bool Settings::AutoStrafe::enabled = false;
AutostrafeType Settings::AutoStrafe::type = AutostrafeType::AS_FORWARDS;
bool Settings::AutoStrafe::silent = true;

//--AUTOKNIFE--//
bool Settings::AutoKnife::enabled = false;
bool Settings::AutoKnife::Filters::enemies = true;
bool Settings::AutoKnife::Filters::allies = false;
bool Settings::AutoKnife::onKey = true;

//--chams--//

bool Settings::ESP::Chams::enabled = false;
bool Settings::ESP::Chams::Arms::enabled = false;
bool Settings::ESP::Chams::Weapon::enabled = false;
ArmsType Settings::ESP::Chams::Arms::type = ArmsType::DEFAULT;
//HealthColorVar Settings::ESP::Chams::allyColor = ImColor(0, 0, 255, 255);
//HealthColorVar Settings::ESP::Chams::allyVisibleColor = ImColor(0, 255, 0, 255);
//HealthColorVar Settings::ESP::Chams::enemyColor = ImColor(255, 0, 0, 255);
//HealthColorVar Settings::ESP::Chams::enemyVisibleColor = ImColor(255, 255, 0, 255);
//HealthColorVar Settings::ESP::Chams::localplayerColor = ImColor(0, 255, 255, 255);
//ColorVar Settings::ESP::Chams::Arms::color = ImColor(255, 255, 255, 255);
//ColorVar Settings::ESP::Chams::Weapon::color = ImColor(255, 255, 255, 255);
ChamsType Settings::ESP::Chams::type = ChamsType::CHAMS;

bool Settings::AntiAimInfos::enabled = false;
bool Settings::AngleHelper::enabled = false;

//--AUTODEFUSE--//
bool Settings::AutoDefuse::enabled = false;
bool Settings::AutoDefuse::silent = false;
//--RADAR--//
bool Settings::Radar::enabled = false;
float Settings::Radar::zoom = 16.f;
bool Settings::Radar::enemies = false;
bool Settings::Radar::allies = false;
bool Settings::Radar::bomb = false;
bool Settings::Radar::defuser = false;
bool Settings::Radar::legit = false;
bool Settings::Radar::visibilityCheck = false;
bool Settings::Radar::smokeCheck = false;
bool Settings::Radar::InGame::enabled = false;
TeamColorType Settings::Radar::teamColorType = TeamColorType::RELATIVE_;
//HealthColorVar Settings::Radar::enemyColor = ImColor(192, 32, 32, 255);
//HealthColorVar Settings::Radar::enemyVisibleColor = ImColor(192, 32, 32, 255);
//HealthColorVar Settings::Radar::allyColor = ImColor(32, 64, 192, 255);
//HealthColorVar Settings::Radar::allyVisibleColor = ImColor(32, 64, 192, 255);
//HealthColorVar Settings::Radar::tColor = ImColor(192, 128, 64, 255);
//HealthColorVar Settings::Radar::tVisibleColor = ImColor(192, 128, 64, 255);
//HealthColorVar Settings::Radar::ctColor = ImColor(64, 128, 192, 255);
//HealthColorVar Settings::Radar::ctVisibleColor = ImColor(64, 128, 192, 255);
//ColorVar Settings::Radar::bombColor = ImColor(192, 192, 64, 255);
//ColorVar Settings::Radar::bombDefusingColor = ImColor(192, 192, 64, 255);
//ColorVar Settings::Radar::defuserColor = ImColor(32, 192, 192, 255);
float Settings::Radar::iconsScale = 4.5f;

//--IRC--//
bool Settings::IRC::Enabled = false;
char Settings::IRC::Message[255] = "";

//--FOVCHANGER--//
bool Settings::FOVChanger::enabled = false;
bool Settings::FOVChanger::ignoreScope = true;
float Settings::FOVChanger::value = 100.f;

bool Settings::FOVChanger::viewmodelEnabled = false;
float Settings::FOVChanger::viewmodelValue = 90.f;

//float OverrideView::currentFOV = 90.0f;
//other//
bool Settings::ScreenshotCleaner::enabled = false;
bool Settings::SniperCrosshair::enabled = false;
bool Settings::ESP::Glow::enabled = false;
//--NADEHELPER--//

//std::vector<GrenadeInfo> Settings::GrenadeHelper::grenadeInfos = {};
bool Settings::GrenadeHelper::enabled = false;
bool Settings::GrenadeHelper::onlyMatchingInfos = true;
bool Settings::GrenadeHelper::aimAssist = false;
float Settings::GrenadeHelper::aimDistance = 75;
float Settings::GrenadeHelper::aimFov = 45;
float Settings::GrenadeHelper::aimStep = 5;

//nadepredict//
bool Settings::GrenadePrediction::enabled = false;
//ColorVar Settings::GrenadePrediction::color = ImColor(26, 104, 173, 255);

//ColorVar Settings::GrenadeHelper::aimDot = ImColor(10, 10, 200, 255);
//ColorVar Settings::GrenadeHelper::aimLine = ImColor(200, 200, 200, 255);
//ColorVar Settings::GrenadeHelper::infoHE = ImColor(7, 183, 7, 255);
//ColorVar Settings::GrenadeHelper::infoMolotov = ImColor(236, 0, 0, 255);
//ColorVar Settings::GrenadeHelper::infoSmoke = ImColor(172, 172, 172, 255);
//ColorVar Settings::GrenadeHelper::infoFlash = ImColor(255, 255, 0, 255);

bool shotLastTick = false;
std::string Settings::GrenadeHelper::actMapName;


//--HITMARKER--//
bool Settings::ESP::Hitmarker::enabled = false;
bool Settings::ESP::Hitmarker::enemies = false;
bool Settings::ESP::Hitmarker::allies = false;
int Settings::ESP::Hitmarker::duration = 2000;
int Settings::ESP::Hitmarker::size = 16;
int Settings::ESP::Hitmarker::innerGap = 5;
bool Settings::ESP::Hitmarker::Damage::enabled = false;

//bhop
bool Settings::BHop::enabled = false;
bool Settings::BHop::Chance::enabled = false;
int Settings::BHop::Chance::value = 70;
bool Settings::BHop::Hops::enabledMax = false;
int Settings::BHop::Hops::Max = 7;
bool Settings::BHop::Hops::enabledMin = false;
int Settings::BHop::Hops::Min = 3;
//circle strafe
bool Settings::CircleStrafe::enabled = false;
//ButtonCode_t Settings::CircleStrafe::key = ButtonCode_t::KEY_F;
//Vector AutoStrafeView;

//--ASUSWALS--//
bool Settings::ASUSWalls::enabled = false;
//ColorVar Settings::ASUSWalls::color = ImColor(255, 255, 255, 150);
float r = 0.0f, g = 0.0f, b = 0.0f, a = 0.0f;

//--nosky--//

bool Settings::NoSky::enabled = false;
bool Settings::SkyBox::enabled = false;
int Settings::SkyBox::skyBoxNumber = 0;
//ColorVar Settings::NoSky::color = ImColor(0, 0, 0, 255);
float r1 = 0.0f, g1 = 0.0f, b1 = 0.0f, a1 = 0.0f;

//--autoaccept--//
bool Settings::AutoAccept::enabled = false;
//-autoblock-//
bool Settings::Autoblock::enabled = false;

bool Settings::Aimbot::NoSpread = false;
bool Settings::NoSmoke::enabled = false;
bool Settings::NameStealer::enabled = false;
int Settings::NameStealer::team = 1;
bool Settings::Noflash::enabled = false;
float Settings::Noflash::value = 160.0f;
bool Settings::Recoilcrosshair::enabled = false;
bool Settings::Recoilcrosshair::showOnlyWhenShooting = false;


//--RESOLVER--//
bool Settings::Resolver::enabled = false;
bool Settings::Resolver::info = false;
bool Settings::Resolver::pitch = false;
float Settings::Resolver::ticks = 2;
float Settings::Resolver::modulo = 2;
bool Settings::Resolver::LagComp = false;
bool Settings::Resolver::lbyOnly = false;
int Settings::Resolver::baimAfter = -1;
//int Resolver::resolvingId = -1;
//std::vector<int64_t> Resolver::playerAngleLogs = {};
//std::array<CResolveInfo, 32> Resolver::m_arrInfos;
//
//ButtonCode_t Settings::Resolver::angleFlip = ButtonCode_t::KEY_F;
bool Settings::Resolver::angleFlipEnabled = false;

//
//thirdperson
bool Settings::ThirdPerson::enabled = false;
ThirdPersonMode Settings::ThirdPerson::mode = ThirdPersonMode::FAKE;
float Settings::ThirdPerson::distance = 30.0f;
//ButtonCode_t Settings::ThirdPerson::key = ButtonCode_t::KEY_DELETE;
float Settings::ThirdPerson::transparency = 0.3f;


//--spammer--//
SpammerType Settings::Spammer::type = SpammerType::SPAMMER_NONE;
bool Settings::Spammer::say_team = false;
bool Settings::Spammer::KillSpammer::enabled = false;
bool Settings::Spammer::KillSpammer::sayTeam = false;
bool Settings::Spammer::KillSpammer::enabledSync = false;
bool Settings::Spammer::KillSpammer::enabledHurt = false;
//bool Settings::Spammer::KillSpammer::enabledBombTimerCountdown = false;
std::vector<std::string> Settings::Spammer::KillSpammer::messages = {
	"Killing Spree",
	"BOOM HEADSHOT sort of...",
	"Killing Frenzy!",
	"Running Riot!",
	"Rampage!",
	"Untouchable!",
	"Invincible!"
};
bool Settings::Spammer::RadioSpammer::enabled = false;
bool Settings::Spammer::RadioSpammer::enabledGetOut = false;
std::vector<std::string> Settings::Spammer::NormalSpammer::messages = {
	"X-HOOK.XYZ",
	"X-HOOK.XYZ | Free & Premium cheats",
	"X-HOOK.XYZ - Legit | Rage"
};


int Settings::Spammer::PositionSpammer::team = 1;
bool Settings::Spammer::PositionSpammer::showName = true;
bool Settings::Spammer::PositionSpammer::showWeapon = true;
bool Settings::Spammer::PositionSpammer::showRank = true;
bool Settings::Spammer::PositionSpammer::showWins = true;
bool Settings::Spammer::PositionSpammer::showHealth = true;
bool Settings::Spammer::PositionSpammer::showMoney = true;
bool Settings::Spammer::PositionSpammer::showLastplace = true;

//--TRIGGERBOT--//
bool Settings::Triggerbot::enabled = false;
bool Settings::Triggerbot::Filters::enemies = true;
bool Settings::Triggerbot::Filters::allies = false;
bool Settings::Triggerbot::Filters::walls = false;
bool Settings::Triggerbot::Filters::smokeCheck = false;
bool Settings::Triggerbot::Filters::flashCheck = false;
bool Settings::Triggerbot::Filters::head = true;
bool Settings::Triggerbot::Filters::chest = true;
bool Settings::Triggerbot::Filters::stomach = true;
bool Settings::Triggerbot::Filters::arms = true;
bool Settings::Triggerbot::Filters::legs = true;
bool Settings::Triggerbot::RandomDelay::enabled = true;
int Settings::Triggerbot::RandomDelay::lowBound = 20;
int Settings::Triggerbot::RandomDelay::highBound = 35;
int Settings::Triggerbot::RandomDelay::lastRoll = 0;
//ButtonCode_t Settings::Triggerbot::key = ButtonCode_t::KEY_LALT;

//--TRACER--//
bool Settings::TracerEffects::enabled = false;
bool Settings::TracerEffects::serverSide = false;
//TracerEffects_t Settings::TracerEffects::effect = TracerEffects_t::TASER;
int Settings::TracerEffects::frequency = 1;

//--CLANTAG--//
char Settings::ClanTagChanger::value[30] = "";
bool Settings::ClanTagChanger::animation = false;
int Settings::ClanTagChanger::animationSpeed = 650;
bool Settings::ClanTagChanger::enabled = false; // TODO find a way to go back to the "official" clan tag for the player? -- Save the current clan tag, before editing, then restore it later
ClanTagType Settings::ClanTagChanger::type = ClanTagType::STATIC;
valueType Settings::ClanTagChanger::preset = valueType::XHOOK;

//POSTPROCESS//
bool Settings::DisablePostProcessing::enabled = false;

//DLIGHT//
bool Settings::Dlights::enabled = false;
float Settings::Dlights::radius = 500.0f;

//WALKBOT//
bool Settings::WalkBot::enabled = false;
bool Settings::WalkBot::forceReset = false; // if a Reset is queued up by the User
bool Settings::WalkBot::autobuy = false; // autobuy during FreezeTime
int Settings::WalkBot::autobuyAt = 0;   // autobuy >= this amount of cash




inline void SetTooltip(const char* text) {
	if (ImGui::IsItemHovered())
		ImGui::SetTooltip("%s", text);
}

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
char buf[128] = "";
const char* BoxTypes[] = { "Flat 2D", "Frame 2D", "Box 3D", "Hitbox" };
const char* TracerTypes[] = { "Bottom", "Cursor" };
const char* BarTypes[] = { "Vertical Left", "Vertical Right", "Horizontal Below", "Horizontal Above", "Interwebz" };
const char* BarColorTypes[] = { "Static", "Health Based" };
const char* TeamColorTypes[] = { "Absolute", "Relative" };
const char* ChamsTypes[] = { "Normal", "Normal - XQZ", "Flat", "Flat - XQZ", "Glass", "Crystal", "BlueCrystal",
"Gold", "Velvet", "Tree", "SpeechInfo", "FishNet", "Letters", "Gloss" ,"Animated", "Blink", "Glow", "Guerilla" };
const char* ArmsTypes[] = { "Default", "Glass", "Crystal", "BlueCrystal", "Gold", "Velvet", "Tree", "SpeechInfo",
"FishNet", "Letters", "Gloss", "Wireframe", "None" };
const char* tpModes[] = { "Fake", "Real", "Lower Body Yaw", "Ghost" };

const char* tabs[] = {
	"Aimbot",
	"Triggerbot",
	"Visuals",
	"HvH",
	"Player List",
	//"Models",
	"Misc",
	"Colors",
	"Skinchanger",
	"ModelChanger"
	//"License"
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


const char* buyBotWeapons[] =
{
	"buy glock",
	"buy hkp2000",
	"buy usp_silencer",
	"buy elite",
	"buy p250",
	"buy tec9",
	"buy fn57",
	"buy deagle",
	"buy galilar",
	"buy famas",
	"buy ak47",
	"buy m4a1",
	"buy m4a1_silencer",
	"buy ssg08",
	"buy aug",
	"buy sg556",
	"buy awp",
	"buy scar20",
	"buy g3sg1",
	"buy nova",
	"buy xm1014",
	"buy mag7",
	"buy m249",
	"buy negev",
	"buy mac10",
	"buy mp9",
	"buy mp7",
	"buy ump45",
	"buy p90",
	"buy bizon"
};
const char* strafeTypes[] = { "Forwards", "Backwards", "Left", "Right", "Rage" };
const char* animationTypes[] = { "Static", "Current Time", "Marquee", "Words", "Letters" };

const char* spammerTypes[] = { "None", "Normal", "Positions" };
const char* presetTypes[] = { "X-HOOK.XYZ", "Dogeii Gang", "Custom" };
const char* lagTypes[] = { "Off", "AimTux", "Normal", "Step", "Reactive", "Nuclear", "Lunico" };
const char* teams[] = { "Allies", "Enemies", "Both" };
const char* grenadeTypes[] = { "Flashbang", "Smoke Grenade", "Molotov", "HE Grenade" };
const char* throwTypes[] = { "NORMAL", "RUN", "JUMP", "WALK" };
static char nickname[127] = "";













// Data
static LPDIRECT3DDEVICE9        g_pd3dDevice = NULL;
static D3DPRESENT_PARAMETERS    g_d3dpp;

extern LRESULT ImGui_ImplDX9_WndProcHandler(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);
LRESULT WINAPI WndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	if (ImGui_ImplDX9_WndProcHandler(hWnd, msg, wParam, lParam))
		return true;

	switch (msg)
	{
	case WM_SIZE:
		if (g_pd3dDevice != NULL && wParam != SIZE_MINIMIZED)
		{
			ImGui_ImplDX9_InvalidateDeviceObjects();
			g_d3dpp.BackBufferWidth = LOWORD(lParam);
			g_d3dpp.BackBufferHeight = HIWORD(lParam);
			HRESULT hr = g_pd3dDevice->Reset(&g_d3dpp);
			if (hr == D3DERR_INVALIDCALL)
				IM_ASSERT(0);
			ImGui_ImplDX9_CreateDeviceObjects();
		}
		return 0;
	case WM_SYSCOMMAND:
		if ((wParam & 0xfff0) == SC_KEYMENU) // Disable ALT application menu
			return 0;
		break;
	case WM_DESTROY:
		PostQuitMessage(0);
		return 0;
	}
	return DefWindowProc(hWnd, msg, wParam, lParam);
}


ColorVar Settings::UI::accentColor = ImColor(43, 115, 178, 74);
ColorVar Settings::UI::mainColor = ImColor(46, 133, 200, 255);
ColorVar Settings::UI::bodyColor = ImColor(28, 33, 32, 228);
ColorVar Settings::UI::fontColor = ImColor(255, 255, 255, 255);


int main(int, char**)
{
	// Create application window
	WNDCLASSEX wc = { sizeof(WNDCLASSEX), CS_CLASSDC, WndProc, 0L, 0L, GetModuleHandle(NULL), NULL, LoadCursor(NULL, IDC_ARROW), NULL, NULL, _T("ImGui Example"), NULL };
	RegisterClassEx(&wc);
	HWND hwnd = CreateWindow(_T("ImGui Example"), _T("ImGui DirectX9 Example"), WS_OVERLAPPEDWINDOW, 100, 100, 1280, 800, NULL, NULL, wc.hInstance, NULL);

	// Initialize Direct3D
	LPDIRECT3D9 pD3D;
	if ((pD3D = Direct3DCreate9(D3D_SDK_VERSION)) == NULL)
	{
		UnregisterClass(_T("ImGui Example"), wc.hInstance);
		return 0;
	}
	ZeroMemory(&g_d3dpp, sizeof(g_d3dpp));
	g_d3dpp.Windowed = TRUE;
	g_d3dpp.SwapEffect = D3DSWAPEFFECT_DISCARD;
	g_d3dpp.BackBufferFormat = D3DFMT_UNKNOWN;
	g_d3dpp.EnableAutoDepthStencil = TRUE;
	g_d3dpp.AutoDepthStencilFormat = D3DFMT_D16;
	g_d3dpp.PresentationInterval = D3DPRESENT_INTERVAL_IMMEDIATE;

	// Create the D3DDevice
	if (pD3D->CreateDevice(D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, hwnd, D3DCREATE_HARDWARE_VERTEXPROCESSING, &g_d3dpp, &g_pd3dDevice) < 0)
	{
		pD3D->Release();
		UnregisterClass(_T("ImGui Example"), wc.hInstance);
		return 0;
	}

	// Setup ImGui binding
	ImGui_ImplDX9_Init(hwnd, g_pd3dDevice);

	// Load Fonts
	// (there is a default font, this is only if you want to change it. see extra_fonts/README.txt for more details)
	//ImGuiIO& io = ImGui::GetIO();
	//io.Fonts->AddFontDefault();
	//io.Fonts->AddFontFromFileTTF("../../extra_fonts/Cousine-Regular.ttf", 15.0f);
	//io.Fonts->AddFontFromFileTTF("../../extra_fonts/DroidSans.ttf", 16.0f);
	//io.Fonts->AddFontFromFileTTF("../../extra_fonts/ProggyClean.ttf", 13.0f);
	//io.Fonts->AddFontFromFileTTF("../../extra_fonts/ProggyTiny.ttf", 10.0f);
	//io.Fonts->AddFontFromFileTTF("c:\\Windows\\Fonts\\ArialUni.ttf", 18.0f, NULL, io.Fonts->GetGlyphRangesJapanese());

	bool show_test_window = true;
	bool show_another_window = false;
	ImVec4 clear_col = ImColor(114, 144, 154);

	// Main loop
	MSG msg;
	ZeroMemory(&msg, sizeof(msg));
	ShowWindow(hwnd, SW_SHOWDEFAULT);
	UpdateWindow(hwnd);
	while (msg.message != WM_QUIT)
	{
		if (PeekMessage(&msg, NULL, 0U, 0U, PM_REMOVE))
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
			continue;
		}
		ImGui_ImplDX9_NewFrame();



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

		ImVec2 PosMENU;// The location of the Doge Picture 

		ImVec2 Pos;// The location of the Doge Picture 
		static ImGuiStyle* style = &ImGui::GetStyle();

		static int page = 0;
		ImGui::SetNextWindowSize(ImVec2(1080, 720), ImGuiSetCond_FirstUseEver);
		static bool MenuBool;

		ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);

		ImGui::Separator();

		ImGui::Begin("X-HOOK V1.3", &MenuBool, ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_ShowBorders);
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


			//if (!Settings::Background::enable)
			//{
			//	//Background Image
			//	ImVec2 pos = ImGui::GetCursorScreenPos();
			//	ImVec2 maxPos = ImVec2(pos.x + ImGui::GetWindowSize().x, pos.y + ImGui::GetWindowSize().y);

			//	ImGui::GetWindowDrawList()->AddImage(
			//		(void *)BackgroundTexture,
			//		ImVec2(pos.x, pos.y),
			//		ImVec2(maxPos),
			//		ImVec2(1, 0), ImVec2(0, 1)
			//	);

			//}

			int currentwidth = ImGui::GetWindowSize().x;
			int nextwidth = (currentwidth - (currentwidth * 2));

			switch (page)
			{
			case 0:

				//Aimbot::RenderTab();
				break;
			case 1:
				//Triggerbot::RenderTab();
				break;
			case 2:


				ImGui::Checkbox("Enabled", &Settings::ESP::enabled);
				ImGui::NextColumn();
				{
					//	ImGui::Combo("##espkey", (int*)&Settings::ESP::key, ButtonNames, IM_ARRAYSIZE(ButtonNames));
					//	UI::KeyBindButton(&Settings::ESP::key);
				}
				ImGui::Separator();

				ImGui::Columns(2, NULL, true);
				{
					ImGui::BeginChild("COL1", ImVec2(400, 0), true);
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
							ImGui::Checkbox("Tracers", &Settings::ESP::Tracers::enabled);
							SetTooltip("Draws a line to each player");

							ImGui::Text("Bar Color");

							ImGui::Text("Team Color");

							ImGui::Checkbox("Bullet Tracers", &Settings::ESP::BulletTracers::enabled);
							SetTooltip("Adds a line showing where a player is aiming");
							ImGui::Checkbox("Head Dot", &Settings::ESP::HeadDot::enabled);
							SetTooltip("Adds a Dot on the Head of a player");
						}
						ImGui::NextColumn();
						{
							ImGui::PushItemWidth(-1);
							ImGui::Combo("##BOXTYPE", (int*)& Settings::ESP::Boxes::type, BoxTypes, IM_ARRAYSIZE(BoxTypes));
							ImGui::Combo("##CHAMSTYPE", (int*)& Settings::ESP::Chams::type, ChamsTypes, IM_ARRAYSIZE(ChamsTypes));
							ImGui::Combo("##BARTYPE", (int*)& Settings::ESP::Bars::type, BarTypes, IM_ARRAYSIZE(BarTypes));
							ImGui::Combo("##TRACERTYPE", (int*)& Settings::ESP::Tracers::type, TracerTypes, IM_ARRAYSIZE(TracerTypes));
							ImGui::Combo("##BARCOLTYPE", (int*)& Settings::ESP::Bars::colorType, BarColorTypes, IM_ARRAYSIZE(BarColorTypes));
							ImGui::Combo("##TEAMCOLTYPE", (int*)& Settings::ESP::teamColorType, TeamColorTypes, IM_ARRAYSIZE(TeamColorTypes));
							ImGui::PopItemWidth();
							ImGui::Checkbox("Skeleton", &Settings::ESP::Skeleton::enabled);
							SetTooltip("Show a players skeleton");
							ImGui::SameLine();
							ImGui::Checkbox("BackTrack line", &Settings::ESP::SkeletonBacktrack::enabled);
							SetTooltip("Draws a backtrack line. So you know where to shoot.");
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
							ImGui::Checkbox("Dlights", &Settings::Dlights::enabled);
							SetTooltip("Adds a light source to players");
							ImGui::Checkbox("No Flash", &Settings::Noflash::enabled);
							SetTooltip("Hide flashbang effect");
					//		ImGui::Checkbox("Show Footsteps", &Settings::ESP::Sounds::enabled);
					//		SetTooltip("Shows you footsteps in 3D space");
							ImGui::Checkbox("No View Punch", &Settings::View::NoViewPunch::enabled);
							SetTooltip("Disables view punch when shooting");
							ImGui::Checkbox("Weapons", &Settings::ESP::Chams::Weapon::enabled);
							SetTooltip("Apply chams to weapons");

							ImGui::Checkbox("No Sky", &Settings::NoSky::enabled);
							SetTooltip("Allows for the skybox to be colored or disabled");
							ImGui::Checkbox("No Smoke", &Settings::NoSmoke::enabled);
							SetTooltip("Disables smoke rendering");
							//
							if (ImGui::Checkbox("Show Watermark", &Settings::UI::Watermark::showwatermark))
								ImGui::Checkbox("Show Watermark Ingame", &Settings::UI::Watermark::displayIngame);
							SetTooltip("Will show the watermark ingame");



						}

						if (ImGui::Button(XorStr("Tracer Effect"), ImVec2(-1, 0)))
							ImGui::OpenPopup(XorStr("##TracerEffectWindow"));
						ImGui::SetNextWindowSize(ImVec2(320, 120), ImGuiSetCond_Always);
						if (ImGui::BeginPopup(XorStr("##TracerEffectWindow"))) {
							ImGui::PushItemWidth(-1);
							if (Settings::TracerEffects::serverSide) {
								Settings::TracerEffects::frequency = 1;
								//	Settings::TracerEffects::effect = TracerEffects_t::TASER;
							}
							//	ImGui::Combo(XorStr("##TracerEffects"), (int*)&Settings::TracerEffects::effect, tracerEffectNames,
								//	IM_ARRAYSIZE(tracerEffectNames));
							ImGui::Checkbox(XorStr("Enable Tracers"), &Settings::TracerEffects::enabled);
							ImGui::Checkbox(XorStr("Server Sided?"), &Settings::TracerEffects::serverSide);
							SetTooltip(
								XorStr("Requires a Taser in your Inventory.\nCan only shoot one shot at a time\nOnly Works with Kisak Snot"));
							ImGui::Columns(2, NULL, false);
							{
								ImGui::SliderInt(XorStr("##TracerFreq"), &Settings::TracerEffects::frequency, 0, 10,
									XorStr("Freq: %0.f"));
							}
							ImGui::NextColumn();
							{
								if (ImGui::Button(XorStr("Restore Tracers"))) {
									//	TracerEffect::RestoreTracers();
								}
							}
							ImGui::PopItemWidth();
							ImGui::EndPopup();
						}
						//if (ImGui::Button(XorStr("Material Config"), ImVec2(-1, 0)))
						//	ImGui::OpenPopup(XorStr("##MaterialConfigWindow"));
						//SetTooltip(XorStr("Advanced CSGO Gfx Settings\nExpect a short delay when changed"));
						//ImGui::SetNextWindowSize(ImVec2(320, 640), ImGuiSetCond_Always);
						//if (ImGui::BeginPopup(XorStr("##MaterialConfigWindow"))) {
						//	ImGui::PushItemWidth(-1);
						//	if (ImGui::Button(XorStr("Reset Changes"))) {
						//		Settings::MaterialConfig::config = MaterialConfig::backupConfig;
						//	}
						//	ImGui::Checkbox(XorStr("Changes Enabled?"), &Settings::MaterialConfig::enabled);
						//	SetTooltip(XorStr("Expect a Short delay when changing these settings."));
						//	ImGui::SliderFloat(XorStr("##MONITORGAMMA"), &Settings::MaterialConfig::config.m_fMonitorGamma, 0.1f, 12.0f, XorStr("Gamma: %.3f"));
						//	ImGui::SliderFloat(XorStr("##GAMMATVRANGEMIN"), &Settings::MaterialConfig::config.m_fGammaTVRangeMin, 0.1f, min(300.0f, Settings::MaterialConfig::config.m_fGammaTVRangeMax), XorStr("TVRangeMin: %.3f"));
						//	ImGui::SliderFloat(XorStr("##GAMMATVRANGEMAX"), &Settings::MaterialConfig::config.m_fGammaTVRangeMax, Settings::MaterialConfig::config.m_fGammaTVRangeMin, 300.0f, XorStr("TVRangeMax: %.3f"));
						//	ImGui::SliderFloat(XorStr("##GAMMATVEXPONENT"), &Settings::MaterialConfig::config.m_fGammaTVExponent, 0.1f, 3.0f, XorStr("TV Exponent: %.3f"));
						//	ImGui::Checkbox(XorStr("GammaTVEnabled"), &Settings::MaterialConfig::config.m_bGammaTVEnabled);
						//	ImGui::Text(XorStr("Width:"));
						//	ImGui::SameLine();
						//	ImGui::InputInt(XorStr("##GAMEWIDTH"), &Settings::MaterialConfig::config.m_VideoMode.m_Width);

						//	ImGui::Text(XorStr("Height:"));
						//	ImGui::SameLine();
						//	ImGui::InputInt(XorStr("##GAMEHEIGHT"), &Settings::MaterialConfig::config.m_VideoMode.m_Height);

						//	ImGui::Text(XorStr("Refresh Rate:"));
						//	ImGui::SameLine();
						//	ImGui::InputInt(XorStr("##GAMEREFRESHRATE"), &Settings::MaterialConfig::config.m_VideoMode.m_RefreshRate);

						//	ImGui::Checkbox(XorStr("TripleBuffered"), &Settings::MaterialConfig::config.m_bTripleBuffered);
						//	ImGui::SliderInt(XorStr("##AASAMPLES"), &Settings::MaterialConfig::config.m_nAASamples, 0, 16, XorStr("AA Samples: %1.f"));
						//	ImGui::SliderInt(XorStr("##FORCEANISOTROPICLEVEL"), &Settings::MaterialConfig::config.m_nForceAnisotropicLevel, 0, 8, XorStr("Anisotropic Level: %1.f"));
						//	ImGui::SliderInt(XorStr("##SKIPMIPLEVELS"), &Settings::MaterialConfig::config.skipMipLevels, 1, 16, XorStr("SkipMipLevels: %1.f"));
						//	if (ImGui::Button(XorStr("Flags"), ImVec2(-1, 0)))
						//		ImGui::OpenPopup(XorStr("##MaterialConfigFlags"));
						//	ImGui::SetNextWindowSize(ImVec2(320, 240), ImGuiSetCond_Always);
						//	if (ImGui::BeginPopup(XorStr("##MaterialConfigFlags"))) {
						//		ImGui::PushItemWidth(-1);

						//		static bool localFlags[] = {
						//			(Settings::MaterialConfig::config.m_Flags & MaterialSystem_Config_Flags_t::MATSYS_VIDCFG_FLAGS_WINDOWED) != 0, // ( 1 << 0 )
						//			(Settings::MaterialConfig::config.m_Flags & MaterialSystem_Config_Flags_t::MATSYS_VIDCFG_FLAGS_RESIZING) != 0, // ( 1 << 1 )
						//			false, // ( 1 << 2 ) is not used.
						//			(Settings::MaterialConfig::config.m_Flags & MaterialSystem_Config_Flags_t::MATSYS_VIDCFG_FLAGS_NO_WAIT_FOR_VSYNC) != 0, // ( 1 << 3 )
						//			(Settings::MaterialConfig::config.m_Flags & MaterialSystem_Config_Flags_t::MATSYS_VIDCFG_FLAGS_STENCIL) != 0, // ...
						//			(Settings::MaterialConfig::config.m_Flags & MaterialSystem_Config_Flags_t::MATSYS_VIDCFG_FLAGS_FORCE_TRILINEAR) != 0,
						//			(Settings::MaterialConfig::config.m_Flags & MaterialSystem_Config_Flags_t::MATSYS_VIDCFG_FLAGS_FORCE_HWSYNC) != 0,
						//			(Settings::MaterialConfig::config.m_Flags & MaterialSystem_Config_Flags_t::MATSYS_VIDCFG_FLAGS_DISABLE_SPECULAR) != 0,
						//			(Settings::MaterialConfig::config.m_Flags & MaterialSystem_Config_Flags_t::MATSYS_VIDCFG_FLAGS_DISABLE_BUMPMAP) != 0,
						//			(Settings::MaterialConfig::config.m_Flags & MaterialSystem_Config_Flags_t::MATSYS_VIDCFG_FLAGS_ENABLE_PARALLAX_MAPPING) != 0,
						//			(Settings::MaterialConfig::config.m_Flags & MaterialSystem_Config_Flags_t::MATSYS_VIDCFG_FLAGS_USE_Z_PREFILL) != 0,
						//			(Settings::MaterialConfig::config.m_Flags & MaterialSystem_Config_Flags_t::MATSYS_VIDCFG_FLAGS_REDUCE_FILLRATE) != 0,
						//			(Settings::MaterialConfig::config.m_Flags & MaterialSystem_Config_Flags_t::MATSYS_VIDCFG_FLAGS_ENABLE_HDR) != 0,
						//			(Settings::MaterialConfig::config.m_Flags & MaterialSystem_Config_Flags_t::MATSYS_VIDCFG_FLAGS_LIMIT_WINDOWED_SIZE) != 0,
						//			(Settings::MaterialConfig::config.m_Flags & MaterialSystem_Config_Flags_t::MATSYS_VIDCFG_FLAGS_SCALE_TO_OUTPUT_RESOLUTION) != 0,
						//			(Settings::MaterialConfig::config.m_Flags & MaterialSystem_Config_Flags_t::MATSYS_VIDCFG_FLAGS_USING_MULTIPLE_WINDOWS) != 0,
						//			(Settings::MaterialConfig::config.m_Flags & MaterialSystem_Config_Flags_t::MATSYS_VIDCFG_FLAGS_DISABLE_PHONG) != 0,
						//			(Settings::MaterialConfig::config.m_Flags & MaterialSystem_Config_Flags_t::MATSYS_VIDCFG_FLAGS_VR_MODE) != 0
						//		};

						//		ImGui::Checkbox(XorStr("Windowed"), &localFlags[0]);
						//		ImGui::Checkbox(XorStr("Resizing"), &localFlags[1]);
						//		ImGui::Checkbox(XorStr("No VSYNC Wait"), &localFlags[3]);
						//		ImGui::Checkbox(XorStr("Stencil"), &localFlags[4]);
						//		ImGui::Checkbox(XorStr("Force Tri-Linear"), &localFlags[5]);
						//		ImGui::Checkbox(XorStr("Force HW Sync"), &localFlags[6]);
						//		ImGui::Checkbox(XorStr("Disable Specular"), &localFlags[7]);
						//		ImGui::Checkbox(XorStr("Disable Bumpmap"), &localFlags[8]);
						//		ImGui::Checkbox(XorStr("Disable Phong"), &localFlags[16]);
						//		ImGui::Checkbox(XorStr("Parallax Mapping"), &localFlags[9]);
						//		ImGui::Checkbox(XorStr("Use Z-Prefill"), &localFlags[10]);
						//		ImGui::Checkbox(XorStr("Reduce FillRate"), &localFlags[11]);
						//		ImGui::Checkbox(XorStr("HDR"), &localFlags[12]);
						//		ImGui::Checkbox(XorStr("Limit Windowed Size"), &localFlags[13]);
						//		ImGui::Checkbox(XorStr("Scale to Output Resolution"), &localFlags[14]);
						//		ImGui::Checkbox(XorStr("Using Multiple Windows"), &localFlags[15]);
						//		ImGui::Checkbox(XorStr("VR-Mode"), &localFlags[17]);

						//		if (ImGui::Button(XorStr("Apply "))) {
						//			for (unsigned short i = 0; i < 18; i++) {
						//				if (i == 2) // ( 1 << 2 ) not used.
						//					continue;
						//				Settings::MaterialConfig::config.SetFlag((unsigned int)(1 << i), localFlags[i]);
						//			}
						//		}

						//		ImGui::PopItemWidth();
						//		ImGui::EndPopup();
						//	}
						//	//m_flags
						//	ImGui::Checkbox(XorStr("EditMode"), &Settings::MaterialConfig::config.bEditMode);
						//	//proxiesTestMode
						//	ImGui::Checkbox(XorStr("Compressed Textures"), &Settings::MaterialConfig::config.bCompressedTextures);
						//	ImGui::Checkbox(XorStr("Filter Lightmaps"), &Settings::MaterialConfig::config.bFilterLightmaps);
						//	ImGui::Checkbox(XorStr("Filter Textures"), &Settings::MaterialConfig::config.bFilterTextures);
						//	ImGui::Checkbox(XorStr("Reverse Depth"), &Settings::MaterialConfig::config.bReverseDepth);
						//	ImGui::Checkbox(XorStr("Buffer Primitives"), &Settings::MaterialConfig::config.bBufferPrimitives);
						//	ImGui::Checkbox(XorStr("Draw Flat"), &Settings::MaterialConfig::config.bDrawFlat);
						//	ImGui::Checkbox(XorStr("Measure Fill-Rate"), &Settings::MaterialConfig::config.bMeasureFillRate);
						//	ImGui::Checkbox(XorStr("Visualize Fill-Rate"), &Settings::MaterialConfig::config.bVisualizeFillRate);
						//	ImGui::Checkbox(XorStr("No Transparency"), &Settings::MaterialConfig::config.bNoTransparency);
						//	ImGui::Checkbox(XorStr("Software Lighting"), &Settings::MaterialConfig::config.bSoftwareLighting); // Crashes game
						//																									   //AllowCheats ?
						//	ImGui::SliderInt(XorStr("##MIPLEVELS"), (int*)&Settings::MaterialConfig::config.nShowMipLevels, 0, 127, XorStr("ShowMipLevels: %1.f"));
						//	ImGui::Checkbox(XorStr("Show Low-Res Image"), &Settings::MaterialConfig::config.bShowLowResImage);
						//	ImGui::Checkbox(XorStr("Show Normal Map"), &Settings::MaterialConfig::config.bShowNormalMap);
						//	ImGui::Checkbox(XorStr("MipMap Textures"), &Settings::MaterialConfig::config.bMipMapTextures);
						//	ImGui::SliderInt(XorStr("##NFULLBRIGHT"), (int*)&Settings::MaterialConfig::config.nFullbright, 0, 127, XorStr("nFullBright: %1.f"));
						//	ImGui::Checkbox(XorStr("Fast NoBump"), &Settings::MaterialConfig::config.m_bFastNoBump);
						//	ImGui::Checkbox(XorStr("Suppress Rendering"), &Settings::MaterialConfig::config.m_bSuppressRendering);
						//	ImGui::Checkbox(XorStr("Draw Gray"), &Settings::MaterialConfig::config.m_bDrawGray);
						//	ImGui::Checkbox(XorStr("Show Specular"), &Settings::MaterialConfig::config.bShowSpecular);
						//	ImGui::Checkbox(XorStr("Show Defuse"), &Settings::MaterialConfig::config.bShowDiffuse);
						//	ImGui::SliderInt(XorStr("##AAQUALITY"), &Settings::MaterialConfig::config.m_nAAQuality, 0, 16, XorStr("AAQuality: %1.f"));
						//	ImGui::Checkbox(XorStr("Shadow Depth Texture"), &Settings::MaterialConfig::config.m_bShadowDepthTexture);
						//	ImGui::Checkbox(XorStr("Motion Blur"), &Settings::MaterialConfig::config.m_bMotionBlur);
						//	ImGui::Checkbox(XorStr("Support Flashlight"), &Settings::MaterialConfig::config.m_bSupportFlashlight);
						//	ImGui::Checkbox(XorStr("Paint Enabled"), &Settings::MaterialConfig::config.m_bPaintEnabled);
						//	// VRMode Adapter?
						//	ImGui::PopItemWidth();
						//	ImGui::EndPopup();
						//}

						ImGui::NextColumn();
						{
							ImGui::PushItemWidth(-1);
							ImGui::Combo("##ARMSTYPE", (int*)& Settings::ESP::Chams::Arms::type, ArmsTypes, IM_ARRAYSIZE(ArmsTypes));
							ImGui::SliderFloat("##DLIGHTRADIUS", &Settings::Dlights::radius, 0, 1000, "Radius: %0.f");
							ImGui::SliderFloat("##NOFLASHAMOUNT", &Settings::Noflash::value, 0, 255, "Amount: %0.f");
					//		ImGui::SliderInt("##SOUNDSTIME", &Settings::ESP::Sounds::time, 250, 5000, "Timeout: %0.f");
					//		ImGui::SliderFloat("##MINIMALVOLUME", &Settings::ESP::Sounds::MinimumVolume, 0.f, 1.f, "Size: %0.f");
							ImGui::PopItemWidth();
							ImGui::Checkbox("No Aim Punch", &Settings::View::NoAimPunch::enabled);
							SetTooltip("Disables aim punch when shooting");
							ImGui::Checkbox("ASUS Walls", &Settings::ASUSWalls::enabled);
							SetTooltip("Makes wall textures transparent");
							ImGui::Checkbox("No Scope Border", &Settings::NoScopeBorder::enabled);
							SetTooltip("Disables black scope silhouette");
							ImGui::Checkbox(XorStr("AntiAim Infos"), &Settings::AntiAimInfos::enabled);
							ImGui::Checkbox(XorStr("Grenade Prediction"), &Settings::GrenadePrediction::enabled);
							ImGui::Checkbox("Autowall Debug", &Settings::ESP::AutoWall::debugView);
							ImGui::Checkbox(XorStr("AimSpot Debug"), &Settings::ESP::AutoAim::drawTarget);
							ImGui::Checkbox(XorStr("BoneMap Debug"), &Settings::ESP::BoneMap::draw);
							if (Settings::ESP::BoneMap::draw)
								ImGui::Checkbox(XorStr("Just Dots"), &Settings::ESP::BoneMap::justDrawDots);
						}



						ImGui::Columns(1);
						ImGui::Separator();
						ImGui::Text("Other gui");
						ImGui::Separator();
						ImGui::Columns(2, NULL, true);
						{
							ImGui::Checkbox("Spectator list", &Settings::ShowSpectators::enabled);
							SetTooltip("Shows a spectator GUI");
							ImGui::SameLine();
							ImGui::Checkbox("Spotify Controls", &Settings::Spotify::ShowControls);
							SetTooltip("Play, Pause, Next, Previous, Display song name. Might give a vac authentication error!");

						}
						ImGui::NextColumn();
						{
							ImGui::Checkbox("IRC", &Settings::IRC::Enabled);
							SetTooltip("Lets you talk to other x-hook users who are playing");
							ImGui::SameLine();
							ImGui::Checkbox("Show resolver info", &Settings::Resolver::info);
							SetTooltip("Shows Resolver information");
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
							//	ImGui::Checkbox("Names", &Settings::Radar::name);
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
							ImGui::Checkbox("Allies", &Settings::Radar::allies);
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
							ImGui::Checkbox("Damage##HITMARKERS", &Settings::ESP::Hitmarker::Damage::enabled);
							SetTooltip("Show dealt damage next to the hitmarker");
							//ImGui::Checkbox(XorStr("Sounds##HITMARKERS"), &Settings::ESP::Hitmarker::Sounds::enabled);
						}
						ImGui::NextColumn();
						{
							ImGui::PushItemWidth(-1);
							ImGui::SliderInt("##HITMARKERDUR", &Settings::ESP::Hitmarker::duration, 250, 3000, "Timeout: %0.f");
							ImGui::SliderInt("##HITMARKERSIZE", &Settings::ESP::Hitmarker::size, 1, 32, "Size: %0.f");
							ImGui::SliderInt("##HITMARKERGAP", &Settings::ESP::Hitmarker::innerGap, 1, 16, "Gap: %0.f");
							//ImGui::Combo(XorStr("Sounds"), (int*)&Settings::ESP::Hitmarker::Sounds::sound, Sounds, IM_ARRAYSIZE(Sounds));
							ImGui::PopItemWidth();
						}
						ImGui::Columns(1);
						ImGui::Separator();

						ImGui::EndChild();
					}
				}
				ImGui::Columns(1);
				break;
			case 3:
				//HvH::RenderTab();
				break;
			case 4:

				break;
			case 5:

				break;
			case 6:

				//for (int i = 0; i < IM_ARRAYSIZE(colors); i++)
				//	colorNames[i] = colors[i].name;


				//ImGui::SetNextWindowSize(ImVec2(540, 325), ImGuiSetCond_Always);
				//ImGui::Columns(2, NULL, true);
				//{
				//	ImGui::PushItemWidth(-1);
				//	ImGui::ListBox("##COLORSELECTION", &colorSelected, colorNames, IM_ARRAYSIZE(colorNames), 12);
				//	ImGui::PopItemWidth();

				//}
				//ImGui::Image(LogoicoTexture, ImVec2(128, 128));
				//ImGui::NextColumn();
				//{
				//	if (colors[colorSelected].type == ColorListVar::HEALTHCOLORVAR_TYPE)
				//	{
				//		ColorPicker4((float*)colors[colorSelected].healthColorVarPtr);
				//		ImGui::Checkbox("Rainbow", &colors[colorSelected].healthColorVarPtr->rainbow);
				//		SetTooltip("Makes the color an animated rainbow.");
				//		ImGui::SameLine();
				//		ImGui::Checkbox("Health-Based", &colors[colorSelected].healthColorVarPtr->hp);
				//		SetTooltip("Takes color from entity health, i.e. 100 - green, 50 - yellow.");
				//		ImGui::PushItemWidth(-1);
				//		ImGui::SliderFloat("##RAINBOWSPEED", &colors[colorSelected].healthColorVarPtr->rainbowSpeed, 0.f, 1.f, "Rainbow Speed: %0.3f");
				//		ImGui::PopItemWidth();
				//	}
				//	else
				//	{
				//	//	ColorPicker4((float*)colors[colorSelected].colorVarPtr);
				//		ImGui::Checkbox("Rainbow", &colors[colorSelected].colorVarPtr->rainbow);
				//		SetTooltip("Makes the color an animated rainbow.");
				//		ImGui::PushItemWidth(-1);
				//		ImGui::SliderFloat("##RAINBOWSPEED", &colors[colorSelected].colorVarPtr->rainbowSpeed, 0.f, 1.f, "Rainbow Speed: %0.3f");
				//		/*if (ImGui::Button("Setup Folders and Download Default Background"))
				//		{

				//		}*/
				//		ImGui::Checkbox("Background", &Settings::Background::enable);

				///*		SetTooltip("Custom Background! Located at: C://Xhook//pictures//bg.jpg! ONLY ENABLE IF THERE IS A BG LOCATED!");
				//		ImGui::SameLine();
				//		if (ImGui::Button("Reload Background"))
				//		{
				//			if (D3DXCreateTextureFromFile(pD3device, "C://xhook//Pictures//bg.jpg", &BackgroundTexture) != D3D_OK) {
				//				ImGui::OpenPopup("Success###Picture Reloaded!");
				//			}
				//		}
				//		SetTooltip("Reload Background! Located at: C://Xhook//pictures//bg.jpg");
				//		ImGui::SameLine();
				//		if (ImGui::Button("Delete Background"))
				//		{
				//			if (D3DXCreateTexture(pD3device, 1080, 720, 0, 0, D3DFMT_A8R8G8B8, D3DPOOL_MANAGED, &BackgroundTexture) != D3D_OK) {
				ImGui::OpenPopup("Success###Picture loaded!");
			
		
			//	}*/

			//ImGui::PopItemWidth();
		
		break;
	case 7:
		//	Skins::RenderTab();

		break;
	case 8:
		//	Models::RenderTab();
		break;
	case 9:
		//HWID INFO And techinal info 

		/*

		for (UINT i = 0; i < GRAPH_ARRAY_SIZE; i++)
		{
		ms_array[i] = ms_array[i + 1];
		}

		fps_array[GRAPH_ARRAY_SIZE - 1] = ImGui::GetIO().Framerate;
		char fps_title[25];
		sprintf_s(fps_title, 25, "Framerate %.1f", fps_array[GRAPH_ARRAY_SIZE - 1]);
		ImGui::PlotHistogram("", fps_array, IM_ARRAYSIZE(fps_array), 30, fps_title, 0.0f, 130.0f, ImVec2(0, 80));

		//MS Graph
		for (UINT i = 0; i < GRAPH_ARRAY_SIZE; i++)
		{
		ms_array[i] = ms_array[i + 1];
		}
		ms_array[GRAPH_ARRAY_SIZE - 1] = 1000.0f / ImGui::GetIO().Framerate;
		char ms_title[25];
		sprintf_s(ms_title, 25, "Milliseconds %.1f", ms_array[GRAPH_ARRAY_SIZE - 1]);
		ImGui::PlotHistogram("", ms_array, IM_ARRAYSIZE(ms_array), 30, ms_title, 0.0f, 130.0f, ImVec2(0, 80));

		*/
		break;
	case 10:
		//	ImGui::ImageButton(CopyrightLogo, ImVec2(50, 50), ImVec2(0, 0), ImVec2(1, 1), 2);
		// License system. Still have to fix the c++ loader php isue
		break;
	}
}
ImGui::End();

ImGui::Render();




//// 1. Show a simple window
//// Tip: if we don't call ImGui::Begin()/ImGui::End() the widgets appears in a window automatically called "Debug"
//{
//    static float f = 0.0f;
//    ImGui::Text("Hello, world!");
//    ImGui::SliderFloat("float", &f, 0.0f, 1.0f);
//    ImGui::ColorEdit3("clear color", (float*)&clear_col);
//    if (ImGui::Button("Test Window")) show_test_window ^= 1;
//    if (ImGui::Button("Another Window")) show_another_window ^= 1;
//    ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
//}


// 2. Show another simple window, this time using an explicit Begin/End pair


//// 3. Show the ImGui test window. Most of the sample code is in ImGui::ShowTestWindow()
//if (show_test_window)
//{
//    ImGui::SetNextWindowPos(ImVec2(650, 20), ImGuiSetCond_FirstUseEver);
//    ImGui::ShowTestWindow(&show_test_window);
//}

// Rendering
g_pd3dDevice->SetRenderState(D3DRS_ZENABLE, false);
g_pd3dDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, false);
g_pd3dDevice->SetRenderState(D3DRS_SCISSORTESTENABLE, false);
D3DCOLOR clear_col_dx = D3DCOLOR_RGBA((int)(clear_col.x*255.0f), (int)(clear_col.y*255.0f), (int)(clear_col.z*255.0f), (int)(clear_col.w*255.0f));
g_pd3dDevice->Clear(0, NULL, D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER, clear_col_dx, 1.0f, 0);
if (g_pd3dDevice->BeginScene() >= 0)
{
	ImGui::Render();
	g_pd3dDevice->EndScene();
}
g_pd3dDevice->Present(NULL, NULL, NULL, NULL);
	}

	ImGui_ImplDX9_Shutdown();
	if (g_pd3dDevice) g_pd3dDevice->Release();
	if (pD3D) pD3D->Release();
	UnregisterClass(_T("ImGui Example"), wc.hInstance);

	return 0;
}
