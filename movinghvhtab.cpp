#include "Settings.h"
#include "ImGui\imgui.h"
#include "ImGui\imgui_internal.h"
#include "Util_Items.h"
#include "movinghvhtab.h"
#include "HvHTab.h"
#include "XorStr.hpp"
#include "Menu.h"

void Moving::RenderTab() {
	ImGui::Columns(2, NULL, true);
	{
		ImGui::BeginChild(XorStr("##ANTIAIM1"), ImVec2(0, 0), true);
		{
			ImGui::Text(XorStr("Anti-Aim"));
			ImGui::BeginChild(XorStr("##ANTIAIM3"), ImVec2(0, 0), true);
			{
				ImGui::Checkbox(XorStr("Yaw"), &Settings::AntiAim::Moving::Yaw::enabled);
				{
					ImGui::Separator();

					ImGui::Columns(2, NULL, true);
					{
						{
							ImGui::ItemSize(ImVec2(0.0f, 0.0f), 0.0f);
							ImGui::Text(XorStr("Fake Yaw"));
							ImGui::ItemSize(ImVec2(0.0f, 0.0f), 0.0f);
							ImGui::Text(XorStr("Fake Yaw Add"));
							ImGui::ItemSize(ImVec2(0.0f, 0.0f), 0.0f);
						}
						//ImGui::Separator();
						{
							ImGui::ItemSize(ImVec2(0.0f, 0.0f), 0.0f);
							ImGui::ItemSize(ImVec2(0.0f, 0.0f), 0.0f);
							ImGui::Text(XorStr("Real Yaw"));
							ImGui::ItemSize(ImVec2(0.0f, 0.0f), 0.0f);
							ImGui::Text(XorStr("Real Yaw Add"));
							ImGui::ItemSize(ImVec2(0.0f, 0.0f), 0.0f);
						}
					}
					ImGui::NextColumn();
					{
						{
							ImGui::PushItemWidth(-3);
							ImGui::Combo(XorStr("##YFAKE"), (int*)&Settings::AntiAim::Moving::Yaw::typeFake,
								HvH::yTypes, (int)AntiAimType_Y::NUMBER_OF_TYPES);
							ImGui::PopItemWidth();
							ImGui::PushItemWidth(-1);
							ImGui::SliderFloat(XorStr("##YFAKECUSTOM"), &Settings::AntiAim::Moving::Yaw::typeFakeAdd,
								Settings::AntiAim::Moving::untrustedAngles ? -1000 : -180,
								Settings::AntiAim::Moving::untrustedAngles ? 1000 : 180,
								XorStr(Settings::AntiAim::Moving::Yaw::typeFakeAdd > 0 ? "+%.1f"
									: "%.1f"));
							ImGui::PopItemWidth();
						}
						ImGui::Separator();
						{
							ImGui::PushItemWidth(-3);
							ImGui::Combo(XorStr("##YREAL"), (int*)&Settings::AntiAim::Moving::Yaw::type,
								HvH::yTypes, (int)AntiAimType_Y::NUMBER_OF_TYPES);
							ImGui::PopItemWidth();
							ImGui::PushItemWidth(-1);
							ImGui::SliderFloat(XorStr("##YCUSTOM"), &Settings::AntiAim::Moving::Yaw::typeAdd,
								Settings::AntiAim::Moving::untrustedAngles ? -1000 : -180,
								Settings::AntiAim::Moving::untrustedAngles ? 1000 : 180,
								XorStr(Settings::AntiAim::Moving::Yaw::typeAdd > 0 ? "+%.1f"
									: "%.1f"));
							ImGui::PopItemWidth();
						}
					}
					ImGui::Columns(1);
				}
				ImGui::Separator();

				ImGui::Checkbox(XorStr("Pitch"), &Settings::AntiAim::Moving::Pitch::enabled);
				{
					ImGui::Separator();

					ImGui::Columns(2, NULL, true);
					{
						ImGui::ItemSize(ImVec2(0.0f, 0.0f), 0.0f);
						ImGui::Text(XorStr("Real Pitch"));
						ImGui::ItemSize(ImVec2(0.0f, 0.0f), 0.0f);
						ImGui::Text(XorStr("Custom Pitch"));
						ImGui::ItemSize(ImVec2(0.0f, 0.0f), 0.0f);
					}
					ImGui::NextColumn();
					{
						ImGui::PushItemWidth(-3);
						ImGui::Combo(XorStr("##XREAL"), (int*)&Settings::AntiAim::Moving::Pitch::type,
							HvH::xTypes, (int)AntiAimType_X::NUMBER_OF_TYPES);
						ImGui::PopItemWidth();
						ImGui::PushItemWidth(-1);
						ImGui::SliderFloat(XorStr("##XCUSTOM"), &Settings::AntiAim::Moving::Pitch::custom,
							Settings::AntiAim::Moving::untrustedAngles ? -1000 : -180,
							Settings::AntiAim::Moving::untrustedAngles ? 1000 : 180,
							XorStr(Settings::AntiAim::Moving::Pitch::custom > 0 ? "+%.1f" : "%.1f"));
						ImGui::PopItemWidth();
					}
					ImGui::Columns(1);
				}
				ImGui::Separator();

				ImGui::Checkbox(XorStr("Roll"), &Settings::AntiAim::Moving::Roll::enabled);
				{
					ImGui::Separator();

					ImGui::Columns(2, NULL, true);
					{
						ImGui::ItemSize(ImVec2(0.0f, 0.0f), 0.0f);
						ImGui::Text(XorStr("Real Roll"));
						ImGui::ItemSize(ImVec2(0.0f, 0.0f), 0.0f);
					}
					ImGui::NextColumn();
					{
						ImGui::PushItemWidth(-3);
						ImGui::Combo(XorStr("##ZREAL"), (int*)&Settings::AntiAim::Moving::Roll::type,
							HvH::zTypes, (int)AntiAimType_Z::NUMBER_OF_TYPES);
						ImGui::PopItemWidth();
					}
					ImGui::Columns(1);
				}
				ImGui::Separator();

				ImGui::Checkbox(XorStr("Switch Anti-Aim"), &Settings::AntiAim::Moving::SwitchAA::enabled);
				{
					ImGui::Separator();

					ImGui::Columns(2, NULL, true);
					{
						ImGui::ItemSize(ImVec2(0.0f, 0.0f), 0.0f);
						ImGui::Text(XorStr("Key"));
						ImGui::ItemSize(ImVec2(0.0f, 0.0f), 0.0f);
					}
					ImGui::NextColumn();
					{
						UI::KeyBindButton(&Settings::AntiAim::Moving::SwitchAA::key);
					}
					ImGui::Columns(1);
				}
				ImGui::Separator();

				ImGui::Checkbox(XorStr("LBY Breaker"), &Settings::AntiAim::Moving::LBY::enabled);
				{
					ImGui::Separator();

					ImGui::Columns(2, NULL, true);
					{
						ImGui::ItemSize(ImVec2(0.0f, 0.0f), 0.0f);
						ImGui::Text(XorStr("Breaker"));
						ImGui::ItemSize(ImVec2(0.0f, 0.0f), 0.0f);
					}
					ImGui::NextColumn();
					{
						ImGui::PushItemWidth(-3);
						ImGui::Combo(XorStr("##LBYREAL"), (int*)&Settings::AntiAim::Moving::LBY::type,
							HvH::lbyTypes, (int)AntiAimType_LBY::NUMBER_OF_TYPES);
						ImGui::PopItemWidth();
					}
					ImGui::Columns(1);
				}
				ImGui::Separator();

				ImGui::Checkbox(XorStr("Head Edge"), &Settings::AntiAim::Airborne::HeadEdge::enabled);
				{
					ImGui::Separator();

					ImGui::Columns(2, NULL, true);
					{
						ImGui::ItemSize(ImVec2(0.0f, 0.0f), 0.0f);
						ImGui::Text(XorStr("Edge Distance"));
						ImGui::ItemSize(ImVec2(0.0f, 0.0f), 0.0f);
						ImGui::Text(XorStr("Edge Real Add"));
						ImGui::ItemSize(ImVec2(0.0f, 0.0f), 0.0f);
						ImGui::Text(XorStr("Edge Fake Add"));
						ImGui::ItemSize(ImVec2(0.0f, 0.0f), 0.0f);
					}
					ImGui::NextColumn();
					{
						ImGui::PushItemWidth(-1);
						ImGui::SliderFloat(XorStr("##EDGEDISTANCE"),
							&Settings::AntiAim::Moving::HeadEdge::distance, 20, 30
						);
						ImGui::SliderFloat(XorStr("##EDGEREALADD"),
							&Settings::AntiAim::Moving::HeadEdge::realAdd,
							Settings::AntiAim::Moving::untrustedAngles ? -1000 : -180,
							Settings::AntiAim::Moving::untrustedAngles ? 1000 : 180,
							XorStr(Settings::AntiAim::Moving::HeadEdge::realAdd > 0 ? "+%.1f"
								: "%.1f")
						);
						ImGui::SliderFloat(XorStr("##EDGEFAKEADD"),
							&Settings::AntiAim::Moving::HeadEdge::fakeAdd,
							Settings::AntiAim::Moving::untrustedAngles ? -1000 : -180,
							Settings::AntiAim::Moving::untrustedAngles ? 1000 : 180,
							XorStr(Settings::AntiAim::Moving::HeadEdge::fakeAdd > 0 ? "+%.1f"
								: "%.1f")
						);
						ImGui::PopItemWidth();
					}
					ImGui::Columns(1);
				}
				ImGui::Separator();
				ImGui::EndChild();
			}
			ImGui::EndChild();
		}
	}
	ImGui::NextColumn();
	{
		ImGui::BeginChild(XorStr("##ANTIAIM2"), ImVec2(0, 0), true);
		{
			ImGui::Text(XorStr("Options"));
			ImGui::BeginChild(XorStr("##AAOPTIONS"), ImVec2(0, 0), true);
			{
				ImGui::Checkbox(XorStr("Anti Resolver"), &Settings::AntiAim::Moving::antiResolver);
				ImGui::Separator();
				ImGui::Checkbox(XorStr("Dynamic Anti-Aim"), &Settings::AntiAim::Moving::dynamicAA);
				ImGui::Separator();
				ImGui::Checkbox(XorStr("Untrusted angles"), &Settings::AntiAim::Moving::untrustedAngles);
				ImGui::Separator();
				{
					if (IsSpin() || IsCustom()) {
						ImGui::BeginChild(XorStr("##ANTIAIM4"), ImVec2(0, 0), true);
						{
							if (IsSpin()) {
								ImGui::Text(XorStr("Spin Options"));
								ImGui::Separator();
								ImGui::Columns(2, NULL, true);
								{
									ImGui::ItemSize(ImVec2(0.0f, 0.0f), 0.0f);
									ImGui::Text(XorStr("Spin Factor"));
									ImGui::ItemSize(ImVec2(0.0f, 0.0f), 0.0f);
								}
								ImGui::NextColumn();
								{
									ImGui::PushItemWidth(-1);
									ImGui::SliderInt(XorStr("##spinFactor"), &Settings::spinFactor::value, 0, 100,
										XorStr("%1.f"));
									ImGui::PopItemWidth();
								}
								ImGui::Columns(1);

								ImGui::Separator();
							}
							if (IsCustom() != 0) {
								if (IsCustom() >= 2) {
									ImGui::Text(XorStr("Custom Yaw"));
									ImGui::Separator();
									ImGui::PushItemWidth(-1);
									ImGui::Checkbox(XorStr("Use LBY"), &Settings::customYaw::lby);
									ImGui::SliderInt(XorStr("##customYaw"), &Settings::customYaw::value,
										!Settings::AntiAim::Moving::untrustedAngles ? -180 : -10000,
										!Settings::AntiAim::Moving::untrustedAngles ? 180 : 10000,
										XorStr("Amount: %1.f"));
									ImGui::PopItemWidth();
									ImGui::Separator();
								}
								if (IsCustom() != 2) {
									ImGui::Text(XorStr("Custom Yaw 2"));
									ImGui::Separator();
									ImGui::PushItemWidth(-1);
									ImGui::Checkbox(XorStr("Use LBY"), &Settings::customYaw2::lby);
									ImGui::SliderInt(XorStr("##customYaw"), &Settings::customYaw2::value,
										!Settings::AntiAim::Moving::untrustedAngles ? -180 : -10000,
										!Settings::AntiAim::Moving::untrustedAngles ? 180 : 10000,
										XorStr("Amount: %1.f"));
									ImGui::PopItemWidth();
									ImGui::Separator();
								}
							}
							ImGui::Separator();
							ImGui::EndChild();
						}
					}
				}
				ImGui::EndChild();
			}
			ImGui::EndChild();
		}
	}
}

bool Moving::IsSpin() {
	AntiAimType_Y aa = Settings::AntiAim::Moving::Yaw::type;
	AntiAimType_Y aaf = Settings::AntiAim::Moving::Yaw::typeFake;

	return aa == AntiAimType_Y::SPIN || aaf == AntiAimType_Y::SPIN ||
		aa == AntiAimType_Y::LBYSPIN || aaf == AntiAimType_Y::LBYSPIN;
}

int Moving::IsCustom() {
	AntiAimType_Y aa = Settings::AntiAim::Moving::Yaw::type;
	AntiAimType_Y aaf = Settings::AntiAim::Moving::Yaw::typeFake;

	if ((aa == AntiAimType_Y::CUSTOM || aaf == AntiAimType_Y::CUSTOM) &&
		(aa == AntiAimType_Y::CUSTOM2 || aaf == AntiAimType_Y::CUSTOM2)) {
		return 3;
	}
	else if (aa == AntiAimType_Y::CUSTOM || aaf == AntiAimType_Y::CUSTOM) {
		return 2;
	}
	else if (aa == AntiAimType_Y::CUSTOM2 || aaf == AntiAimType_Y::CUSTOM2) {
		return 1;
	}
	else {
		return 0;
	}
}
