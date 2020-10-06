#include "Features.h"
#include "AimBotTab.h"
#include "XorStr.hpp"


static ItemDefinitionIndex currentWeapon = ItemDefinitionIndex::INVALID;
static bool enabled = false;
static bool silent = false;
static bool friendly = false;
static bool moveMouse = false;
static bool pSilent = false;
static bool closestBone = false;
static bool desiredBones[] = { true, true, true, true, true, true, true, // center mass
false, false, false, false, false, false, false, // left arm
false, false, false, false, false, false, false, // right arm
false, false, false, false, false, // left leg
false, false, false, false, false  // right leg
};
static bool engageLock = false;
static bool engageLockTR = false; // Target Reacquisition
static int engageLockTTR = 700; // Time to Target Reacquisition ( in ms )
static Bone bone = Bone::BONE_HEAD;
static ButtonCode_t aimkey = ButtonCode_t::MOUSE_MIDDLE;
static bool aimkeyOnly = false;
static bool smoothEnabled = false;
static float smoothValue = 0.5f;
static SmoothType smoothType = SmoothType::SLOW_END;
static bool smoothSaltEnabled = false;
static float smoothSaltMultiplier = 0.0f;
static bool errorMarginEnabled = false;
static float errorMarginValue = 0.0f;
static bool curveEnabled = false;
static float curveAmount = 0.5f;
static bool autoAimEnabled = false;
static float autoAimValue = 180.0f;
static bool aimStepEnabled = false;
static float aimStepMin = 25.0f;
static float aimStepMax = 35.0f;
static bool rcsEnabled = false;
static bool rcsAlwaysOn = false;
static float rcsAmountX = 2.0f;
static float rcsAmountY = 2.0f;
static bool autoCockRevolver = false;
static bool autoPistolEnabled = false;
static bool autoShootEnabled = false;
static bool autoScopeEnabled = false;
static bool noShootEnabled = false;
static bool ignoreJumpEnabled = false;
static bool smokeCheck = false;
static bool flashCheck = false;
static bool spreadLimitEnabled = false;
static bool spreadLimitDistance = false;
static float spreadLimit = 0.1f;
static bool autoWallEnabled = false;
static float autoWallValue = 10.0f;
static bool autoAimRealDistance = false;
static bool autoSlow = false;
static bool predEnabled = false;
static bool backtrack = false;
static bool velocitycheck = false;
//HitChance
static bool hitChanceEnabled = false;
static int hitChanceRays = 100;
static float hitChanceValue = 0.5f;
static bool smooth = false;
static float smoothvalue = 1.0f;

//AC
static bool Faceit = false;
void UI::ReloadWeaponSettings()
{
	ItemDefinitionIndex index = ItemDefinitionIndex::INVALID;
	if (Settings::Aimbot::weapons.find(currentWeapon) != Settings::Aimbot::weapons.end())
		index = currentWeapon;

	enabled = Settings::Aimbot::weapons.at(index).enabled;
	silent = Settings::Aimbot::weapons.at(index).silent;
	pSilent = Settings::Aimbot::weapons.at(index).pSilent;
	backtrack = Settings::Aimbot::weapons.at(index).backtrack;
	friendly = Settings::Aimbot::weapons.at(index).friendly;
	//moveMouse = Settings::Aimbot::weapons.at(index).moveMouse;
	closestBone = Settings::Aimbot::weapons.at(index).closestBone;
	engageLock = Settings::Aimbot::weapons.at(index).engageLock;
	engageLockTR = Settings::Aimbot::weapons.at(index).engageLockTR;
	engageLockTTR = Settings::Aimbot::weapons.at(index).engageLockTTR;
	bone = Settings::Aimbot::weapons.at(index).bone;
	aimkey = Settings::Aimbot::weapons.at(index).aimkey;
	aimkeyOnly = Settings::Aimbot::weapons.at(index).aimkeyOnly;
	smoothEnabled = Settings::Aimbot::weapons.at(index).smoothEnabled;
	smoothValue = Settings::Aimbot::weapons.at(index).smoothAmount;
	smoothType = Settings::Aimbot::weapons.at(index).smoothType;
	smoothSaltEnabled = Settings::Aimbot::weapons.at(index).smoothSaltEnabled;
	smoothSaltMultiplier = Settings::Aimbot::weapons.at(index).smoothSaltMultiplier;
	errorMarginEnabled = Settings::Aimbot::weapons.at(index).errorMarginEnabled;
	errorMarginValue = Settings::Aimbot::weapons.at(index).errorMarginValue;
	curveEnabled = Settings::Aimbot::weapons.at(index).curveAmount;
	curveAmount = Settings::Aimbot::weapons.at(index).curveAmount;
	autoAimEnabled = Settings::Aimbot::weapons.at(index).autoAimEnabled;
	autoAimValue = Settings::Aimbot::weapons.at(index).autoAimFov;
	aimStepEnabled = Settings::Aimbot::weapons.at(index).aimStepEnabled;
	aimStepMin = Settings::Aimbot::weapons.at(index).aimStepMin;
	aimStepMax = Settings::Aimbot::weapons.at(index).aimStepMax;
	rcsEnabled = Settings::Aimbot::weapons.at(index).rcsEnabled;
	rcsAlwaysOn = Settings::Aimbot::weapons.at(index).rcsAlwaysOn;
	rcsAmountX = Settings::Aimbot::weapons.at(index).rcsAmountX;
	rcsAmountY = Settings::Aimbot::weapons.at(index).rcsAmountY;
	autoCockRevolver = Settings::Aimbot::weapons.at(index).autoCockRevolver;
	autoPistolEnabled = Settings::Aimbot::weapons.at(index).autoPistolEnabled;
	autoShootEnabled = Settings::Aimbot::weapons.at(index).autoShootEnabled;
	autoScopeEnabled = Settings::Aimbot::weapons.at(index).autoScopeEnabled;
	noShootEnabled = Settings::Aimbot::weapons.at(index).noShootEnabled;
	ignoreJumpEnabled = Settings::Aimbot::weapons.at(index).ignoreJumpEnabled;
	smokeCheck = Settings::Aimbot::weapons.at(index).smokeCheck;
	flashCheck = Settings::Aimbot::weapons.at(index).flashCheck;
	spreadLimitEnabled = Settings::Aimbot::weapons.at(index).spreadLimitEnabled;
	spreadLimitDistance = Settings::Aimbot::weapons.at(index).spreadLimitDistance;
	spreadLimit = Settings::Aimbot::weapons.at(index).spreadLimit;
	autoWallEnabled = Settings::Aimbot::weapons.at(index).autoWallEnabled;
	autoWallValue = Settings::Aimbot::weapons.at(index).autoWallValue;
	autoAimRealDistance = Settings::Aimbot::weapons.at(index).autoAimRealDistance;
	autoSlow = Settings::Aimbot::weapons.at(index).autoSlow;
	predEnabled = Settings::Aimbot::weapons.at(index).predEnabled;
	moveMouse = Settings::Aimbot::weapons.at(index).moveMouse;
	hitChanceEnabled = Settings::Aimbot::weapons.at(index).hitChanceEnabled;
	hitChanceRays = Settings::Aimbot::weapons.at(index).hitChanceRays;
	hitChanceValue = Settings::Aimbot::weapons.at(index).hitChanceValue;
	smooth = Settings::Aimbot::weapons.at(index).smooth;
	smoothvalue = Settings::Aimbot::weapons.at(index).smoothvalue;

	for (int bone = (int)DesiredBones::BONE_PELVIS; bone <= (int)DesiredBones::BONE_RIGHT_SOLE; bone++)
		desiredBones[bone] = Settings::Aimbot::weapons.at(index).desiredBones[bone];
}

void UI::UpdateWeaponSettings()
{
	if (Settings::Aimbot::weapons.find(currentWeapon) == Settings::Aimbot::weapons.end())
		Settings::Aimbot::weapons[currentWeapon] = AimbotWeapon_t();

	AimbotWeapon_t settings = {
		enabled, silent, pSilent, friendly, closestBone, engageLock, engageLockTR, engageLockTTR, bone, aimkey, aimkeyOnly,
		smoothEnabled, smoothValue, smoothType, smoothSaltEnabled, smoothSaltMultiplier,
		errorMarginEnabled, errorMarginValue,
		curveEnabled, curveAmount,
		autoAimEnabled, autoAimValue, aimStepEnabled, aimStepMin, aimStepMax,
		rcsEnabled, rcsAlwaysOn, rcsAmountX, rcsAmountY,
		autoCockRevolver, autoPistolEnabled, autoShootEnabled, autoScopeEnabled,
		noShootEnabled, ignoreJumpEnabled, smokeCheck, flashCheck, spreadLimitEnabled, spreadLimitDistance, spreadLimit, autoWallEnabled, autoWallValue, autoAimRealDistance, autoSlow, moveMouse, predEnabled, backtrack, hitChanceEnabled, hitChanceRays, hitChanceValue, velocitycheck, smooth, smoothvalue
	};


	for (int bone = (int)DesiredBones::BONE_PELVIS; bone <= (int)DesiredBones::BONE_RIGHT_SOLE; bone++)
		settings.desiredBones[bone] = desiredBones[bone];

	Settings::Aimbot::weapons.at(currentWeapon) = settings;

	if (Settings::Aimbot::weapons.at(currentWeapon) == Settings::Aimbot::weapons.at(ItemDefinitionIndex::INVALID) &&
		currentWeapon != ItemDefinitionIndex::INVALID)
	{
		Settings::Aimbot::weapons.erase(currentWeapon);
		UI::ReloadWeaponSettings();
	}
}

//
#define IM_ARRAYSIZE(_ARR)      ((int)(sizeof(_ARR)/sizeof(*_ARR)))
//
void Aimbot::RenderTab()
{
	const char* targets[] = { "PELVIS", "", "", "HIP", "LOWER SPINE", "MIDDLE SPINE", "UPPER SPINE", "NECK", "HEAD" };
	const char* smoothTypes[] = { "Slow Near End", "Constant Speed", "Fast Near End" };
	static char filterWeapons[32];

	if (ImGui::Checkbox("Enabled", &enabled))
		UI::UpdateWeaponSettings();
	ImGui::Separator();

	ImGui::Columns(3, NULL, true);
	{
		ImGui::SetColumnOffset(1, 200);
		ImGui::PushItemWidth(-1);
		ImGui::InputText("##FILTERWEAPONS", filterWeapons, IM_ARRAYSIZE(filterWeapons));
		ImGui::PopItemWidth();
		ImGui::ListBoxHeader("##GUNS", ImVec2(-1, -1));
	
		for (auto it : ItemDefinitionIndexMap) {
			bool isDefault = (int)it.first < 0;
			if (!isDefault && !Util::Contains(Util::ToLower(std::string(filterWeapons)),
				Util::ToLower(Util::Items::GetItemDisplayName(it.first).c_str())))
				continue;

			if (Util::Items::IsKnife(it.first) || Util::Items::IsGlove(it.first) ||
				Util::Items::IsUtility(it.first))
				continue;

			const bool item_selected = ((int)it.first == (int)currentWeapon);
			ImGui::PushID((int)it.first);
			std::string formattedName;
			char changeIndicator = ' ';
			bool isChanged = Settings::Aimbot::weapons.find(it.first) != Settings::Aimbot::weapons.end();
			if (!isDefault && isChanged)
				changeIndicator = '*';
			formattedName = changeIndicator + (isDefault ? Util::Items::GetItemDisplayName(it.first).c_str()
				: Util::Items::GetItemDisplayName(it.first));
			if (ImGui::Selectable(formattedName.c_str(), item_selected)) {
				currentWeapon = it.first;
				UI::ReloadWeaponSettings();
			}
			ImGui::PopID();
		}
		ImGui::ListBoxFooter();
	}
	ImGui::NextColumn();
	{
		ImGui::SetColumnOffset(2, ImGui::GetWindowWidth() / 2 + 75);
		ImGui::BeginChild("COL1", ImVec2(0, 0), true);
		{
			ImGui::Text("Target");
			ImGui::Separator();
			ImGui::Columns(2, NULL, true);
			{
				if (ImGui::Checkbox("FriendlyFire", &friendly))
					UI::UpdateWeaponSettings();
				//SetTooltip("Whether to target friendlies");
			}
			ImGui::NextColumn();
			{

				ImGui::PushItemWidth(-1);
				ImGui::Text(XorStr("Aimbot Target"));
				if (!closestBone) {
					if (ImGui::Combo(XorStr("##AIMTARGET"), (int*)& bone, targets, IM_ARRAYSIZE(targets)))
						UI::UpdateWeaponSettings();
				}
				if (closestBone)
				{
					if (ImGui::Button(XorStr("Bones"), ImVec2(-1, 0)))
						ImGui::OpenPopup(XorStr("optionBones"));
					ImGui::SetNextWindowSize(ImVec2((ImGui::GetWindowWidth() / 1.25f), ImGui::GetWindowHeight()), ImGuiSetCond_Always);
					if (ImGui::BeginPopup(XorStr("optionBones")))
					{
						ImGui::PushItemWidth(-1);
						ImGui::Text(XorStr("Center Mass"));
						if (ImGui::Checkbox(XorStr("Head"), &desiredBones[(int)DesiredBones::BONE_HEAD]))
							UI::UpdateWeaponSettings();
						if (ImGui::Checkbox(XorStr("Neck"), &desiredBones[(int)DesiredBones::BONE_NECK]))
							UI::UpdateWeaponSettings();
						if (ImGui::Checkbox(XorStr("Upper Spine"), &desiredBones[(int)DesiredBones::BONE_UPPER_SPINAL_COLUMN]))
							UI::UpdateWeaponSettings();
						if (ImGui::Checkbox(XorStr("Middle Spine"), &desiredBones[(int)DesiredBones::BONE_MIDDLE_SPINAL_COLUMN]))
							UI::UpdateWeaponSettings();
						if (ImGui::Checkbox(XorStr("Lower Spine"), &desiredBones[(int)DesiredBones::BONE_LOWER_SPINAL_COLUMN]))
							UI::UpdateWeaponSettings();
						if (ImGui::Checkbox(XorStr("Pelvis"), &desiredBones[(int)DesiredBones::BONE_PELVIS]))
							UI::UpdateWeaponSettings();
						if (ImGui::Checkbox(XorStr("Hip"), &desiredBones[(int)DesiredBones::BONE_HIP]))
							UI::UpdateWeaponSettings();
						ImGui::Separator();

						ImGui::Columns(2, NULL, false);
						{
							ImGui::Text(XorStr("Player's Right Arm"));
							if (ImGui::Checkbox(XorStr("Collarbone"), &desiredBones[(int)DesiredBones::BONE_RIGHT_COLLARBONE]))
								UI::UpdateWeaponSettings();
							if (ImGui::Checkbox(XorStr("Shoulder"), &desiredBones[(int)DesiredBones::BONE_RIGHT_SHOULDER]))
								UI::UpdateWeaponSettings();
							if (ImGui::Checkbox(XorStr("Armpit"), &desiredBones[(int)DesiredBones::BONE_RIGHT_ARMPIT]))
								UI::UpdateWeaponSettings();
							if (ImGui::Checkbox(XorStr("Bicep"), &desiredBones[(int)DesiredBones::BONE_RIGHT_BICEP]))
								UI::UpdateWeaponSettings();
							if (ImGui::Checkbox(XorStr("Elbow"), &desiredBones[(int)DesiredBones::BONE_RIGHT_ELBOW]))
								UI::UpdateWeaponSettings();
							if (ImGui::Checkbox(XorStr("Forearm"), &desiredBones[(int)DesiredBones::BONE_RIGHT_FOREARM]))
								UI::UpdateWeaponSettings();
							if (ImGui::Checkbox(XorStr("Wrist"), &desiredBones[(int)DesiredBones::BONE_RIGHT_WRIST]))
								UI::UpdateWeaponSettings();
							ImGui::Text(XorStr("Player's Right Leg"));
							if (ImGui::Checkbox(XorStr("Buttcheek"), &desiredBones[(int)DesiredBones::BONE_RIGHT_BUTTCHEEK]))
								UI::UpdateWeaponSettings();
							if (ImGui::Checkbox(XorStr("Thigh"), &desiredBones[(int)DesiredBones::BONE_RIGHT_THIGH]))
								UI::UpdateWeaponSettings();
							if (ImGui::Checkbox(XorStr("Knee"), &desiredBones[(int)DesiredBones::BONE_RIGHT_KNEE]))
								UI::UpdateWeaponSettings();
							if (ImGui::Checkbox(XorStr("Ankle"), &desiredBones[(int)DesiredBones::BONE_RIGHT_ANKLE]))
								UI::UpdateWeaponSettings();
							if (ImGui::Checkbox(XorStr("Sole"), &desiredBones[(int)DesiredBones::BONE_RIGHT_SOLE]))
								UI::UpdateWeaponSettings();
						}
						ImGui::NextColumn();
						{   // these spaces are here in the strings because checkboxes can't have duplicate titles.
							ImGui::Text(XorStr("Player's Left Arm"));
							if (ImGui::Checkbox(XorStr("Collarbone "), &desiredBones[(int)DesiredBones::BONE_LEFT_COLLARBONE]))
								UI::UpdateWeaponSettings();
							if (ImGui::Checkbox(XorStr("Shoulder "), &desiredBones[(int)DesiredBones::BONE_LEFT_SHOULDER]))
								UI::UpdateWeaponSettings();
							if (ImGui::Checkbox(XorStr("Armpit "), &desiredBones[(int)DesiredBones::BONE_LEFT_ARMPIT]))
								UI::UpdateWeaponSettings();
							if (ImGui::Checkbox(XorStr("Bicep "), &desiredBones[(int)DesiredBones::BONE_LEFT_BICEP]))
								UI::UpdateWeaponSettings();
							if (ImGui::Checkbox(XorStr("Elbow "), &desiredBones[(int)DesiredBones::BONE_LEFT_ELBOW]))
								UI::UpdateWeaponSettings();
							if (ImGui::Checkbox(XorStr("Forearm "), &desiredBones[(int)DesiredBones::BONE_LEFT_FOREARM]))
								UI::UpdateWeaponSettings();
							if (ImGui::Checkbox(XorStr("Wrist "), &desiredBones[(int)DesiredBones::BONE_LEFT_WRIST]))
								UI::UpdateWeaponSettings();

							ImGui::Text(XorStr("Player's Left Leg"));
							if (ImGui::Checkbox(XorStr("Buttcheek "), &desiredBones[(int)DesiredBones::BONE_LEFT_BUTTCHEEK]))
								UI::UpdateWeaponSettings();
							if (ImGui::Checkbox(XorStr("Thigh "), &desiredBones[(int)DesiredBones::BONE_LEFT_THIGH]))
								UI::UpdateWeaponSettings();
							if (ImGui::Checkbox(XorStr("Knee "), &desiredBones[(int)DesiredBones::BONE_LEFT_KNEE]))
								UI::UpdateWeaponSettings();
							if (ImGui::Checkbox(XorStr("Ankle "), &desiredBones[(int)DesiredBones::BONE_LEFT_ANKLE]))
								UI::UpdateWeaponSettings();
							if (ImGui::Checkbox(XorStr("Sole "), &desiredBones[(int)DesiredBones::BONE_LEFT_SOLE]))
								UI::UpdateWeaponSettings();
						}
						ImGui::PopItemWidth();
						ImGui::EndPopup();
					}
				}
				if (ImGui::Checkbox(XorStr("ClosestBone"), &closestBone))
					UI::UpdateWeaponSettings();

				if (ImGui::Checkbox(XorStr("EngageLock"), &engageLock))
					UI::UpdateWeaponSettings();
				SetTooltip("For Legit gameplay. Disable this when using Autowall");
				if (engageLock)
				{
					if (ImGui::Checkbox(XorStr("Target Reacquisition"), &engageLockTR))
						UI::UpdateWeaponSettings();
					if (engageLockTR)
					{
						if (ImGui::SliderInt(XorStr("##TTR"), &engageLockTTR, 0, 1000))
							UI::UpdateWeaponSettings();
					}
				}
				ImGui::PopItemWidth();
			}
			ImGui::Columns(1);
			ImGui::Separator();
			ImGui::Text("Accuracy");
			ImGui::Separator();
			ImGui::Columns(2, NULL, true);
			{
				if (ImGui::Checkbox("Auto Aim", &autoAimEnabled))
					UI::UpdateWeaponSettings();
				SetTooltip("Locks onto enemies within a certain FOV amount");
				if (ImGui::Checkbox("Recoil Control", &rcsEnabled))
					UI::UpdateWeaponSettings();
				SetTooltip("Automatically controls recoil");
				if (ImGui::Checkbox("Distance-Based FOV", &autoAimRealDistance))
					UI::UpdateWeaponSettings();
				SetTooltip("Takes perspective into account when calculating FOV");
			}
			ImGui::NextColumn();
			{
				ImGui::PushItemWidth(-1);
				if (ImGui::SliderFloat("##AA", &autoAimValue, 0, 360))
					UI::UpdateWeaponSettings();
				ImGui::PopItemWidth();
				if (ImGui::Button("RCS Settings", ImVec2(-1, 0)))
					ImGui::OpenPopup("optionRCSAmount");
				ImGui::SetNextWindowSize(ImVec2(200, 100), ImGuiSetCond_Always);
				if (ImGui::BeginPopup("optionRCSAmount"))
				{
					ImGui::PushItemWidth(-1);
					if (ImGui::Checkbox("RCS Always on", &rcsAlwaysOn))
						UI::UpdateWeaponSettings();
					//SetTooltip("Whether Recoil Control always controls recoil (even when not aimbotting)");
					if (ImGui::SliderFloat("##RCSX", &rcsAmountX, 0, 2, "X: %0.3f"))
						UI::UpdateWeaponSettings();
					if (ImGui::SliderFloat("##RCSY", &rcsAmountY, 0, 2, "Y: %0.3f"))
						UI::UpdateWeaponSettings();
					if (ImGui::Checkbox(XorStr("Smooth RCS"), &smooth))
						UI::UpdateWeaponSettings();
					if (ImGui::SliderFloat(XorStr("##SMOOTH"), &smoothvalue, 0, 150, XorStr("Smooth: %0.1f")))
						UI::UpdateWeaponSettings();
					ImGui::PopItemWidth();

					ImGui::EndPopup();
				}
			}
			ImGui::Columns(1);
			ImGui::Separator();
			ImGui::Text("Humanizing");
			ImGui::Separator();
			ImGui::Columns(2, NULL, true);
			{
				if (ImGui::Checkbox("Curve", &curveEnabled))
					UI::UpdateWeaponSettings();
				SetTooltip("Curve makes aim not go in a straight line. Used with smoothing");
				if (ImGui::Checkbox("Smoothing", &smoothEnabled))
					UI::UpdateWeaponSettings();
				SetTooltip("Smoothing reduces the aimbot \"snap\". 0 for full snap. 1 for full smoothing");
				if (ImGui::Checkbox("Smooth Salting", &smoothSaltEnabled))
					UI::UpdateWeaponSettings();
				SetTooltip("Breaks the smoothing into smaller steps, high smooth + low salt is slightly stuttery");
				if (ImGui::Checkbox("Error Margin", &errorMarginEnabled))
					UI::UpdateWeaponSettings();
				SetTooltip("Adds a margin of error to the aim, it will be obvious what it does when using it");
				ImGui::PushItemWidth(-1);
				if (ImGui::Combo("##SMOOTHTYPE", (int*)& smoothType, smoothTypes, IM_ARRAYSIZE(smoothTypes)))
					UI::UpdateWeaponSettings();
				ImGui::PopItemWidth();
			}
			ImGui::NextColumn();
			{
				ImGui::PushItemWidth(-1);
				if (ImGui::SliderFloat("##CURVE", &curveAmount, 0, 2))
					UI::UpdateWeaponSettings();
				if (ImGui::SliderFloat("##SMOOTH", &smoothValue, 0, 1))
					UI::UpdateWeaponSettings();
				if (ImGui::SliderFloat("##SALT", &smoothSaltMultiplier, 0, smoothValue))
					UI::UpdateWeaponSettings();
				if (ImGui::SliderFloat("##ERROR", &errorMarginValue, 0, 2))
					UI::UpdateWeaponSettings();
				ImGui::PopItemWidth();
			}
			ImGui::Columns(1);
			ImGui::Separator();
			ImGui::EndChild();
		}
	}
	ImGui::NextColumn();
	{
		ImGui::BeginChild("COL2", ImVec2(0, 0), true);
		{
			ImGui::Text("Aimkey Only");
			ImGui::Separator();
			ImGui::Columns(2, NULL, true);
			{
				if (ImGui::Checkbox("Enabled", &aimkeyOnly))
					UI::UpdateWeaponSettings();
				//SetTooltip("Enabling this means it you need to press a specific key to aimlock");
			}
			ImGui::NextColumn();
			{
				UI::KeyBindButton(&aimkey);
				/*ImGui::Combo("##Aimkey", (int*)&aimkey, ButtonNamesA, IM_ARRAYSIZE(ButtonNamesA));*/
			}
			ImGui::Columns(1);
			ImGui::Separator();
			ImGui::Text("Casual / DM Only");
			ImGui::Separator();
			ImGui::Columns(2, NULL, true);
			{
				if (ImGui::Checkbox(XorStr("Aim Step"), &aimStepEnabled))
					UI::UpdateWeaponSettings();
				SetTooltip("Stops you getting VAC auth kicked in Casual / DM");
			}
			ImGui::NextColumn();
			{
				ImGui::PushItemWidth(-1);
				ImGui::Text(XorStr("Min"));
				if (ImGui::SliderFloat(XorStr("##STEPMIN"), &aimStepMin, 5, 35))
					UI::UpdateWeaponSettings();
				ImGui::Text(XorStr("Max"));
				if (ImGui::SliderFloat(XorStr("##STEPMAX"), &aimStepMax, (aimStepMin)+1.0f, 35))
					UI::UpdateWeaponSettings();
				ImGui::PopItemWidth();
			}
			ImGui::Columns(1);
			ImGui::Separator();
			ImGui::Text("Other");
			ImGui::Separator();
			ImGui::Columns(2, NULL, true);
			{
				/*if (ImGui::Checkbox("Mouse Movement", &moveMouse))
					UI::UpdateWeaponSettings();
				SetTooltip("Emulates mouse movement instead of defining angles. Recommended for Face-It servers. (Does not work with silent aim *Disabled!");*/
				switch (currentWeapon)
				{
				case ItemDefinitionIndex::INVALID:
				case ItemDefinitionIndex::WEAPON_DEAGLE:
				case ItemDefinitionIndex::WEAPON_ELITE:
				case ItemDefinitionIndex::WEAPON_FIVESEVEN:
				case ItemDefinitionIndex::WEAPON_GLOCK:
				case ItemDefinitionIndex::WEAPON_TEC9:
				case ItemDefinitionIndex::WEAPON_HKP2000:
				case ItemDefinitionIndex::WEAPON_USP_SILENCER:
				case ItemDefinitionIndex::WEAPON_P250:
				case ItemDefinitionIndex::WEAPON_CZ75A:
				case ItemDefinitionIndex::WEAPON_REVOLVER:
					if (ImGui::Checkbox("Auto Cock Revolver", &autoCockRevolver))
						UI::UpdateWeaponSettings();
					SetTooltip("Automatically cocks revolver so it shoots instantly");
					if (ImGui::Checkbox("Auto Pistol", &autoPistolEnabled))
						UI::UpdateWeaponSettings();
					SetTooltip("Automatically shoots the pistol when holding fire");
					break;
				default:
					break;
				}

				if (ImGui::Checkbox("Auto Shoot", &autoShootEnabled))
					UI::UpdateWeaponSettings();
				SetTooltip("Automatically shoots when locking to an enemy");
	

				if (ImGui::Checkbox("Prediction", &predEnabled))
					UI::UpdateWeaponSettings();
				SetTooltip("Use velocity prediction");

				if (ImGui::Checkbox("Silent Aim", &silent))
					UI::UpdateWeaponSettings();
				SetTooltip("Prevents the camera from locking to an enemy, doesn't work for demos");

				if (ImGui::Checkbox("pSilent", &pSilent))
					UI::UpdateWeaponSettings();
				if (ImGui::Checkbox("Spread Limit", &spreadLimitEnabled))
					UI::UpdateWeaponSettings();
				SetTooltip("Auto Shoot below a certain inaccuracy threshold.\nGood for weapons such as deagle");
				if (ImGui::Checkbox("Distance-Based Limit", &spreadLimitDistance))
					UI::UpdateWeaponSettings();
				SetTooltip("Distance based spread limit. Makes the limit lower if the target is further away.");
				
				//not in the weapon struct config so its not saving for the different guns...
				//if (ImGui::Checkbox("Toggle Backtrack", &Settings::Aimbot::backtrack))
				//{
				//	UI::UpdateWeaponSettings(); // for later.
				//	Backtracking::ToggleRequiredCVars(backtrack);
				//}
				if (ImGui::Checkbox(XorStr("Rage Backtrack"), &backtrack)) {
					UI::UpdateWeaponSettings();
					Backtracking::ToggleRequiredCVars(backtrack);
					
				}
				//ImGui::SliderFloat(XorStr("##SMOOTH"), &smoothvalue, 0, 150, XorStr("Smooth: %0.1f")
				ImGui::SliderInt(XorStr("##FAKELATENCY"), &Settings::Aimbot::FakeLatencyMode, 0, 2000, XorStr("Fakelatency: %0.1f"));
			
				//if (ImGui::Checkbox("No Spread", &Settings::Aimbot::NoSpread))
				//	if (((*pGameRules) && (*pGameRules)->IsValveDS()) && Settings::Aimbot::NoSpread)
				//	{
				//		Settings::Aimbot::NoSpread = false;
				//		ImGui::OpenPopup("Error###UNTRUSTED_FEATURE");
				//	}
				//SetTooltip("Experimental NoSpread.");
			}
			ImGui::NextColumn();
			{
				if (ImGui::Checkbox("No Shoot", &noShootEnabled))
					UI::UpdateWeaponSettings();
				if (ImGui::Checkbox("Auto Scope", &autoScopeEnabled))
					UI::UpdateWeaponSettings();
				if (ImGui::Checkbox("Ignore Jump", &ignoreJumpEnabled))
					UI::UpdateWeaponSettings();
				SetTooltip("Prevents you from aimbotting while jumping");
				ImGui::Checkbox("^Velocity Check", &Settings::Aimbot::AutoShoot::velocityCheck);
				SetTooltip("Auto Shoot when below move penalty threshold.\nRecommend using with Auto-Slow");
				if (ImGui::Checkbox("Flash Check", &flashCheck))
					UI::UpdateWeaponSettings();
				SetTooltip("Disable aimbot while flashed");
				if (ImGui::Checkbox("Smoke Check", &smokeCheck))
					UI::UpdateWeaponSettings();
				SetTooltip("Ignore players that are blocked by smoke");
				if (ImGui::SliderFloat("##SPREADLIMIT", &spreadLimit, 0, 0.1))
					UI::UpdateWeaponSettings();
				SetTooltip("Visuals - Show Spreadlimit to see how big this is");

				/*if (ImGui::Checkbox("Hit Chance", &hitChanceEnabled))
					UI::UpdateWeaponSettings();
				SetTooltip("Spread limit, but by simulating spread of the weapon");
				if (ImGui::SliderFloat("##HITCHANCEVALUE", &hitChanceValue, 0, 1))
					UI::UpdateWeaponSettings();
				SetTooltip("Below this value, autoshoot does not shoot");
				if (ImGui::SliderInt("##HITCHANCERAYS", &hitChanceRays, 1, 250))
					UI::UpdateWeaponSettings();
				SetTooltip("Amount of rays to be shot");*/
			}

			if (autoWallEnabled)
			{
				ImGui::Columns(1);
				ImGui::Separator();
				ImGui::Text("AutoSlow");
				ImGui::Separator();
				ImGui::Columns(2, NULL, true);
				{
					if (ImGui::Checkbox("Enabled##AUTOSLOW", &autoSlow))
						UI::UpdateWeaponSettings();
					SetTooltip("Automatically slows your movement speed when an enemy is shootable");
				}
	
			}


			ImGui::Columns(1);
			ImGui::Separator();
			ImGui::Text(XorStr("AutoWall"));
			ImGui::Separator();
			ImGui::Columns(2, NULL, true);
			{
				if (ImGui::Checkbox(XorStr("Enabled##AUTOWALL"), &autoWallEnabled))
					UI::UpdateWeaponSettings();
				if (ImGui::Button(XorStr("Autowall Bones"), ImVec2(-1, 0)))
					ImGui::OpenPopup(XorStr("optionBones"));
				ImGui::SetNextWindowSize(ImVec2((ImGui::GetWindowWidth() / 1.25f), ImGui::GetWindowHeight()), ImGuiSetCond_Always);
				if (ImGui::BeginPopup(XorStr("optionBones")))
				{
					ImGui::PushItemWidth(-1);
					ImGui::Text(XorStr("Center Mass"));
					if (ImGui::Checkbox(XorStr("Head"), &desiredBones[(int)DesiredBones::BONE_HEAD]))
						UI::UpdateWeaponSettings();
					if (ImGui::Checkbox(XorStr("Neck"), &desiredBones[(int)DesiredBones::BONE_NECK]))
						UI::UpdateWeaponSettings();
					if (ImGui::Checkbox(XorStr("Upper Spine"), &desiredBones[(int)DesiredBones::BONE_UPPER_SPINAL_COLUMN]))
						UI::UpdateWeaponSettings();
					if (ImGui::Checkbox(XorStr("Middle Spine"), &desiredBones[(int)DesiredBones::BONE_MIDDLE_SPINAL_COLUMN]))
						UI::UpdateWeaponSettings();
					if (ImGui::Checkbox(XorStr("Lower Spine"), &desiredBones[(int)DesiredBones::BONE_LOWER_SPINAL_COLUMN]))
						UI::UpdateWeaponSettings();
					if (ImGui::Checkbox(XorStr("Pelvis"), &desiredBones[(int)DesiredBones::BONE_PELVIS]))
						UI::UpdateWeaponSettings();
					if (ImGui::Checkbox(XorStr("Hip"), &desiredBones[(int)DesiredBones::BONE_HIP]))
						UI::UpdateWeaponSettings();
					ImGui::Separator();

					ImGui::Columns(2, NULL, false);
					{
						ImGui::Text(XorStr("Player's Right Arm"));
						if (ImGui::Checkbox(XorStr("Collarbone"), &desiredBones[(int)DesiredBones::BONE_RIGHT_COLLARBONE]))
							UI::UpdateWeaponSettings();
						if (ImGui::Checkbox(XorStr("Shoulder"), &desiredBones[(int)DesiredBones::BONE_RIGHT_SHOULDER]))
							UI::UpdateWeaponSettings();
						if (ImGui::Checkbox(XorStr("Armpit"), &desiredBones[(int)DesiredBones::BONE_RIGHT_ARMPIT]))
							UI::UpdateWeaponSettings();
						if (ImGui::Checkbox(XorStr("Bicep"), &desiredBones[(int)DesiredBones::BONE_RIGHT_BICEP]))
							UI::UpdateWeaponSettings();
						if (ImGui::Checkbox(XorStr("Elbow"), &desiredBones[(int)DesiredBones::BONE_RIGHT_ELBOW]))
							UI::UpdateWeaponSettings();
						if (ImGui::Checkbox(XorStr("Forearm"), &desiredBones[(int)DesiredBones::BONE_RIGHT_FOREARM]))
							UI::UpdateWeaponSettings();
						if (ImGui::Checkbox(XorStr("Wrist"), &desiredBones[(int)DesiredBones::BONE_RIGHT_WRIST]))
							UI::UpdateWeaponSettings();
						ImGui::Text(XorStr("Player's Right Leg"));
						if (ImGui::Checkbox(XorStr("Buttcheek"), &desiredBones[(int)DesiredBones::BONE_RIGHT_BUTTCHEEK]))
							UI::UpdateWeaponSettings();
						if (ImGui::Checkbox(XorStr("Thigh"), &desiredBones[(int)DesiredBones::BONE_RIGHT_THIGH]))
							UI::UpdateWeaponSettings();
						if (ImGui::Checkbox(XorStr("Knee"), &desiredBones[(int)DesiredBones::BONE_RIGHT_KNEE]))
							UI::UpdateWeaponSettings();
						if (ImGui::Checkbox(XorStr("Ankle"), &desiredBones[(int)DesiredBones::BONE_RIGHT_ANKLE]))
							UI::UpdateWeaponSettings();
						if (ImGui::Checkbox(XorStr("Sole"), &desiredBones[(int)DesiredBones::BONE_RIGHT_SOLE]))
							UI::UpdateWeaponSettings();
					}
					ImGui::NextColumn();
					{
						ImGui::Text(XorStr("Player's Left Arm"));
						if (ImGui::Checkbox(XorStr("Collarbone "), &desiredBones[(int)DesiredBones::BONE_LEFT_COLLARBONE]))
							UI::UpdateWeaponSettings();
						if (ImGui::Checkbox(XorStr("Shoulder "), &desiredBones[(int)DesiredBones::BONE_LEFT_SHOULDER]))
							UI::UpdateWeaponSettings();
						if (ImGui::Checkbox(XorStr("Armpit "), &desiredBones[(int)DesiredBones::BONE_LEFT_ARMPIT]))
							UI::UpdateWeaponSettings();
						if (ImGui::Checkbox(XorStr("Bicep "), &desiredBones[(int)DesiredBones::BONE_LEFT_BICEP]))
							UI::UpdateWeaponSettings();
						if (ImGui::Checkbox(XorStr("Elbow "), &desiredBones[(int)DesiredBones::BONE_LEFT_ELBOW]))
							UI::UpdateWeaponSettings();
						if (ImGui::Checkbox(XorStr("Forearm "), &desiredBones[(int)DesiredBones::BONE_LEFT_FOREARM]))
							UI::UpdateWeaponSettings();
						if (ImGui::Checkbox(XorStr("Wrist "), &desiredBones[(int)DesiredBones::BONE_LEFT_WRIST]))
							UI::UpdateWeaponSettings();

						ImGui::Text(XorStr("Player's Left Leg"));
						if (ImGui::Checkbox(XorStr("Buttcheek "), &desiredBones[(int)DesiredBones::BONE_LEFT_BUTTCHEEK]))
							UI::UpdateWeaponSettings();
						if (ImGui::Checkbox(XorStr("Thigh "), &desiredBones[(int)DesiredBones::BONE_LEFT_THIGH]))
							UI::UpdateWeaponSettings();
						if (ImGui::Checkbox(XorStr("Knee "), &desiredBones[(int)DesiredBones::BONE_LEFT_KNEE]))
							UI::UpdateWeaponSettings();
						if (ImGui::Checkbox(XorStr("Ankle "), &desiredBones[(int)DesiredBones::BONE_LEFT_ANKLE]))
							UI::UpdateWeaponSettings();
						if (ImGui::Checkbox(XorStr("Sole "), &desiredBones[(int)DesiredBones::BONE_LEFT_SOLE]))
							UI::UpdateWeaponSettings();
					}
					ImGui::PopItemWidth();
					ImGui::EndPopup();
				}
			}
			ImGui::NextColumn();
			{
				ImGui::PushItemWidth(-1);
				if (ImGui::SliderFloat(XorStr("##AUTOWALLDMG"), &autoWallValue, 0, 100, XorStr("Min Damage: %f")))
					UI::UpdateWeaponSettings();
	
				ImGui::PopItemWidth();
			}

			ImGui::Columns(1);
			ImGui::Separator();
			if (currentWeapon > ItemDefinitionIndex::INVALID && Settings::Aimbot::weapons.find(currentWeapon) != Settings::Aimbot::weapons.end())
			{
				if (ImGui::Button("Clear Weapon Settings", ImVec2(-1, 0)))
				{
					Settings::Aimbot::weapons.erase(currentWeapon);
					UI::ReloadWeaponSettings();
				}
			}
			ImGui::EndChild();
		}
	}
}