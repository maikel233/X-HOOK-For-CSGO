#pragma once
#include "ImGui\imgui.h"
#include "Interfaces.h"
#include "Renderer.h"
void Menu();

namespace UI {

	void KeyBindButton(ButtonCode_t* key);
}

inline void SetTooltip(const char* text) {
	if (ImGui::IsItemHovered())
		ImGui::SetTooltip("%s", text);
}
