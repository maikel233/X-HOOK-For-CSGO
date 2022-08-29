#pragma once

#include "../SDK/SDK.h"
#include "../interfaces.h"
#include "util.h"

namespace Util
{
	char GetButtonString(ButtonCode_t key);
	std::string GetButtonName(ButtonCode_t buttonCode);
	ButtonCode_t GetButtonCode(std::string buttonName);

//	void InitKeyValues(KeyValues* keyValues, char const * name, int, int);
//	void LoadFromBuffer(KeyValues* keyValues, char const *resourceName, const char *pBuffer);
	                 //(KeyValues* KeyValues, const char* resourceName, const char* pBuffer, /*IBaseFileSystem**/void* pFileSystem = nullptr, const char* pPathID = NULL, void* pfnEvaluateSymbolProc = nullptr)
	//void LoadFromBuffer(KeyValues* keyValues, char const *resourceName, const char *pBuffer, /*IBaseFileSystem**/void* pFileSystem, const char* pPathID, void* pfnEvaluateSymbolProc);

	IMaterial* CreateMaterial(std::string type, std::string texture, bool ignorez, bool nofog, bool model, bool nocull, bool halflambert);

	

}