#include "modsupport.h"

ModType ModSupport::current_mod = ModType::NO_MOD;

void ModSupport::OnInit()
{
	const char* output;

//	pCommandline->CheckParm("-game", &output);

	if (strstr(output, "csco"))
	{
		current_mod = ModType::CSCO;
		pCvar->ConsoleColorPrintf(ColorRGBA(255, 255, 255), ("Counter-Strike: Classic Offensive detected!\n"));
	}
}