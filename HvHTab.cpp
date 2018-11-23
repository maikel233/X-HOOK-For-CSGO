#include "Settings.h"
#include "ImGui\imgui.h"
#include "ImGui\imgui_internal.h"
#include "Util_Items.h"
#include "hvhtab.h"
#include "standinghvhtab.h"
#include "movinghvhtab.h"
#include "airbornehvhtab.h"
#include "MiscHvHTab.h"
#include "Xorstr.hpp"

int Settings::spinFactor::value = 1;
int Settings::customYaw::value = 1;
int Settings::customYaw2::value = 1;
bool Settings::customYaw::lby;
bool Settings::customYaw2::lby;

inline void SetTooltip(const char* text)
{
	if (ImGui::IsItemHovered())
		ImGui::SetTooltip("%s", text);
}

static int page = 0;

static const char* tabs[] = {
	"Standing", "Moving", "Airborne", "Miscellaneous"
};
const char* HvH::lbyTypes[] = { "Breaker 1", "Breaker 2", "Breaker 3", "Avoz", "Myrrib", "Myrrib 2", "Suicide Breaker", "None" };
const char* HvH::xTypes[] = { "Up", "Flip", "Down", "Fake Zer0", "Fake Up" };
const char* HvH::yTypes[] = {
	"No AA", "Forwards", "Backwards", "Spin", "Fake Sideways", "Backjitter","Backjitter2", "FJitter", "Kidua AA", "LBY Jitter", "LBY Spin", "LBY", "LBY Breaker", "Fake LBY", "LBY on Ground", "MasterLooser", //LBY Based
	"Tank 1", "Tank 2", "Tank 3", "pAA Fake", "pAA Real", "Myrrib's eap", "Richi's eap", "Rasp 1", "Rasp 2", "FeetWiggle", "Legit Trolling 1", "Legit Trolling 2", "Custom 1", "Custom 2" // Other
};
const char* HvH::zTypes[] = { "TEST" };


void HvH::RenderTab() {
	for (int i = 0; i < IM_ARRAYSIZE(tabs); i++) {
		int distance = i == page ? 0 : i > page ? i - page : page - i;

		ImVec4 previousCol = ImGui::GetStyle().Colors[ImGuiCol_Button];
		ImGui::GetStyle().Colors[ImGuiCol_Button] = ImVec4(
			Settings::UI::mainColor.Color().Value.x - (distance * 0.035f),
			Settings::UI::mainColor.Color().Value.y - (distance * 0.035f),
			Settings::UI::mainColor.Color().Value.z - (distance * 0.035f),
			Settings::UI::mainColor.Color().Value.w
		);

		if (ImGui::Button(tabs[i], ImVec2(ImGui::GetWindowSize().x / IM_ARRAYSIZE(tabs) - 9, 0))) {
			page = i;
		}

		ImGui::GetStyle().Colors[ImGuiCol_Button] = previousCol;

		if (i < IM_ARRAYSIZE(tabs) - 1) {
			ImGui::SameLine();
		}
	}

	ImGui::Separator();

	switch (page) {
	case 0:
		Standing::RenderTab();
		break;
	case 1:
		Moving::RenderTab();
		break;
	case 2:
		Airborne::RenderTab();
		break;
	case 3:
		MiscHvHTab::RenderTab();
		break;
	}
}
