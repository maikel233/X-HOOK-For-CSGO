#include "xhook.h"
bool Open;
void OpenConsole()
{
	if (!FreeConsole()) {
		FILE* fp;
		if(AllocConsole())
			Open = true;
		SetConsoleTitleA("X-HOOK");
		freopen_s(&fp, "CONOUT$", "w", stdout);
	}
}