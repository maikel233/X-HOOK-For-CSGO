#pragma once
#include <windows.h>
#include <Psapi.h>
#include <stdint.h>
#include <fstream>
uintptr_t FindPattern(const char* module, const char* pattern_string, const char* mask);
DWORD FindPatternV2(std::string moduleName, std::string pattern);
uintptr_t FindPatternMask(std::string module, BYTE* mask, char* szMask);
