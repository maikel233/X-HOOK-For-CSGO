#include "Settings.h"
#include "ImGui\imgui.h"
#include "ImGui\imgui_internal.h"
#include "Util_Items.h"
#include "airbornehvhtab.h"
#include "HvHTab.h"
#include "XorStr.hpp"
#include "Menu.h"

void Airborne::RenderTab() {
	ImGui::Columns(2, NULL, true);
	{
		ImGui::BeginChild(XorStr("##ANTIAIM1"), ImVec2(0, 0), true);
		{
			ImGui::Text(XorStr("Anti-Aim"));
			ImGui::BeginChild(XorStr("##ANTIAIM3"), ImVec2(0, 0), true);
			{
				ImGui::Checkbox(XorStr("Yaw"), &Settings::AntiAim::Airborne::Yaw::enabled);
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
							ImGui::Combo(XorStr("##YFAKE"), (int*)&Settings::AntiAim::Airborne::Yaw::typeFake,
								HvH::yTypes, (int)AntiAimType_Y::NUMBER_OF_TYPES);
							ImGui::PopItemWidth();
							ImGui::PushItemWidth(-1);
							ImGui::SliderFloat(XorStr("##YFAKECUSTOM"),
								&Settings::AntiAim::Airborne::Yaw::typeFakeAdd,
								Settings::AntiAim::Airborne::untrustedAngles ? -1000 : -180,
								Settings::AntiAim::Airborne::untrustedAngles ? 1000 : 180,
								XorStr(Settings::AntiAim::Airborne::Yaw::typeFakeAdd > 0 ? "+%.1f"
									: "%.1f"));
							ImGui::PopItemWidth();
						}
						ImGui::Separator();
						{
							ImGui::PushItemWidth(-3);
							ImGui::Combo(XorStr("##YREAL"), (int*)&Settings::AntiAim::Airborne::Yaw::type,
								HvH::yTypes, (int)AntiAimType_Y::NUMBER_OF_TYPES);
							ImGui::PopItemWidth();
							ImGui::PushItemWidth(-1);
							ImGui::SliderFloat(XorStr("##YCUSTOM"), &Settings::AntiAim::Airborne::Yaw::typeAdd,
								Settings::AntiAim::Airborne::untrustedAngles ? -1000 : -180,
								Settings::AntiAim::Airborne::untrustedAngles ? 1000 : 180,
								XorStr(Settings::AntiAim::Airborne::Yaw::typeAdd > 0 ? "+%.1f"
									: "%.1f"));
							ImGui::PopItemWidth();
						}
					}
					ImGui::Columns(1);
				}
				ImGui::Separator();

				ImGui::Checkbox(XorStr("Pitch"), &Settings::AntiAim::Airborne::Pitch::enabled);
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
						ImGui::Combo(XorStr("##XREAL"), (int*)&Settings::AntiAim::Airborne::Pitch::type,
							HvH::xTypes, (int)AntiAimType_X::NUMBER_OF_TYPES);
						ImGui::PopItemWidth();
						ImGui::PushItemWidth(-1);
						ImGui::SliderFloat(XorStr("##XCUSTOM"), &Settings::AntiAim::Airborne::Pitch::custom,
							Settings::AntiAim::Airborne::untrustedAngles ? -1000 : -180,
							Settings::AntiAim::Airborne::untrustedAngles ? 1000 : 180,
							XorStr(Settings::AntiAim::Airborne::Pitch::custom > 0 ? "+%.1f"
								: "%.1f"));
						ImGui::PopItemWidth();
					}
					ImGui::Columns(1);
				}
				ImGui::Separator();

				ImGui::Checkbox(XorStr("Roll"), &Settings::AntiAim::Airborne::Roll::enabled);
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
						ImGui::Combo(XorStr("##ZREAL"), (int*)&Settings::AntiAim::Airborne::Roll::type,
							HvH::zTypes, (int)AntiAimType_Z::NUMBER_OF_TYPES);
						ImGui::PopItemWidth();
					}
					ImGui::Columns(1);
				}
				ImGui::Separator();

				ImGui::Checkbox(XorStr("Switch Anti-Aim"), &Settings::AntiAim::Airborne::SwitchAA::enabled);
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
					//	Publics::KeyBindButton(&Settings::AntiAim::Airborne::SwitchAA::key);
					//	ImGui::Combo("##AirborneSwitchAA", (int*)&Settings::AntiAim::Airborne::SwitchAA::key, ButtonNames2, IM_ARRAYSIZE(ButtonNames2));
						UI::KeyBindButton(&Settings::AntiAim::Airborne::SwitchAA::key);

					}
					ImGui::Columns(1);
				}
				ImGui::Separator();

				ImGui::Checkbox(XorStr("LBY Breaker"), &Settings::AntiAim::Airborne::LBY::enabled);
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
						ImGui::Combo(XorStr("##LBYREAL"), (int*)&Settings::AntiAim::Airborne::LBY::type,
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
							&Settings::AntiAim::Airborne::HeadEdge::distance, 20, 30
						);
						ImGui::SliderFloat(XorStr("##EDGEREALADD"),
							&Settings::AntiAim::Airborne::HeadEdge::realAdd,
							Settings::AntiAim::Airborne::untrustedAngles ? -1000 : -180,
							Settings::AntiAim::Airborne::untrustedAngles ? 1000 : 180,
							XorStr(Settings::AntiAim::Airborne::HeadEdge::realAdd > 0 ? "+%.1f"
								: "%.1f")
						);
						ImGui::SliderFloat(XorStr("##EDGEFAKEADD"),
							&Settings::AntiAim::Airborne::HeadEdge::fakeAdd,
							Settings::AntiAim::Airborne::untrustedAngles ? -1000 : -180,
							Settings::AntiAim::Airborne::untrustedAngles ? 1000 : 180,
							XorStr(Settings::AntiAim::Airborne::HeadEdge::fakeAdd > 0 ? "+%.1f"
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
				ImGui::Checkbox(XorStr("Anti Resolver"), &Settings::AntiAim::Airborne::antiResolver);
				ImGui::Separator();
				ImGui::Checkbox(XorStr("Dynamic Anti-Aim"), &Settings::AntiAim::Airborne::dynamicAA);
				ImGui::Separator();
				ImGui::Checkbox(XorStr("Untrusted angles"), &Settings::AntiAim::Airborne::untrustedAngles);
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
										!Settings::AntiAim::Airborne::untrustedAngles ? -180 : -10000,
										!Settings::AntiAim::Airborne::untrustedAngles ? 180 : 10000,
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
										!Settings::AntiAim::Airborne::untrustedAngles ? -180 : -10000,
										!Settings::AntiAim::Airborne::untrustedAngles ? 180 : 10000,
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

bool Airborne::IsSpin() {
	AntiAimType_Y aa = Settings::AntiAim::Airborne::Yaw::type;
	AntiAimType_Y aaf = Settings::AntiAim::Airborne::Yaw::typeFake;

	return aa == AntiAimType_Y::SPIN || aaf == AntiAimType_Y::SPIN ||
		aa == AntiAimType_Y::LBYSPIN || aaf == AntiAimType_Y::LBYSPIN;
}

int Airborne::IsCustom() {
	AntiAimType_Y aa = Settings::AntiAim::Airborne::Yaw::type;
	AntiAimType_Y aaf = Settings::AntiAim::Airborne::Yaw::typeFake;

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
