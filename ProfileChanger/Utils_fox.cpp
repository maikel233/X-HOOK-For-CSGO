#include "Utils_fox.h"
//
//std::string randomName(int length) {
//
//	char consonents[] = { 'b','c','d','f','g','h','j','k','l','m','n','p','q','r','s','t','v','w','x','z' };
//	char vowels[] = { 'a','e','i','o','u','y' };
//
//	std::string name = "";
//
//	int random = rand() % 2;
//	int count = 0;
//
//	for (int i = 0; i < length; i++) {
//
//		if (random < 2 && count < 2) {
//			name = name + consonents[rand() % 19];
//			count++;
//		}
//		else {
//			name = name + vowels[rand() % 5];
//			count = 0;
//		}
//
//		random = rand() % 2;
//
//	}
//
//	return name;
//
//}
//
//
//void HexDumpMemory(void *addr, int len)
//{
//	int i;
//	unsigned char buff[17];
//	unsigned char *pc = (unsigned char*)addr;
//
//	// Process every byte in the data.
//	for (i = 0; i < len; i++) {
//		// Multiple of 16 means new line (with line offset).
//
//		if ((i % 16) == 0) {
//			// Just don't print ASCII for the zeroth line.
//			if (i != 0)
//				printf("  %s\n", buff);
//
//			// Output the offset.
//			printf("  %04X ", i);
//		}
//
//		// Now the hex code for the specific character.
//		printf(" %02X", pc[i]);
//		// And store a printable ASCII character for later.
//		if ((pc[i] < 0x20) || (pc[i] > 0x7e)) {
//			buff[i % 16] = '.';
//		}
//		else {
//			buff[i % 16] = pc[i];
//		}
//
//		buff[(i % 16) + 1] = '\0';
//	}
//
//	// Pad out last line if not exactly 16 characters.
//	while ((i % 16) != 0) {
//		printf("   ");
//		i++;
//	}
//
//	// And print the final ASCII bit.
//	printf("  %s\n", buff);
//}

//KeyValuess* KeyValuess::FindKey(const char *keyName, bool bCreate)
//{
//	static auto key_values_find_key = reinterpret_cast<KeyValuess*(__thiscall*)(void*, const char*, bool)>(FindPattern("client.dll", "55 8B EC 83 EC 1C 53 8B D9 85 DB"));
//	return key_values_find_key(this, keyName, bCreate);
//}
//
//void KeyValuess::SetString(const char* keyName, const char* value)
//{
//	auto key = FindKey(keyName, true);
//	if (key)
//	{
//		static auto key_values_set_string = reinterpret_cast<void(__thiscall*)(void*, const char*)>(FindPattern("client.dll", "55 8B EC A1 ? ? ? ? 53 56 57 8B F9 8B 08 8B 01"));
//		key_values_set_string(key, value);
//	}
//}
//void KeyValuess::InitKeyValues(const char* name)
//{
//	static auto key_values = reinterpret_cast<void(__thiscall*)(void*, const char*)>(FindPattern("client.dll", "55 8B EC 51 33 C0 C7 45 ? ? ? ? ? 56 8B F1 81 26 ? ? ? ? C6 46 03 ? 89 46 10 89 46 18 89 46 14 89 46 1C 89 46 04 89 46 08 89 46 0C FF 15 ? ? ? ? 6A 01 FF 75 08 8D 4D FC 8B 10 51 8B C8 FF 52 24 8B 0E 33 4D FC 81 E1 ? ? ? ? 31 0E 88 46 03"));
//	key_values(this, name);
//}
//void KeyValuess::SetUint64(const char* keyName, int value, int value2)
//{
//	static auto key_values_set_uint64 = reinterpret_cast<void(__thiscall*)(void*, const char*, int, int)>(FindPattern("client.dll", "55 8B EC 56 6A 01 FF 75 08"));
//	key_values_set_uint64(this, keyName, value, value2);
//}
//const char* KeyValuess::GetString(const char *keyName, const char *defaultValue)
//{
//	static auto key_values_get_string = reinterpret_cast<const char*(__thiscall*)(void*, const char*, const char*)>(FindPattern("client.dll", "55 8B EC 83 E4 C0 81 EC ? ? ? ? 53 8B 5D 08"));
//	return key_values_get_string(this, keyName, defaultValue);
//}
//int KeyValuess::GetInt(const char *keyName, int defaultValue)
//{
//	static auto key_values_get_int = reinterpret_cast<int(__thiscall*)(void*, const char*, int)>(FindPattern("client.dll", "55 8B EC 6A ? FF 75 08 E8 ? ? ? ? 85 C0 74 45"));
//	return key_values_get_int(this, keyName, defaultValue);
//}
//void KeyValuess::SetInt(const char *keyName, int Value)
//{
//	auto key_int = FindKey(keyName, true);
//	if (key_int)
//	{
//		*reinterpret_cast<int*>(reinterpret_cast<uintptr_t>(key_int) + 0xC) = Value;
//		*reinterpret_cast<char*>(reinterpret_cast<uintptr_t>(key_int) + 0x10) = 2;
//	}
//}
//
//

