#pragma once

#include <vector>

#include "features.h"

namespace SkinParser
{
	extern std::vector<std::pair<int, std::string>> k_skins;
	extern std::vector<std::pair<int, std::string>> k_gloves;
	extern std::vector<std::pair<int, std::string>> k_stickers;

	extern void InitializeKits();

}