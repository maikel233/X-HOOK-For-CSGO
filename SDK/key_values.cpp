#include "key_values.h"
#include "../utils/FindPattern.h"
#include "../utils/XorStr.hpp"


//_DWORD *__thiscall key_values_INIT(_DWORD *this, int a2, int a3, signed int a4)
c_key_values::c_key_values( const char* key_name, void* unk, void* unk2 ) {

	static const auto init_address = FindPattern(XorStr("client.dll"), XorStr("55 8B EC 56 8B F1 33 C0 8B 4D 0C 81"));
	reinterpret_cast<void( __thiscall* )(void*, const char*, void*, void*)>(init_address)(this, key_name, unk, unk2);
}


//char __stdcall key_values_load_from_buffer(int a1, int a2, int a3, int a4, int a5, int a6)
void c_key_values::load_from_buffer( const char* resource_name, const char* buf, void* file_sys, const char* path_id, void* eval_sym_proc, void* unk ) {
	static const auto key_values_load_from_buffer = FindPattern("client.dll", "55 8B EC 83 E4 F8 83 EC 34 53 8B 5D 0C 89");
	return reinterpret_cast<void( __thiscall* )(void*, const char*, const char*, void*, const char*, void*, void*)>(key_values_load_from_buffer)(this, resource_name, buf, file_sys, path_id, eval_sym_proc, unk);
}

c_key_values* c_key_values::FindKey(const char* keyName, bool bCreate)
{
	static auto key_values_find_key = reinterpret_cast<c_key_values * (__thiscall*)(void*, const char*, bool)>(FindPattern("client.dll", "55 8B EC 83 EC 1C 53 8B D9 85 DB"));
	return key_values_find_key(this, keyName, bCreate);
}

void c_key_values::SetString(const char* keyName, const char* value)
{
	auto key = FindKey(keyName, true);
	if (key)
	{
		static auto key_values_set_string = reinterpret_cast<void(__thiscall*)(void*, const char*)>(FindPattern("client.dll", "55 8B EC A1 ? ? ? ? 53 56 57 8B F9 8B 08 8B 01"));
		key_values_set_string(key, value);
	}
}
//void c_key_values::InitKeyValues(const char* name)
//{
//	static auto key_values = reinterpret_cast<void(__thiscall*)(void*, const char*)>(Utils_fox.FindPattern("client.dll", "55 8B EC 51 33 C0 C7 45 ? ? ? ? ? 56 8B F1 81 26 ? ? ? ? C6 46 03 ? 89 46 10 89 46 18 89 46 14 89 46 1C 89 46 04 89 46 08 89 46 0C FF 15 ? ? ? ? 6A 01 FF 75 08 8D 4D FC 8B 10 51 8B C8 FF 52 24 8B 0E 33 4D FC 81 E1 ? ? ? ? 31 0E 88 46 03"));
//	key_values(this, name);
//}
void c_key_values::SetUint64(const char* keyName, int value, int value2)
{
	static auto key_values_set_uint64 = reinterpret_cast<void(__thiscall*)(void*, const char*, int, int)>(FindPattern("client.dll", "55 8B EC 56 6A 01 FF 75 08"));
	key_values_set_uint64(this, keyName, value, value2);
}
const char* c_key_values::GetString(const char* keyName, const char* defaultValue)
{
	static auto key_values_get_string = reinterpret_cast<const char* (__thiscall*)(void*, const char*, const char*)>(FindPattern("client.dll", "55 8B EC 83 E4 C0 81 EC ? ? ? ? 53 8B 5D 08"));
	return key_values_get_string(this, keyName, defaultValue);
}
int c_key_values::GetInt(const char* keyName, int defaultValue)
{
	static auto key_values_get_int = reinterpret_cast<int(__thiscall*)(void*, const char*, int)>(FindPattern("client.dll", "55 8B EC 6A ? FF 75 08 E8 ? ? ? ? 85 C0 74 45"));
	return key_values_get_int(this, keyName, defaultValue);
}
void c_key_values::SetInt(const char* keyName, int Value)
{
	auto key_int = FindKey(keyName, true);
	if (key_int)
	{
		*reinterpret_cast<int*>(reinterpret_cast<uintptr_t>(key_int) + 0xC) = Value;
		*reinterpret_cast<char*>(reinterpret_cast<uintptr_t>(key_int) + 0x10) = 2;
	}
}



//_DWORD *__thiscall key_values_INIT(_DWORD *this, int a2, int a3, signed int a4)
//_DWORD *__thiscall KeyValues::FindKey(_DWORD *this, char *a2, int a3)
//void *__thiscall KeyValues::SetString(_DWORD *this, void *a2)
//int *__thiscall KeyValues::SetUint64(_DWORD *this, char *a1, int a2, int a3)



//bool KeyValues_FindKey(c_key_values* key, const char* keyName, bool bCreate = false)
//{
//	auto key_values_find_key = reinterpret_cast<c_key_values * (__thiscall*)(void*, const char*, bool)>(FindPattern("client.dll", "55 8B EC 83 EC 1C 53 8B D9 85 DB"));//Utils_fox.FindPatternMask("client_panorama.dll", (PBYTE)"\x55\x8B\xEC\x83\xEC\x1C\x53\x8B\xD9\x85\xDB", "xxxxxxxxxxx"));
//	return key_values_find_key(key, keyName, bCreate);
//}
//
//void KeyValues_SetString(c_key_values* key, const char* value)
//{
//	auto key_values_set_string = reinterpret_cast<void(__thiscall*)(void*, const char*)>(FindPattern("client.dll", "55 8B EC A1 ? ? ? ? 53 56 57 8B F9 8B 08 8B 01")); //Utils_fox.FindPatternMask("client_panorama.dll", (PBYTE)"\x55\x8B\xEC\xA1\x00\x00\x00\x00\x53\x56\x57\x8B\xF9\x8B\x08\x8B\x01", "xxxx????xxxxxxxxx"));
//	key_values_set_string(key, value);
//}