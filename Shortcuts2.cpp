#include "Features.h"

void Shortcuts::SetKeyCodeState(ButtonCode_t code, bool bPressed) {
#ifdef EXPERIMENTAL_SETTINGS
	if (pInputSystem->IsButtonDown(ButtonCode_t::KEY_LALT) && code == ButtonCode_t::KEY_F && bPressed)
		Settings::ThirdPerson::enabled = !Settings::ThirdPerson::enabled;
#endif
}