#pragma once

#include <Windows.h>
#include <string>
#include <Psapi.h>
#include <vector>
#include <string>
#include "../SDK/key_values.h"
#include "../Utils/FindPattern.h"
#include "../SDK/getvfunc.h"


#pragma comment(lib, "libprotobuf.lib")
#pragma comment(lib, "libprotoc.lib")


typedef PVOID(__cdecl* oKeyValuesSystem)();

namespace KeyValues_Func
{

	inline void GameFunc_InitKeyValues(c_key_values* key, const char* name, void* unk, void* unk2)
	{
		auto key_values = reinterpret_cast<void(__thiscall*)(void*, const char*, void*, void*)>(FindPattern("client.dll", "55 8B EC 56 8B F1 33 C0 8B 4D 0C 81"));
		key_values(key, name, unk, unk2);
	}

	inline void KeyValues_SetUint64(c_key_values* key, const char* keyName, int value, int value2)
	{
		auto key_values_set_uint64 = reinterpret_cast<void(__thiscall*)(void*, const char*, int, int)>(FindPattern("client.dll", "55 8B EC 56 6A 01 FF 75 08"));
		key_values_set_uint64(key, keyName, value, value2);
	}

	inline const char* KeyValues_GetString(c_key_values* key, const char* keyName, const char* defaultValue)
	{
		auto key_values_get_string = reinterpret_cast<const char* (__thiscall*)(void*, const char*, const char*)>(FindPattern("client.dll", "55 8B EC 83 E4 C0 81 EC ? ? ? ? 53 8B 5D 08"));//Utils_fox.FindPatternMask("client_panorama.dll", (PBYTE)"\x55\x8B\xEC\x83\xE4\xC0\x81\xEC\x00\x00\x00\x00\x53\x8B\x5D\x08", "xxxxxxxx????xxxx"));
		return key_values_get_string(key, keyName, defaultValue);
	}

	inline int KeyValues_GetInt(c_key_values* key, const char* keyName, int defaultValue)
	{
		auto key_values_get_int = reinterpret_cast<int(__thiscall*)(void*, const char*, int)>(FindPattern("client.dll", "55 8B EC 6A ? FF 75 08 E8 ? ? ? ? 85 C0 74 45"));/*Utils_fox.FindPatternMask("client_panorama.dll", (PBYTE)"\x55\x8B\xEC\x6A\x00\xFF\x75\x08\xE8\x00\x00\x00\x00\x85\xC0\x74\x45", "xxxxxxxxx????xxxx"));*/
		return key_values_get_int(key, keyName, defaultValue);
	}

	inline void KeyValues_SetString(c_key_values* key, const char* value)
	{
		auto key_values_set_string = reinterpret_cast<void(__thiscall*)(void*, const char*)>(FindPattern("client.dll", "55 8B EC A1 ? ? ? ? 53 56 57 8B F9 8B 08 8B 01")); //Utils_fox.FindPatternMask("client_panorama.dll", (PBYTE)"\x55\x8B\xEC\xA1\x00\x00\x00\x00\x53\x56\x57\x8B\xF9\x8B\x08\x8B\x01", "xxxx????xxxxxxxxx"));
		key_values_set_string(key, value);
	}

	inline c_key_values* FindKey(c_key_values* key, const char* keyName, bool bCreate = false)
	{
		auto key_values_find_key = reinterpret_cast<c_key_values * (__thiscall*)(void*, const char*, bool)>(FindPattern("client.dll", "55 8B EC 83 EC 1C 53 8B D9 85 DB"));
		return key_values_find_key(key, keyName, bCreate);
	}

	inline void KeyValues_SetInt(c_key_values* key, const char* keyName, int Value)
	{
		auto key_int = FindKey(key, keyName);
		if (key_int)
		{
			*reinterpret_cast<int*>(reinterpret_cast<uintptr_t>(key_int) + 0xC) = Value;
			*reinterpret_cast<char*>(reinterpret_cast<uintptr_t>(key_int) + 0x10) = 2;
		}
	}
}

class PlayerLocal
{
public:
	int GetXUIDLow()
	{
		return *reinterpret_cast<int*>(reinterpret_cast<uintptr_t>(this) + 0x8);
	}

	int GetXUIDHigh()
	{
		return *reinterpret_cast<int*>(reinterpret_cast<uintptr_t>(this) + 0xC);
	}

	const char* GetName()
	{
		typedef const char* (__thiscall* tPaster)(void*);
		return getvfunc<tPaster>(this, 2)(this);
	}
};

class PlayerManager
{
public:
	PlayerLocal* GetLocalPlayer(int un)
	{
		typedef PlayerLocal* (__thiscall* tTo)(void*, int);
		return getvfunc<tTo>(this, 1)(this, un);
	}
};

class CMatchSystem
{
public:
	PlayerManager* GetPlayerManager()
	{
		typedef PlayerManager* (__thiscall* tKapis)(void*);
		return getvfunc<tKapis>(this, 0)(this);
	}
};

class CMatchSessionOnlineHost;

class CMatchFramework
{
public:
	/*CMatchEventsSubscription* GetEventsSubscription()
	{
		typedef CMatchEventsSubscription*(__thiscall* tGetEventsSubscription)(void*);
		return call_vfunc<tGetEventsSubscription>(this, 11)(this);
	}*/

	CMatchSessionOnlineHost* GetMatchSession()
	{
		typedef CMatchSessionOnlineHost* (__thiscall* tGetMatchSession)(void*);
		return getvfunc<tGetMatchSession>(this, 13)(this);
	}

	CMatchSystem* GetMatchSystem()
	{
		typedef CMatchSystem* (__thiscall* tGetMatchSystem)(void*);
		return getvfunc<tGetMatchSystem>(this, 15)(this);
	}
};


class CMatchSessionOnlineHost
{
public:

	void Command(c_key_values* vl)
	{
		typedef void(__thiscall* FoxFox)(void*, c_key_values*);
		return getvfunc<FoxFox>(this, 3)(this, vl);
	}
	c_key_values* GetSessionSettings()
	{
		typedef c_key_values* (__thiscall* tGetSessionSettings)(void*);
		return getvfunc<tGetSessionSettings>(this, 1)(this);
	}
	void UpdateSessionSettings(c_key_values* packet)
	{
		typedef void(__thiscall* tUpdateSessionSettings)(void*, c_key_values*);
		getvfunc<tUpdateSessionSettings>(this, 2)(this, packet);
	}
};

extern CMatchFramework* pMatchFramework;

