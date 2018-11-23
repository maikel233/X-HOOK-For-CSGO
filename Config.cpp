#include "Config.h"
#include "XorStr.hpp"

#include <iostream>
#include <fstream>
Config* ConSys = new Config();

inline bool Check(std::string File)
{
	struct stat buf;
	return (stat(File.c_str(), &buf) == 0);
}

char Pic1[255], Sound[255], Dust2[255], Mirage[255], Cbble[255], Overpass[255], Cache[255];

bool Config::CheckConfigs()
{
	/*=====================================*/
	strcat(Path, "C:\\Xhook");
	HANDLE File1, File2, File3, File4;
	CreateDirectoryA(Path, nullptr);
	strcpy(Path2, Path);
	strcpy(Path3, Path);
	strcpy(Path4, Path);
	strcpy(Picture, Path);
	strcpy(Configs, Path);
	strcpy(ConfigsNades, Path);
	strcpy(ConfigsNades_de_cache, Path);
	strcpy(ConfigsNades_de_dust2, Path);
	strcpy(ConfigsNades_de_mirage, Path);
	strcpy(ConfigsNades_de_cbble, Path);
	strcpy(ConfigsNades_de_overpass, Path);
	strcpy(Sound, Path);
	strcat(Picture, "\\Pictures");
	strcat(Configs, "\\Configs");
	strcat(ConfigsNades, "\\Configs\\GrenadeHelper");
	strcat(ConfigsNades_de_cache, "\\Configs\\GrenadeHelper\\de_cache");
	strcat(ConfigsNades_de_dust2, "\\Configs\\GrenadeHelper\\de_dust2");
	strcat(ConfigsNades_de_mirage, "\\Configs\\GrenadeHelper\\de_mirage");
	strcat(ConfigsNades_de_cbble, "\\Configs\\GrenadeHelper\\de_cbble");
	strcat(ConfigsNades_de_overpass, "\\Configs\\GrenadeHelper\\de_overpass");

	//Creating Dirs...
	CreateDirectoryA(Picture, nullptr);
	CreateDirectoryA(Configs, nullptr);
	CreateDirectoryA(ConfigsNades, nullptr);
	CreateDirectoryA(ConfigsNades_de_cache, nullptr);
	CreateDirectoryA(ConfigsNades_de_dust2, nullptr);
	CreateDirectoryA(ConfigsNades_de_mirage, nullptr);
	CreateDirectoryA(ConfigsNades_de_cbble, nullptr);
	CreateDirectoryA(ConfigsNades_de_overpass, nullptr);

	strcpy(Pic1, Picture);
	strcat(Pic1, "\\config.json");

	strcpy(Dust2, ConfigsNades_de_dust2);
	strcat(Dust2, "\\config.json");

	strcpy(Mirage, ConfigsNades_de_mirage);
	strcat(Mirage, "\\config.json");

	strcpy(Cache, ConfigsNades_de_cache);
	strcat(Cache, "\\config.json");

	strcpy(Overpass, ConfigsNades_de_overpass);
	strcat(Overpass, "\\config.json");

	strcpy(Cbble, ConfigsNades_de_cbble);
	strcat(Cbble, "\\config.json");

		/*
		URLDownloadToFile(nullptr, XorStr("http://x-hook.xyz/Xhook/pictures/bg.jpg"), Pic1, 0, nullptr); Sleep(400);
		URLDownloadToFile(nullptr, XorStr("http://x-hook.xyz/Xhook/Grenadehelper/de_dust2/config.json"), Dust2, 0, nullptr); Sleep(400);
		URLDownloadToFile(nullptr, XorStr("http://x-hook.xyz/Xhook/Grenadehelper/de_mirage/config.json"), Mirage, 0, nullptr); Sleep(400);
		URLDownloadToFile(nullptr, XorStr("http://x-hook.xyz/Xhook/Grenadehelper/de_cache/config.json"), Cache, 0, nullptr); Sleep(400);
		URLDownloadToFile(nullptr, XorStr("http://x-hook.xyz/Xhook/Grenadehelper/de_overpass/config.json"), Overpass, 0, nullptr); Sleep(400);
		URLDownloadToFile(nullptr, XorStr("http://x-hook.xyz/Xhook/Grenadehelper/de_cbble/config.json"), Cbble, 0, nullptr); Sleep(400);
		*/
		
	//strcat(Path, "\\Rage.ini");
	strcat(Path2, "\\Configs\\Default.ini");

	/*=====================================*/
	File1 = CreateFileA(Path, GENERIC_ALL, FILE_SHARE_READ | FILE_SHARE_WRITE | FILE_SHARE_DELETE, nullptr, CREATE_NEW, FILE_ATTRIBUTE_NORMAL, nullptr);
	if (File1 != INVALID_HANDLE_VALUE)
	{
		CloseHandle(File1);
	}

	File2 = CreateFileA(Path2, GENERIC_ALL, FILE_SHARE_READ | FILE_SHARE_WRITE | FILE_SHARE_DELETE, nullptr, CREATE_NEW, FILE_ATTRIBUTE_NORMAL, nullptr);
	if (File2 != INVALID_HANDLE_VALUE)
	{
		CloseHandle(File2);
	}

	File3 = CreateFileA(Path3, GENERIC_ALL, FILE_SHARE_READ | FILE_SHARE_WRITE | FILE_SHARE_DELETE, nullptr, CREATE_NEW, FILE_ATTRIBUTE_NORMAL, nullptr);
	if (File3 != INVALID_HANDLE_VALUE)
	{
		CloseHandle(File3);
	}

	File4 = CreateFileA(Path4, GENERIC_ALL, FILE_SHARE_READ | FILE_SHARE_WRITE | FILE_SHARE_DELETE, nullptr, CREATE_NEW, FILE_ATTRIBUTE_NORMAL, nullptr);
	if (File4 != INVALID_HANDLE_VALUE)
	{
		CloseHandle(File4);
	}
	return true;
}
