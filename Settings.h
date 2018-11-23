#pragma once
#include <Windows.h>
#include <unordered_map>
#include <fstream>
#include <vector>
#include <sys/types.h>
#include <sys/stat.h>
#include <stdlib.h>
#include "json.h"
#include "SDK/SDK.h"
#include "SDK\Definitions.h"
#include "ImGui\imgui.h"
#include "Util.h"
#include "Interfaces.h"
#include "materialsystem_config.h"

namespace G // Global Stuff
{
	extern CUserCmd*	UserCmd;
	extern CUserCmd* UserCmdForBacktracking;
	extern INetChannel* test;
	extern C_BasePlayer* LocalPlayer;
	extern bool PressedKeys[256];
	extern bool input_shouldListen;
	extern bool init;
	extern bool is_renderer_active;
	extern const char* text;
	extern float FakeAngle;
	extern ITexture* m_texture_render_target;
	//extern ModelRenderInfo_t& pInfo;
}

enum class SmoothType : int
{
	SLOW_END,
	CONSTANT,
	FAST_END,
};

enum class  TracerType : int
{
	BOTTOM,
	CURSOR,
};

enum class ClanTagType : int
{
	STATIC,
	CURTIME,
	MARQUEE,
	WORDS,
	LETTERS,
};

enum class valueType : int {
	XHOOK,
	Dogeii_Gang,
	CUSTOM,
};
enum class AutostrafeType : int
{
	AS_FORWARDS,
	AS_BACKWARDS,
	AS_LEFTSIDEWAYS,
	AS_RIGHTSIDEWAYS,
	AS_RAGE,
};

enum class AntiAimType_Y : int {

	NOAA,
	FORWARDS,
	BACKWARDS,
	SPIN,
	FAKESIDEWAYS,
	BACKJITTER,
	BACKJITTER2,
	FJITTER,
	KIDUA,
	LBYJITTER,
	LBYSPIN,
	LOWERBODY,
	LBYBREAK,
	FAKELBY,
	LBYONGROUND,
	MASTERLOOSER,
	TANK1,
	TANK2,
	TANK3,
	PAAFAKE,
	PAAREAL,
	MYRRIB,
	richieap,
	RASP,
	RASP2,
	FEETWIGGLE,
	LEGITTROLLING,
	LEGITTROLLING2,
	CUSTOM,
	CUSTOM2,

	NUMBER_OF_TYPES // Leave at bottom
};

enum class AntiAimType_LBY : int {
	ONE,
	TWO,
	THREE,
	FOUR,
	MYRRIB,
	MYRRIB2,
	SUICIDE,
	NONE,

	NUMBER_OF_TYPES // Leave at bottom
};

enum class AntiAimType_Z : int {
	TEST,

	NUMBER_OF_TYPES // Leave at bottom
};

enum class AntiAimType_X : int {
	STATIC_UP,
	FLIP,
	STATIC_DOWN,
	FAKEZERO,
	FAKEUP,

	NUMBER_OF_TYPES // Leave at bottom
};


enum class ChamsType : int {
	CHAMS,
	CHAMS_XQZ,
	CHAMS_FLAT,
	CHAMS_FLAT_XQZ,
	CHAMS_GLASS,
	CHAMS_CRYSTAL,
	CHAMS_CRYSTALBLUE,
	CHAMS_GOLD,
	CHAMS_VELVET,
	CHAMS_TREE,
	CHAMS_SPEECHINFO,
	CHAMS_FISHNET,
	CHAMS_LETTERS,
	CHAMS_GLOSS,
	CHAMS_ANIMATED,
	CHAMS_BLINK,
	CHAMS_GLOW,
	CHAMS_GUERILLA,
};

enum class ThirdPersonMode : int {
	FAKE,
	REAL,
	LBY,

	NUMBER_OF_TYPES // Leave at bottom
};

enum class BoxType : int
{
	FLAT_2D,
	FRAME_2D,
	BOX_3D,
	HITBOX,
};

enum class BarType : int
{
	VERTICAL,
	VERTICAL_RIGHT,
	HORIZONTAL,
	HORIZONTAL_UP,
	INTERWEBZ,
};

enum class BarColorType : int
{
	STATIC_COLOR,
	HEALTH_BASED,
};

enum class TeamColorType : int
{
	ABSOLUTE_,
	RELATIVE_,
};

enum class WeaponType : int {
	DEFAULT,
	WIREFRAME,
	NONE,
};

enum class ArmsType : int {
	DEFAULT,
	GLASS,
	CRYSTAL,
	CRYSTALBLUE,
	GOLD,
	VELVET,
	TREE,
	SPEECHINFO,
	FISHNET,
	LETTERS,
	GLOSS,
	WIREFRAME,
	NONE,
};
enum class AimTargetType : int
{
	FOV,
	DISTANCE,
	REAL_DISTANCE,
	HP,
};

enum class FakeLagType : int {
	OFF,
	TUX,
	KMETH,
	STEP,
	REACTIVE,
	NUCLEAR,
	LUNICO,
};

enum class Radio : int {
	NONE,
	Normal,
	Bombsounds,
};

enum class Sound : int {

	NONE,
	METALDOOR2,
	DOORSTOP,
	METAL,
	DONTLEAVE,
	THUNDER,
	BEEP,
	BASS,
	SK00TER,
	Medkit,
};

enum class SpammerType : int
{
	SPAMMER_NONE,
	SPAMMER_NORMAL,
	SPAMMER_POSITIONS,
};



struct AimbotWeapon_t
{
	bool enabled, silent, pSilent, backtrack, friendly, closestBone, desiredBones[31], engageLock, engageLockTR;
	int engageLockTTR, hitChanceRays;
	Bone bone;
	SmoothType smoothType;
	ButtonCode_t aimkey;
	bool aimkeyOnly, smoothEnabled, smoothSaltEnabled, errorMarginEnabled, curveEnabled, autoAimEnabled, aimStepEnabled, rcsEnabled, rcsAlwaysOn, spreadLimitEnabled, spreadLimitDistance;
	float smoothAmount, smoothSaltMultiplier, curveAmount, errorMarginValue, autoAimFov, aimStepMin, aimStepMax, rcsAmountX, rcsAmountY, autoWallValue, spreadLimit, hitChanceValue, smoothvalue;
	bool autoCockRevolver, autoPistolEnabled, autoShootEnabled, autoScopeEnabled, noShootEnabled, ignoreJumpEnabled, smokeCheck, flashCheck, autoWallEnabled, autoAimRealDistance, autoSlow, moveMouse, predEnabled, hitChanceEnabled, velocityCheck, smooth;

	AimbotWeapon_t(bool enabled, bool silent, bool pSilent, bool friendly, bool closestBone, bool engageLock, bool engageLockTR, int engageLockTTR, Bone bone, ButtonCode_t aimkey, bool aimkeyOnly,
		bool smoothEnabled, float smoothValue, SmoothType smoothType, bool smoothSaltEnabled, float smoothSaltMultiplier,
		bool errorMarginEnabled, float errorMarginValue,
		bool curveEnabled, float curveAmount,
		bool autoAimEnabled, float autoAimValue, bool aimStepEnabled, float aimStepMin, float aimStepMax,
		bool rcsEnabled, bool rcsAlwaysOn, float rcsAmountX, float rcsAmountY, //23
		bool autoCockRevolver, bool autoPistolEnabled, bool autoShootEnabled, bool autoScopeEnabled,
		bool noShootEnabled, bool ignoreJumpEnabled, bool smokeCheck, bool flashCheck, // 8 na RCS
		bool spreadLimitEnabled, bool spreadLimitDistance, float spreadLimit,
		bool autoWallEnabled, float autoWallValue, bool autoAimRealDistance, bool autoSlow,
		bool moveMouse, bool predEnabled, bool backtrack, bool hitChanceEnabled, int hitChanceRays, float hitChanceValue, bool velocityCheck, bool smooth,
		float smoothvalue) {
	
		this->enabled = enabled;
		this->silent = silent;
		this->pSilent = pSilent;
		this->backtrack = backtrack;
		this->friendly = friendly;
		this->closestBone = closestBone;
		this->engageLock = engageLock;
		this->engageLockTR = engageLockTR;
		this->engageLockTTR = engageLockTTR;
		this->bone = bone;
		this->aimkey = aimkey;
		this->aimkeyOnly = aimkeyOnly;
		this->smoothEnabled = smoothEnabled;
		this->smoothAmount = smoothValue;
		this->smoothType = smoothType;
		this->smoothSaltEnabled = smoothSaltEnabled;
		this->smoothSaltMultiplier = smoothSaltMultiplier;
		this->errorMarginEnabled = errorMarginEnabled;
		this->errorMarginValue = errorMarginValue;
		this->curveEnabled = curveEnabled;
		this->curveAmount = curveAmount;
		this->autoAimEnabled = autoAimEnabled;
		this->autoAimFov = autoAimValue;
		this->aimStepEnabled = aimStepEnabled;
		this->aimStepMin = aimStepMin;
		this->aimStepMax = aimStepMax;
		this->rcsEnabled = rcsEnabled;
		this->rcsAlwaysOn = rcsAlwaysOn;
		this->rcsAmountX = rcsAmountX;
		this->rcsAmountY = rcsAmountY;
		this->autoCockRevolver = autoCockRevolver;
		this->autoPistolEnabled = autoPistolEnabled;
		this->autoShootEnabled = autoShootEnabled;
		this->autoScopeEnabled = autoScopeEnabled;
		this->noShootEnabled = noShootEnabled;
		this->ignoreJumpEnabled = ignoreJumpEnabled;
		this->smokeCheck = smokeCheck;
		this->flashCheck = flashCheck;
		this->spreadLimitEnabled = spreadLimitEnabled;
		this->spreadLimitDistance = spreadLimitDistance;
		this->spreadLimit = spreadLimit;
		this->autoWallEnabled = autoWallEnabled;
		this->autoWallValue = autoWallValue;
		this->autoSlow = autoSlow;
		this->predEnabled = predEnabled;
		this->moveMouse = moveMouse;
		this->hitChanceEnabled = hitChanceEnabled;
		this->hitChanceRays = hitChanceRays;
		this->hitChanceValue = hitChanceValue;
		this->velocityCheck = velocityCheck;
		this->smooth = smooth;
		this->smoothvalue = smoothvalue;
		

		for (int bone = (int)DesiredBones::BONE_PELVIS; bone <= (int)DesiredBones::BONE_RIGHT_SOLE; bone++)
			this->desiredBones[bone] = desiredBones[bone];

		this->autoAimRealDistance = autoAimRealDistance;
	}

	AimbotWeapon_t() {
	};

	bool operator==(const AimbotWeapon_t& another) const {
		for (int bone = (int)DesiredBones::BONE_PELVIS; bone <= (int)DesiredBones::BONE_RIGHT_SOLE; bone++) {
			if (this->desiredBones[bone] != another.desiredBones[bone])
				return false;
		}

		return this->enabled == another.enabled &&
			this->silent == another.silent &&
			this->pSilent == another.pSilent &&
			this->backtrack == another.backtrack &&
			this->friendly == another.friendly &&
			this->closestBone == another.closestBone &&
			this->engageLock == another.engageLock &&
			this->engageLockTR == another.engageLockTR &&
			this->engageLockTTR == another.engageLockTTR &&
			this->bone == another.bone &&
			this->aimkey == another.aimkey &&
			this->aimkeyOnly == another.aimkeyOnly &&
			this->smoothEnabled == another.smoothEnabled &&
			this->smoothAmount == another.smoothAmount &&
			this->smoothType == another.smoothType &&
			this->smoothSaltEnabled == another.smoothSaltEnabled &&
			this->smoothSaltMultiplier == another.smoothSaltMultiplier &&
			this->errorMarginEnabled == another.errorMarginEnabled &&
			this->errorMarginValue == another.errorMarginValue &&
			this->autoAimEnabled == another.autoAimEnabled &&
			this->autoAimFov == another.autoAimFov &&
			this->aimStepEnabled == another.aimStepEnabled &&
			this->aimStepMin == another.aimStepMin &&
			this->aimStepMax == another.aimStepMax &&
			this->rcsEnabled == another.rcsEnabled &&
			this->rcsAlwaysOn == another.rcsAlwaysOn &&
			this->rcsAmountX == another.rcsAmountX &&
			this->rcsAmountY == another.rcsAmountY &&
			this->autoCockRevolver == another.autoCockRevolver &&
			this->autoPistolEnabled == another.autoPistolEnabled &&
			this->autoShootEnabled == another.autoShootEnabled &&
			this->autoScopeEnabled == another.autoScopeEnabled &&
			this->noShootEnabled == another.noShootEnabled &&
			this->ignoreJumpEnabled == another.ignoreJumpEnabled &&
			this->smokeCheck == another.smokeCheck &&
			this->flashCheck == another.flashCheck &&
			this->spreadLimitEnabled == another.spreadLimitEnabled &&
			this->spreadLimitDistance == another.spreadLimitDistance &&
			this->spreadLimit == another.spreadLimit &&
			this->autoWallEnabled == another.autoWallEnabled &&
			this->autoWallValue == another.autoWallValue &&
			this->autoSlow == another.autoSlow &&
			this->predEnabled == another.predEnabled &&
			this->moveMouse == another.moveMouse &&
			this->autoAimRealDistance == another.autoAimRealDistance&&
			this->hitChanceEnabled == another.hitChanceEnabled &&
			this->hitChanceRays == another.hitChanceRays &&
			this->hitChanceValue == another.hitChanceValue &&
			this->velocityCheck == another.velocityCheck;
	}
};


class ColorVar
{
public:
	ImColor color;
	bool rainbow;
	float rainbowSpeed;

	ColorVar() {}

	ColorVar(ImColor color)
	{
		this->color = color;
		this->rainbow = false;
		this->rainbowSpeed = 0.5f;
	}

	ImColor Color()
	{
		ImColor result = this->rainbow ? Util::GetRainbowColor(this->rainbowSpeed) : this->color;
		result.Value.w = this->color.Value.w;
		return result;
	}
};

class HealthColorVar : public ColorVar
{
public:
	bool hp;

	HealthColorVar(ImColor color)
	{
		this->color = color;
		this->rainbow = false;
		this->rainbowSpeed = 0.5f;
		this->hp = false;
	}

	ImColor Color(C_BasePlayer* player)
	{
		ImColor result = this->rainbow ? Util::GetRainbowColor(this->rainbowSpeed) : (this->hp ? Color::ToImColor(Util::GetHealthColor(player)) : this->color);
		result.Value.w = this->color.Value.w;
		return result;
	}
};

namespace Settings
{


	namespace TracerEffects
	{
		extern bool enabled;
		extern bool serverSide;
		extern TracerEffects_t effect;
		extern int frequency;
	}

	namespace WalkBot
	{
		extern bool enabled;
		extern bool plantbomb;
		extern bool forceReset;
		extern bool autobuy;
		extern int autobuyAt;
		extern int marker;
		extern std::string actMapName;
		extern ButtonCode_t Key;
	}

	namespace AutoKnife
	{
		extern bool enabled;
		extern bool onKey;

		namespace Filters
		{
			extern bool enemies;
			extern bool allies;
		}
	}

	namespace Background
	{
		extern bool enable;
	}

	namespace HWID
	{	
		extern bool LoggedIn;
		extern bool Enabled;
		extern char Message[255];
		extern bool uninject;
	}

	namespace CONFIGHANDLER
	{
	//	extern void HandleConfigs();
		//extern bool AimbotWeaponConfigs;
		extern int CurrentConfig;
	}

	namespace UI
	{

		extern ColorVar mainColor;
		extern ColorVar bodyColor;
		extern ColorVar fontColor;
		extern ColorVar accentColor;

		extern bool MainUI;	
		extern bool Pie;
		extern bool middle;
		extern bool right;




		namespace Windows {
			namespace Config {
				extern int posX;
				extern int posY;
				extern int sizeX;
				extern int sizeY;
				extern bool open;
				extern bool reload; // True on config load, used to change Window Position.
			}
		}

		namespace Watermark {
			extern ColorVar color;
			extern bool displayIngame;
			extern bool showwatermark;
		}

		namespace Fonts
		{
			namespace ESP
			{
				//extern char* family;
				//extern int size;
				//extern int flags;
			}
		}
	}


	namespace Aimbot {
		extern bool enabled;
		extern bool silent;
		extern bool pSilent;
		extern bool backtrack;
		extern bool LegitBackTrack;
		extern bool friendly;
		extern Bone bone;
		extern ButtonCode_t aimkey;
		extern bool aimkeyOnly;
		extern bool moveMouse;
		extern bool legitMode;
		extern int FakeLatencyMode;
		extern bool NoSpread;


		namespace Smooth {
			extern bool enabled;
			extern float value;
			extern SmoothType type;

			namespace Salting {
				extern bool enabled;
				extern float multiplier;
			}
		}

		namespace ErrorMargin
		{
			extern bool enabled;
			extern float value;
		}

		namespace AutoAim
		{
			extern bool enabled;
			extern float fov;
			extern int test;
			extern bool realDistance;
			extern bool closestBone;
			extern bool desiredBones[];
			extern bool engageLock;
			extern bool engageLockTR;
			extern int engageLockTTR;
			extern bool PsuedoSpread;
		//	extern int afterAim;
			extern int aftershots;
			extern Bone boneBEFOREandAFTER;
			extern Bone bonePreAim;
		}

		namespace AutoWall
		{
			extern bool enabled;
			extern float value;
			extern bool bones[];
			extern bool AutoWallType;
		}

		namespace AimStep
		{
			extern bool enabled;
			extern float min;
			extern float max;
		
		}

		namespace RCS
		{
			extern bool enabled;
			extern bool always_on;
			extern float valueX;
			extern float valueY;
			extern bool smooth;
			extern float smoothvalue;
		}

		namespace AutoPistol
		{
			extern bool enabled;
		}

		namespace AutoCockRevolver
		{
			extern bool enabled;
		}


		namespace AutoShoot
		{
			extern bool enabled;
			extern bool velocityCheck;
			extern bool autoscope;
		}

		namespace AutoCrouch
		{
			extern bool enabled;
		}

		namespace AutoSlow
		{
			extern bool goingToSlow;
			extern bool enabled;
			extern float minDamage;
		}

		namespace NoShoot
		{
			extern bool enabled;
		}

		namespace IgnoreJump
		{
			extern bool enabled;
		}

		namespace SmokeCheck
		{
			extern bool enabled;
		}

		namespace FlashCheck
		{
			extern bool enabled;
		}

		namespace SpreadLimit
		{
			extern bool enabled;
			extern bool distanceBased;
			extern float value;
		}

		namespace HitChance
		{
			extern bool enabled;
			extern int hitRays;
			extern float value;
		}

		namespace Prediction
		{
			extern bool enabled;
		}

		namespace Curve
		{
			extern bool enabled;
			extern float value;
		}

		extern std::unordered_map<ItemDefinitionIndex, AimbotWeapon_t, Util::IntHash<ItemDefinitionIndex>> weapons;
	}

	namespace Triggerbot
	{
		extern bool enabled;
		extern ButtonCode_t key;

		namespace Filters
		{
			extern bool enemies;
			extern bool allies;
			extern bool walls;
			extern bool smokeCheck;
			extern bool flashCheck;
			extern bool head;
			extern bool chest;
			extern bool stomach;
			extern bool arms;
			extern bool legs;
		}

		namespace RandomDelay
		{
			extern bool enabled;
			extern int lowBound; // in ms
			extern int highBound;// in ms
			extern int lastRoll;
		}

	}

	namespace IRC
	{
		extern bool Enabled;
		extern char Message[255];
	}

	namespace AntiAim
	{
		namespace Moving {
			namespace Yaw {
				extern bool enabled;
				extern AntiAimType_Y type;
				extern AntiAimType_Y typeFake;
				extern float typeAdd;
				extern float typeFakeAdd;
			}

			namespace Pitch {
				extern bool enabled;
				extern AntiAimType_X type;
				extern float custom;
			}

			namespace Roll {
				extern bool enabled;
				extern AntiAimType_Z type;
			}

			namespace SwitchAA {
				extern bool enabled;
				extern ButtonCode_t key;
			}

			namespace LBY {
				extern bool enabled;
				extern AntiAimType_LBY type;
			}

			namespace HeadEdge {
				extern bool enabled;
				extern float realAdd;
				extern float fakeAdd;
				extern float distance;
			}

			extern bool antiResolver;
			extern bool dynamicAA;
			extern bool untrustedAngles;
		}

		namespace Standing {
			namespace Yaw {
				extern bool enabled;
				extern AntiAimType_Y type;
				extern AntiAimType_Y typeFake;
				extern float typeAdd;
				extern float typeFakeAdd;
			}

			namespace Pitch {
				extern bool enabled;
				extern AntiAimType_X type;
				extern float custom;
			}

			namespace Roll {
				extern bool enabled;
				extern AntiAimType_Z type;
			}

			namespace SwitchAA {
				extern bool enabled;
				extern ButtonCode_t key;
			}

			namespace LBY {
				extern bool enabled;
				extern AntiAimType_LBY type;
			}

			namespace HeadEdge {
				extern bool enabled;
				extern float realAdd;
				extern float fakeAdd;
				extern float distance;
			}

			extern bool antiResolver;
			extern bool dynamicAA;
			extern bool untrustedAngles;
		}

		namespace Airborne {
			namespace Yaw {
				extern bool enabled;
				extern AntiAimType_Y type;
				extern AntiAimType_Y typeFake;
				extern float typeAdd;
				extern float typeFakeAdd;
			}

			namespace Pitch {
				extern bool enabled;
				extern AntiAimType_X type;
				extern float custom;
			}

			namespace Roll {
				extern bool enabled;
				extern AntiAimType_Z type;
			}

			namespace SwitchAA {
				extern bool enabled;
				extern ButtonCode_t key;
			}

			namespace LBY {
				extern bool enabled;
				extern AntiAimType_LBY type;
			}

			namespace HeadEdge {
				extern bool enabled;
				extern float realAdd;
				extern float fakeAdd;
				extern float distance;
			}

			extern bool antiResolver;
			extern bool dynamicAA;
			extern bool untrustedAngles;
		}

		namespace Misc {
			extern bool LegitAA;
			namespace AutoDisable {
				extern bool noEnemy;
				extern bool knifeHeld;
				extern bool bombHeld;
				extern bool freezeTime;
			}
		}
	

	}



	namespace Resolver
	{
		extern bool AnimResolve;
		extern bool LagComp;
		extern bool enabled;
		extern bool info;
		extern float ticks;
		extern float modulo;
		extern bool pitch;
		extern bool lbyOnly;
		extern bool angleFlipEnabled;
		extern ButtonCode_t angleFlip;
		extern int baimAfter;
		extern bool ResolverOverride;
		extern ButtonCode_t ResolverOverrideKey;
	}

	namespace AngleIndicator {

		extern bool enabled;
		extern bool Veloc;

	}

	namespace lbyindicator {
		extern bool enabled;
	}
	namespace SpeedIndicator {
		extern bool enabled;
	}

	namespace ESP
	{
		extern bool enabled;
		extern bool AntiOBS;
		extern ButtonCode_t key;
		extern TeamColorType teamColorType;
		extern HealthColorVar enemyColor;
		extern HealthColorVar allyColor;
		extern HealthColorVar enemyVisibleColor;
		extern HealthColorVar allyVisibleColor;
		extern HealthColorVar ctColor;
		extern HealthColorVar tColor;
		extern HealthColorVar ctVisibleColor;
		extern HealthColorVar tVisibleColor;
		extern ColorVar bombColor;
		extern ColorVar bombDefusingColor;
		extern ColorVar hostageColor;
		extern ColorVar defuserColor;
		extern ColorVar weaponColor;
		extern ColorVar chickenColor;
		extern ColorVar fishColor;
		extern ColorVar smokeColor;
		extern ColorVar decoyColor;
		extern ColorVar flashbangColor;
		extern ColorVar grenadeColor;
		extern ColorVar molotovColor;
		extern HealthColorVar localplayerColor;


		namespace Test
		{
			extern bool testfunction1;
			extern bool testfunction2;
			extern bool testfunction3;
		}

		namespace Glow
		{
			extern bool enabled;
			extern HealthColorVar allyColor;
			extern HealthColorVar enemyColor;
			extern HealthColorVar enemyVisibleColor;
			extern ColorVar weaponColor;
			extern ColorVar grenadeColor;
			extern ColorVar defuserColor;
			extern ColorVar chickenColor;
			extern HealthColorVar localplayerColor;
		}

		namespace AutoWall
		{
			extern bool enabled;
			extern float value;
			extern bool bones[];
			extern bool debugView;
		}

		namespace AutoAim
		{
			extern bool drawTarget;
			extern Vector target;
		}
		namespace BoneMap
		{
			extern bool draw;
			extern bool justDrawDots;
		}

		namespace Filters
		{
			extern bool legit;
			extern bool visibilityCheck;
			extern bool smokeCheck;
			extern bool flashCheck;
			extern bool enemies;
			extern bool allies;
			extern bool bomb;
			extern bool hostages;
			extern bool defusers;
			extern bool weapons;
			extern bool chickens;
			extern bool fishes;
			extern bool throwables;
			extern bool localplayer;
		}

		namespace Info
		{
			extern bool name;
			extern bool clan;
			extern bool steamId;
			extern bool rank;
			extern bool health;
			extern bool weapon;
			extern bool Ammo;
			extern bool scoped;
			extern bool reloading;
			extern bool flashed;
			extern bool planting;
			extern bool hasDefuser;
			extern bool defusing;
			extern bool distance;
			extern bool grabbingHostage;
			extern bool rescuing;
			extern bool location;
			extern bool lby;
			extern bool resolveInfo;
			extern bool showxhookcopyright;
			extern bool FPS;
		}

		namespace PlayerIndicator
		{
			extern bool enabled;
		}

		namespace Skeleton
		{
			extern bool enabled;
			extern ColorVar color;
		}

		

		namespace SkeletonBacktrack
		{
			extern bool enabled;
			extern ColorVar color;
		}

		namespace Boxes
		{
			extern bool enabled;
			extern BoxType type;
		}

		namespace Bars
		{
			extern bool enabled;
			extern BarType type;
			extern BarColorType colorType;
		}

		namespace Tracers
		{
			extern bool enabled;
			extern TracerType type;
		}

		namespace BulletTracers
		{
			extern bool enabled;
		}

		namespace Bomb
		{
			extern bool enabled;
		}

		namespace FOVCrosshair
		{
			extern bool enabled;
			extern bool filled;
			extern ColorVar color;
		}

		namespace Chams
		{
			extern bool enabled;
			extern HealthColorVar allyColor;
			extern HealthColorVar allyVisibleColor;
			extern HealthColorVar allyBacktrackColor;
			extern HealthColorVar allyBacktracVisibleColor;
			extern HealthColorVar enemyColor;
			extern HealthColorVar enemyVisibleColor;
			extern HealthColorVar enemyBacktrackColor;
			extern HealthColorVar enemyBacktrackVisibleColor;
			extern HealthColorVar localplayerColor;

			extern ChamsType type;

			namespace Arms {
				extern bool enabled;
				extern ColorVar color;
				extern ArmsType type;
			}

			namespace Weapon {
				extern bool enabled;
				extern bool DroppedWeapons;
				extern ColorVar color;
				extern WeaponType type;
			}
		}


		

		namespace Sounds
		{
			extern bool enabled;
			extern int time;
			extern float MinimumVolume;
		}

		namespace Hitmarker
		{
			extern bool enabled;
			extern bool enemies;
			extern bool allies;
			extern ColorVar color;
			extern int duration;
			extern int size;
			extern int innerGap;

			namespace Damage
			{
				extern bool enabled;
			}

			namespace Sounds {
				extern bool enabled;
				extern Sound sound;
			}
		}

		namespace HeadDot
		{
			extern bool enabled;
			extern float size;
		}
		namespace Spread
		{
			extern bool enabled; // show current spread
			extern bool spreadLimit; // show spreadLimit value
			extern ColorVar color;
			extern ColorVar spreadLimitColor;
		}
	}

	namespace Dlights
	{
		extern bool enabled;
		extern float radius;
	}

	namespace MaterialConfig {
		extern bool enabled;
		extern MaterialSystem_Config_t config;
	}

	namespace Spammer
	{

		extern SpammerType type;
		extern bool say_team;

		namespace KillSpammer
		{
			extern bool enabled;
			extern bool enabledSync;
			extern bool enableCustomSounds;
			extern bool enabledHurt;
			extern bool sayTeam;
			extern std::vector<std::string> messages;
		}

		namespace RadioSpammer
		{
			extern Radio radio;

		}

		namespace NormalSpammer
		{
			extern std::vector<std::string> messages;
		}

		namespace PositionSpammer
		{
			extern int team;
			extern bool showName;
			extern bool showWeapon;
			extern bool showRank;
			extern bool showWins;
			extern bool showHealth;
			extern bool showMoney;
			extern bool showLastplace;
		}
	}

	namespace BHop
	{
		extern bool enabled;

		namespace Chance
		{
			extern bool enabled;
			extern int value;
		}
		namespace Hops
		{
			extern bool enabledMax;
			extern int Max;
			extern bool enabledMin;
			extern int Min;
		}
	}

	namespace AutoStrafe
	{
		extern bool enabled;
		extern AutostrafeType type;
		extern bool silent;
	}

	namespace Noflash
	{
		extern bool enabled;
		extern float value;
	}

	namespace FOVChanger
	{
		extern bool enabled;
		extern bool viewmodelEnabled;
		extern float value;
		extern float viewmodelValue;
		extern bool ignoreScope;
	}

	namespace Radar
	{
		extern bool enabled;
		extern float zoom;
		extern bool enemies;
		extern bool allies;
		extern bool name;
		extern bool bomb;
		extern bool defuser;
		extern bool legit;
		extern bool visibilityCheck;
		extern bool smokeCheck;
		extern TeamColorType teamColorType;
		extern HealthColorVar enemyColor;
		extern HealthColorVar enemyVisibleColor;
		extern HealthColorVar allyColor;
		extern HealthColorVar allyVisibleColor;
		extern HealthColorVar tColor;
		extern HealthColorVar tVisibleColor;
		extern HealthColorVar ctColor;
		extern HealthColorVar ctVisibleColor;
		extern ColorVar bombColor;
		extern ColorVar bombDefusingColor;
		extern ColorVar defuserColor;
		extern float iconsScale;

		namespace InGame
		{
			extern bool enabled;
		}
	}

	namespace Recoilcrosshair
	{
		extern bool enabled;
		extern bool showOnlyWhenShooting;
	}

	namespace Airstuck
	{
		extern bool enabled;
		extern ButtonCode_t key;
	}

	namespace FakewalkAW {
		extern bool enabled;
		extern ButtonCode_t key;
	}
	namespace Fakewalk {
		extern bool enabled;
		extern ButtonCode_t key;
	}
	namespace ChokePackets {
		extern bool enabled;
		extern ButtonCode_t key;
	}

	namespace Autoblock
	{
		extern bool enabled;
		extern ButtonCode_t key;
	}

	namespace Skinchanger
	{
		namespace Skins
		{
			extern bool enabled;
			extern bool perTeam;
			extern bool stickersenabled;
			extern int sticker1;
			extern int sticker2;
			extern int sticker3;
			extern int sticker4;
		}

		namespace Models
		{
			extern bool Gloves;
			extern bool enabled;
		}

		extern std::unordered_map<ItemDefinitionIndex, AttribItem_t, Util::IntHash<ItemDefinitionIndex>> skinsCT;
		extern std::unordered_map<ItemDefinitionIndex, AttribItem_t, Util::IntHash<ItemDefinitionIndex>> skinsT;
	}

	namespace SmartAim {
		extern bool enabled;
	}

	namespace ShowRanks
	{
		extern bool enabled;
	}

	namespace Misc
	{
		extern int misc_Rank;
		extern int  misc_Level;
		extern int  misc_XP;
		extern int  misc_Wins;
		extern int  misc_Friendly;
		extern int  misc_Leader;
		extern int misc_Teacher;
	}

	namespace InventoryChanger
	{
		extern bool enabled;
		extern bool medalsEnabled;
		extern std::vector<uint32_t> medals; // MEDAL IDS
		extern bool equipped_medal_override;
		extern uint32_t equipped_medal;
	}

	namespace ShowSpectators
	{
		extern bool enabled;
	}

	namespace ClanTagChanger
	{
		extern char value[30];
		extern valueType preset;
		extern bool animation;
		extern int animationSpeed;
		extern bool enabled;
		extern ClanTagType type;
	}

	namespace View
	{
		namespace NoAimPunch
		{
			extern bool enabled;
		}

		namespace NoViewPunch
		{
			extern bool enabled;
		}
	}

	//namespace Teleport
	//{
	//	extern bool enabled;
	//	extern ButtonCode_t key;
	//}

	namespace FakeLag {
		extern FakeLagType type;
		extern int value;
	}
	namespace spinFactor {
		extern int value;
	}
	namespace customYaw {
		extern int value;
		extern bool lby;
	}
	namespace customYaw2 {
		extern int value;
		extern bool lby;
	}

	namespace FakeWalk
	{
		extern bool enabled;
	}
	namespace AutoAccept
	{
		extern bool enabled;
	}

	namespace NoSky
	{
		extern bool enabled;
		extern ColorVar color;
	}

	namespace NoFog {
		extern bool enabled;
	}

	namespace SkyBox
	{
		extern bool enabled;
		extern int skyBoxNumber; // number in skyBoxNames
	}
	namespace ASUSWalls
	{
		extern bool enabled;
		extern ColorVar color;
	}

	namespace NoScopeBorder
	{
		extern bool enabled;
	}

	namespace SniperCrosshair
	{
		extern bool enabled;
		extern ColorVar color;
	}

	namespace AutoDefuse
	{
		extern bool enabled;
		extern bool silent;
	}

	namespace Console
	{
		extern bool enabled;
	}

	/*namespace Spotify
	{
		extern bool ShowControls;
		extern bool ShowCurrentlyPlaying;
	}*/

	namespace NoSmoke
	{
		extern bool enabled;
	}

	namespace ScreenshotCleaner
	{
		extern bool enabled;
	}

	namespace EdgeJump
	{
		extern bool enabled;
		extern ButtonCode_t key;
	}

	namespace SlowMo {
		extern bool enabled;
		extern ButtonCode_t key;
	}

	namespace CircleStrafe
	{
		extern bool enabled;
		extern ButtonCode_t key;
	}

	namespace MoonWalk
	{
		extern bool enabled;
		extern ButtonCode_t key;
	}

	/*
	namespace AntiAimInfos
	{
		extern bool enabled;
	}

	namespace AngleHelper
	{
		extern bool enabled;
		extern ButtonCode_t key;
		extern ButtonCode_t key2;
		extern ButtonCode_t key3;
		extern ButtonCode_t key4;
	}
	*/
	
	namespace NameStealer
	{
		extern bool enabled;
		extern int team;
	}

	namespace ThirdPerson
	{
		extern bool enabled;
		extern ThirdPersonMode mode;
		extern float distance;
		extern float transparency;
		extern ButtonCode_t key;
	}

	namespace JumpThrow
	{
		extern bool enabled;
		extern ButtonCode_t key;
	}

	//namespace Buybot {
	//	extern bool enabled;
	//	extern char* weps;
	//}

	namespace EventLogger {

		extern bool enabled;
		extern bool OnFreezeTime;
	}

	namespace DisablePostProcessing
	{
		extern bool enabled;
	}

	namespace GrenadeHelper
	{
		extern std::vector<GrenadeInfo> grenadeInfos;
		extern bool enabled;
		extern bool onlyMatchingInfos;
		extern bool aimAssist;
		extern float aimStep;
		extern float aimDistance;
		extern float aimFov;
		extern ColorVar aimDot;
		extern ColorVar aimLine;
		extern ColorVar infoHE;
		extern ColorVar infoSmoke;
		extern ColorVar infoFlash;
		extern ColorVar infoMolotov;
		extern std::string actMapName;
	}

	namespace GrenadePrediction {
		extern bool enabled;
		extern ColorVar color;
	}

	void LoadDefaultsOrSave(std::string path);
	void LoadConfig(std::string path);
	void LoadSettings();
	void DeleteConfig(std::string path);
	void SaveGrenadeInfo(std::string path);
	void LoadGrenadeInfo(std::string path);
}
