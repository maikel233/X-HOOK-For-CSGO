#pragma once
#include <cstdint>


using symbol_fn = bool( __cdecl* )(const char*);
class c_key_values {
public:
	void load_from_buffer( const char* resource_name, const char* buf, void* file_sys = nullptr, const char* path_id = nullptr, void* eval_sym_proc = nullptr, void* unk = nullptr );
	c_key_values( const char* key_name, void* unk = nullptr, void* unk2 = nullptr );
	//c_key_values KeyValues_FindKey(c_key_values* key, const char* keyName, bool bCreate = false);
	//void KeyValues_SetString(c_key_values* key, const char* value);


	//_DWORD *__thiscall KeyValues::FindKey(_DWORD *this, char *a2, int a3)

	c_key_values* FindKey(const char* keyName, bool bCreate = false);
	void SetString(const char* keyName, const char* value);
	void InitKeyValues(const char* name);
	void SetUint64(const char* keyName, int value, int value2);
	const char* GetString(const char* keyName, const char* defaultValue);
	int GetInt(const char* keyName, int defaultValue);
	void SetInt(const char* keyName, int Value);

	//key_values_INIT(v2, (int)"Game::ChatReportError", 0, 0);

	//_DWORD* __thiscall key_values_INIT(_DWORD* this, int a2, int a3, signed int a4)

	//inline void GameFunc_InitKeyValues(c_key_values* key, const char* name, int ,int)
	//{
	//	auto key_values = reinterpret_cast<void(__thiscall*)(void*, const char*)>(FindPattern("client.dll", "55 8B EC 51 33 C0 C7 45 ? ? ? ? ? 56 8B F1 81 26 ? ? ? ? C6 46 03 ? 89 46 10 89 46 18 89 46 14 89 46 1C 89 46 04 89 46 08 89 46 0C FF 15 ? ? ? ? 6A 01 FF 75 08 8D 4D FC 8B 10 51 8B C8 FF 52 24 8B 0E 33 4D FC 81 E1 ? ? ? ? 31 0E 88 46 03"));//(Utils_fox.FindPatternMask("client_panorama.dll", (PBYTE)"\x55\x8B\xEC\x51\x33\xC0\xC7\x45\x00\x00\x00\x00\x00\x56\x8B\xF1\x81\x26\x00\x00\x00\x00\xC6\x46\x03\x00\x89\x46\x10\x89\x46\x18\x89\x46\x14\x89\x46\x1C\x89\x46\x04\x89\x46\x08\x89\x46\x0C\xFF\x15\x00\x00\x00\x00\x6A\x01\xFF\x75\x08\x8D\x4D\xFC\x8B\x10\x51\x8B\xC8\xFF\x52\x24\x8B\x0E\x33\x4D\xFC\x81\xE1\x00\x00\x00\x00\x31\x0E\x88\x46\x03", "xxxxxxxx?????xxxxx????xxxxxxxxxxxxxxxxxxxxxxxxxxx????xxxxxxxxxxxxxxxxxxxxxxx????xxxxx"));
	//	key_values(key, name);
	//}


private:
	std::uint32_t m_key_name : 24;
	std::uint32_t m_key_name_case_sensitive1 : 8;
	char* m_sz_value{};
	wchar_t* m_wsz_value{};

	union {
		int m_i_value{};
		float m_f_value;
		void* m_value;
		unsigned char m_color[4];
	};

	std::int8_t m_data_type{};
	bool m_has_escape_sequences{};
	std::uint16_t m_key_name_case_sensitive2{};
	void* m_unk{};
	bool m_has_case_insensitive_key_symbol{};
	c_key_values* m_peer{};
	c_key_values* m_sub{};
	c_key_values* m_chain{};
	symbol_fn m_expression_get_symbol_proc{};
};

