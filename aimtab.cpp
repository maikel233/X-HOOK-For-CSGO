#include "aimtab.h"
#include "AimBotTab.h"
#include "triggerbottab.h"

static int page = 0;

const char* aimTabs[] = {
	"Aimbot", "Triggerbot"
};

void AimTab::RenderTab() {
	for (int i = 0; i < IM_ARRAYSIZE(aimTabs); i++) {
		int distance = i == page ? 0 : i > page ? i - page : page - i;

		ImVec4 previousCol = ImGui::GetStyle().Colors[ImGuiCol_Button];
		ImGui::GetStyle().Colors[ImGuiCol_Button] = ImVec4(
			Settings::UI::mainColor.Color().Value.x - (distance * 0.035f),
			Settings::UI::mainColor.Color().Value.y - (distance * 0.035f),
			Settings::UI::mainColor.Color().Value.z - (distance * 0.035f),
			Settings::UI::mainColor.Color().Value.w
		);

		if (ImGui::Button(aimTabs[i], ImVec2(ImGui::GetWindowSize().x / IM_ARRAYSIZE(aimTabs) - 9, 0))) {
			page = i;
		}

		ImGui::GetStyle().Colors[ImGuiCol_Button] = previousCol;

		if (i < IM_ARRAYSIZE(aimTabs) - 1) {
			ImGui::SameLine();
		}
	}

	ImGui::Separator();

	switch (page) {
	case 0:
		Aimbot::RenderTab();
		break;
	case 1:
		Triggerbot::RenderTab();
		break;
	}
}
