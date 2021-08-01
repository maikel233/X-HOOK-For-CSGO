#include "util_sdk.h"
#include "FindPattern.h"



char Util::GetButtonString(ButtonCode_t key)
{
	switch (key)
	{
	case KEY_PAD_0:
		return '0';
	case KEY_PAD_1:
		return '1';
	case KEY_PAD_2:
		return '2';
	case KEY_PAD_3:
		return '3';
	case KEY_PAD_4:
		return '4';
	case KEY_PAD_5:
		return '5';
	case KEY_PAD_6:
		return '6';
	case KEY_PAD_7:
		return '7';
	case KEY_PAD_8:
		return '8';
	case KEY_PAD_9:
		return '9';
	case KEY_PAD_DIVIDE:
		return '/';
	case KEY_PAD_MULTIPLY:
		return '*';
	case KEY_PAD_MINUS:
		return '-';
	case KEY_PAD_PLUS:
		return '+';
	case KEY_SEMICOLON:
		return ';';
	default:
		return pInputSystem->ButtonCodeToString(key)[0];
	}
}

std::string Util::GetButtonName(ButtonCode_t buttonCode)
{
	return pInputSystem->ButtonCodeToString(buttonCode);
}

ButtonCode_t Util::GetButtonCode(std::string buttonName)
{
	for (int i = 0; i < ButtonCode_t::KEY_XSTICK2_UP; i++)
	{
		const char* currentButton = pInputSystem->ButtonCodeToString((ButtonCode_t)i);
		if (strcmp(currentButton, buttonName.c_str()) == 0)
			return (ButtonCode_t)i;
	}

	return ButtonCode_t::BUTTON_CODE_INVALID;
}



typedef void(__thiscall* orgGameFunc_LoadFromBuffer)(void* thisptr, const char* resourceName, const char* pBuffer, /*IBaseFileSystem**/void* pFileSystem, const char* pPathID, void* pfnEvaluateSymbolProc);
orgGameFunc_LoadFromBuffer pLoadFromBuffer;

//void Util::LoadFromBuffer(KeyValues* pKeyValues, const char* resourceName, const char* pBuffer, /*IBaseFileSystem**/void* pFileSystem = nullptr, const char* pPathID = NULL, void* pfnEvaluateSymbolProc = nullptr)
//{
//	auto dwLoadFromBuffer = FindPattern("client.dll", "55 8B EC 83 E4 F8 83 EC 34 53 8B 5D 0C 89"); //55 8B EC 83 E4 F8 83 EC 34 53 8B 5D 0C 89
//
//	if (!pLoadFromBuffer)
//		pLoadFromBuffer = reinterpret_cast<orgGameFunc_LoadFromBuffer>(dwLoadFromBuffer);
//
//	pLoadFromBuffer(pKeyValues, resourceName, pBuffer, pFileSystem, pPathID, pfnEvaluateSymbolProc);
//}


void Util::InitKeyValues(KeyValues* keyValues, char const* name)
{
	DWORD dwFunction = (DWORD)KeyValues_KeyValues;
	__asm
	{
		push name
		mov ecx, keyValues
		call dwFunction
	}



}

void Util::LoadFromBuffer(KeyValues* keyValues, char const* resourceName, const char* pBuffer, /*IBaseFileSystem**/void* pFileSystem, const char* pPathID, void* pfnEvaluateSymbolProc)
{
	DWORD dwFunction = (DWORD)KeyValues_LoadFromBuffer;

	__asm
	{
		push 0
		push 0
		push 0
		push pBuffer
		push resourceName
		mov ecx, keyValues
		call dwFunction
	}
}
int RandomInt2(int min, int max)
{
	return rand() % (max - min + 1) + min;
}
IMaterial* Util::CreateMaterial(std::string type, std::string texture, bool ignorez, bool nofog, bool model, bool nocull, bool halflambert)
{
	std::stringstream materialData;
	materialData << "\"" + type + "\"\n"
		"{\n"
		"\t\"$basetexture\" \"" + texture + "\"\n"
		"\t\"$ignorez\" \"" + std::to_string(ignorez) + "\"\n"
		"\t\"$nofog\" \"" + std::to_string(nofog) + "\"\n"
		"\t\"$model\" \"" + std::to_string(model) + "\"\n"
		"\t\"$nocull\" \"" + std::to_string(nocull) + "\"\n"
		"\t\"$halflambert\" \"" + std::to_string(halflambert) + "\"\n"
		"}\n" << std::flush;

	std::string materialName = "XHOOK" + std::to_string(RandomInt2(10, 100000));
	KeyValues* keyValues = new KeyValues(materialName.c_str());

	Util::InitKeyValues(keyValues, type.c_str());
	Util::LoadFromBuffer(keyValues, materialName.c_str(), materialData.str().c_str(), nullptr, NULL, nullptr);

	return pMaterial->CreateMaterial(materialName.c_str(), keyValues);
}

