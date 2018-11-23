#include "Settings.h"
#include "ImGui\imgui.h"
#include "ImGui\imgui_internal.h"
#include "Util_Items.h"
#include "MiscHvHTab.h"
#include "HvHTab.h"
#include "XorStr.hpp"
#include "Menu.h"


void MiscHvHTab::RenderTab() {
	ImGui::Columns(2, NULL, true);
	{
		ImGui::BeginChild(XorStr("##HVHMISC1"), ImVec2(0, 0), true);
		{
			ImGui::Text(XorStr("Options"));
			ImGui::BeginChild(XorStr("##HVHMISC3"), ImVec2(0, 0), true);
			{
				ImGui::Checkbox(XorStr("Resolver"), &Settings::Resolver::enabled);
				{
					ImGui::Separator();

					/*ImGui::Columns( 2, NULL, true );
					{
					ImGui::ItemSize( ImVec2( 0.0f, 0.0f ), 0.0f );
					ImGui::Text( XorStr( "Ticks" ) );
					ImGui::ItemSize( ImVec2( 0.0f, 0.0f ), 0.0f );
					ImGui::Text( XorStr( "Modulo" ) );
					ImGui::ItemSize( ImVec2( 0.0f, 0.0f ), 0.0f );
					}
					ImGui::NextColumn();
					{
					ImGui::PushItemWidth( -1 );
					ImGui::SliderFloat( XorStr( "##HUGTICKS" ), &Settings::Resolver::ticks, 0, 50,
					XorStr( "%.1f" ) );
					ImGui::PopItemWidth();
					ImGui::PushItemWidth( -1 );
					ImGui::SliderFloat( XorStr( "##HUGMODULO" ), &Settings::Resolver::modulo, 0, 50,
					XorStr( "%.1f" ) );
					ImGui::PopItemWidth();
					}*/
					  // this isnt used why have it in menu ?
					ImGui::Columns(1);

					ImGui::Columns(2, NULL, true);
					{
						ImGui::Checkbox(XorStr("BackTrack LBY Lagcomp"), &Settings::Resolver::LagComp);
						SetTooltip(XorStr("Backtracks to latest LBY update."));

					}
					ImGui::NextColumn();
					{
						ImGui::Checkbox(XorStr("Resolve Pitch"), &Settings::Resolver::pitch);
						SetTooltip(XorStr("Resolve enemy pitch."));
					}
					ImGui::Columns(1);

					ImGui::Columns(2, NULL, true);
					{
						ImGui::Checkbox(XorStr("Flip angles"), &Settings::Resolver::angleFlipEnabled);
						SetTooltip(XorStr("Adds 180 to the enemy yaw angle."));
						ImGui::Checkbox(XorStr("Override angles"), &Settings::Resolver::ResolverOverride);
						SetTooltip(XorStr("Disabled* Needs testing."));
						ImGui::Checkbox(XorStr("Headshot LBY only"), &Settings::Resolver::lbyOnly);
						SetTooltip(XorStr("If LBY is updated hitscan whole body, else baim."));
					}
					ImGui::NextColumn();
					{
						UI::KeyBindButton(&Settings::Resolver::angleFlip);
						UI::KeyBindButton(&Settings::Resolver::ResolverOverrideKey);
					}
					ImGui::Columns(1);

					ImGui::Separator();

					ImGui::Columns(2, NULL, true);
					{
						ImGui::ItemSize(ImVec2(0.0f, 0.0f), 0.0f);
						ImGui::Text(XorStr("Smart Aim"));
						SetTooltip(XorStr("Will body aim after X shots. Put it on -1 to disable it."));
						ImGui::ItemSize(ImVec2(0.0f, 0.0f), 0.0f);
						ImGui::Checkbox(XorStr("Toggle Smart Aim"), &Settings::SmartAim::enabled);
						SetTooltip(XorStr("*EXPERIMENTAL FEATURE*"));
					}
					ImGui::NextColumn();
					{
						ImGui::PushItemWidth(-1);
						ImGui::SliderInt(XorStr("##BAIMAFTER"), &Settings::Resolver::baimAfter, -1, 10);
						ImGui::PopItemWidth();
					}
					ImGui::Columns(1);
				}
				ImGui::Separator();

				ImGui::Text(XorStr("Disable"));
				{
					ImGui::Separator();

					ImGui::Columns(2, NULL, true);
					{
						ImGui::Checkbox(XorStr("Knife"), &Settings::AntiAim::Misc::AutoDisable::knifeHeld);
						ImGui::Checkbox(XorStr("Bomb"), &Settings::AntiAim::Misc::AutoDisable::bombHeld);
					}
					ImGui::NextColumn();
					{
						ImGui::Checkbox(XorStr("No Enemy"), &Settings::AntiAim::Misc::AutoDisable::noEnemy);
						ImGui::Checkbox(XorStr("Freeze Time"), &Settings::AntiAim::Misc::AutoDisable::freezeTime);
					}
					ImGui::Columns(1);
				}
				ImGui::Separator();

				ImGui::Text(XorStr("Indicators"));
				{
					ImGui::Separator();

					ImGui::Columns(2, NULL, true);
					{
						ImGui::Checkbox(XorStr("Angles"), &Settings::AngleIndicator::enabled);
						ImGui::Checkbox(XorStr("Velocity"), &Settings::AngleIndicator::Veloc);
					}
					ImGui::NextColumn();
					{
						ImGui::Checkbox(XorStr("Lower Body Yaw"), &Settings::lbyindicator::enabled);
						ImGui::Checkbox(XorStr("MirrorCam"), &Settings::SpeedIndicator::enabled);
					}
					ImGui::Columns(1);
				}
				ImGui::Separator();

				ImGui::Text(XorStr("Miscellaneous"));
				ImGui::Separator();

				ImGui::Checkbox(XorStr("Auto Crouch"), &Settings::Aimbot::AutoCrouch::enabled);
				ImGui::Separator();
				ImGui::EndChild();
			}
			ImGui::EndChild();
		}

	}
	ImGui::NextColumn();
	{
		ImGui::BeginChild(XorStr("##HVHMISC2"), ImVec2(0, 0), true);
		{
			ImGui::Text(XorStr("Key Bindings"));
			ImGui::BeginChild(XorStr("##HVHMISC4"), ImVec2(0, 0), true);
			{
				ImGui::Text(XorStr("Movement"));
				{
					ImGui::Separator();

					ImGui::Columns(2, NULL, true);
					{
						ImGui::Checkbox(XorStr("Fakewalk - AIMWARE"), &Settings::FakewalkAW::enabled);
						ImGui::Checkbox(XorStr("Fakewalk"), &Settings::Fakewalk::enabled);
						ImGui::Checkbox(XorStr("Choke Packets"), &Settings::ChokePackets::enabled);
					//	ImGui::Checkbox(XorStr("Circle Strafer"), &Settings::CircleStrafe::enabled);
					}
					ImGui::NextColumn();
					{
						
				/*		UI::KeyBindButton(&Settings::FakewalkAW::key);
						UI::KeyBindButton(&Settings::Fakewalk::key);
						UI::KeyBindButton(&Settings::ChokePackets::key);
						UI::KeyBindButton(&Settings::CircleStrafe::key);*/
						UI::KeyBindButton(&Settings::FakewalkAW::key);
						UI::KeyBindButton(&Settings::Fakewalk::key);
						UI::KeyBindButton(&Settings::ChokePackets::key);
						UI::KeyBindButton(&Settings::CircleStrafe::key);

					/*	ImGui::Combo("##FakewalkAW", (int*)&Settings::FakewalkAW::key, ButtonNamess, IM_ARRAYSIZE(ButtonNamess));
						ImGui::Combo("##FakeWalk", (int*)&Settings::Fakewalk::key, ButtonNamess, IM_ARRAYSIZE(ButtonNamess));
						ImGui::Combo("##ChokePackets", (int*)&Settings::ChokePackets::key, ButtonNamess, IM_ARRAYSIZE(ButtonNamess));
						ImGui::Combo("##CircleStrafe", (int*)&Settings::CircleStrafe::key, ButtonNamess, IM_ARRAYSIZE(ButtonNamess));*/
					}
					ImGui::Columns(1);
				}
				ImGui::Separator();

				ImGui::Text(XorStr("Miscellaneous"));
				{
					ImGui::Separator();

					ImGui::Columns(2, NULL, true);
					{
						ImGui::Checkbox(XorStr("Slow Motion"), &Settings::SlowMo::enabled);
						SetTooltip(XorStr("WARNING: This feature is not working as intended."));
					}
					ImGui::NextColumn();
					{
						UI::KeyBindButton(&Settings::SlowMo::key);
						//ImGui::Combo("##SlowMo", (int*)&Settings::SlowMo::key, ButtonNamess, IM_ARRAYSIZE(ButtonNamess));
					
					}
					ImGui::Columns(1);
				}
				ImGui::Separator();

				ImGui::Text(XorStr("Console"));
				{
					ImGui::Separator();

					// TODO:
					// Implement a console in which it is possible to bind keys
					// like AimWare. e.g: bind m "rbot_aa_yaw_real_add 90"

				}
				ImGui::Separator();

				ImGui::EndChild();
			}
			ImGui::EndChild();
		}
	}
}

