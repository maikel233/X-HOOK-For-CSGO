#pragma once

#include "SDK/SDK.h"
#include "interfaces.h"
#include "util.h"

namespace Util
{
	char GetButtonString(ButtonCode_t key);
	std::string GetButtonName(ButtonCode_t buttonCode);
	ButtonCode_t GetButtonCode(std::string buttonName);

	void InitKeyValues(KeyValues* keyValues, char const * name);
	void LoadFromBuffer(KeyValues* keyValues, char const *resourceName, const char *pBuffer);

	IMaterial* CreateMaterial(std::string type, std::string texture, bool ignorez, bool nofog, bool model, bool nocull, bool halflambert);
}