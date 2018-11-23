#pragma once
#include "sdk.h"
#include "Shared.h"
#include "Settings.h"
#pragma comment(lib,"urlmon.lib")
class Config
{
public:
	bool CheckConfigs();
	bool Legit, Rage, RageMM;
private:
	char Path[255];
	char Path2[255];
	char Path3[255];
	char Path4[255];
	char Picture[255];
	char Configs[255];
	char ConfigsNades[255];
	char ConfigsNades_de_dust2[255];
	char ConfigsNades_de_cache[255];
	char ConfigsNades_de_mirage[255];
	char ConfigsNades_de_cbble[255];
	char ConfigsNades_de_overpass[255];
};

extern Config* ConSys;