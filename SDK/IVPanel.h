#pragma once

class IVPanel
{
public:
	void set_keyboard_input_enabled(unsigned int panel_id, bool state) {
		using original_fn = void(__thiscall*)(IVPanel*, unsigned int, bool);
		return (*(original_fn**)this)[31](this, panel_id, state);
	}

	void set_mouse_input_enabled(unsigned int panel_id, bool state) {
		using original_fn = void(__thiscall*)(IVPanel*, unsigned int, bool);
		return (*(original_fn**)this)[32](this, panel_id, state);
	}

	const char* GetName(unsigned int panel_id) {
		using original_fn = const char* (__thiscall*)(IVPanel*, unsigned int);
		return (*(original_fn**)this)[36](this, panel_id);
	}
};