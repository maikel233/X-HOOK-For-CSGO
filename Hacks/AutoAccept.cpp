#include "../Features.h"
IsReady IsReadyCallback;
bool Settings::AutoAccept::enabled = false;



void AutoAccept::PlaySound(const char* filename)
{

	if (!Settings::AutoAccept::enabled)
		return;

	if (pEngine->IsInGame())
		return;

	if (!strstr(filename, "UIPanorama.popup_accept_match_beep"))
		return;

	IsReadyCallback();

	FLASHWINFO fi;
	fi.cbSize = sizeof(FLASHWINFO);
	fi.hwnd = FindWindow("Valve001", NULL);
	fi.dwFlags = FLASHW_ALL | FLASHW_TIMERNOFG;
	fi.uCount = 0;
	fi.dwTimeout = 0;
	FlashWindowEx(&fi);
}